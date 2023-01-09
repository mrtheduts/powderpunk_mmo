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

#include "login_manager.hpp"

// External Headers
#include <boost/log/trivial.hpp>
#include <boost/smart_ptr/make_shared_object.hpp>

// Static variables initialization
spLoginManager LoginManager::singleton_ = NULL;

LoginManager::LoginManager() { logger_ = Logger::getLogger("LoginManager"); }
LoginManager::~LoginManager() {}

boost::shared_ptr<LoginManager> LoginManager::getLoginManager() {
  if (singleton_ == NULL) {
    singleton_ = boost::make_shared<LoginManager>();
  }

  return singleton_;
}

// TODO: throw exception regarding user already logged
spUser LoginManager::authenticateUser(std::string username) {
  spUser user = NULL;

  if (!online_users_.has(username)) {
    user = boost::make_shared<User>(online_users_.size(), username);
    online_users_.put(username, user);
  }

  return user;
}

void LoginManager::disconnectUser(std::string username) {
  online_users_.erase(username);
}

