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

#include <DebugTools/assert_debug_print.h>
#include <TelnetInterface/telnet_interface.h>

#include <boost/thread.hpp>
#include <iostream>

int main() {
  std::cout << "It compiled!" << std::endl;

  DEBUG("Debug mode activated.\n");

  try {
    TelnetServer telnet_server;
    boost::thread t_telnet_server(&TelnetServer::Start, &telnet_server);

    t_telnet_server.join();

  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}

