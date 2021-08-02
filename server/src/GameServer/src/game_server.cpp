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

#include "game_server.h"

#include <boost/smart_ptr/make_shared_object.hpp>

GameServer::GameServer(const unsigned int id) : id{id} {}
GameServer::~GameServer() {}

void GameServer::start() {}

void GameServer::addTelnetServer(spTelnetServer telnet_server) {
  telnet_servers_.put(telnet_server->id, telnet_server);
}

