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

// External Headers
#include <boost/smart_ptr/shared_ptr.hpp>

// C++ Headers
#include <string>
#include <vector>

class UserCommand {
 public:
  UserCommand(unsigned int server_id, unsigned int conn_id,
              unsigned int user_id, std::string mod, std::string cmd,
              std::vector<std::string> args, std::vector<std::string> targets);
  ~UserCommand();

  const unsigned int server_id;
  const unsigned int conn_id;
  const unsigned int user_id;

  const std::string mod;
  const std::string cmd;

  const std::vector<std::string> args;
  const std::vector<std::string> targets;

  std::string toString();
};

typedef boost::shared_ptr<UserCommand> spUserCommand;

#endif

