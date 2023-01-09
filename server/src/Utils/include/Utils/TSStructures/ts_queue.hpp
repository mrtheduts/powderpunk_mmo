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
#include <iostream>
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
    boost::unique_lock<boost::mutex> lock(q_mutex);
    return queue_.empty();
  }

  /*
   * Checks queue size.
   *
   * @return a unsigned int indicating queue size.
   */
  unsigned int size() {
    boost::unique_lock<boost::mutex> lock(q_mutex);
    return queue_.size();
  }

  /*
   * Pops the first element of the queue.
   *
   * @return the first element of the queue.
   */
  T pop() {
    boost::unique_lock<boost::mutex> lock(q_mutex);
    T elem = queue_.front();
    queue_.pop();
    lock.unlock();

    return elem;
  }

  /*
   * Pops all the elements of the queue.
   *
   * @return all elements of the queue.
   */
  std::queue<T> popAll() {
    boost::unique_lock<boost::mutex> lock(q_mutex);

    std::queue<T> elems;
    elems.swap(queue_);

    return elems;
  }

  /*
   * Pushes a new element to the queue.
   *
   * @param new element to be pushed to queue.
   */
  void push(const T& elem) {
    boost::unique_lock<boost::mutex> lock(q_mutex);
    queue_.push(elem);
    lock.unlock();
  }

  /*
   * Pushes a new element to the queue.
   *
   * @param new element to be pushed to queue.
   */
  void pushAll(std::queue<T> elems) {
    boost::unique_lock<boost::mutex> lock(q_mutex);
    while (!elems.empty()) {
      queue_.push(elems.front());
      elems.pop();
    }
  }

  /*
   * Checks if queue is empty.
   *
   * @return a boolean indicating if queue is empty.
   */
  bool fIsEmpty() {
    boost::unique_lock<boost::fibers::mutex> lock(q_f_mutex);
    return queue_.empty();
  }

  /*
   * Checks queue size.
   *
   * @return a unsigned int indicating queue size.
   */
  unsigned int fSize() {
    boost::unique_lock<boost::fibers::mutex> lock(q_f_mutex);
    return queue_.size();
  }

  /*
   * Pops the first element of the queue.
   *
   * @return the first element of the queue.
   */
  T fPop() {
    boost::unique_lock<boost::fibers::mutex> lock(q_f_mutex);
    T elem = queue_.front();
    queue_.pop();

    return elem;
  }

  /*
   * Pops all the elements of the queue.
   *
   * @return all elements of the queue.
   */
  std::queue<T> fPopAll() {
    boost::unique_lock<boost::fibers::mutex> lock(q_f_mutex);

    std::queue<T> elems;
    elems.swap(queue_);

    return elems;
  }

  /*
   * Pushes a new element to the queue.
   *
   * @param new element to be pushed to queue.
   */
  void fPush(const T& elem) {
    boost::unique_lock<boost::fibers::mutex> lock(q_f_mutex);
    queue_.push(elem);
  }

  /* Thread-level condition variable */
  boost::condition_variable q_cv;

  /* Fiber-level condition variable */
  boost::fibers::condition_variable q_f_cv;

 private:
  /* Thread-level mutex */
  boost::mutex q_mutex;

  /* Fiber-level mutex */
  boost::fibers::mutex q_f_mutex;

  /* Non-thread-safe queue */
  std::queue<T> queue_;
};

#endif

