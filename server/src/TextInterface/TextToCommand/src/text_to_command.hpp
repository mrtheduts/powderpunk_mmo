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

#ifndef SERVER_SRC_TEXTINTERFACE_TEXTTOCOMMAND_TEXT_TO_COMMAND_HPP_
#define SERVER_SRC_TEXTINTERFACE_TEXTTOCOMMAND_TEXT_TO_COMMAND_HPP_

// C++ Headers
#include <string>

// Src Headers
#include <DataStructures/user_command.hpp>

boost::shared_ptr<UserCommand> msgToUsrCmd(unsigned long int server_id,
                                           unsigned long int connection_id,
                                           unsigned long int user_id,
                                           const std::string& msg);

#endif  // SERVER_SRC_TEXTINTERFACE_TEXTTOCOMMAND_TEXT_TO_COMMAND_HPP_
