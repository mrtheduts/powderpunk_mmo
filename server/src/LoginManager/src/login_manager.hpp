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

#ifndef LOGIN_MANAGER_H
#define LOGIN_MANAGER_H

// Src Headers
#include <DataStructures/user.hpp>
#include <Logger/logger.hpp>
#include <Utils/TSStructures/ts_map.hpp>
#include <Utils/TSStructures/ts_queue.hpp>

// External Headers
#include <boost/smart_ptr/shared_ptr.hpp>

// C++ Headers
#include <utility>

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

#endif
