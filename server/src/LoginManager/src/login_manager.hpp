/*
 *  Powderpunk Server - A Pretentious Project
 *
 *  Author: Eduardo "mrtheduts" Souza
 *          eduardomoreirafds@gmail.com
 *
 *  Date:   13 Aug 2021
 *
 *  (C) Copyright 2021 Eduardo "mrtheduts" Souza All Rights Reserved
 *
 */

#ifndef SERVER_SRC_LOGINMANAGER_SRC_LOGIN_MANAGER_HPP_
#define SERVER_SRC_LOGINMANAGER_SRC_LOGIN_MANAGER_HPP_

// C++ Headers
#include <string>
#include <utility>

// Src Headers
#include <DataStructures/user.hpp>
#include <Logger/logger.hpp>
#include <Utils/TSStructures/ts_map.hpp>
#include <Utils/TSStructures/ts_queue.hpp>

// External Headers
#include <boost/smart_ptr/shared_ptr.hpp>

class LoginManager {
 public:
  LoginManager();
  ~LoginManager();

  static boost::shared_ptr<LoginManager> getLoginManager();
  spUser authenticateUser(std::string username);
  void disconnectUser(std::string username);

 private:
  /* Logger object */
  spLogger logger_;

  static boost::shared_ptr<LoginManager> singleton_;

  TSMap<std::string, spUser> online_users_;
};

typedef boost::shared_ptr<LoginManager> spLoginManager;

#endif  // SERVER_SRC_LOGINMANAGER_SRC_LOGIN_MANAGER_HPP_
