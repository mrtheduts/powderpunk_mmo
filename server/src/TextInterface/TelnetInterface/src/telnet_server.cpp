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

#include <Utils/DebugTools/assert_debug_print.h>

#include <boost/bind.hpp>
#include <boost/fiber/algo/round_robin.hpp>
#include <boost/fiber/all.hpp>
#include <boost/fiber/operations.hpp>
#include <boost/log/trivial.hpp>
#include <boost/smart_ptr/make_shared_object.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>

#include "telnet_connection.h"

TelnetServer::TelnetServer()
    : acceptor_(io_context_, tcp::endpoint(tcp::v4(), DEFAULT_PORT)) {
  next_id_ = 0;
}

TelnetServer::TelnetServer(unsigned int port)
    : acceptor_(io_context_, tcp::endpoint(tcp::v4(), port)) {
  next_id_ = 0;
}

TelnetServer::~TelnetServer() {
  DEBUG("Vou deletar o TelnetServer...");
  /* t_curr_connections.join_all(); */
}

void TelnetServer::Start() {
  t_connections_ =
      boost::make_shared<boost::thread>(&TelnetServer::FiberManager, this);
  StartAccept();
  io_context_.run();  // Should be last: it takes control of the thread
}

/* StartAccept() -> void
 *
 * Asynchronously accepts new connection and adds it to the new connection
 * queue, calling its Receive function. At the end of the callback, it calls
 * itself again to maintain the main "recursive" loop.
 */
void TelnetServer::StartAccept() {
  acceptor_.async_accept(
      [this](std::error_code error, boost::asio::ip::tcp::socket new_socket) {
        if (!error) {
          boost::shared_ptr<TelnetConnection> new_conn =
              boost::make_shared<TelnetConnection>(
                  io_context_, std::move(new_socket), next_id_++);

          new_conns_.Push(new_conn);
          BOOST_LOG_TRIVIAL(info) << "[TelnetServer] Pushed new connection ("
                                  << new_conn->GetId() << ") to new_conns_";
          cv_new_conns_.notify_one();  // He's the one supposed to
                                       // initialize the receive loop

          new_conn->Receive();  // It's here to stay in the io_context thread
        } else {
          BOOST_LOG_TRIVIAL(error) << "[TelnetServer] " << error.message();
        }

        StartAccept();
      });
}

void TelnetServer::FiberManager() {
  while (true) {
    std::unique_lock<boost::fibers::mutex> lock_new_conns(m_new_conns_);
    cv_new_conns_.wait(lock_new_conns);
    BOOST_LOG_TRIVIAL(debug) << "[TelnetServer] Received new connection(s)";

    while (!new_conns_.IsEmpty()) {
      boost::shared_ptr<TelnetConnection> new_conn = new_conns_.Pop();

      // TODO: store these fibers somewhere
      boost::make_shared<boost::fibers::fiber>(&TelnetConnection::StartReceive,
                                               new_conn)
          ->detach();
      boost::make_shared<boost::fibers::fiber>(&TelnetConnection::StartSend,
                                               new_conn)
          ->detach();
      BOOST_LOG_TRIVIAL(info) << "[TelnetServer] Started new connection ("
                              << new_conn->GetId() << ")";
    };
  }
}

/* void TelnetServer::HandleAccept(const boost::system::error_code &error, */
/*                                 boost::asio::ip::tcp::socket new_socket) {
 */
/*   if (!error) { */
/*     boost::shared_ptr<TelnetConnection> new_connection = */
/*         boost::make_shared<TelnetConnection>(io_context_,
 * std::move(new_socket), */
/*                                              next_id_++); */
/*     boost::thread t_new_connection(&TelnetConnection::Start,
 * new_connection);
 */

/*     curr_connections.push_back(new_connection); */
/*     t_curr_connections.add_thread(&t_new_connection); */
/*   } else { */
/*     DEBUG(error.message()); */
/*   } */

/*   StartAccept(); */
/* } */
/* void TelnetServer::HandleAccept(TelnetConnection::Ptr new_connection, */
/*                                 const boost::system::error_code& error) {
 */
/*   if (!error) { */
/*     boost::thread t_new_connection(&TelnetConnection::Start, */
/*                                    &(*new_connection)); */

/*     curr_connections.push_back(new_connection); */
/*     t_curr_connections.add_thread(&t_new_connection); */
/*   } else { */
/*     DEBUG(error.message()); */
/*   } */

/*   this->StartAccept(); */
/* } */
