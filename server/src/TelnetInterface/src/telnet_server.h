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

#include <vector>

#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "telnet_connection.h"

using boost::asio::ip::tcp;

class TelnetServer {

    public:
        TelnetServer(boost::asio::io_context& io_context);
        void StartAccept();

    private:
        void HandleAccept(TelnetConnection::Ptr new_connection, const boost::system::error_code& error);

        boost::asio::io_context& io_context_;
        tcp::acceptor acceptor_;

        boost::thread_group t_curr_connections;
        std::vector<TelnetConnection::Ptr> curr_connections; 
};

#endif
