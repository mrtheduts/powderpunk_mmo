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

using boost::asio::ip::tcp;

class TelnetConnection : public boost::enable_shared_from_this<TelnetConnection> {

    public:
        typedef boost::shared_ptr<TelnetConnection> Ptr;
        static Ptr CreatePtr(boost::asio::io_context& io_context);

        tcp::socket& GetSocket();
        void Start();

    private:
        TelnetConnection(boost::asio::io_context& io_context);
        void HandleWrite(const boost::system::error_code& /*error*/, size_t /*bytes_transf*/);

        tcp::socket socket_;
        std::string message_;
};

#endif
