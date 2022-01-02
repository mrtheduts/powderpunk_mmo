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

#include "logger.h"

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

// C++ Headers
#include <iomanip>
#include <iostream>

using namespace boost;

void Logger::init() {
  log::add_common_attributes();

  // TODO: Make file logging work
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
          log::expressions::max_size_decor<char>(30)
              [log::expressions::stream
               << std::setw(30)
               << log::expressions::format_date_time<boost::posix_time::ptime>(
                      "TimeStamp", "%Y-%m-%d %H:%M:%S.%f")] %
          log::expressions::max_size_decor<char>(
              7)[log::expressions::stream << std::setw(7)
                                          << log::trivial::severity] %
          log::expressions::max_size_decor<char>(
              19)[log::expressions::stream
                  << std::setw(19)
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

Logger::Logger(std::string class_name)
    : class_name_{class_name}, has_id_{false}, id_{0} {
  logger_.add_attribute("Class",
                        log::attributes::constant<std::string>(class_name));
}

Logger::Logger(std::string class_name, unsigned int id)
    : class_name_{class_name}, has_id_{true}, id_{id} {
  logger_.add_attribute("Class",
                        log::attributes::constant<std::string>(
                            class_name + " [" + std::to_string(id) + "]"));
}

Logger::~Logger() {}

void Logger::logWithSeverity(std::string& msg,
                             log::trivial::severity_level severity) {
  BOOST_LOG_SEV(logger_, severity) << msg;
}

