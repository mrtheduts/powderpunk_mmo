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

// C++ Headers
#include <numeric>
#include <string>

UserCommand::UserCommand(unsigned int server_id, unsigned int conn_id,
                         std::string mod, std::string cmd,
                         std::vector<std::string> args,
                         std::vector<std::string> targets)
    : server_id(server_id),
      conn_id(conn_id),
      mod(mod),
      cmd(cmd),
      args(args),
      targets(targets) {}

UserCommand::~UserCommand() {}

std::string UserCommand::toString() {
  std::string result = "{ [S" + std::to_string(server_id) + ":C" +
                       std::to_string(conn_id) + "] mod {" + mod + "} {" + cmd;

  auto concat_func = [](const std::string &a, const std::string &b) {
    if (a.empty() && !b.empty()) {
      return "\"" + b + "\"";
    } else if (!a.empty() && b.empty()) {
      return "\"" + a + "\"";
    } else if (!a.empty() && !b.empty()) {
      return "\"" + a + "\";\"" + b + "\"";
    } else {
      return std::string("");
    }
  };

  result += "} : args [" + std::to_string(args.size()) + "] {";
  if (args.size() > 0) {
    result +=
        std::accumulate(args.begin(), args.end(), std::string(""), concat_func);
  }
  result += "} targets [" + std::to_string(targets.size()) + "] {";
  if (targets.size() > 0) {
    result += std::accumulate(targets.begin(), targets.end(), std::string(""),
                              concat_func);
  }
  result += "} }";

  return result;
}

