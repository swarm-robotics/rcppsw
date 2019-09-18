/**
 * @file bi_tdgraph_allocator.cpp
 *
 * @copyright 2019 John Harwell, All rights reserved.
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
#include "rcppsw/ta/bi_tdgraph_allocator.hpp"

#include <vector>
#include <algorithm>

#include "rcppsw/ta/ds/bi_tdgraph.hpp"
#include "rcppsw/ta/executable_task.hpp"
#include "rcppsw/ta/polled_task.hpp"

/*******************************************************************************
 * Namespaces/Decls
 ******************************************************************************/
NS_START(rcppsw, ta);

/*******************************************************************************
 * Member Functions
 ******************************************************************************/
polled_task* bi_tdgraph_allocator::alloc_stoch_greedy_nbhd(
    const polled_task* current_task) const {

    /*
     * If there is no active TAB, then the root task is not partitionable, so
     * return it.
     */
    if (nullptr == m_graph->active_tab()) {
      return m_graph->root();
    } else {
      /*
       * Update our active TAB so that we perform partitioning from the correct
       * place. We have to pass in the current_task(), because the TAB's active
       * task has already been updated to be NULL after the task finish/abort
       * that brought us to this function.
       */
      m_graph->active_tab_update(current_task, m_rng);

      return m_graph->active_tab()->task_allocate(m_rng);
    }
} /* alloc_stoch_greedy_nbhd() */

polled_task* bi_tdgraph_allocator::alloc_greedy_global(void) const {
    std::vector<const ta::time_estimate*> exec_ests(m_graph->n_vertices());
    m_graph->walk([&](const auto* task) {
        exec_ests[m_graph->vertex_id(task)] =
            &static_cast<const executable_task*>(task)->task_exec_estimate();
      });
    auto min_task = std::min_element(exec_ests.begin(), exec_ests.end(),
                                     [&](const auto* t1, const auto* t2) {
                                       return *t1 < *t2;
                                     });
    return m_graph->find_vertex(std::distance(exec_ests.begin(), min_task));
  } /* alloc_greedy_global() */

polled_task* bi_tdgraph_allocator::alloc_random(void) const {
  uint id = m_rng->uniform(0, m_graph->n_vertices() -1);
    return m_graph->find_vertex(id);
} /* alloc_random() */

NS_END(ta, rcppsw);