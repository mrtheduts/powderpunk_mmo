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

#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/fiber/condition_variable.hpp>

#include "ts_queue.h"

template <typename T>
class BasicConnection
    : public boost::enable_shared_from_this<BasicConnection<T>> {
 public:
  BasicConnection(boost::asio::io_context& io_context,
                  boost::asio::ip::tcp::socket socket, unsigned long int id)
      : io_context_(io_context), socket_(std::move(socket)) {
    id_ = id;
  }
  virtual ~BasicConnection(){};

  void Disconnect() {
    boost::asio::post(io_context_, [this] {
      socket_.cancel();
      socket_.close();
    });
  }
  bool IsConnected() { return socket_.is_open(); }
  unsigned int GetId() { return id_; };

 private:
  virtual void StartReceive(){};
  virtual void StartSend(){};
  virtual T Read(){};
  virtual void Receive(){};
  virtual void Send(const T& /* message */){};

  virtual bool Authenticate(){};

 protected:
  boost::asio::io_context& io_context_;
  boost::asio::ip::tcp::socket socket_;

  unsigned long int id_;
  TSQueue<T> received_msgs_;
  boost::fibers::mutex m_new_received_msgs_;
  boost::fibers::condition_variable cv_new_received_msgs_;
  TSQueue<T> msgs_to_send_;
  boost::fibers::mutex m_new_msgs_to_send_;
  boost::fibers::condition_variable cv_new_msgs_to_send_;
};

#endif

