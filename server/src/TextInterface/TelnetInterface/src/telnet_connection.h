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

using boost::asio::ip::tcp;

class TelnetConnection
    : public boost::enable_shared_from_this<TelnetConnection> {
 public:
  typedef boost::shared_ptr<TelnetConnection> Ptr;
  static Ptr CreatePtr(boost::asio::io_context& io_context);
  ~TelnetConnection();

  /* Get Telnet connection socket */
  tcp::socket& GetSocket();

  void Start();

  void ActivateNoEcho();
  void DeactivateNoEcho();

 private:
  TelnetConnection(boost::asio::io_context& io_context);

  /* Install default options on client session */
  void SetupOptions();

  void Send(std::string message);
  void Write(telnetpp::element const& data);
  void RawWrite(telnetpp::bytes data);
  void HandleWrite(const boost::system::error_code& error, size_t bytes_transf);

  void Receive();
  void ReadFromClient(telnetpp::bytes data);
  void HandleRead(const boost::system::error_code& error, size_t recv_len);

  /* Telnetpp session handler - Client side */
  telnetpp::session telnet_session_;
  telnetpp::options::echo::server t_echo_server_;
  telnetpp::options::naws::client t_naws_client_;
  telnetpp::options::terminal_type::client t_termtype_client_;

  /* Socket file descriptor to raw async-write to and sync read from. */
  tcp::socket socket_;
};

#endif
