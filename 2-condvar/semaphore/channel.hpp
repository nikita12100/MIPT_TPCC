#pragma once

#include "semaphore.hpp"

#include <deque>
#include <utility>

namespace solutions {

template <typename T>
class BufferedChannel {
 public:
  explicit BufferedChannel(size_t capacity)
      : not_empty_(Semaphore(capacity)),
        not_full_(Semaphore(0)),
        sem_mutex_(1) {
  }

  void Send(T item) {
    not_empty_.Acquire();
    sem_mutex_.Acquire();
    deque_.push_back(std::move(item));
    sem_mutex_.Release();
    not_full_.Release();
  }

  T Receive() {
    not_full_.Acquire();
    sem_mutex_.Acquire();
    T result = std::move(deque_.front());
    deque_.pop_front();
    sem_mutex_.Release();
    not_empty_.Release();
    return result;
  }

 private:
  std::deque<T> deque_;
  Semaphore not_empty_;  // from capacity -> 0
  Semaphore not_full_;   // from 0 - > capacity
  Semaphore sem_mutex_;  // mutex on semaphore
};

}  // namespace solutions
