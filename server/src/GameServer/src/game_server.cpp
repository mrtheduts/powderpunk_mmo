/*
 *  Powderpunk Server - A Pretentious Project
 *
 *  Author: Eduardo "mrtheduts" Souza
 *          eduardomoreirafds@gmail.com
 *
 *  Date:   14 Mar 2021
 *
 *  (C) Copyright 2021 Eduardo "mrtheduts" Souza All Rights Reserved
 *
 */

#include "game_server.h"

#include <boost/log/trivial.hpp>

GameServer::GameServer(const unsigned int id) : id{id} {}
GameServer::~GameServer() {}

void GameServer::start() {
  t_read_telnet_usr_cmds_ = boost::make_shared<boost::thread>(
      &GameServer::createReadTelnetUsrCmdsFiber, this);
}

void GameServer::addTelnetServer(spTelnetServer telnet_server) {
  new_telnet_servers_.push(telnet_server);
  new_telnet_servers_.q_f_cv.notify_one();
  BOOST_LOG_TRIVIAL(info) << "[GameServer] Adding new TelnetServer "
                          << telnet_server->id << " to queue of size "
                          << new_telnet_servers_.size();
}

void GameServer::createReadTelnetUsrCmdsFiber() {
  while (true) {
    BOOST_LOG_TRIVIAL(info) << "[GameServer] Waiting for new TelnetServer ";

    {
      std::unique_lock<boost::fibers::mutex> lock_new_telnet(
          new_telnet_servers_m_f_);
      new_telnet_servers_.q_f_cv.wait(
          lock_new_telnet, [this]() { return new_telnet_servers_.size(); });
    }

    std::queue<spTelnetServer> telnet_servers = new_telnet_servers_.popAll();

    BOOST_LOG_TRIVIAL(info)
        << "[GameServer] Adding " << new_telnet_servers_.size()
        << " new TelnetServers";

    while (!telnet_servers.empty()) {
      BOOST_LOG_TRIVIAL(info) << "[GameServer] Entering while loop";
      spTelnetServer new_telnet_server = telnet_servers.front();
      telnet_servers.pop();

      BOOST_LOG_TRIVIAL(info)
          << "[GameServer] Adding TelnetServer [" << new_telnet_server->id
          << "] to telnet servers map";
      telnet_servers_.put(new_telnet_server->id, new_telnet_server);

      BOOST_LOG_TRIVIAL(info) << "[GameServer] Fiber creation of TelnetServer ["
                              << new_telnet_server->id << "]";
      // TODO: store these fibers somewhere
      boost::make_shared<boost::fibers::fiber>(&GameServer::readTelnetUsrCmds,
                                               this, new_telnet_server)
          ->detach();
      BOOST_LOG_TRIVIAL(info)
          << "[GameServer] Finished fiber creation of TelnetServer ["
          << new_telnet_server->id << "]";
    }
  }
}

void GameServer::readTelnetUsrCmds(spTelnetServer telnet_server) {
  while (true) {
    BOOST_LOG_TRIVIAL(info) << "[GameServer] Starting TelnetServer "
                            << telnet_server->id << " read loop";
    {
      std::unique_lock<boost::fibers::mutex> lock_new_usr_cmds(
          telnet_server->q_usr_cmds_m_f_);
      telnet_server->q_usr_cmds_.q_f_cv.wait(
          lock_new_usr_cmds,
          [telnet_server]() { return !telnet_server->q_usr_cmds_.isEmpty(); });
    }

    while (!telnet_server->q_usr_cmds_.isEmpty()) {
      BOOST_LOG_TRIVIAL(info)
          << "[GameServer] Received new UserCommands from TelnetServer "
          << telnet_server->id;
      incoming_usr_cmds_.pushAll(telnet_server->q_usr_cmds_.popAll());
    }
  }
}

