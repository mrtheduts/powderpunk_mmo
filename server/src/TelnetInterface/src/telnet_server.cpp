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

#include "telnet_server.h"

#include <iostream>

#include <boost/bind.hpp>

#include <DebugTools/assert_debug_print.h>

TelnetServer::TelnetServer(boost::asio::io_context& io_context)
    : io_context_(io_context),
      acceptor_(io_context, tcp::endpoint(tcp::v4(), PORT_NUMBER)){

    // this->StartAccept();
}

void TelnetServer::StartAccept() {

    std::cout << "Entrei em StartAccept" << std::endl;
    TelnetConnection::Ptr new_connection;
    new_connection = TelnetConnection::CreatePtr(io_context_);

    acceptor_.async_accept(new_connection->GetSocket(),
                           boost::bind(&TelnetServer::HandleAccept, 
                                       this, 
                                       new_connection, 
                                       boost::asio::placeholders::error
                                      )
                          );
}

void TelnetServer::HandleAccept(TelnetConnection::Ptr new_connection, const boost::system::error_code& error){

    std::cout << "Entrei em HandleAccept" << std::endl;

    if(!error){

        boost::thread t_new_connection(&TelnetConnection::Start, &(*new_connection));

        curr_connections.push_back(new_connection);
        t_curr_connections.add_thread(&t_new_connection);

        t_new_connection.detach();
    }
    else {

        std::cout << error.message() << std::endl;
    }

    this->StartAccept();
}
