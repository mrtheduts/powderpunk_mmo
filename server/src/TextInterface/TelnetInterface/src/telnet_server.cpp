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
#include <boost/fiber/all.hpp>
#include <boost/smart_ptr/make_shared_object.hpp>

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
  t_curr_connections.join_all();
}

void TelnetServer::Start() {
  StartAccept();
  io_context_.run();
}

void TelnetServer::StartAccept() {
  acceptor_.async_accept([this](std::error_code error,
                                boost::asio::ip::tcp::socket new_socket) {
    if (!error) {
      boost::shared_ptr<TelnetConnection> new_connection =
          boost::make_shared<TelnetConnection>(
              io_context_, std::move(new_socket), next_id_++);
      boost::thread t_new_connection(&TelnetConnection::Start, new_connection);

      curr_connections.push_back(new_connection);
      t_curr_connections.add_thread(&t_new_connection);
    } else {
      DEBUG(error.message());
    }

    StartAccept();
  });
}

/* void TelnetServer::HandleAccept(const boost::system::error_code &error, */
/*                                 boost::asio::ip::tcp::socket new_socket) { */
/*   if (!error) { */
/*     boost::shared_ptr<TelnetConnection> new_connection = */
/*         boost::make_shared<TelnetConnection>(io_context_,
 * std::move(new_socket), */
/*                                              next_id_++); */
/*     boost::thread t_new_connection(&TelnetConnection::Start, new_connection);
 */

/*     curr_connections.push_back(new_connection); */
/*     t_curr_connections.add_thread(&t_new_connection); */
/*   } else { */
/*     DEBUG(error.message()); */
/*   } */

/*   StartAccept(); */
/* } */
/* void TelnetServer::HandleAccept(TelnetConnection::Ptr new_connection, */
/*                                 const boost::system::error_code& error) { */
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
