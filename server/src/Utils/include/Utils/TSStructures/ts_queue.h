/*
 *  Powderpunk Server - A Pretentious Project
 *
 *  Author: Eduardo "mrtheduts" Souza
 *          eduardomoreirafds@gmail.com
 *
 *  Date:   09 Feb 2021
 *
 *  (C) Copyright 2021 Eduardo "mrtheduts" Souza All Rights Reserved
 *
 */
#ifndef TS_QUEUE_H
#define TS_QUEUE_H

#include <boost/fiber/condition_variable.hpp>
#include <boost/fiber/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include <mutex>
#include <queue>

/*
 * Basic class for tread-safe and fiber-safe queue operations.
 */
template <typename T>
class TSQueue {
 public:
  TSQueue() = default;
  virtual ~TSQueue(){};

  /*
   * Checks if queue is empty.
   *
   * @return a boolean indicating if queue is empty.
   */
  bool isEmpty() {
    std::lock_guard<std::mutex> lock(q_mutex);
    return queue_.empty();
  }

  /*
   * Checks queue size.
   *
   * @return a unsigned int indicating queue size.
   */
  unsigned int size() {
    std::lock_guard<std::mutex> lock(q_mutex);
    return queue_.size();
  }

  /*
   * Pops the first element of the queue.
   *
   * @return the first element of the queue.
   */
  T pop() {
    std::lock_guard<std::mutex> lock(q_mutex);
    T elem = queue_.front();
    queue_.pop();

    return elem;
  }

  /*
   * Pushes a new element to the queue.
   *
   * @param new element to be pushed to queue.
   */
  void push(const T& elem) {
    std::lock_guard<std::mutex> lock(q_mutex);
    queue_.push(elem);
  }

  /*
   * Checks if queue is empty.
   *
   * @return a boolean indicating if queue is empty.
   */
  bool fIsEmpty() {
    std::lock_guard<boost::fibers::mutex> lock(q_f_mutex);
    return queue_.empty();
  }

  /*
   * Checks queue size.
   *
   * @return a unsigned int indicating queue size.
   */
  unsigned int fSize() {
    std::lock_guard<boost::fibers::mutex> lock(q_f_mutex);
    return queue_.size();
  }

  /*
   * Pops the first element of the queue.
   *
   * @return the first element of the queue.
   */
  T fPop() {
    std::lock_guard<boost::fibers::mutex> lock(q_f_mutex);
    T elem = queue_.front();
    queue_.pop();

    return elem;
  }

  /*
   * Pushes a new element to the queue.
   *
   * @param new element to be pushed to queue.
   */
  void fPush(const T& elem) {
    std::lock_guard<boost::fibers::mutex> lock(q_f_mutex);
    queue_.push(elem);
  }

  /* Thread-level mutex */
  std::mutex q_mutex;

  /* Fiber-level mutex */
  boost::fibers::mutex q_f_mutex;

  /* Thread-level condition variable */
  boost::condition_variable q_cv;

  /* Fiber-level condition variable */
  boost::fibers::condition_variable q_f_cv;

 private:
  /* Non-thread-safe queue */
  std::queue<T> queue_;
};

#endif

