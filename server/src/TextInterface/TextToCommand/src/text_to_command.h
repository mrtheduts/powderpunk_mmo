/*
 *  Powderpunk Server - A Pretentious Project
 *
 *  Author: Eduardo "mrtheduts" Souza
 *          eduardomoreirafds@gmail.com
 *
 *  Date:   13 Mar 2021
 *
 *  (C) Copyright 2021 Eduardo "mrtheduts" Souza All Rights Reserved
 *
 */

#ifndef TEXT_TO_COMMAND_H
#define TEXT_TO_COMMAND_H

#include <DataStructures/user_command.h>

#include <string>
#include <tuple>

boost::shared_ptr<UserCommand> msgToUsrCmd(unsigned long int server_id,
                                           unsigned long int connection_id,
                                           std::string msg);

#endif

