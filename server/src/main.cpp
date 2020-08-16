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

#include <iostream>

#include <boost/thread.hpp>

#include <DebugTools/assert_debug_print.h>
#include <TelnetInterface/telnet_interface.h>

int main() {

    std::cout << "It compiled!" << std::endl;

    DEBUG("Debug mode activated.");

    try {

        boost::asio::io_context io_context;

        TelnetServer telnet_server(io_context);
        boost::thread t_telnet_server(&TelnetServer::StartAccept, &telnet_server);
        t_telnet_server.join();

        io_context.run();
    }
    catch(std::exception& e) {

        std::cerr << e.what() << std::endl;
    }

    return 0;
}

