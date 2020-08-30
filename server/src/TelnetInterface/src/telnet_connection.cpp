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

#include "telnet_connection.h"

#include <DebugTools/assert_debug_print.h>

#include <string>

TelnetConnection::Ptr TelnetConnection::CreatePtr(
    boost::asio::io_context& io_context) {
  return Ptr(new TelnetConnection(io_context));
}

TelnetConnection::~TelnetConnection() {}

tcp::socket& TelnetConnection::GetSocket() { return socket_; }

void TelnetConnection::Start() {
  DEBUG("Starting TelnetConnection for IP: ");
  DEBUG(this->socket_.remote_endpoint());
  DEBUG("\n");

  SetupOptions();

  try {
    while (1) {
      Receive();
    }
  } catch (boost::system::system_error error) {
    if (error.code() == boost::asio::error::eof) {
      DEBUG("E aí, vamo fecha?\n");
      return;
    }
  }
}

TelnetConnection::TelnetConnection(boost::asio::io_context& io_context)
    : telnet_session_(), socket_(io_context) {}

void TelnetConnection::SetupOptions() {
  telnet_session_.install(t_echo_server_);

  // auto const& write_continuation = [this](telnetpp::element const& elem) {
  // Write(elem);
  // };

  // t_echo_server_.activate(write_continuation);
}

void TelnetConnection::Send(std::string message) {
  std::vector<uint8_t> tmp_vector(message.begin(), message.end());
  telnetpp::element message_bytes(tmp_vector);

  Write(message_bytes);
}

void TelnetConnection::Write(telnetpp::element const& data) {
  telnet_session_.send(data, [this](telnetpp::bytes data) { RawWrite(data); });
}

void TelnetConnection::RawWrite(telnetpp::bytes data) {
  std::cout << "Enviando:[ ";
  for (const auto& i : data) printf("%3u ", i);
  printf("]\n");

  boost::asio::async_write(
      socket_, boost::asio::buffer(data.data(), data.size_bytes()),
      boost::bind(&TelnetConnection::HandleWrite, shared_from_this(),
                  boost::asio::placeholders::error,
                  boost::asio::placeholders::bytes_transferred));
}

void TelnetConnection::HandleWrite(const boost::system::error_code& error,
                                   size_t bytes_transf) {
  std::cout << error.message() << " - " << bytes_transf << std::endl;
}

void TelnetConnection::ReadFromClient(telnetpp::bytes data) {
  std::string message(data.begin(), data.end());

  message.erase(std::remove(message.begin(), message.end(), 10), message.end());
  message.erase(std::remove(message.begin(), message.end(), 13), message.end());
  std::cout << message << std::endl;
}

void TelnetConnection::Receive() {
  boost::system::error_code error;
  telnetpp::byte buffer[INPUT_BUFFER_SIZE];
  size_t num_recv_bytes;

  num_recv_bytes =
      socket_.read_some(boost::asio::buffer(buffer, INPUT_BUFFER_SIZE), error);

  std::cout << "Recebido: [ ";
  for (size_t i = 0; i < num_recv_bytes; ++i) printf("%3u ", buffer[i]);
  printf("]\n");

  if (error == boost::asio::error::eof) {
    throw boost::system::system_error(boost::asio::error::eof);
  }

  telnet_session_.receive(
      telnetpp::bytes{buffer, num_recv_bytes},
      [this](telnetpp::bytes data,
             std::function<void(telnetpp::bytes)> const& /*send*/) {
        ReadFromClient(data);
      },
      [this](telnetpp::bytes data) { RawWrite(data); });
}

