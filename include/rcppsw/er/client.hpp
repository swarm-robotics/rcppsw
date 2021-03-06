/**
 * \file client.hpp
 *
 * Interface for classes that want to be able to use the ER Server.
 *
 * \copyright 2017 John Harwell, All rights reserved.
 *
 * This file is part of RCPPSW.
 *
 * RCPPSW is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * RCPPSW is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * RCPPSW.  If not, see <http://www.gnu.org/licenses/
 */

#ifndef INCLUDE_RCPPSW_ER_CLIENT_HPP_
#define INCLUDE_RCPPSW_ER_CLIENT_HPP_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "rcppsw/er/er.hpp"

#if (RCPPSW_ER == RCPPSW_ER_ALL)
#include <log4cxx/consoleappender.h>
#include <log4cxx/fileappender.h>
#include <log4cxx/logger.h>
#include <log4cxx/ndc.h>
#include <log4cxx/patternlayout.h>
#include <log4cxx/xml/domconfigurator.h>
#endif


#include "rcppsw/rcppsw.hpp"
#include "rcppsw/er/macros.hpp"

/*******************************************************************************
 * Macros
 ******************************************************************************/
#if (RCPPSW_ER == RCPPSW_ER_NONE)

#define ER_CLIENT_INIT(name) rer::client<typename std::remove_reference<decltype(*this)>::type>()
#define ER_LOGGING_INIT(fname)
#define ER_LOGFILE_SET(logger, fname)
#define ER_NDC_PUSH(s)
#define ER_NDC_POP(...)
#define ER_ENV_VERIFY(...)

#elif (RCPPSW_ER == RCPPSW_ER_FATAL)

#define ER_CLIENT_INIT(name) rer::client<typename std::remove_reference<decltype(*this)>::type>()
#define ER_LOGGING_INIT(fname)
#define ER_LOGFILE_SET(logger, fname)
#define ER_NDC_PUSH(s)
#define ER_NDC_POP(...)
#define ER_ENV_VERIFY(...)


#elif (RCPPSW_ER == RCPPSW_ER_ALL)

/**
 * \def ER_CLIENT_INIT(name)
 *
 * Initialize a logging client with the specified name (easier to do a macro
 * than to have to try do the casting every single time).
 */
#define ER_CLIENT_INIT(name) \
  rcppsw::er::client<typename std::remove_reference_t<decltype(*this)>>(name)

/**
 * \def ER_LOGGING_INIT(fname)
 *
 * Initialize logging by specifying the log4cxx configuration file.
 */
#define ER_LOGGING_INIT(fname) \
  rcppsw::er::client<          \
      typename std::remove_reference_t<decltype(*this)>>::logging_init(fname)

/**
 * \def ER_LOGFILE_SET(logger, fname)
 *
 * Set the logfile for the specified logger. Idempotent.
 */
#define ER_LOGFILE_SET(logger, fname)                                             \
  rcppsw::er::client<                                                             \
      typename std::remove_reference_t<decltype(*this)>>::logfile_set(logger, \
                                                                          fname)

/**
 * \def ER_NDC_PUSH(s)
 *
 * Push the specified NDC context (prepended to every message).
 */
#define ER_NDC_PUSH(s) \
  rcppsw::er::client<  \
      typename std::remove_reference_t<decltype(*this)>>::push_ndc(s)

/**
 * \def ER_NDC_POP()
 *
 * Pop the last pushed NDC context.
 */
#define ER_NDC_POP(...) \
  rcppsw::er::client<   \
      typename std::remove_reference_t<decltype(*this)>>::pop_ndc()

/**
 * \def ER_ENV_VERIFY()
 *
 * Verify the correct environment variables were set up for event reporting
 * before the application was launched.
 */
#define ER_ENV_VERIFY(...) \
  rcppsw::er::client<      \
      typename std::remove_reference_t<decltype(*this)>>::env_verify()

#endif

/*******************************************************************************
 * Namespaces/Decls
 ******************************************************************************/
NS_START(rcppsw, er);

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
/**
 * \class client
 * \ingroup er
 *
 * \brief A class that can connect to a logging server for logging of important
 * events. Basically a thin wrapper around log4cxx. If ER_NDEBUG is defined,
 * then this class will mostly compile away to nothing, and most member
 * functions will not be defined.
 */
#if (RCPPSW_ER == RCPPSW_ER_ALL)
template <typename T>
class client {
 public:
  /**
   * \brief Initialize logging by specifying the path to the log4cxx
   * configuration file.
   */
  static void logging_init(const std::string& fpath) {
    /*
     * Multiple initializations will cause duplicate messages to show up in
     * logfiles.
     */
    if (!m_initialized) {
      log4cxx::xml::DOMConfigurator::configure(fpath);
      m_initialized = true;
    }
  }

  /**
   * \brief Set the log file for the specified logger. Idempotent. If the
   * logfile currently exists, it will be truncated.
   *
   * Logfile is an appender, which is given the same name as the logfile
   * itself.
   */
  static void logfile_set(const log4cxx::LoggerPtr& logger,
                          const std::string& name) {
    for (auto& a : logger->getAllAppenders()) {
      if (a->getName() == name) {
        return;
      }
    } /* for(&a..) */

    log4cxx::LayoutPtr layout = new log4cxx::PatternLayout(kFileLayout);
    log4cxx::AppenderPtr appender =
        new log4cxx::FileAppender(layout, name, false);
    appender->setName(name);
    logger->addAppender(appender);
  }

  /**
   * \brief Push a log4cxx NDC context.
   *
   * \param s The context.
   */
  static void push_ndc(const std::string& s) { log4cxx::NDC::push(s); }

  /**
   * \brief Pop the top of the log4cxx NDC stack.
   */
  static void pop_ndc(void) { log4cxx::NDC::pop(); }

  /**
   * \param name Name of client/new logger.
   */
  explicit client(const std::string& name)
      : m_logger(log4cxx::Logger::getLogger(name)) {}

  virtual ~client(void) = default;

  client(const client&) = default;
  client& operator=(const client&) = default;

  /**
   * \brief Set the logfile of the current logger. Not idempotent.
   */
  void logfile_set(const std::string& name) {
    log4cxx::LayoutPtr layout = new log4cxx::PatternLayout(kFileLayout);
    log4cxx::AppenderPtr appender = new log4cxx::FileAppender(layout, name);
    logger()->addAppender(appender);
  }

  /**
   * \brief Get current logger name.
   */
  std::string logger_name(void) const {
    std::string name;
    m_logger->getName(name);
    return name;
  }

  /**
   * \brief Get a reference to the ER logger.
   */
  log4cxx::LoggerPtr logger(void) const { return m_logger; }

  /**
   * \brief Verify that the execution environment was properly set up for
   * logging, and abort the program if it was not.
   */
  void env_verify(void) {
    if (const char* env_p = std::getenv("LOG4CXX_CONFIGURATION")) {
      ER_LOGGING_INIT(std::string(env_p));
    } else {
      std::cerr << "LOG4CXX_CONFIGURATION not defined" << std::endl;
      std::exit(EXIT_FAILURE);
    }
  }

 private:
  /* clang-format off */
  static const std::string kConsoleLayout;
  static const std::string kFileLayout;

  static bool              m_initialized;

  log4cxx::LoggerPtr        m_logger{};
  /* clang-format on */
};

template <typename T>
bool client<T>::m_initialized = false;
template <typename T>
const std::string client<T>::kConsoleLayout = "%x [%-5p] %c - %m%n";
template <typename T>
const std::string client<T>::kFileLayout = "%x [%-5p] %c %l - %m%n";

#else
template<typename T>
class client {
 public:
  virtual ~client(void) = default;
};
#endif

NS_END(rcppsw, er);

#endif /* INCLUDE_RCPPSW_ER_CLIENT_HPP_ */
