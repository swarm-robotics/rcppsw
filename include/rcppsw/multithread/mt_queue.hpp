/**
 * @file mt_queue.hpp
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

#ifndef INCLUDE_RCPPSW_MULTITHREAD_MT_QUEUE_HPP_
#define INCLUDE_RCPPSW_MULTITHREAD_MT_QUEUE_HPP_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <deque>
#include <boost/thread.hpp>
#include <boost/thread/locks.hpp>

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
/**
 * @brief A simple multiple-producer/consumer queue with locking.
 */
template <typename T>
class mt_queue {
 public:
  mt_queue(void) : queue_(), mtx_(), cv_() {}
  /* type definitions */
  typedef typename std::deque<T>::const_iterator const_iterator;
  typename std::deque<T>::const_iterator begin(void) const { return queue_.begin(); }
  typename std::deque<T>::const_iterator end(void) const { return queue_.end(); }

  // Add data to the queue and notify others
  void enqueue(const T& data) {
    // Acquire lock on the queue
    boost::unique_lock<boost::mutex> lock(mtx_);

    // Add the data to the queue
    queue_.push_back(data);

    // Notify others that data is ready
    cv_.notify_one();
  }  // Lock is automatically released here

  // Get data from the queue. Wait for data if not available
  T dequeue() {
    // Acquire lock on the queue
    boost::unique_lock<boost::mutex> lock(mtx_);

    // When there is no data, wait till someone fills it.
    // Lock is automatically released in the wait and obtained
    // again after the wait
    while (queue_.size() == 0) cv_.wait(lock);

    // Retrieve the data from the queue
    T result = static_cast<T>(queue_.front());
    queue_.pop_front();
    return result;
  }  // Lock is automatically released here

  // get the size of the queue
  size_t size() const { return queue_.size(); }
  T front(void) { return queue_.front(); }
  void clear(void) { queue_.clear(); }
  const T& operator[](std::size_t pos) const { return queue_[pos]; }

 private:
  std::deque<T> queue_;           // Use STL queue to store data
  boost::mutex mtx_;              // The mutex to synchronise on
  boost::condition_variable cv_;  // The condition to wait for
};

#endif /* INCLUDE_RCPPSW_MULTITHREAD_MT_QUEUE_HPP_ */
