/*
 *  Powderpunk Server - A Pretentious Project
 *
 *  Author: Eduardo "mrtheduts" Souza
 *          eduardomoreirafds@gmail.com
 *
 *  Date:   24 May 2020
 *
 *  (C) Copyright 2020 Eduardo "mrtheduts" Souza All Rights Reserved
 *
 */

#include "telnet_server.h"

#include <DataStructures/user_command.h>
#include <TextToCommand/text_to_command.h>
#include <Utils/DebugTools/assert_debug_print.h>

#include <boost/bind.hpp>
#include <boost/fiber/all.hpp>
#include <boost/log/trivial.hpp>
#include <boost/smart_ptr/make_shared_object.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>

TelnetServer::TelnetServer(unsigned int id)
    : BasicServer(id),
      next_id_{0},
      acceptor_(io_context_, tcp::endpoint(tcp::v4(), DEFAULT_PORT)) {}

TelnetServer::~TelnetServer() {}

void TelnetServer::start() {
  t_conn_auth_and_send_fibers_ = boost::make_shared<boost::thread>(
      &TelnetServer::connAuthAndSendFibers, this);
  t_conn_read_messages_fibers_ = boost::make_shared<boost::thread>(
      &TelnetServer::readConnMessagesFibers, this);

  startAccept();
  io_context_.run();  // Should be last: it takes control of the thread
}

/* StartAccept()
 *
 * Asynchronously accepts new connection and adds it to the new connection
 * queue, calling its Receive function. At the end of the callback, it calls
 * itself again to maintain the main "recursive" loop.
 */
void TelnetServer::startAccept() {
  acceptor_.async_accept(
      [this](std::error_code error, boost::asio::ip::tcp::socket new_socket) {
        if (!error) {
          boost::shared_ptr<TelnetConnection> new_conn =
              boost::make_shared<TelnetConnection>(
                  io_context_, std::move(new_socket), next_id_++, id);

          q_auth_and_send_.push(new_conn);
          q_read_messages_.push(new_conn);
          BOOST_LOG_TRIVIAL(info) << "[TelnetServer] Pushed new connection ("
                                  << new_conn->id << ") to new_conns_";
          cv_new_conns_.notify_all();  // He's the one supposed to
                                       // alert the ConnAuthAndSendFibers and
                                       // ReadConnMessages

          new_conn->receive();  // It's here to stay in the io_context thread
        } else {
          BOOST_LOG_TRIVIAL(error) << "[TelnetServer] " << error.message();
        }

        startAccept();
      });
}

void TelnetServer::connAuthAndSendFibers() {
  while (true) {
    std::unique_lock<boost::fibers::mutex> lock_new_conns(
        q_auth_and_send_.q_f_mutex);
    cv_new_conns_.wait(lock_new_conns);

    while (!q_auth_and_send_.isEmpty()) {
      boost::shared_ptr<TelnetConnection> new_conn = q_auth_and_send_.pop();

      // TODO: store these fibers somewhere
      boost::make_shared<boost::fibers::fiber>(&TelnetConnection::startReceive,
                                               new_conn)
          ->detach();
      boost::make_shared<boost::fibers::fiber>(&TelnetConnection::startSend,
                                               new_conn)
          ->detach();
      BOOST_LOG_TRIVIAL(info)
          << "[TelnetServer] Started new connection (" << new_conn->id << ")";
    };
  }
}

void TelnetServer::readConnMessagesFibers() {
  while (true) {
    std::unique_lock<boost::fibers::mutex> lock_new_conns(
        q_read_messages_.q_f_mutex);
    cv_new_conns_.wait(lock_new_conns);

    while (!q_read_messages_.isEmpty()) {
      boost::shared_ptr<TelnetConnection> new_conn = q_read_messages_.pop();

      // TODO: store these fibers somewhere
      boost::make_shared<boost::fibers::fiber>(
          &TelnetServer::sendNewMsgsToGameServer, this, new_conn)
          ->detach();
      BOOST_LOG_TRIVIAL(info)
          << "[TelnetServer] Started new read from (" << new_conn->id << ")";
    };
  }
}

void TelnetServer::sendNewMsgsToGameServer(
    boost::shared_ptr<TelnetConnection> telnet_connection) {
  while (true) {
    std::string msg = telnet_connection->read();
    boost::shared_ptr<UserCommand> usr_cmd =
        msgToUsrCmd(telnet_connection->server_id, telnet_connection->id, msg);
    q_usr_cmds_.fPush(usr_cmd);
    q_usr_cmds_.q_cv.notify_one();
  };
}

