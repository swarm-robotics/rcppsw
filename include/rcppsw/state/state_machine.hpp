#ifndef INCLUDE_RCPPSW_STATE_STATE_MACHINE_HPP_
#define INCLUDE_RCPPSW_STATE_STATE_MACHINE_HPP_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdio.h>
#include <typeinfo>
#include <cstddef>
#include "rcsw/common/common.h"
#include "rcppsw/state/event_data.hpp"
#include "rcppsw/state/state.hpp"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
namespace rcppsw {
namespace patterns {
namespace state_machine {

/*******************************************************************************
 * Structure Definitions
 ******************************************************************************/
/// @brief A structure to hold a single row within the state map.
struct StateMapRow {
  const state_base* const State;
};

/// @brief A structure to hold a single row within the extended state map.
struct StateMapRowEx {
  const state_base* const state;
  const guard_base* const guard;
  const entry_base* const entry;
  const exit_base* const exit;
};

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
/// @brief simple_fsm implements a software-based state machine.
class simple_fsm {
 public:
  enum { EVENT_IGNORED = 0xFE, CANNOT_HAPPEN };

  /**
   * @param max_states The maximum number of state machine states.
   * @param initial_state Initial state machine state.
   */
  simple_fsm(uint8_t max_states, uint8_t initial_state = 0);

  virtual ~simple_fsm() {}

  uint8_t current_state() { return current_state_; }
  uint8_t max_allowed_states() { return MAX_STATES; }

 protected:
  /**
   * @brief Generates an external event. called once per external event
   * to start the state machine executing
   *
   * @param new_state The state machine state to transition to.
   * @param data The event data sent to the state.
   */
  void ExternalEvent(uint8_t new_state, const event_data* data = NULL);

  /**
   * @brief Generates an internal event. These events are generated while executing
   * within a state machine state.
   * @param new_state The state machine state to transition to.
   * @param data The event data sent to the state.
   */
  void InternalEvent(uint8_t new_state, const event_data* data = NULL);

 private:
  /// The maximum number of state machine states.
  const uint8_t MAX_STATES;

  /// The current state machine state.
  uint8_t current_state_;

  /// The new state the state machine has yet to transition to.
  uint8_t m_new_state;

  /// Set to TRUE when an event is generated.
  bool m_eventGenerated;

  /// The state event data pointer.
  const event_data* m_pevent_data;

  /**
   * @brief Gets the state map as defined in the derived class.
   *
   * The BEGIN_STATE_MAP, STATE_MAP_ENTRY and END_STATE_MAP macros are used to
   * assist in creating the map. A state machine only needs to return a state
   * map using either GetStateMap() or GetStateMapEx() but not both.
   *
   @return An array of StateMapRow pointers with the array size MAX_STATES or
   *       NULL if the state machine uses the GetStateMapEx().
   */
  virtual const StateMapRow* GetStateMap() = 0;

  /**
   * @brief Gets the extended state map as defined in the derived class.
   *
   * The BEGIN_STATE_MAP_EX, STATE_MAP_ENTRY_EX, STATE_MAP_ENTRY_ALL_EX, and
   * END_STATE_MAP_EX macros are used to assist in creating the map. A state
   * machine only needs to return a state map using either GetStateMap() or
   * GetStateMapEx() but not both.
   *
   * @return An array of StateMapRowEx pointers with the array size MAX_STATES
   * or NULL if the state machine uses the GetStateMap().
   */
  virtual const StateMapRowEx* GetStateMapEx() = 0;

  void set_current_state(uint8_t new_state) { current_state_ = new_state; }

  /*
   * State machine engine that executes the external event and, optionally, all
   * internal events generated during state execution.
   */
  void StateEngine(void);
  void StateEngine(const StateMapRow* const pStateMap);
  void StateEngine(const StateMapRowEx* const pStateMapEx);
};

#define STATE_DECLARE(stateMachine, stateName, eventData)               \
  void ST_##stateName(const eventData*);                                \
  state_action<stateMachine, eventData, &stateMachine::ST_##stateName> stateName;

#define STATE_DEFINE(stateMachine, stateName, eventData)        \
  void stateMachine::ST_##stateName(const eventData* data)

#define GUARD_DECLARE(stateMachine, guardName, eventData)               \
  bool GD_##guardName(const eventData*);                                \
  state_guard_condition<stateMachine, eventData, &stateMachine::GD_##guardName> guardName;

#define GUARD_DEFINE(stateMachine, guardName, eventData)        \
  bool stateMachine::GD_##guardName(const eventData* data)

#define ENTRY_DECLARE(stateMachine, entryName, eventData)               \
  void EN_##entryName(const eventData*);                                \
  state_entry_action<stateMachine, eventData, &stateMachine::EN_##entryName> entryName;

#define ENTRY_DEFINE(stateMachine, entryName, eventData)        \
  void stateMachine::EN_##entryName(const eventData* data)

#define EXIT_DECLARE(stateMachine, exitName)                            \
  void EX_##exitName(void);                                             \
  state_exit_action<stateMachine, &stateMachine::EX_##exitName> exitName;

#define EXIT_DEFINE(stateMachine, exitName)     \
  void stateMachine::EX_##exitName(void)

#define BEGIN_TRANSITION_MAP                    \
  static const uint8_t TRANSITIONS[] = {        \

#define TRANSITION_MAP_ENTRY(entry)             \
    entry,

#define END_TRANSITION_MAP(data)                                        \
    };                                                                  \
  ASSERT_TRUE(current_state() < ST_MAX_STATES);                       \
  ExternalEvent(TRANSITIONS[current_state()], data);                  \
  C_ASSERT((sizeof(TRANSITIONS)/sizeof(uint8_t)) == ST_MAX_STATES);

#define PARENT_TRANSITION(state)                \
  if (current_state() >= ST_MAX_STATES &&     \
      current_state() < max_allowed_states()) {     \
    ExternalEvent(state);                       \
    return; }

#define BEGIN_STATE_MAP                                         \
  private:                                                      \
  virtual const StateMapRowEx* GetStateMapEx() { return NULL; } \
  virtual const StateMapRow* GetStateMap() {                    \
    static const StateMapRow STATE_MAP[] = {

#define STATE_MAP_ENTRY(stateName)              \
      stateName,

#define END_STATE_MAP                                                   \
      };                                                                \
    C_ASSERT((sizeof(STATE_MAP)/sizeof(StateMapRow)) == ST_MAX_STATES); \
    return &STATE_MAP[0]; }

#define BEGIN_STATE_MAP_EX                                      \
  private:                                                      \
  virtual const StateMapRow* GetStateMap() { return NULL; }     \
  virtual const StateMapRowEx* GetStateMapEx() {                \
    static const StateMapRowEx STATE_MAP[] = {

#define STATE_MAP_ENTRY_EX(stateName)           \
      { stateName, 0, 0, 0 },

#define STATE_MAP_ENTRY_ALL_EX(stateName, guardName, entryName, exitName) \
      { stateName, guardName, entryName, exitName },

#define END_STATE_MAP_EX                                                \
      };                                                                \
    C_ASSERT((sizeof(STATE_MAP)/sizeof(StateMapRowEx)) == ST_MAX_STATES); \
    return &STATE_MAP[0]; }

} /* namespace state_machine */
} /* namespace patterns */
} /* namespace rcppsw */

#endif /* INCLUDE_RCPPSW_STATE_STATE_MACHINE_HPP_ */
