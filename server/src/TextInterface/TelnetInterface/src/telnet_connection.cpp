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

#include "telnet_connection.hpp"

// C++ Headers
#include <queue>
#include <string>
#include <utility>

// Src Headers
#include <LoginManager/login_manager.hpp>

// External Headers
#include <boost/chrono/duration.hpp>
#include <boost/fiber/operations.hpp>
#include <boost/log/trivial.hpp>
#include <boost/thread/lock_types.hpp>

TelnetConnection::TelnetConnection(const boost::asio::io_context& io_context,
                                   boost::asio::ip::tcp::socket socket,
                                   unsigned long int id,
                                   unsigned long int server_id)
    : BasicConnection(io_context, std::move(socket), id, server_id) {
  logger_ = Logger::getLogger("TelnetConnection", id, server_id);
}

TelnetConnection::~TelnetConnection() {}

void TelnetConnection::startReceive() {
  logger_->debug("Started receive loop for %s", getConnIP().c_str());

  try {
    setupOptions();

    addToSendQueue(WELCOME_BANNER);
    authenticate();

    if (authenticated_) {
      logger_->info("Authenticated as \"%s\"", user_->username.c_str());
      addToSendQueue("Welcome, " + user_name_ + "!\n");
    } else {
      send("User already logged. Bye!\n");
      disconnect();
    }
  } catch (boost::system::system_error error) {
    if (error.code() == boost::asio::error::eof) {
      logger_->warning("Ending connection");
      return;
    }
  }
}

void TelnetConnection::startSend() {
  logger_->debug("Started send loop for %s", getConnIP().c_str());

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

      // TODO(mrtheduts): make proper msg preparation
      /* msg += PROMPT; */

      send(msg);
    }
  } catch (boost::system::system_error error) {
    if (error.code() == boost::asio::error::eof) {
      logger_->warning("Ending connection");
      return;
    }
  }
}

bool TelnetConnection::authenticate() {
  std::string username = readPreAuth();
  user_name_.erase(std::remove(user_name_.begin(), user_name_.end(), 10),
                   user_name_.end());
  user_name_.erase(std::remove(user_name_.begin(), user_name_.end(), 13),
                   user_name_.end());

  spLoginManager login_manager = LoginManager::getLoginManager();
  user_ = login_manager->authenticateUser(username);
  authenticated_ = user_ != NULL;

  return authenticated_;
}

void TelnetConnection::activateNoEcho() {
  logger_->debug("Activate No Echo");
  t_echo_server_.activate([this](telnetpp::element elem) { write(elem); });
}

void TelnetConnection::deactivateNoEcho() {
  logger_->debug("Deactivate No Echo");
  t_echo_server_.deactivate([this](telnetpp::element elem) { write(elem); });
}

void TelnetConnection::setupOptions() {
  t_naws_client_.on_window_size_changed.connect(
      [this](const int& width, const int& height, auto&& /*continuation*/) {
        logger_->debug("(W,H) = (%d,%d)", width, height);
      });

  t_termtype_client_.on_state_changed.connect([this](auto&& continuation) {
    if (t_termtype_client_.active()) {
      t_termtype_client_.request_terminal_type(continuation);
    }
  });

  t_termtype_client_.on_terminal_type.connect(
      [this](auto&& type, auto&& /*continuation*/) {
        logger_->debug("Terminal type: %s",
                       std::string(type.begin(), type.end()).c_str());
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
    logger_->error("Bytes transferred: %d. Error: %s", bytes_transf,
                   error.message().c_str());
  }
}

std::string TelnetConnection::readPreAuth() {
  return read(cv_pre_auth_received_msgs_);
}

std::string TelnetConnection::read() { return read(received_msgs_.q_f_cv); }

std::string TelnetConnection::read(boost::fibers::condition_variable& cv) {
  std::string message = "";

  while (message.empty() || message.back() != '\n') {
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

  message.erase(std::remove(message.begin(), message.end(), '\r'),
                message.end());
  message.erase(std::remove(message.begin(), message.end(), '\n'),
                message.end());

  return message;
}

void TelnetConnection::receive() {
  socket_.async_receive(
      boost::asio::buffer(input_buffer_, INPUT_BUFFER_SIZE),

      [this](const boost::system::error_code& error, size_t num_recv_bytes) {
        if (error == boost::asio::error::eof) {
          logger_->info("Closed by client");
          disconnect();
        } else {
          telnet_session_.receive(
              telnetpp::bytes{input_buffer_, num_recv_bytes},
              [this](telnetpp::bytes data,
                     std::function<void(telnetpp::bytes)> const& /*send*/) {
                std::string msg(data.begin(), data.end());

                received_msgs_.push(msg);

                // TODO(mrtheduts): Is it guaranteed to have a '\n' at the end
                // of a message?
                if (msg.back() == '\n') {
                  logger_->debug("Received message of size %d", msg.size());
                  if (authenticated_) {
                    received_msgs_.q_f_cv
                        .notify_one();  // Notifies TelnetServer of new message
                  } else {
                    cv_pre_auth_received_msgs_
                        .notify_one();  // Notifies TelnetServer of new pre-auth
                                        // message
                  }
                }
              },
              [this](telnetpp::bytes data) { rawWrite(data); });

          receive();
        }
      });
}
