/*
 *  Powderpunk Server - A Pretentious Project
 *
 *  Author: Eduardo "mrtheduts" Souza
 *          eduardomoreirafds@gmail.com
 *
 *  Date:   27 Dez 2021
 *
 *  (C) Copyright 2021 Eduardo "mrtheduts" Souza All Rights Reserved
 *
 */

#ifndef SERVER_SRC_LOGGER_SRC_LOGGER_HPP_
#define SERVER_SRC_LOGGER_SRC_LOGGER_HPP_

// C++ Headers
#include <memory>
#include <string>

// External Headers
#include <boost/log/core.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/trivial.hpp>
#include <boost/smart_ptr/enable_shared_from_this.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>

// Defines
#define MB 1024 * 1024
#define LOG_FILE_SIZE 10

using namespace boost;

template <typename... Args>
std::string stringFormat(const std::string& format, Args... args) {
  int size_s = std::snprintf(nullptr, 0, format.c_str(), args...) +
               1;  // Extra space for '\0'
  if (size_s <= 0) {
    throw std::runtime_error("Error during formatting.");
  }
  auto size = static_cast<size_t>(size_s);
  auto buf = std::make_unique<char[]>(size);
  std::snprintf(buf.get(), size, format.c_str(), args...);
  return std::string(buf.get(),
                     buf.get() + size - 1);  // We don't want the '\0' inside
}

class Logger : public boost::enable_shared_from_this<Logger> {
 public:
  static void init();
  static boost::shared_ptr<Logger> getLogger(std::string class_name);
  static boost::shared_ptr<Logger> getLogger(std::string class_name,
                                             unsigned int id);
  static boost::shared_ptr<Logger> getLogger(std::string class_name,
                                             unsigned int server_id,
                                             unsigned int id);

  explicit Logger(std::string class_name);
  Logger(std::string class_name, unsigned int id);
  Logger(std::string class_name, unsigned int id, unsigned int server_id);
  ~Logger();

  template <typename... Args>
  void debug(std::string msg, Args... args) {
    std::string formated_msg = stringFormat(msg, args...);
    logWithSeverity(formated_msg, log::trivial::severity_level::debug);
  }

  template <typename... Args>
  void info(std::string msg, Args... args) {
    std::string formated_msg = stringFormat(msg, args...);
    logWithSeverity(formated_msg, log::trivial::severity_level::info);
  }

  template <typename... Args>
  void warning(std::string msg, Args... args) {
    std::string formated_msg = stringFormat(msg, args...);
    logWithSeverity(formated_msg, log::trivial::severity_level::warning);
  }

  template <typename... Args>
  void error(std::string msg, Args... args) {
    std::string formated_msg = stringFormat(msg, args...);
    logWithSeverity(formated_msg, log::trivial::severity_level::error);
  }

  template <typename... Args>
  void fatal(std::string msg, Args... args) {
    std::string formated_msg = stringFormat(msg, args...);
    logWithSeverity(formated_msg, log::trivial::severity_level::fatal);
  }

 private:
  void logWithSeverity(const std::string& msg,
                       log::trivial::severity_level severity);

  log::sources::severity_logger_mt<log::trivial::severity_level> logger_;

  const std::string class_name_;

  const bool has_id_;
  const unsigned int id_;

  const bool has_server_id_;
  const unsigned int server_id_;
};

typedef boost::shared_ptr<Logger> spLogger;

#endif  // SERVER_SRC_LOGGER_SRC_LOGGER_HPP_
