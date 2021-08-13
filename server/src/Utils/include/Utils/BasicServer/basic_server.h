/*
 *  Powderpunk Server - A Pretentious Project
 *
 *  Author: Eduardo "mrtheduts" Souza
 *          eduardomoreirafds@gmail.com
 *
 *  Date:   01 Aug 2021
 *
 *  (C) Copyright 2020 Eduardo "mrtheduts" Souza All Rights Reserved
 *
 */

#ifndef BASIC_SERVER_H
#define BASIC_SERVER_H

#include <DataStructures/user_command.h>
#include <Utils/TSStructures/ts_queue.h>

#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/fiber/condition_variable.hpp>
#include <boost/thread.hpp>
#include <boost/thread/pthread/condition_variable_fwd.hpp>

typedef boost::shared_ptr<boost::thread> spThread;

/*
 * Class template for creating connection servers.
 * It handles income connections through IO ports and creates them as classes.
 */
template <class T>
class BasicServer : public boost::enable_shared_from_this<BasicServer<T>> {
 public:
  /*
   * Server main loop.
   */
  virtual void start() = 0;

  unsigned int id;

  /* Queue to store new UserCommands to be read by GameServer */
  TSQueue<spUserCommand> q_usr_cmds_;
  boost::fibers::mutex q_usr_cmds_m_f_;

 protected:
  BasicServer(unsigned int id) : id{id} {};
  ~BasicServer(){};

  /*
   * Thread function to use as fibers to start connection authentication process
   * and message sending loop.
   */
  virtual void connAuthAndSendFibers() = 0;

  /*
   * Thread function to use as fibers to start connection reading loop.
   */
  virtual void readConnMessagesFibers() = 0;

  /*
   * Fiber function loop to send incoming messages to GameServer.
   *
   * @param connection BasicConnection extended class to get new message and
   * push them as UserCommand to the GameServer through q_user_cmds_ queue.
   */
  virtual void sendNewMsgsToGameServer(T connection) = 0;

  /* Boost IO context handler for thread-safe operations within it. */
  boost::asio::io_context io_context_;

  /* Thread of connAuthAndSendFibers() */
  spThread t_conn_auth_and_send_fibers_;

  /* Thread of readConnMessagesFibers() */
  spThread t_conn_read_messages_fibers_;

  /* Responsible to alert of new conn in auth_and_send_ and read_messages_ */
  boost::fibers::condition_variable cv_new_conns_;

  /* Queue to store new connections waiting to have their auth process and
   * message sending loop to be started */
  TSQueue<T> q_auth_and_send_;
  boost::fibers::mutex q_auth_and_send_f_m_;

  /* Queue to store new connections waiting to have their auth process and
   * message sending loop to be started */
  TSQueue<T> q_read_messages_;
  boost::fibers::mutex q_read_messages_f_m_;
};

#endif

