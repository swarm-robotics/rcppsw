/**
 * @file bifurcating_tab.hpp
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

#ifndef INCLUDE_RCPPSW_TASK_ALLOCATION_BIFURCATING_TAB_HPP_
#define INCLUDE_RCPPSW_TASK_ALLOCATION_BIFURCATING_TAB_HPP_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "rcppsw/metrics/tasks/bifurcating_tab_metrics.hpp"
#include "rcppsw/er/client.hpp"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NS_START(rcppsw, task_allocation);
class polled_task;
class bifurcating_tdgraph;

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
/**
 * @class bifurcating_tab
 * @ingroup task_allocation
 *
 * @brief Represents Bifurcating Task Allocation Block (TAB) which consists of a
 * root task and two subtasks the root task decomposes into. The subtasks are
 * may or may not be capable of being further decomposed, and therefore the
 * roots of additional TABs.
 */
class bifurcating_tab : public metrics::tasks::bifurcating_tab_metrics,
                        public er::client<bifurcating_tab> {
 public:
  bifurcating_tab(const bifurcating_tdgraph* const graph,
                  const polled_task* root,
                  const polled_task* child1,
                  const polled_task* child2);

  ~bifurcating_tab(void) override = default;

  bifurcating_tab& operator=(const bifurcating_tab& other) = delete;
  bifurcating_tab(const bifurcating_tab& other) = delete;

  /**
   * @brief Set the active task to the passed in task.
   */
  void change_active_task(const polled_task* active_task);

  /**
   * @brief Returns \c TRUE iff the argument is one of the 3 tasks in the TAB.
   */
  bool contains_task(const polled_task* task) const;

  /**
   * @brief Returns \c TRUE iff the argument is the root task in the TAB.
   */
  bool task_is_root(const polled_task* task) const;

  /**
   * @brief Returns \c TRUE iff the argument is one of the child tasks in the TAB.
   */
  bool task_is_child(const polled_task* task) const;

  const polled_task* root(void) const { return m_root; }
  const polled_task* child1(void) const { return m_child1; }
  const polled_task* child2(void) const { return m_child2; }

  /* bifurcating TAB metrics */
  bool subtask1_active(void) const override { return m_active_task == m_child1; }
  bool subtask2_active(void) const override { return m_active_task == m_child2; }
  bool root_active(void) const override { return m_active_task == m_root; }
  bool employed_partitioning(void) const override;
  bool task_changed(void) const override { return m_active_task != m_last_task; }
  bool task_depth_changed(void) const override;
  double root_partition_prob(void) const override;
  double root_subtask_selection_prob(void) const override;

 private:
  // clang-format off
  const bifurcating_tdgraph* const mc_graph;
  const polled_task*               m_last_task{nullptr};
  const polled_task*               m_active_task{nullptr};
  const polled_task* const         m_root;
  const polled_task* const         m_child1;
  const polled_task* const         m_child2;
  // clang-format on
};

NS_END(task_allocation, rcppsw);

#endif /* INCLUDE_RCPPSW_TASK_ALLOCATION_BIFURCATING_TAB_HPP_ */
