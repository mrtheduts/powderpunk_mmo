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

#include <DebugTools/assert_debug_print.h>
#include <TelnetInterface/telnet_interface.h>

using namespace std;

int main() {

    cout << "It compiled!" << endl;

    DEBUG("Debug mode activated.");
    DEBUG("Hello.");

    try {

        boost::asio::io_context io_context;
        TelnetServer telnet_server(io_context);
        io_context.run();
    }
    catch(exception& e){

        cerr << e.what() << endl;
    }

    return 0;
}

