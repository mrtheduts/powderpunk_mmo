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

#include <DataStructures/user_command.h>
#include <TelnetInterface/telnet_interface.h>
#include <Utils/TSStructures/ts_map.h>

#include <boost/smart_ptr/shared_ptr.hpp>

class GameServer {
 public:
  GameServer(const unsigned int id);
  ~GameServer();

  void start();
  void addTelnetServer(spTelnetServer telnet_server);

  const unsigned int id;

 private:
  TSMap<unsigned int, spTelnetServer> telnet_servers_;
};

typedef boost::shared_ptr<GameServer> spGameServer;

#endif
