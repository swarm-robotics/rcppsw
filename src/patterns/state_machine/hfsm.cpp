/**
 * \file hfsm.cpp
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

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "rcppsw/patterns/fsm/hfsm.hpp"

/*******************************************************************************
 * Namespaces/Decls
 ******************************************************************************/
NS_START(rcppsw, patterns, fsm);

/*******************************************************************************
 * Member Functions
 ******************************************************************************/
void hfsm::state_engine_step(const state_map_row* const c_row) {
  ER_ASSERT(nullptr != c_row->state(), "null state?");
  ER_TRACE("Invoking state action: state%d, data=%p",
           current_state(),
           reinterpret_cast<const void*>(event_data()));
  auto* state = static_cast<const hfsm_state*>(c_row->state());
  int rval = event_signal::ekUNHANDLED;
  while (rval != event_signal::ekHANDLED) {
    rval = state->invoke_state_action(this, event_data());

    if (event_signal::ekHANDLED == rval) {
      break;
    }
    /*
     * It is possible that we have gotten the HANDLED signal from a parent state
     * of a child that returned UNHANDLED. As such, we need to change both the
     * event type and the signal of the event so execution can continue
     * normally.
     */
    state = static_cast<hfsm_state*>(state->parent());
    event_data()->type(event_type::ekCHILD);
    event_data()->signal(rval);
  } /* while() */
} /* state_engine_step() */

void hfsm::state_engine_step(const state_map_ex_row* const c_row_ex) {
  ER_ASSERT(nullptr != c_row_ex->state(), "null state?");
  ER_TRACE("Invoking state action: state%d, data=%p",
           current_state(),
           reinterpret_cast<const void*>(event_data()));
  auto* state = static_cast<const hfsm_state*>(c_row_ex->state());
  int rval = event_signal::ekUNHANDLED;
  while (rval != event_signal::ekHANDLED) {
    rval = state->invoke_state_action(this, event_data());

    if (event_signal::ekHANDLED == rval) {
      break;
    }
    /*
     * It is possible that we have gotten the HANDLED signal from a parent state
     * of a child that returned UNHANDLED. As such, we need to change both the
     * event type and the signal of the event so execution can continue
     * normally.
     */
    state = static_cast<hfsm_state*>(state->parent());
    event_data()->type(event_type::ekCHILD);
    event_data()->signal(rval);
  } /* while() */
} /* state_engine_step() */

void hfsm::change_parent(uint8_t state,
                         rcppsw::patterns::fsm::state* new_parent) {
  auto* row = state_map(state);
  auto* row_ex = state_map_ex(state);

  ER_ASSERT(!(nullptr == row && nullptr == row_ex), "Both state maps are NULL!");

  if (nullptr != row) {
    auto* self = static_cast<hfsm_state*>(row->state());
    self->parent(new_parent);
  } else {
    auto* self = static_cast<hfsm_state*>(row_ex->state());
    self->parent(new_parent);
  }
} /* change_parent() */

NS_END(fsm, patterns, rcppsw);
