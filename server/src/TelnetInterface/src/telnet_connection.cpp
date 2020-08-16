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

#include "telnet_connection.h"

#include <iostream>

#include <DebugTools/assert_debug_print.h>

TelnetConnection::Ptr TelnetConnection::CreatePtr(boost::asio::io_context &io_context) {
    return Ptr(new TelnetConnection(io_context));
}

tcp::socket& TelnetConnection::GetSocket() {
    return socket_;
}

void TelnetConnection::Start() {

    DEBUG("Starting TelnetConnection...");

    for (int i = 0; i < 10; ++i) {
        std::ostringstream tmp_str;
        tmp_str << "You are receiving: " << i << std::endl;
        WriteToClient(tmp_str.str());
        tmp_str.clear();
    }
}

TelnetConnection::TelnetConnection(boost::asio::io_context& io_context) : socket_(io_context) {
}

void TelnetConnection::WriteToClient(const std::string message) {

    boost::asio::async_write(socket_,
                             boost::asio::buffer(message),
                             boost::bind(&TelnetConnection::HandleWrite,
                                         shared_from_this(),
                                         boost::asio::placeholders::error,
                                         boost::asio::placeholders::bytes_transferred
                                        )
                            );

    message_ = message;
}

void TelnetConnection::HandleWrite(const boost::system::error_code& /*error*/, size_t /*bytes_transf*/){
}
