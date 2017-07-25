/**
 * @file forkable.hpp
 *
 * @copyright 2017 John Harwell, All rights reserved.
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

#ifndef INCLUDE_RCPPSW_MULTIPROCESS_FORKABLE_HPP_
#define INCLUDE_RCPPSW_MULTIPROCESS_FORKABLE_HPP_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <sys/wait.h>
#include <unistd.h>
#include <string>
#include <thread>
#include <vector>
#include "rcsw/common/common.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
namespace rcppsw {

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
class forkable {
 public:
  /* member functions */
  forkable(void) : proc_run_(false), pid_(0) {}
  virtual ~forkable(void) {}
  pid_t pid(void) { return pid_; }

  /**
   * @brief Start a process.
   *
   * @param core The core to bind the process (and any threads it might spawn)
   * to. By default, no binding.
   *
   * @return PID of child process in parent
   */
  virtual pid_t start(int core = -1);

  /**
   * @brief Start a process in a new directory.
   * @param core The core to bind the process (and any threads it might spawn)
   *             to. By default, no binding.
   *
   * @return PID of child process in parent
   */
  virtual pid_t start(const std::string& new_wd, int core = -1);


  /**
   * @brief Signal a process that it should terminate, from outside the process.
   */
  virtual void term(void) { proc_run_ = false; }

  /**
   * @brief Entry point for the new process.
   */
  virtual void proc_main(void) = 0;

 protected:
  /**
   * @brief Check if a process object has been told to terminate elsewhere.
   */
  bool terminated(void) { return (false == proc_run_); }

 private:
  /* data members */
  bool proc_run_;
  pid_t pid_;

  /* non-member functions */
  static void entry_point(void* this_p) {
    forkable* pt = static_cast<forkable*>(this_p);
    pt->proc_main();
  } /* entry_point() */
};

} /* namespace rcppsw */

#endif /* INCLUDE_RCPPSW_MULTIPROCESS_FORKABLE_HPP_ */
