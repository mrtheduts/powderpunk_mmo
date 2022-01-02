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

// Src Headers
#include <Logger/logger.h>
#include <Utils/BasicServer/basic_server.h>

// External Headers
#include <boost/asio.hpp>
#include <boost/fiber/condition_variable.hpp>
#include <boost/fiber/mutex.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <telnetpp/session.hpp>

// C++ Headers
#include <vector>

// Local Headers
#include "telnet_connection.h"

// Defines
#define DEFAULT_PORT 22222
#define BOOST_ASIO_NO_DEPRECATED

using boost::asio::ip::tcp;

/*
 * Class responsible for listening to and creating Telnet connections.
 */
class TelnetServer : public BasicServer<spTelnetConnection> {
 public:
  TelnetServer(unsigned int id);
  ~TelnetServer();

  void start() override;

 private:
  /* Starts accepting loop for new connections. */
  void startAccept();

  void connAuthAndSendFibers() override;
  void connReadMessagesFibers() override;

  void sendNewMsgsToGameServer(spTelnetConnection telnet_connection) override;

  unsigned long int next_id_;

  /* Responsible for asynchronously accepting new tcp connections */
  tcp::acceptor acceptor_;

  /* Telnetpp session handler - Server side */
  telnetpp::session telnet_session_;

  spLogger logger_;
};

typedef boost::shared_ptr<TelnetServer> spTelnetServer;

#endif
