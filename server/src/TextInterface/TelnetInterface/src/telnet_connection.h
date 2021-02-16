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

#define INPUT_BUFFER_SIZE 1024

#include <Utils/BasicConnection/basic_connection.h>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <string>
#include <telnetpp/core.hpp>
#include <telnetpp/options/echo/server.hpp>
#include <telnetpp/options/naws/client.hpp>
#include <telnetpp/options/terminal_type/client.hpp>
#include <telnetpp/session.hpp>
#include <vector>

class TelnetConnection : public BasicConnection<std::string> {
 public:
  TelnetConnection(boost::asio::io_context& io_context,
                   boost::asio::ip::tcp::socket socket, unsigned long int id);
  ~TelnetConnection();

  void Start();
  void Send(const std::string& message);
  void Receive();

  void ActivateNoEcho();
  void DeactivateNoEcho();

 protected:
  /* Install default options on client session */
  void SetupOptions();

  void Write(telnetpp::element const& data);
  void RawWrite(telnetpp::bytes data);
  void HandleWrite(const boost::system::error_code& error, size_t bytes_transf);

  void ReadFromClient(telnetpp::bytes data);
  void HandleRead(const boost::system::error_code& error, size_t recv_len);

  /* Telnetpp session handler - Client side */
  telnetpp::session telnet_session_;
  telnetpp::options::echo::server t_echo_server_;  // Echo on user screen
  telnetpp::options::naws::client t_naws_client_;  // Terminal Size
  telnetpp::options::terminal_type::client t_termtype_client_;  // Terminal type
  telnetpp::byte buffer[INPUT_BUFFER_SIZE];
};

#endif
