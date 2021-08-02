/*
 *  Powderpunk Server - A Pretentious Project
 *
 *  Author: Eduardo "mrtheduts" Souza
 *          eduardomoreirafds@gmail.com
 *
 *  Date:   01 Aug 2021
 *
 *  (C) Copyright 2021 Eduardo "mrtheduts" Souza All Rights Reserved
 *
 */
#ifndef TS_MAP_H
#define TS_MAP_H

#include <boost/fiber/condition_variable.hpp>
#include <boost/fiber/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include <mutex>
#include <queue>

/*
 * Basic class for tread-safe and fiber-safe map operations.
 */
template <class Key, class T>
class TSMap {
 public:
  TSMap() = default;
  virtual ~TSMap(){};

  /*
   * Checks if map is empty.
   *
   * @return a boolean indicating if map is empty.
   */
  bool isEmpty() {
    std::lock_guard<std::mutex> lock(m_mutex);
    return map_.empty();
  }

  /*
   * Checks map size.
   *
   * @return a unsigned int indicating map size.
   */
  unsigned int size() {
    std::lock_guard<std::mutex> lock(m_mutex);
    return map_.size();
  }

  /*
   * Get the element associated with parameter.
   *
   * @param key
   * @return the first element of the queue.
   */
  T get(Key key) {
    std::lock_guard<std::mutex> lock(m_mutex);
    T elem = map_[key];
    return elem;
  }

  /*
   * Put a new element to the map with associated key.
   *
   * @param key of the new element.
   * @param new element to be added to the map.
   */
  void put(const Key key, const T& elem) {
    std::lock_guard<std::mutex> lock(m_mutex);
    map_[key] = elem;
  }

  /*
   * Checks if map is empty.
   *
   * @return a boolean indicating if map is empty.
   */
  bool fIsEmpty() {
    std::lock_guard<boost::fibers::mutex> lock(m_f_mutex);
    return map_.empty();
  }

  /*
   * Checks map size.
   *
   * @return a unsigned int indicating map size.
   */
  unsigned int fSize() {
    std::lock_guard<boost::fibers::mutex> lock(m_f_mutex);
    return map_.size();
  }

  /*
   * Get the element associated with parameter.
   *
   * @param key
   * @return the first element of the queue.
   */
  T fGet(Key key) {
    std::lock_guard<boost::fibers::mutex> lock(m_f_mutex);
    T elem = map_[key];
    return elem;
  }

  /*
   * Put a new element to the map with associated key.
   *
   * @param key of the new element.
   * @param new element to be added to the map.
   */
  void fPut(const Key key, const T& elem) {
    std::lock_guard<boost::fibers::mutex> lock(m_f_mutex);
    map_[key] = elem;
  }

  /* Thread-level mutex */
  std::mutex m_mutex;

  /* Fiber-level mutex */
  boost::fibers::mutex m_f_mutex;

  /* Thread-level condition variable */
  boost::condition_variable m_cv;

  /* Fiber-level condition variable */
  boost::fibers::condition_variable m_f_cv;

 private:
  /* Non-thread-safe map */
  std::map<Key, T> map_;
};

#endif

