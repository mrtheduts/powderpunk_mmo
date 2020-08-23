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

#include <string>

#include <DebugTools/assert_debug_print.h>

TelnetConnection::Ptr TelnetConnection::CreatePtr(boost::asio::io_context &io_context) {
    return Ptr(new TelnetConnection(io_context));
}

TelnetConnection::~TelnetConnection() {
}

tcp::socket& TelnetConnection::GetSocket() {
    return socket_;
}

void TelnetConnection::Start() {

    DEBUG("Starting TelnetConnection for IP: ");
    DEBUG(this->socket_.remote_endpoint());
    DEBUG("\n");

    while(1){
        std::cout << Receive();
    }
}

TelnetConnection::TelnetConnection(boost::asio::io_context& io_context)
    : telnet_session_(),
      socket_(io_context) {
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

void TelnetConnection::TranslateData(telnetpp::bytes data) {

    last_msg_received_ = new std::string(data.begin(), data.end());
}

std::string TelnetConnection::Receive() {

    telnetpp::byte buffer[INPUT_BUFFER_SIZE];
    size_t num_recv_bytes;

    try {
        num_recv_bytes = ReadFromClient(buffer, sizeof(buffer));
    }
    catch(const boost::system::system_error error) {

        // Tratar erro EOF - significa que conexão foi fechada por cliente
        DEBUG("Peguei um erro aqui: ");
        DEBUG(error.what());
        DEBUG("\n");

        return "";
    }

    telnet_session_.receive(telnetpp::bytes(buffer, num_recv_bytes),
                            [this](telnetpp::bytes data, std::function<void (telnetpp::bytes)> const &/*send*/){
                                TranslateData(data);
                            },
                            [this](telnetpp::bytes data){
                                WriteToClient(data);
                            }
                           );

    std::string message(*last_msg_received_);
    delete(last_msg_received_);
    last_msg_received_ = NULL;

    return message;
}

size_t TelnetConnection::ReadFromClient(telnetpp::byte *buffer, size_t size) {

    boost::system::error_code error;
    size_t num_recv_bytes;

    // boost::asio::async_read(socket_,
                            // boost::asio::buffer(buffer, INPUT_BUFFER_SIZE),
                            // boost::bind(&TelnetConnection::HandleRead,
                                        // shared_from_this(),
                                        // boost::asio::placeholders::error,
                                        // boost::asio::placeholders::bytes_transferred
                                       // )
                           // );
 
    num_recv_bytes = socket_.read_some(boost::asio::buffer(buffer, size), error);

    if(error)
        throw boost::system::system_error(error);

    return num_recv_bytes;
}

// void TelnetConnection::HandleRead(const boost::system::error_code& error, size_t recv_len){

    // if(!error && error != boost::asio::error::message_size) {
        // num_received_bytes_ = recv_len;
    // }
    // else {
        // num_received_bytes_ = 0;
    // }
// }
