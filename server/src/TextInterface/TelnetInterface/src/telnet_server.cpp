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
  /* t_connections_ = */
  /*     boost::make_shared<boost::thread>(&TelnetServer::FiberManager, this);
   */
  StartAccept();
  io_context_.run();
}

void TelnetServer::StartAccept() {
  acceptor_.async_accept(
      [this](std::error_code error, boost::asio::ip::tcp::socket new_socket) {
        if (!error) {
          boost::shared_ptr<TelnetConnection> new_conn =
              boost::make_shared<TelnetConnection>(
                  io_context_, std::move(new_socket), next_id_++);

          new_conns_.Push(new_conn);
          std::cout << "Pushed connection " << new_conn->GetId()
                    << " to tsqueue!\n";
          new_conn->Receive();
          cv_new_conns_.notify_one();
        } else {
          DEBUG(error.message());
        }

        StartAccept();
      });
}

void TelnetServer::FiberManager() {
  while (true) {
    std::unique_lock<boost::fibers::mutex> lock_new_conns(m_new_conns_);
    cv_new_conns_.wait(lock_new_conns);
    while (!new_conns_.IsEmpty()) {
      boost::shared_ptr<TelnetConnection> new_conn = new_conns_.Pop();
      boost::make_shared<boost::fibers::fiber>(&TelnetConnection::Start,
                                               new_conn)
          ->detach();
      std::cout << "Started connection " << new_conn->GetId() << " !\n";
    }
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
