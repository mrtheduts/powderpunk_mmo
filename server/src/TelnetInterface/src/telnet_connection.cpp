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

    Send("Hello, it worked!\n");
    Send("Well, that's it. Bye!\n");
}

TelnetConnection::TelnetConnection(boost::asio::io_context& io_context)
    : telnet_session_(),
      socket_(io_context) {
}

TelnetConnection::~TelnetConnection() {
}

void TelnetConnection::Send(std::string message) {

    std::vector<uint8_t> tmp_vector(message.begin(), message.end());
    telnetpp::element message_bytes(tmp_vector);

    telnet_session_.send(
        message_bytes,
        [this](telnetpp::bytes data){
            WriteToClient(data);
        }
);
}

void TelnetConnection::WriteToClient(telnetpp::bytes data) {

    boost::asio::async_write(socket_,
                             boost::asio::buffer(data.data(), data.size_bytes()),
                             boost::bind(&TelnetConnection::HandleWrite,
                                         shared_from_this(),
                                         boost::asio::placeholders::error,
                                         boost::asio::placeholders::bytes_transferred
                                        )
                            );
}

void TelnetConnection::HandleWrite(const boost::system::error_code& /*error*/, size_t /*bytes_transf*/){
}

// telnetpp::bytes TelnetConnection::ReadFromClient() {
// }

// void TelnetConnection::HandleRead(const boost::system::error_code& [>error<]){
// }
