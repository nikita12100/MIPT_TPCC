#pragma once

#include <twist/stdlike/mutex.hpp>
#include <twist/stdlike/condition_variable.hpp>

#include <cstddef>

namespace solutions {

class CyclicBarrier {
 public:
  explicit CyclicBarrier(size_t number_threads)
      : barrier_threads_(number_threads),
        need_threads_(number_threads),
        generation_(0) {
  }

  void Arrive() {
    std::unique_lock<twist::stdlike::mutex> lock(mutex_);
    auto last_generation = generation_;
    --need_threads_;
    if (need_threads_ == 0) {
      ++generation_;
      need_threads_ = barrier_threads_;
      cv_.notify_all();
    } else {
      cv_.wait(lock, [this, last_generation] {
        return last_generation != generation_;
      });
    }
  }

 private:
  twist::stdlike::mutex mutex_;
  const size_t barrier_threads_;  // our barrier
  size_t need_threads_;           // count of need threads

  twist::stdlike::condition_variable cv_;
  size_t generation_;
};

}  // namespace solutions
