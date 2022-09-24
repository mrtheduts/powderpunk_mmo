/*
 *  Powderpunk Server - A Pretentious Project
 *
 *  Author: Eduardo "mrtheduts" Souza
 *          eduardomoreirafds@gmail.com
 *
 *  Date:   14 Mar 2021
 *
 *  (C) Copyright 2021 Eduardo "mrtheduts" Souza All Rights Reserved
 *
 */

#ifndef GAME_SERVER_H
#define GAME_SERVER_H

// Src Headers
#include <DataStructures/user_command.h>
#include <Logger/logger.h>
#include <TelnetInterface/telnet_interface.h>
#include <Utils/TSStructures/ts_map.h>
#include <Utils/TSStructures/ts_queue.h>

// External Headers
#include <boost/smart_ptr/shared_ptr.hpp>

class GameServer {
 public:
  GameServer(const unsigned int id);
  ~GameServer();

  void start();
  void addTelnetServer(spTelnetServer telnet_server);

  const unsigned int id;

 private:
  void createReadTelnetUsrCmdsFiber();
  void readTelnetUsrCmds(spTelnetServer telnet_Server);

  void processUsrCmds();
  void runUsrCmds(spUserCommand cmd);

  void sendEventToEveryone();

  /* Commands */
  void say(spUserCommand cmd);

  /* Logger object */
  spLogger logger_;

  TSQueue<spTelnetServer> new_telnet_servers_;

  /* Thread-level condition variable */
  boost::fibers::mutex new_telnet_servers_m_f_;
  boost::condition_variable new_telnet_servers_cv_;
  TSMap<unsigned int, spTelnetServer> telnet_servers_;

  boost::mutex q_incoming_usr_cmds_m_;
  TSQueue<spUserCommand> q_incoming_usr_cmds_;

  spThread t_read_telnet_usr_cmds_;
  spThread t_process_usr_cmds_;
};

typedef boost::shared_ptr<GameServer> spGameServer;

#endif
