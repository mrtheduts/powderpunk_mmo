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

#ifndef LOCATION_H
#define LOCATION_H

#include <string>
#include <unordered_map>

class Location {
 public:
  Location(std::string id);
  ~Location();

  const std::string id_;

 private:
  // Maybe add interior, same level and outside locations
  std::unordered_map<std::string, std::string> connected_places_;
};

#endif

