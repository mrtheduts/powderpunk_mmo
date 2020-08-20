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

#include <DebugTools/assert_debug_print.h>

TelnetConnection::Ptr TelnetConnection::CreatePtr(boost::asio::io_context &io_context) {
    return Ptr(new TelnetConnection(io_context));
}

tcp::socket& TelnetConnection::GetSocket() {
    return socket_;
}

void TelnetConnection::Start() {

    DEBUG("Starting TelnetConnection...");

    using telnetpp::literals::operator""_tb;
    telnetpp::byte_storage message("Oi, consigo enviar!\n"_tb);

    WriteToClient(message);
}

TelnetConnection::TelnetConnection(boost::asio::io_context& io_context)
    : telnet_session_(),
      socket_(io_context){
}

void TelnetConnection::WriteToClient(telnetpp::bytes message) {

    boost::asio::async_write(socket_,
                             boost::asio::buffer(message.data(), message.size_bytes()),
                             boost::bind(&TelnetConnection::HandleWrite,
                                         shared_from_this(),
                                         boost::asio::placeholders::error,
                                         boost::asio::placeholders::bytes_transferred
                                        )
                            );

    last_message_ = message;

    while(1);
}

void TelnetConnection::HandleWrite(const boost::system::error_code& /*error*/, size_t /*bytes_transf*/){
}
