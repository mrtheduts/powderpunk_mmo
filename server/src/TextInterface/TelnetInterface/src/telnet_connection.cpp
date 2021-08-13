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
                                   unsigned long int id,
                                   unsigned long int server_id)
    : BasicConnection(io_context, std::move(socket), id, server_id) {}

TelnetConnection::~TelnetConnection() {}

void TelnetConnection::startReceive() {
  BOOST_LOG_TRIVIAL(info) << "TC[" << id << "] Started receive loop for IP: "
                          << socket_.remote_endpoint();

  try {
    setupOptions();
    authenticate();
  } catch (boost::system::system_error error) {
    if (error.code() == boost::asio::error::eof) {
      BOOST_LOG_TRIVIAL(info)
          << "TC[" << id << "] Ending for IP: " << socket_.remote_endpoint();
      return;
    }
  }
}

void TelnetConnection::startSend() {
  BOOST_LOG_TRIVIAL(info) << "TC[" << id << "] Started send loop for IP: "
                          << socket_.remote_endpoint();

  try {
    while (true) {
      std::string msg = "";

      {
        std::unique_lock<boost::fibers::mutex> lock_new_send_msg(
            msgs_to_send_f_m_);
        msgs_to_send_.q_f_cv.wait(
            lock_new_send_msg, [this]() { return !msgs_to_send_.fIsEmpty(); });
      }

      std::queue<std::string> msgs = msgs_to_send_.fPopAll();

      while (!msgs.empty()) {
        msg += msgs.front();
        msgs.pop();
      }

      /* msg += PROMPT; */

      send(msg);
      BOOST_LOG_TRIVIAL(debug)
          << "TC[" << id << "] message to be sent: " << msg;
    }
  } catch (boost::system::system_error error) {
    if (error.code() == boost::asio::error::eof) {
      BOOST_LOG_TRIVIAL(info)
          << "TC[" << id << "] Ending for IP: " << socket_.remote_endpoint();
      return;
    }
  }
}

bool TelnetConnection::authenticate() {
  addToSendQueue(WELCOME_BANNER);

  user_name_ = readPreAuth();

  user_name_.erase(std::remove(user_name_.begin(), user_name_.end(), 10),
                   user_name_.end());
  user_name_.erase(std::remove(user_name_.begin(), user_name_.end(), 13),
                   user_name_.end());

  authenticated_ = true;

  BOOST_LOG_TRIVIAL(debug) << "TC[" << id << "] Defined user_name_ as "
                           << user_name_;

  addToSendQueue("Welcome, " + user_name_ + "!\n");

  return true;
}

void TelnetConnection::activateNoEcho() {
  t_echo_server_.activate([this](telnetpp::element elem) { write(elem); });
}

void TelnetConnection::deactivateNoEcho() {
  t_echo_server_.deactivate([this](telnetpp::element elem) { write(elem); });
}

void TelnetConnection::setupOptions() {
  t_naws_client_.on_window_size_changed.connect(
      [this](const int& width, const int& height, auto&& /*continuation*/) {
        BOOST_LOG_TRIVIAL(debug) << "TC[" << this->id << "] (W,H) = "
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
            << "TC[" << this->id
            << "] Terminal type: " << std::string(type.begin(), type.end());
      });

  telnet_session_.install(t_echo_server_);
  telnet_session_.install(t_naws_client_);
  telnet_session_.install(t_termtype_client_);

  auto const& write_continuation = [this](telnetpp::element const& elem) {
    write(elem);
  };

  t_naws_client_.activate(write_continuation);
  t_termtype_client_.activate(write_continuation);
}

void TelnetConnection::addToSendQueue(const std::string& message) {
  msgs_to_send_.push(message);
  msgs_to_send_.q_cv.notify_one();
}

void TelnetConnection::send(const std::string& message) {
  std::vector<uint8_t> tmp_vector(message.begin(), message.end());
  telnetpp::element message_bytes(tmp_vector);

  write(message_bytes);
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
void TelnetConnection::write(telnetpp::element const& data) {
  telnet_session_.send(data, [this](telnetpp::bytes data) { rawWrite(data); });
}

void TelnetConnection::rawWrite(telnetpp::bytes data) {
  boost::asio::async_write(
      socket_, boost::asio::buffer(data.data(), data.size_bytes()),
      boost::bind(&TelnetConnection::handleWrite, this,
                  boost::asio::placeholders::error,
                  boost::asio::placeholders::bytes_transferred));
}

void TelnetConnection::handleWrite(const boost::system::error_code& error,
                                   size_t bytes_transf) {
  if (error) {
    BOOST_LOG_TRIVIAL(error)
        << "TC[" << this->id << "] Bytes transferred:" << bytes_transf << ". "
        << error.message();
  }
}

std::string TelnetConnection::readPreAuth() {
  return read(cv_pre_auth_received_msgs_);
}

std::string TelnetConnection::read() { return read(received_msgs_.q_f_cv); }

std::string TelnetConnection::read(boost::fibers::condition_variable& cv) {
  std::string message = "";

  while (message.back() != '\n') {
    {
      std::unique_lock<boost::fibers::mutex> lock_new_rec_msg(
          received_msgs_f_m_);
      cv.wait(lock_new_rec_msg,
              [this]() { return !received_msgs_.fIsEmpty(); });
    }

    std::queue<std::string> msgs = received_msgs_.fPopAll();

    while (!msgs.empty()) {
      message += msgs.front();
      msgs.pop();
    }
  }

  return message;
}

void TelnetConnection::readFromClient(telnetpp::bytes data) {
  std::string message(data.begin(), data.end());

  message.erase(std::remove(message.begin(), message.end(), 10), message.end());
  message.erase(std::remove(message.begin(), message.end(), 13), message.end());
  std::cout << message << std::endl;

  addToSendQueue("Server: " + message + "\n");
}

void TelnetConnection::receive() {
  socket_.async_receive(
      boost::asio::buffer(input_buffer_, INPUT_BUFFER_SIZE),

      [this](const boost::system::error_code& error, size_t num_recv_bytes) {
        if (error == boost::asio::error::eof) {
          BOOST_LOG_TRIVIAL(info) << "TC[" << id << "] Closed by client";
          disconnect();
        } else {
          telnet_session_.receive(
              telnetpp::bytes{input_buffer_, num_recv_bytes},
              [this](telnetpp::bytes data,
                     std::function<void(telnetpp::bytes)> const& /*send*/) {
                std::string message(data.begin(), data.end());

                received_msgs_.push(message);

                // TODO: Is it guaranteed to have a '\n' at the end of a
                // message?
                if (message.back() == '\n') {
                  if (authenticated_) {
                    received_msgs_.q_f_cv
                        .notify_one();  // Notifies TelnetServer of new message
                    BOOST_LOG_TRIVIAL(debug)
                        << "TC[" << id << "] Received message["
                        << message.size() << "], pushed to received_msgs_";
                  } else {
                    cv_pre_auth_received_msgs_
                        .notify_one();  // Notifies TelnetServer of new pre-auth
                                        // message
                    BOOST_LOG_TRIVIAL(debug)
                        << "TC[" << id << "] Received message["
                        << message.size()
                        << "], pushed to pre_auth_received_msgs_";
                  }
                }
              },
              [this](telnetpp::bytes data) { rawWrite(data); });

          receive();
        }
      });
}

