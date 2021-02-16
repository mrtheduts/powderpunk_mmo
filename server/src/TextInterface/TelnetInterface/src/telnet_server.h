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

#include <boost/fiber/condition_variable.hpp>
#include <boost/fiber/mutex.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>

#include "Utils/BasicConnection/ts_queue.h"
#define DEFAULT_PORT 22222

#include <vector>

#define BOOST_ASIO_NO_DEPRECATED
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <telnetpp/session.hpp>

#include "telnet_connection.h"

using boost::asio::ip::tcp;

/*
 * Responsible for listening to and creating Telnet connections.
 */
class TelnetServer {
 public:
  TelnetServer();
  TelnetServer(unsigned int port);
  ~TelnetServer();

  void Start();

 private:
  /* Starts accepting loop for new connections. */
  void StartAccept();
  void FiberManager();

  /* Boost IO context handler for thread-safe operations within it. */
  boost::asio::io_context io_context_;

  /* TODO: I'm sure I had plans with it. Now, I have no clue anymore. */
  boost::shared_ptr<boost::thread> t_connections_;

  /* Responsible for asynchronously accepting new tcp connections */
  tcp::acceptor acceptor_;

  unsigned long int next_id_;

  /* Thread group responsible for managing TelnetConnections threads. */
  /* boost::thread_group t_curr_connections; */

  /* Thread group responsible for managing TelnetConnections. */
  TSQueue<boost::shared_ptr<TelnetConnection>> new_conns_;
  boost::fibers::mutex m_new_conns_;
  boost::fibers::condition_variable cv_new_conns_;

  /* Telnetpp session handler - Server side */
  telnetpp::session telnet_session_;
};

#endif
