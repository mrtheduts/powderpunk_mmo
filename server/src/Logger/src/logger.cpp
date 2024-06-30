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

#include "logger.hpp"

// C++ Headers
#include <iomanip>
#include <iostream>

// External Headers
#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/log/detail/format.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/expressions/attr.hpp>
#include <boost/log/expressions/formatters/date_time.hpp>
#include <boost/log/expressions/formatters/format.hpp>
#include <boost/log/expressions/formatters/max_size_decorator.hpp>
#include <boost/log/expressions/keyword.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>

// Defines
#define TIMESTAMP_SIZE 30
#define SEVERITY_SIZE 7
#define CLASS_SIZE 30

using namespace boost;

void Logger::init() {
  log::add_common_attributes();

  // TODO(mrtheduts): Make file logging work
  /* log::add_file_log( */
  /*     log::keywords::file_name = "active.log", */
  /*     log::keywords::target_file_name = "powderpunk_%Y-%m-%d_%2N.log", */
  /*     log::keywords::rotation_size = LOG_FILE_SIZE * MB, */
  /*     log::keywords::time_based_rotation = */
  /*         log::sinks::file::rotation_at_time_point(12, 0, 0), */
  /*     log::keywords::format = */
  /*         "[%TimeStamp%] <%Severity%> @ %Class%: %Message%"); */

  log::add_console_log(
      std::clog,
      log::keywords::format =
          log::expressions::format("[%1%] %2% @ %3% : %4%") %
          log::expressions::max_size_decor<char>(TIMESTAMP_SIZE)
              [log::expressions::stream
               << std::setw(TIMESTAMP_SIZE)
               << log::expressions::format_date_time<boost::posix_time::ptime>(
                      "TimeStamp", "%Y-%m-%d %H:%M:%S.%f")] %
          log::expressions::max_size_decor<char>(
              SEVERITY_SIZE)[log::expressions::stream
                             << std::setw(SEVERITY_SIZE)
                             << log::trivial::severity] %
          log::expressions::max_size_decor<char>(
              CLASS_SIZE)[log::expressions::stream
                          << std::setw(CLASS_SIZE)
                          << log::expressions::attr<std::string>("Class")] %
          log::expressions::smessage);
}

spLogger Logger::getLogger(std::string class_name) {
  spLogger new_logger = boost::make_shared<Logger>(class_name);
  return new_logger;
}

spLogger Logger::getLogger(std::string class_name, unsigned int id) {
  spLogger new_logger = boost::make_shared<Logger>(class_name, id);
  return new_logger;
}

spLogger Logger::getLogger(std::string class_name, unsigned int id,
                           unsigned int server_id) {
  spLogger new_logger = boost::make_shared<Logger>(class_name, id, server_id);
  return new_logger;
}

Logger::Logger(std::string class_name)
    : class_name_{class_name},
      has_id_{false},
      id_{0},
      has_server_id_{false},
      server_id_{0} {
  logger_.add_attribute("Class",
                        log::attributes::constant<std::string>(class_name));
}

Logger::Logger(std::string class_name, unsigned int id)
    : class_name_{class_name},
      has_id_{true},
      id_{id},
      has_server_id_{false},
      server_id_{0} {
  logger_.add_attribute("Class",
                        log::attributes::constant<std::string>(
                            class_name + " [" + std::to_string(id) + "]"));
}

Logger::Logger(std::string class_name, unsigned int id, unsigned int server_id)
    : class_name_{class_name},
      has_id_{true},
      id_{id},
      has_server_id_{true},
      server_id_{server_id} {
  logger_.add_attribute(
      "Class", log::attributes::constant<std::string>(
                   class_name + " [S" + std::to_string(server_id) + ":" +
                   std::to_string(id) + "]"));
}

Logger::~Logger() {}

void Logger::logWithSeverity(const std::string& msg,
                             log::trivial::severity_level severity) {
  BOOST_LOG_SEV(logger_, severity) << msg;
}
