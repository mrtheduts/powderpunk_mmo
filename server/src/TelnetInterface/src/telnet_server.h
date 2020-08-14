/*
 *  Powderpunk Server - A Pretentious Project
 *
 *  Author: Eduardo "mrtheduts" Souza
 *          eduardomoreirafds@gmail.com
 *
 *  Date:   24 May 2020
 *
 *  (C) Copyright 2020 Eduardo "mrtheduts" Souza All Rights Reserved
 *
 */
#ifndef TELNET_SERVER_H
#define TELNET_SERVER_H

#define PORT_NUMBER 22222

#include <boost/asio.hpp>

#include "telnet_connection.h"

using boost::asio::ip::tcp;

class TelnetServer {

    public:
        TelnetServer(boost::asio::io_context& io_context);

    private:
        void StartAccept();
        void HandleAccept(TelnetConnection::Ptr new_connection, const boost::system::error_code& error);

        boost::asio::io_context& io_context_;
        tcp::acceptor acceptor_;
};

#endif
