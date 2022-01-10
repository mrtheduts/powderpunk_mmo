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

// External Headers
#include <boost/log/trivial.hpp>
#include <boost/smart_ptr/make_shared_object.hpp>

GameServer::GameServer(const unsigned int id) : id{id} {
  logger_ = Logger::getLogger("GameServer", id);
}
GameServer::~GameServer() {}

void GameServer::start() {
  t_read_telnet_usr_cmds_ = boost::make_shared<boost::thread>(
      &GameServer::createReadTelnetUsrCmdsFiber, this);
  t_process_usr_cmds_ =
      boost::make_shared<boost::thread>(&GameServer::processUsrCmds, this);
}

void GameServer::addTelnetServer(spTelnetServer telnet_server) {
  logger_->info("Adding new TelnetServer(%d) to queue", telnet_server->id);

  new_telnet_servers_.push(telnet_server);
  new_telnet_servers_.q_f_cv.notify_one();
}

void GameServer::createReadTelnetUsrCmdsFiber() {
  while (true) {
    logger_->debug("Waiting for new TelnetServer");

    {
      std::unique_lock<boost::fibers::mutex> lock_new_telnet(
          new_telnet_servers_m_f_);
      new_telnet_servers_.q_f_cv.wait(
          lock_new_telnet, [this]() { return new_telnet_servers_.size(); });
    }

    std::queue<spTelnetServer> telnet_servers = new_telnet_servers_.popAll();

    while (!telnet_servers.empty()) {
      spTelnetServer new_telnet_server = telnet_servers.front();
      telnet_servers.pop();

      logger_->info("Adding new TelnetServer (%d) to the GameServer",
                    new_telnet_server->id);
      telnet_servers_.put(new_telnet_server->id, new_telnet_server);

      logger_->debug("Starte fiber creation of TelnetServer [%d]",
                     new_telnet_server->id);
      // TODO: store these fibers somewhere
      boost::make_shared<boost::fibers::fiber>(&GameServer::readTelnetUsrCmds,
                                               this, new_telnet_server)
          ->detach();
      logger_->debug("Finished fiber creation of TelnetServer [%d]",
                     new_telnet_server->id);
    }
  }
}

void GameServer::readTelnetUsrCmds(spTelnetServer telnet_server) {
  logger_->info("Starting TelnetServer [%d] read loop", telnet_server->id);
  while (true) {
    {
      std::unique_lock<boost::fibers::mutex> lock_new_usr_cmds(
          telnet_server->q_usr_cmds_m_f_);
      telnet_server->q_usr_cmds_.q_f_cv.wait(
          lock_new_usr_cmds,
          [telnet_server]() { return !telnet_server->q_usr_cmds_.isEmpty(); });
    }

    bool has_new_usr_cmds = !telnet_server->q_usr_cmds_.isEmpty();
    {
      std::unique_lock<boost::mutex> lock_new_usr_cmds(q_incoming_usr_cmds_m_);
      while (!telnet_server->q_usr_cmds_.isEmpty()) {
        logger_->debug("Received new UserCommands from TelnetServer [%d]",
                       telnet_server->id);
        q_incoming_usr_cmds_.pushAll(telnet_server->q_usr_cmds_.popAll());
      }
    }
    if (has_new_usr_cmds) {
      q_incoming_usr_cmds_.q_cv.notify_one();
    }
  }
}

void GameServer::processUsrCmds() {
  while (true) {
    {
      boost::unique_lock<boost::mutex> lock_new_usr_cmds(
          q_incoming_usr_cmds_m_);
      q_incoming_usr_cmds_.q_cv.wait(lock_new_usr_cmds, [this]() {
        return !this->q_incoming_usr_cmds_.isEmpty();
      });

      while (!q_incoming_usr_cmds_.isEmpty()) {
        logger_->debug("Cmd: %s",
                       q_incoming_usr_cmds_.pop()->toString().c_str());
      }
    }
  }
}
