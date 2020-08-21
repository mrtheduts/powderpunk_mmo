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
#ifndef TELNET_CONNECTION_H
#define TELNET_CONNECTION_H

#include <string>

#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>

#include <telnetpp/core.hpp>
#include <telnetpp/session.hpp>

using boost::asio::ip::tcp;

class TelnetConnection : public boost::enable_shared_from_this<TelnetConnection> {

    public:
        typedef boost::shared_ptr<TelnetConnection> Ptr;
        static Ptr CreatePtr(boost::asio::io_context& io_context);
        ~TelnetConnection();

        tcp::socket& GetSocket();
        void Start();

    private:
        TelnetConnection(boost::asio::io_context& io_context);

        void Send(std::string message);
        void WriteToClient(telnetpp::bytes message);
        void HandleWrite(const boost::system::error_code& /*error*/, size_t /*bytes_transf*/);

        // telnetpp::bytes ReadFromClient();
        // void HandleRead(const boost::system::error_code& [>error<]);

        telnetpp::session telnet_session_;
        tcp::socket socket_;
};

#endif
