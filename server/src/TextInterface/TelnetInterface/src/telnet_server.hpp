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

#ifndef SERVER_SRC_TEXTINTERFACE_TELNETINTERFACE_TELNET_SERVER_HPP_
#define SERVER_SRC_TEXTINTERFACE_TELNETINTERFACE_TELNET_SERVER_HPP_

// C++ Headers
#include <vector>

// Src Headers
#include <Utils/BasicServer/basic_server.hpp>

// External Headers
#include <boost/asio.hpp>
#include <boost/fiber/condition_variable.hpp>
#include <boost/fiber/mutex.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <telnetpp/session.hpp>

// Local Headers
#include "telnet_connection.hpp"

// Defines
#define DEFAULT_PORT 22222
#define BOOST_ASIO_NO_DEPRECATED

using boost::asio::ip::tcp;

/*
 * Class responsible for listening to and creating Telnet connections.
 */
class TelnetServer : public BasicServer<spTelnetConnection> {
 public:
  explicit TelnetServer(unsigned int id);
  ~TelnetServer();

  void start() override;

 private:
  /* Starts accepting loop for new connections. */
  void startAccept();

  void connAuthAndSendFibers() override;
  void connReadMessagesFibers() override;

  void sendNewMsgsToGameServer(spTelnetConnection telnet_connection) override;

  /* Responsible for asynchronously accepting new tcp connections */
  tcp::acceptor acceptor_;

  /* Telnetpp session handler - Server side */
  telnetpp::session telnet_session_;
};

typedef boost::shared_ptr<TelnetServer> spTelnetServer;

#endif  // SERVER_SRC_TEXTINTERFACE_TELNETINTERFACE_TELNET_SERVER_HPP_
