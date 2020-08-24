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
#ifndef TELNET_SERVER_H
#define TELNET_SERVER_H

#define DEFAULT_PORT 22222

#include <vector>

#define BOOST_ASIO_NO_DEPRECATED
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <telnetpp/session.hpp>

#include "telnet_connection.h"

using boost::asio::ip::tcp;

class TelnetServer {
 public:
  TelnetServer();
  TelnetServer(unsigned int port);
  ~TelnetServer();

  void Start();

 private:
  void StartAccept();
  void HandleAccept(TelnetConnection::Ptr new_connection,
                    const boost::system::error_code &error);

  boost::asio::io_context io_context_;

  boost::thread *t_start_accept_;
  tcp::acceptor *acceptor_;

  boost::thread_group t_curr_connections;
  std::vector<TelnetConnection::Ptr> curr_connections;

  telnetpp::session telnet_session_;
};

#endif
