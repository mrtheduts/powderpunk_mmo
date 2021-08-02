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
#include "user_command.h"

UserCommand::UserCommand(unsigned int server_id, unsigned int conn_id,
                         std::string mod, std::string cmd,
                         std::vector<std::string> args,
                         std::vector<std::string> targets)
    : server_id_(server_id),
      conn_id_(conn_id),
      mod_(mod),
      cmd_(cmd),
      args_(args),
      targets_(targets) {}

UserCommand::~UserCommand() {}
