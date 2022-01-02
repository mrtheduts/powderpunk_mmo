/*
 *  Powderpunk Server - A Pretentious Project
 *
 *  Author: Eduardo "mrtheduts" Souza
 *          eduardomoreirafds@gmail.com
 *
 *  Date:   12 Feb 2021
 *
 *  (C) Copyright 2020 Eduardo "mrtheduts" Souza All Rights Reserved
 *
 */

#ifndef BASIC_CONNECTION_H
#define BASIC_CONNECTION_H

#include <Utils/TSStructures/ts_queue.h>

#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/fiber/condition_variable.hpp>
#include <boost/lexical_cast.hpp>

template <class T>
class BasicConnection
    : public boost::enable_shared_from_this<BasicConnection<T>> {
 public:
  BasicConnection(boost::asio::io_context& io_context,
                  boost::asio::ip::tcp::socket socket, unsigned int id,
                  unsigned int server_id)
      : id{id},
        server_id{server_id},
        io_context_(io_context),
        socket_(std::move(socket)) {}

  virtual ~BasicConnection(){};

  void disconnect() {
    boost::asio::post(io_context_, [this] {
      socket_.cancel();
      socket_.close();
    });
  }

  bool isConnected() { return socket_.is_open(); }
  std::string getConnIP() {
    return boost::lexical_cast<std::string>(socket_.remote_endpoint());
  }

  unsigned int id;
  unsigned int server_id;

 private:
  virtual void startReceive() = 0;
  virtual void startSend() = 0;
  virtual T read() = 0;
  virtual void receive() = 0;
  virtual void send(const T& /* message */) = 0;

  virtual bool authenticate() = 0;

 protected:
  boost::asio::io_context& io_context_;
  boost::asio::ip::tcp::socket socket_;

  bool authenticated_;

  TSQueue<T> received_msgs_;
  boost::fibers::mutex received_msgs_f_m_;

  TSQueue<T> msgs_to_send_;
  boost::fibers::mutex msgs_to_send_f_m_;
};

#endif

