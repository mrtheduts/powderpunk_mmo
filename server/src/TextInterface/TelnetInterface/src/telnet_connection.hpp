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

#ifndef TELNET_CONNECTION_H
#define TELNET_CONNECTION_H

// Src Headers
#include <Utils/BasicConnection/basic_connection.hpp>

// External Headers
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/fiber/condition_variable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <telnetpp/core.hpp>
#include <telnetpp/options/echo/server.hpp>
#include <telnetpp/options/naws/client.hpp>
#include <telnetpp/options/terminal_type/client.hpp>
#include <telnetpp/session.hpp>

// C++ Headers
#include <string>
#include <vector>

// Defines
#define INPUT_BUFFER_SIZE 1024
#define OUTPUT_BUFFER_SIZE 1024

static const char WELCOME_BANNER[] =
    "Welcome to PowderPunk Test Version!\nWhat is your name?\n";

static const char PROMPT[] = "\nsay > ";

class TelnetConnection : public BasicConnection<std::string> {
 public:
  TelnetConnection(boost::asio::io_context& io_context,
                   boost::asio::ip::tcp::socket socket, unsigned long int id,
                   unsigned long int server_id);
  ~TelnetConnection();

  void startReceive() override;
  void startSend() override;
  void receive() override;

  void addToSendQueue(const std::string& message);

  void activateNoEcho();
  void deactivateNoEcho();

  std::string read() override;

 protected:
  /* Install default options on client session */
  void setupOptions();
  bool authenticate() override;

  void send(const std::string& message) override;

  void write(telnetpp::element const& data);
  void rawWrite(telnetpp::bytes data);
  void handleWrite(const boost::system::error_code& error, size_t bytes_transf);

  std::string readPreAuth();
  std::string read(boost::fibers::condition_variable& cv);
  void handleRead(const boost::system::error_code& error, size_t recv_len);

  /* Telnetpp session handler - Client side */
  telnetpp::session telnet_session_;
  telnetpp::options::echo::server t_echo_server_;  // Echo on user screen
  telnetpp::options::naws::client t_naws_client_;  // Terminal Size
  telnetpp::options::terminal_type::client t_termtype_client_;  // Terminal type
  telnetpp::byte input_buffer_[INPUT_BUFFER_SIZE];

  boost::fibers::condition_variable cv_pre_auth_received_msgs_;

  std::string user_name_;
};

typedef boost::shared_ptr<TelnetConnection> spTelnetConnection;

#endif
