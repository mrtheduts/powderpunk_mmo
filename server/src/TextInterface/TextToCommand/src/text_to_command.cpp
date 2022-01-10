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

#include "text_to_command.h"

// External Headers
#include <boost/make_shared.hpp>

// C++ Headers
#include <iostream>

std::string preprocessMsg(std::string msg) {
  std::string res = msg;

  return res;
}

boost::shared_ptr<UserCommand> msgToUsrCmd(unsigned long int server_id,
                                           unsigned long int connection_id,
                                           unsigned long int user_id,
                                           std::string& msg) {
  std::string preparedMsg = preprocessMsg(msg);

  std::string mod = "";
  std::string cmd = "say";
  std::vector<std::string> args = std::vector<std::string>();
  std::vector<std::string> targets = std::vector<std::string>();

  args.push_back(preparedMsg);

  spUserCommand usr_cmd = boost::make_shared<UserCommand>(
      server_id, connection_id, user_id, mod, cmd, args, targets);

  return usr_cmd;
}

/*
spUserCommand TranslateInput(const id_input input) {
  UserCommand::ConnectionType conn_type = UserCommand::telnet;
  unsigned long int server_id = std::get<0>(input);
  unsigned long int conn_id = std::get<1>(input);
  ConnectionState conn_state = std::get<2>(input);
  boost::shared_ptr<std::string> action;

  if (conn_state == ConnectionState::auth) {
    action = boost::make_shared<std::string>("auth");
  } else if (conn_state == ConnectionState::logged) {
    action = boost::make_shared<std::string>("say");
  } else if (conn_state == ConnectionState::closed) {
    action = boost::make_shared<std::string>("close_conn");
  }

  boost::shared_ptr<std::string> msg =
      boost::make_shared<std::string>(std::get<3>(input));

  spUserCommand cmd = boost::make_shared<UserCommand>(
      conn_type, server_id, conn_id, conn_state, action);

  cmd->AddArg(msg);

  return cmd;
}
*/

