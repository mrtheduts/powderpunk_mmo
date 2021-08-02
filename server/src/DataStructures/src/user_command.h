/*
 *  Powderpunk Server - A Pretentious Project
 *
 *  Author: Eduardo "mrtheduts" Souza
 *          eduardomoreirafds@gmail.com
 *
 *  Date:   09 Feb 2021
 *
 *  (C) Copyright 2021 Eduardo "mrtheduts" Souza All Rights Reserved
 *
 */
#ifndef USER_COMMAND_H
#define USER_COMMAND_H

#include <boost/smart_ptr/shared_ptr.hpp>
#include <string>
#include <vector>

class UserCommand {
 public:
  UserCommand(unsigned int server_id, unsigned int conn_id, std::string mod,
              std::string cmd, std::vector<std::string> args,
              std::vector<std::string> targets);
  ~UserCommand();

  const unsigned int server_id_;
  const unsigned int conn_id_;

  const std::string mod_;
  const std::string cmd_;

  const std::vector<std::string> args_;
  const std::vector<std::string> targets_;
};

typedef boost::shared_ptr<UserCommand> spUserCommand;

#endif

