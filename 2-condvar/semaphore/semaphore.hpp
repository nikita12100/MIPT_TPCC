#pragma once

#include <twist/stdlike/mutex.hpp>
#include <twist/stdlike/condition_variable.hpp>

namespace solutions {

class Semaphore {
 public:
  explicit Semaphore(size_t initial_count) : initial_count_(initial_count) {
  }

  void Acquire() {
    std::unique_lock<twist::stdlike::mutex> lock(mutex_);
    while (initial_count_ == 0) {
      cv_.wait(lock);
    }
    --initial_count_;
  }

  void Release() {
    std::unique_lock<twist::stdlike::mutex> lock(mutex_);
    ++initial_count_;
    cv_.notify_one();
  }

 private:
  size_t initial_count_;
  twist::stdlike::mutex mutex_;
  twist::stdlike::condition_variable cv_;
};

}  // namespace solutions
