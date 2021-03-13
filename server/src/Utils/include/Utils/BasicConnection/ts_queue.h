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
#ifndef TSQUEUE_H
#define TSQUEUE_H

#include <boost/fiber/mutex.hpp>
#include <mutex>
#include <queue>

template <typename T>
class TSQueue {
 public:
  TSQueue() = default;
  virtual ~TSQueue(){};

  bool IsEmpty() {
    std::lock_guard<std::mutex> lock(mutex_);
    return queue_.empty();
  }

  unsigned int Size() {
    std::lock_guard<std::mutex> lock(mutex_);
    return queue_.size();
  }

  T Pop() {
    std::lock_guard<std::mutex> lock(mutex_);
    T elem = queue_.front();
    queue_.pop();

    return elem;
  }
  void Push(const T& elem) {
    std::lock_guard<std::mutex> lock(mutex_);
    queue_.push(elem);
  }

  boost::fibers::mutex GetFMutex() { return this->f_mutex_; }
  std::mutex GetMutex() { return this->mutex_; }

 protected:
  std::mutex mutex_;
  boost::fibers::mutex f_mutex_;
  std::queue<T> queue_;
};

#endif

