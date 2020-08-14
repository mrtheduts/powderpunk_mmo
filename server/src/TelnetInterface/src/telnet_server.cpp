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

#include <boost/bind.hpp>

TelnetServer::TelnetServer(boost::asio::io_context& io_context)
    : io_context_(io_context),
      acceptor_(io_context, tcp::endpoint(tcp::v4(), PORT_NUMBER)){

    this->StartAccept();
}

void TelnetServer::StartAccept() {

    TelnetConnection::Ptr new_connection;
    new_connection = TelnetConnection::CreatePtr(this->io_context_);

    this->acceptor_.async_accept(new_connection->GetSocket(),
                                 boost::bind(&TelnetServer::HandleAccept, 
                                             this, 
                                             new_connection, 
                                             boost::asio::placeholders::error
                                             )
                                );
}

void TelnetServer::HandleAccept(TelnetConnection::Ptr new_connection, const boost::system::error_code& error){

    if(!error){
        new_connection->Start();
    }

    this->StartAccept();
}
