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

#include <boost/chrono/duration.hpp>
#include <boost/fiber/operations.hpp>
#include <boost/log/trivial.hpp>
#include <boost/thread/lock_types.hpp>
#include <chrono>
#include <string>

TelnetConnection::TelnetConnection(boost::asio::io_context& io_context,
                                   boost::asio::ip::tcp::socket socket,
                                   unsigned long int id)
    : BasicConnection<std::string>(io_context, std::move(socket), id) {}

TelnetConnection::~TelnetConnection() {}

void TelnetConnection::StartReceive() {
  BOOST_LOG_TRIVIAL(info) << "TC[" << id_ << "] Started receive loop for IP: "
                          << socket_.remote_endpoint();

  try {
    /* boost::this_fiber::sleep_for(std::chrono::seconds(5)); */

    SetupOptions();
    Authenticate();

    while (true) {
      std::string msg = Read();
      BOOST_LOG_TRIVIAL(debug) << "TC[" << id_ << "] msg: " << msg;
      /* boost::this_fiber::yield(); */
      AddToSendQueue(user_name_ + ": " + msg);
    }
  } catch (boost::system::system_error error) {
    if (error.code() == boost::asio::error::eof) {
      BOOST_LOG_TRIVIAL(info)
          << "TC[" << id_ << "] Ending for IP: " << socket_.remote_endpoint();
      return;
    }
  }
}

void TelnetConnection::StartSend() {
  BOOST_LOG_TRIVIAL(info) << "TC[" << id_ << "] Started send loop for IP: "
                          << socket_.remote_endpoint();

  try {
    while (true) {
      std::string msg = "";
      /* boost::this_fiber::sleep_for(std::chrono::seconds(5)); */

      do {
        std::unique_lock<boost::fibers::mutex> lock_new_send_msg(
            m_new_msgs_to_send_);
        cv_new_msgs_to_send_.wait(
            lock_new_send_msg, [this]() { return msgs_to_send_.Size() > 0; });

        msg += msgs_to_send_.Pop();

      } while (!msgs_to_send_.IsEmpty());

      msg += PROMPT;

      Send(msg);
      BOOST_LOG_TRIVIAL(debug)
          << "TC[" << id_ << "] message to be sent: " << msg;
    }
  } catch (boost::system::system_error error) {
    if (error.code() == boost::asio::error::eof) {
      BOOST_LOG_TRIVIAL(info)
          << "TC[" << id_ << "] Ending for IP: " << socket_.remote_endpoint();
      return;
    }
  }
}

bool TelnetConnection::Authenticate() {
  AddToSendQueue(WELCOME_BANNER);

  user_name_ = Read();

  user_name_.erase(std::remove(user_name_.begin(), user_name_.end(), 10),
                   user_name_.end());
  user_name_.erase(std::remove(user_name_.begin(), user_name_.end(), 13),
                   user_name_.end());

  BOOST_LOG_TRIVIAL(debug) << "TC[" << id_ << "] Defined user_name_ as "
                           << user_name_;

  AddToSendQueue("Welcome, " + user_name_ + "!\n");
  return true;
}

void TelnetConnection::ActivateNoEcho() {
  t_echo_server_.activate([this](telnetpp::element elem) { Write(elem); });
}

void TelnetConnection::DeactivateNoEcho() {
  t_echo_server_.deactivate([this](telnetpp::element elem) { Write(elem); });
}

void TelnetConnection::SetupOptions() {
  t_naws_client_.on_window_size_changed.connect(
      [this](const int& width, const int& height, auto&& /*continuation*/) {
        BOOST_LOG_TRIVIAL(debug) << "TC[" << this->id_ << "] (W,H) = "
                                 << "(" << width << "," << height << ")";
      });

  t_termtype_client_.on_state_changed.connect([this](auto&& continuation) {
    if (t_termtype_client_.active()) {
      t_termtype_client_.request_terminal_type(continuation);
    }
  });

  t_termtype_client_.on_terminal_type.connect(
      [this](auto&& type, auto&& /*continuation*/) {
        BOOST_LOG_TRIVIAL(debug)
            << "TC[" << this->id_
            << "] Terminal type: " << std::string(type.begin(), type.end());
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

void TelnetConnection::AddToSendQueue(const std::string& message) {
  msgs_to_send_.Push(message);
  cv_new_msgs_to_send_.notify_one();
}

void TelnetConnection::Send(const std::string& message) {
  std::vector<uint8_t> tmp_vector(message.begin(), message.end());
  telnetpp::element message_bytes(tmp_vector);

  Write(message_bytes);
}

/* Write() -> void
 *
 * Params:
 *    data: telnetpp element to be written to socket
 *
 * Writes data to socket to be sent to client.
 *
 * Required for write_continuation functions.
 */
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
  if (error) {
    BOOST_LOG_TRIVIAL(error)
        << "TC[" << this->id_ << "] Bytes transferred:" << bytes_transf << ". "
        << error.message();
  }
}

std::string TelnetConnection::Read() {
  std::string message = "";

  do {
    std::unique_lock<boost::fibers::mutex> lock_new_rec_msg(
        m_new_received_msgs_);
    cv_new_received_msgs_.wait(lock_new_rec_msg,
                               [this]() { return received_msgs_.Size() > 0; });

    message += received_msgs_.Pop();
  } while (message.back() != '\n' && !received_msgs_.IsEmpty());

  return message;
}

void TelnetConnection::ReadFromClient(telnetpp::bytes data) {
  std::string message(data.begin(), data.end());

  message.erase(std::remove(message.begin(), message.end(), 10), message.end());
  message.erase(std::remove(message.begin(), message.end(), 13), message.end());
  std::cout << message << std::endl;

  AddToSendQueue("Server: " + message + "\n");
}

void TelnetConnection::Receive() {
  socket_.async_receive(
      boost::asio::buffer(input_buffer_, INPUT_BUFFER_SIZE),

      [this](const boost::system::error_code& error, size_t num_recv_bytes) {
        if (error == boost::asio::error::eof) {
          BOOST_LOG_TRIVIAL(info) << "TC[" << id_ << "] Closed by client";
          Disconnect();
        } else {
          telnet_session_.receive(
              telnetpp::bytes{input_buffer_, num_recv_bytes},
              [this](telnetpp::bytes data,
                     std::function<void(telnetpp::bytes)> const& /*send*/) {
                std::string message(data.begin(), data.end());

                received_msgs_.Push(message);

                // Is it guaranteed to have a '\n' at the end of a message?
                if (message.back() == '\n') {
                  cv_new_received_msgs_.notify_one();
                }

                BOOST_LOG_TRIVIAL(debug)
                    << "TC[" << id_ << "] Received message[" << message.size()
                    << "], pushed to received_msgs_";
              },
              [this](telnetpp::bytes data) { RawWrite(data); });

          Receive();
        }
      });
}

