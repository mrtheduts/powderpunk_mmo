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

#ifndef USER_H
#define USER_H

#include <boost/smart_ptr/shared_ptr.hpp>
#include <string>

class User {
 public:
  User(unsigned int id, std::string username);
  ~User();

  const unsigned int id;
  const std::string username;
};

typedef boost::shared_ptr<User> spUser;

#endif

