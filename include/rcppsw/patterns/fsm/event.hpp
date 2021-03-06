/**
 * \file event.hpp
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

#ifndef INCLUDE_RCPPSW_PATTERNS_FSM_EVENT_HPP_
#define INCLUDE_RCPPSW_PATTERNS_FSM_EVENT_HPP_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "rcppsw/rcppsw.hpp"

/*******************************************************************************
 * Namespaces/Decls
 ******************************************************************************/
NS_START(rcppsw, patterns, fsm);

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
/**
 * \class event_signal
 * \ingroup patterns fsm
 *
 * \brief A class representing the different signals that can be passed between
 * states in an FSM, and/or between FSMs.
 */
class event_signal {
 public:
  enum type {
    ekIGNORED = 0xFE,  /// A signal that can be ignored
    /**
     * \brief A signal indicating that something VERY BAD has happened. Will
     * cause the parent FSM to halt the program.
     */
    ekFATAL = 0xFF,

    /**
     * \brief A signal indicating that all signals input into a given state have
     * been handled successfully. All states should return this signal in the
     * nomitive case.
     */
    ekHANDLED = 0,

    /**
     * \brief A signal from a lower/child state that could not be handled
     * by a lower state, OR a signal that a lower state sent to
     * indicate that it encountered a situation that it does not
     * know how to handle in the normal turn of events.
     */
    ekUNHANDLED = 1,

    /**
     * A signal indicating nominal run operation should occur upon injection.
     */
    ekRUN = 2,

    /**
     * \brief Applications wishing to defined their own event signals
     * should start here.
     */
    ekEXTERNAL_SIGNALS,
  };
};

/**
 * \class event_type
 *
 * \brief A class representing the types of \ref event_signal that are passed as
 * part of \ref event_data to states.
 */
class event_type {
 public:
  enum {
    ekNORMAL = 0,  /// A normal, external state machine event

    /**
     * \brief A signal from a child state, conveying something that it couldn't
     * handle or that something important happened that it felt a parent FSM
     * should know about.
     */
    ekCHILD = 1,

    /**
     * \brief Applications wishing to defined their own event types
     * should start here.
     */
    ekEXTERNAL_TYPES,
  };
};

/**
 * \class event_data
 *
 * \brief Base class for all data that will be passed to state machine states
 * upon execution of their callback functions. Custom application event data
 * classes must derive from here, or things will not compile.
 */
class event_data {
 public:
  event_data(void) = default;
  explicit event_data(int signal, int type = event_type::ekNORMAL)
      : m_signal(signal), m_type(type) {}
  virtual ~event_data(void) = default;

  int signal(void) const { return m_signal; }
  void signal(int signal) { m_signal = signal; }
  int type(void) const { return m_type; }
  void type(int type) { m_type = type; }

  /**
   * \brief Reset the event data type and signal to nominal/normal values.
   */
  void reset(void) {
    signal(event_signal::ekIGNORED);
    type(event_type::ekNORMAL);
  }

 private:
  /* clang-format off */
  int m_signal{event_signal::ekIGNORED};
  int m_type{event_type::ekNORMAL};
  /* clang-format on */
};

/**
 * \class no_event_data
 *
 * \brief This class does NOT derived from \ref event_data so that compiler
 * errors are generated if you attempt to use it outside of its intent to
 * indicate that a state does not take any data as input.
 */
class no_event_data {};

NS_END(fsm, patterns, rcppsw);

#endif /* INCLUDE_RCPPSW_PATTERNS_FSM_EVENT_HPP_ */
