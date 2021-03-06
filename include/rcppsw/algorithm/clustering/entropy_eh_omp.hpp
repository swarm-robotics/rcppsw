/**
 * \file entropy_eh_omp.hpp
 *
 * \copyright 2018 John Harwell, All rights reserved.
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
#ifndef INCLUDE_RCPPSW_ALGORITHM_CLUSTERING_ENTROPY_EH_OMP_HPP_
#define INCLUDE_RCPPSW_ALGORITHM_CLUSTERING_ENTROPY_EH_OMP_HPP_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <omp.h>
#include <vector>

#include "rcppsw/rcppsw.hpp"
#include "rcppsw/algorithm/clustering/eh_clustering_impl.hpp"

/*******************************************************************************
 * Namespaces/Decls
 ******************************************************************************/
NS_START(rcppsw, algorithm, clustering);

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
/**
 * \class entropy_eh_omp
 * \ingroup algorithm clustering
 *
 * \brief Parallel clustering using the Event Horizon (EH) membership policy
 * with OpenMP. Suitable for information entropy calculations.
 */
template <typename T>
class entropy_eh_omp final : public eh_clustering_impl<T> {
 public:
  using typename eh_clustering_impl<T>::cluster_vector;
  using typename eh_clustering_impl<T>::dist_calc_ftype;
  using eh_clustering_impl<T>::horizon;

  explicit entropy_eh_omp(size_t n_threads) : mc_n_threads(n_threads) {}

  void initialize(std::vector<T>* const data,
                  membership_type<policy::EH>* const membership) override {
    for (auto &m : *membership) {
      m.reserve(data->size());
    } /* for(&m..) */
  }

  void iterate(const std::vector<T>& data,
               const dist_calc_ftype& dist_func,
               cluster_vector* const clusters) override {
    #pragma omp parallel for num_threads(mc_n_threads)
    for (size_t i = 0; i < data.size(); ++i) {
      for (size_t j = 0; j < data.size(); ++j) {
        if (dist_func(data[i], data[j]) <= horizon()) {
          (*clusters)[i].add_point(j);
        }
      } /* for(j..) */
    } /* for(i..) */
  }

  bool converged(const cluster_vector& ) const override { return false; }
  void post_iter_update(cluster_vector* const) override {}

  size_t n_threads(void) const { return mc_n_threads; }

 private:
  /* clang-format off */
  const size_t mc_n_threads;
  /* clang-format on */
};

NS_END(clustering, algorithm, rcppsw);

#endif /* INCLUDE_RCPPSW_ALGORITHM_CLUSTERING_ENTROPY_EH_OMP_HPP_ */
