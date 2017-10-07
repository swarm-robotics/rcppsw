/**
 * @file partitionable_task.hpp
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

#ifndef INCLUDE_RCPPSW_TASK_ALLOCATION_PARTITIONABLE_TASK_HPP_
#define INCLUDE_RCPPSW_TASK_ALLOCATION_PARTITIONABLE_TASK_HPP_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "rcppsw/task_allocation/logical_task.hpp"
#include "rcppsw/task_allocation/partition_probability.hpp"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NS_START(rcppsw, task_allocation);

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
/**
 * @brief A task that is capable of being partitioned into two subtasks that
 * when executed in sequence have the sum effect as the parent task.
 *
 * If, during task execution, the task is aborted, then one of the subtasks is
 * chosen randomly to be the new task.
 *
 * Any class that inherits from this class must also inherit from
 * \ref logical_task.
 */
template<class T1, class T2>
class partitionable_task {
  static_assert(std::is_base_of<logical_task, T1>::value,
                "FATAL: template argument must be a logical task");
  static_assert(std::is_base_of<logical_task, T2>::value,
                "FATAL: template argument must be a logical task");

 public:
  partitionable_task(void) {}
  virtual ~partitionable_task(void) {}

  virtual double partition_prob(void) = 0;

  logical_task* partition(void) {
    /* we are going to abort, so pick one of the two subtasks randomly */
    if ((static_cast<double>(rand()) / (RAND_MAX)) >= partition_prob()) {
      if (rand() % 2) {
        return m_partition1;
      } else {
        return m_partition2;
      }
    }
    return nullptr;
  }

 protected:
  const T1* partition1(void) const { return m_partition1; }
  const T2* partition2(void) const { return m_partition2; }
  void partition1(T1* partition1) { m_partition1 = partition1; }
  void partition2(T2* partition2) { m_partition2 = partition2; }

 private:
  T1 *m_partition1;
  T2 *m_partition2;
};

NS_END(task_allocation, rcppsw);

#endif /* INCLUDE_RCPPSW_TASK_ALLOCATION_PARTITIONABLE_TASK_TASK_HPP_ */