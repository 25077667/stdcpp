#ifndef __SCC_STDCPP_SHARED_MUTEX_HPP__
#define __SCC_STDCPP_SHARED_MUTEX_HPP__
#pragma once

#include <condition_variable>
#include <mutex>
#include <thread>

namespace stdcpp {
namespace v1 {
class shared_mutex {
 public:
  shared_mutex() = default;
  ~shared_mutex() = default;

  shared_mutex(const shared_mutex&) = delete;
  shared_mutex& operator=(const shared_mutex&) = delete;

  // Exclusive locking
  void lock() {
    std::unique_lock<std::mutex> lk(mtx);
    exclusive_wait.wait(lk, [this] { return shared_count == 0 && !exclusive; });
    exclusive = true;
  }

  bool try_lock() {
    std::lock_guard<std::mutex> lk(mtx);
    if (shared_count == 0 && !exclusive) {
      exclusive = true;
      return true;
    }
    return false;
  }

  void unlock() {
    {
      std::lock_guard<std::mutex> lk(mtx);
      exclusive = false;
    }
    cv.notify_all();
  }

  // Shared locking
  void lock_shared() {
    std::unique_lock<std::mutex> lk(mtx);
    cv.wait(lk, [this] { return !exclusive; });
    ++shared_count;
  }

  bool try_lock_shared() {
    std::lock_guard<std::mutex> lk(mtx);
    if (!exclusive) {
      ++shared_count;
      return true;
    }
    return false;
  }

  void unlock_shared() {
    {
      std::lock_guard<std::mutex> lk(mtx);
      --shared_count;
      if (shared_count == 0) {
        exclusive_wait.notify_one();
      }
    }
    if (shared_count == 0) {
      cv.notify_one();
    }
  }

  // Native handle (implementation-defined)
  // This example does not implement a native_handle method.

 private:
  std::mutex mtx;
  std::condition_variable cv;
  std::condition_variable exclusive_wait;
  int shared_count = 0;
  bool exclusive = false;
};
}  // namespace v1

using shared_mutex = v1::shared_mutex;
}  // namespace stdcpp

#endif  // __SCC_STDCPP_SHARED_MUTEX_HPP__