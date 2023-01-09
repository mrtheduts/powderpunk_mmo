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

#include "telnet_server.hpp"

// Src Headers
#include <DataStructures/user_command.hpp>
#include <TextToCommand/text_to_command.hpp>
#include <Utils/DebugTools/assert_debug_print.hpp>

// External Headers
#include <boost/bind.hpp>
#include <boost/fiber/all.hpp>
#include <boost/log/trivial.hpp>
#include <boost/smart_ptr/make_shared_object.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>

TelnetServer::TelnetServer(unsigned int id)
    : BasicServer(id),
      acceptor_(io_context_, tcp::endpoint(tcp::v4(), DEFAULT_PORT)) {
  logger_ = Logger::getLogger("TelnetServer", id);
}

TelnetServer::~TelnetServer() {}

void TelnetServer::start() {
  t_conn_auth_and_send_fibers_ = boost::make_shared<boost::thread>(
      &TelnetServer::connAuthAndSendFibers, this);
  t_conn_read_messages_fibers_ = boost::make_shared<boost::thread>(
      &TelnetServer::connReadMessagesFibers, this);

  logger_->info("Started accept connections");
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
          logger_->info("Accepted new connection [%d] from %s", new_conn->id,
                        new_conn->getConnIP().c_str());
          cv_new_conns_.notify_all();  // He's the one supposed to
                                       // alert the ConnAuthAndSendFibers and
                                       // ReadConnMessages

          new_conn->receive();  // It's here to stay in the io_context thread
        } else {
          logger_->error(error.message());
        }

        startAccept();
      });
}

void TelnetServer::connAuthAndSendFibers() {
  logger_->debug("Starting auth and send fibers");
  while (true) {
    {
      std::unique_lock<boost::fibers::mutex> lock_new_conns(
          q_auth_and_send_f_m_);
      cv_new_conns_.wait(lock_new_conns,
                         [this]() { return !q_auth_and_send_.fIsEmpty(); });
    }

    std::queue<spTelnetConnection> new_conns = q_auth_and_send_.fPopAll();

    while (!new_conns.empty()) {
      spTelnetConnection new_conn = new_conns.front();
      new_conns.pop();

      // TODO: store these fibers somewhere
      boost::make_shared<boost::fibers::fiber>(&TelnetConnection::startReceive,
                                               new_conn)
          ->detach();
      boost::make_shared<boost::fibers::fiber>(&TelnetConnection::startSend,
                                               new_conn)
          ->detach();
      logger_->debug("Started send and receive fibers for connection [%d]",
                     new_conn->id);
    };
  }
}

void TelnetServer::connReadMessagesFibers() {
  logger_->debug("Starting read fibers");
  while (true) {
    {
      std::unique_lock<boost::fibers::mutex> lock_new_conns(
          q_read_messages_f_m_);
      cv_new_conns_.wait(lock_new_conns,
                         [this]() { return !q_read_messages_.fIsEmpty(); });
    }

    std::queue<spTelnetConnection> new_conns = q_read_messages_.fPopAll();

    while (!new_conns.empty()) {
      spTelnetConnection new_conn = new_conns.front();
      new_conns.pop();

      // TODO: store these fibers somewhere
      boost::make_shared<boost::fibers::fiber>(
          &TelnetServer::sendNewMsgsToGameServer, this, new_conn)
          ->detach();
      logger_->debug("Started new read fiber for connection [%d]",
                     new_conn->id);
    };
  }
}

void TelnetServer::sendNewMsgsToGameServer(
    boost::shared_ptr<TelnetConnection> telnet_connection) {
  while (true) {
    std::string msg = telnet_connection->read();
    boost::shared_ptr<UserCommand> usr_cmd =
        msgToUsrCmd(telnet_connection->server_id, telnet_connection->id,
                    telnet_connection->getUsrId(), msg);
    q_usr_cmds_.push(usr_cmd);
    q_usr_cmds_.q_f_cv.notify_one();
    logger_->debug("Received command %s", usr_cmd->toString().c_str());
  };
}

