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

#ifdef __unix__
    #include <sys/socket.h>
    #include <arpa/inet.h>
#elif _Win32
    #include <winsock2.h>
#endif

#include <DebugTools/assert_debug_print.h>

#include <iostream>

using namespace std;

int main() {

    cout << "It compiled!" << endl;

    DEBUG("Debug mode activated.");

    return 0;
}

