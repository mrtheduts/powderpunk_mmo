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

#include <Utils/DebugTools/assert_debug_print.h>

#include <string>

TelnetConnection::TelnetConnection(boost::asio::io_context& io_context,
                                   boost::asio::ip::tcp::socket socket,
                                   unsigned long int id)
    : BasicConnection<std::string>(io_context, std::move(socket), id) {}

TelnetConnection::~TelnetConnection() {}

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
      DEBUG("Ending TelnetConnection for IP: ");
      DEBUG(this->socket_.remote_endpoint());
      DEBUG("\n");
      return;
    }
  }
}

void TelnetConnection::ActivateNoEcho() {
  t_echo_server_.activate([this](telnetpp::element elem) { Write(elem); });
}

void TelnetConnection::DeactivateNoEcho() {
  t_echo_server_.deactivate([this](telnetpp::element elem) { Write(elem); });
}

void TelnetConnection::SetupOptions() {
  t_naws_client_.on_window_size_changed.connect(
      [/*this*/](const int& width, const int& height, auto&& /*continuation*/) {
        std::cout << "(W,H) = (" << width << "," << height << ")" << std::endl;
      });

  t_termtype_client_.on_state_changed.connect([this](auto&& continuation) {
    if (t_termtype_client_.active()) {
      t_termtype_client_.request_terminal_type(continuation);
    }
  });

  t_termtype_client_.on_terminal_type.connect(
      [/*this*/](auto&& type, auto&& /*continuation*/) {
        std::cout << "Terminal type: " << std::string(type.begin(), type.end())
                  << std::endl;
      });

  telnet_session_.install(t_echo_server_);
  telnet_session_.install(t_naws_client_);
  telnet_session_.install(t_termtype_client_);

  auto const& write_continuation = [this](telnetpp::element const& elem) {
    Write(elem);
  };

  t_naws_client_.activate(write_continuation);
  t_termtype_client_.activate(write_continuation);
}

void TelnetConnection::Send(const std::string& message) {
  std::vector<uint8_t> tmp_vector(message.begin(), message.end());
  telnetpp::element message_bytes(tmp_vector);

  Write(message_bytes);
}

void TelnetConnection::Write(telnetpp::element const& data) {
  telnet_session_.send(data, [this](telnetpp::bytes data) { RawWrite(data); });
}

void TelnetConnection::RawWrite(telnetpp::bytes data) {
  boost::asio::async_write(
      socket_, boost::asio::buffer(data.data(), data.size_bytes()),
      boost::bind(&TelnetConnection::HandleWrite, this,
                  boost::asio::placeholders::error,
                  boost::asio::placeholders::bytes_transferred));
}

void TelnetConnection::HandleWrite(const boost::system::error_code& error,
                                   size_t bytes_transf) {
  std::cout << "Write: [" << error.message() << " - " << bytes_transf << "]"
            << std::endl;
}

void TelnetConnection::ReadFromClient(telnetpp::bytes data) {
  std::string message(data.begin(), data.end());

  message.erase(std::remove(message.begin(), message.end(), 10), message.end());
  message.erase(std::remove(message.begin(), message.end(), 13), message.end());
  std::cout << message << std::endl;

  Send("Server: " + message + "\n");
}

void TelnetConnection::Receive() {
  boost::system::error_code error;
  telnetpp::byte buffer[INPUT_BUFFER_SIZE];
  size_t num_recv_bytes;

  num_recv_bytes =
      socket_.read_some(boost::asio::buffer(buffer, INPUT_BUFFER_SIZE), error);

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

