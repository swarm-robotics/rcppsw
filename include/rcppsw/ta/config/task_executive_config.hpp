/**
 * @File task_executive_config.hpp
 *
 * @copyright 2018 John Harwell, All rights reserved.
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

#ifndef INCLUDE_RCPPSW_TA_CONFIG_TASK_EXECUTIVE_CONFIG_HPP_
#define INCLUDE_RCPPSW_TA_CONFIG_TASK_EXECUTIVE_CONFIG_HPP_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <string>
#include "rcppsw/config/base_config.hpp"

/*******************************************************************************
 * Namespaces/Decls
 ******************************************************************************/
NS_START(rcppsw, ta, config);

/*******************************************************************************
 * Structure Definitions
 ******************************************************************************/
/**
 * @struct task_executive_config
 * @ingroup rcppsw config ta
 */
struct task_executive_config final : public rcppsw::config::base_config {
  /**
   * @brief Should the executive automatically update execution time estimates
   * for tasks, or will that be handled in the application via callbacks?
   */
  bool update_exec_ests{true};

  /**
   * @brief Should the executive automatically update interface time estimates
   * for tasks, or will that be handled in the application via callbacks?
   */
  bool update_interface_ests{true};

  /**
   * @brief Policy for specifying how the initially active TAB in the executive
   * will be chosen. Valid values are: [root, random, max_depth].
   */
  std::string tab_init_policy{"root"};

  /**
   * @brief Policy for specifying how tasks will be allocated in the executive
   * from the data structure containing the tasks to run.
   */
  std::string alloc_policy{"random"};
};

NS_END(config, ta, rcppsw);

#endif /* INCLUDE_RCPPSW_TA_CONFIG_TASK_EXECUTIVE_CONFIG_HPP_ */