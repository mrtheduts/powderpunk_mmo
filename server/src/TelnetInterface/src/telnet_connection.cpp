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

TelnetConnection::Ptr TelnetConnection::CreatePtr(boost::asio::io_context &io_context) {
    return Ptr(new TelnetConnection(io_context));
}

tcp::socket& TelnetConnection::GetSocket() {
    return socket_;
}

void TelnetConnection::Start() {

    message_ = "Hello, friend. How are you?";
    boost::asio::async_write(socket_,
                             boost::asio::buffer(message_),
                             boost::bind(&TelnetConnection::HandleWrite,
                                         shared_from_this(),
                                         boost::asio::placeholders::error,
                                         boost::asio::placeholders::bytes_transferred
                                        )
                            );

}

TelnetConnection::TelnetConnection(boost::asio::io_context& io_context) : socket_(io_context) {
}

void TelnetConnection::HandleWrite(const boost::system::error_code& /*error*/, size_t /*bytes_transf*/){
}
