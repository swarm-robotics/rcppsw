/*******************************************************************************
 * Name            : kmeans_cluster_worker.hpp
 * Project         : rcsw
 * Module          : algorithm
 * Description     : Workers for pthread-based implementation of K-means
 *                   clustering
 * Creation Date   : 02/02/17
 * Copyright       : Copyright 2017 John Harwell, All rights reserved
 *
 ******************************************************************************/

#ifndef INCLUDE_KMEANS_CLUSTER_WORKER_HPP_
#define INCLUDE_KMEANS_CLUSTER_WORKER_HPP_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <vector>
#include <limits>
#include "include/threadable.hpp"
#include "include/kmeans_cluster.hpp"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
namespace rcppsw {

/*******************************************************************************
 * Structure Definitions
 ******************************************************************************/

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
template <typename T> class kmeans_cluster_worker: public threadable {
 public:
  /* constructors */
  kmeans_cluster_worker(void);

  /* member functions */
  static void clusters(std::vector<kmeans_cluster<T>>* c) { clusters_ = c; }
  static void n_threads(int n) { n_threads_ = n; }

  void* thread_main(void* arg) {
    if (NULL == arg) {
          clusters_->at(self()).update_center();
          return NULL;
    }
    std::vector<T>* data = static_cast<std::vector<T>*>(arg);
    for (std::size_t i = data->size()/n_threads_; i < data->size()/n_threads_ + n_threads_; ++i) {
      int min_queue;
      float min_dist = std::numeric_limits<float>::max();
      for (std::size_t j = 0; j < clusters_->at(j).size(); ++j) {
        float dist = euclidean_dist(clusters_->at(j).center(), data->at(i));
        if (dist < min_dist) {
          min_dist = dist;
          min_queue = j;
        }
      } /* for(j..) */
      clusters_->at(min_queue).add(&data->at(i));
    } /* for(i..) */
    return NULL;
  } /* kmeans_cluster_worker::thread_main() */

 private:
  /* member functions */
  float euclidean_dist(const T& p1,
                       const T& p2) const {
    float dist = -1;
    for (auto& e1 : p1) {
      for (auto& e2 : p2) {
        dist += std::pow(e1 - e2, 2);
      } /* for(e2...) */
    } /* for(e1...) */

    return dist;
  } /* kmeans_cluster_worker::euclidean_dist() */

  /* data members */
  static std::vector<kmeans_cluster<T>>* clusters_;
  static int n_threads_;
};

} /* namespace rcppsw */

#endif /* INCLUDE_KMEANS_CLUSTER_WORKER_HPP_ */