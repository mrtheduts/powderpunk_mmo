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

#include <DebugTools/assert_debug_print.h>

#include <boost/bind.hpp>

TelnetServer::TelnetServer() {
  acceptor_ =
      new tcp::acceptor(io_context_, tcp::endpoint(tcp::v4(), DEFAULT_PORT));
}

TelnetServer::TelnetServer(unsigned int port) {
  acceptor_ = new tcp::acceptor(io_context_, tcp::endpoint(tcp::v4(), port));
}

TelnetServer::~TelnetServer() {
  DEBUG("Vou deletar o TelnetServer...");
  t_curr_connections.join_all();
  delete (acceptor_);
}

void TelnetServer::Start() {
  StartAccept();
  io_context_.run();
}

void TelnetServer::StartAccept() {
  TelnetConnection::Ptr new_connection;
  new_connection = TelnetConnection::CreatePtr(io_context_);

  acceptor_->async_accept(
      new_connection->GetSocket(),
      boost::bind(&TelnetServer::HandleAccept, this, new_connection,
                  boost::asio::placeholders::error));
}

void TelnetServer::HandleAccept(TelnetConnection::Ptr new_connection,
                                const boost::system::error_code& error) {
  if (!error) {
    boost::thread t_new_connection(&TelnetConnection::Start,
                                   &(*new_connection));

    curr_connections.push_back(new_connection);
    t_curr_connections.add_thread(&t_new_connection);
  } else {
    DEBUG(error.message());
  }

  this->StartAccept();
}
