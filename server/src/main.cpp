/*
 *  Powderpunk Server - A Pretentious Project
 *
 *  Author: Eduardo "mrtheduts" Souza
 *          eduardomoreirafds@gmail.com
 *
 *  Date:   16 May 2020
 *
 *  (C) Copyright 2020 Eduardo "mrtheduts" Souza All Rights Reserved
 *
 */

// Src Headers
#include <GameServer/game_server.h>
#include <Logger/logger.h>
#include <TelnetInterface/telnet_interface.h>
#include <Utils/DebugTools/assert_debug_print.h>

// External Headers
#include <boost/smart_ptr/make_shared_object.hpp>
#include <boost/thread.hpp>

// C++ Headers
#include <iostream>

int main() {
  try {
    Logger::init();
    spLogger logger = Logger::getLogger("Main Function");
    logger->info("Powderpunk MMO Server started!");

    GameServer game_server(0);
    game_server.start();

    spTelnetServer telnet_server = boost::make_shared<TelnetServer>(0);
    boost::thread t_telnet_server(&TelnetServer::start, telnet_server);

    game_server.addTelnetServer(telnet_server);

    t_telnet_server.join();

  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}

