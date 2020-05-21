#pragma once

#include <twist/stdlike/atomic.hpp>
#include <twist/twisted/futex.hpp>

namespace solutions {

class ConditionVariable {
 public:
  ConditionVariable() {
    state_.store(0);
  }

  template <class Mutex>
  void Wait(Mutex& mutex) {
    uint32_t curr_state = state_.load();
    mutex.unlock();
    futex_.Wait(curr_state);
    mutex.lock();
  }

  void NotifyOne() {
    state_.fetch_add(1);
    futex_.WakeOne();
  }

  void NotifyAll() {
    state_.fetch_add(1);
    futex_.WakeAll();
  }

 private:
  twist::stdlike::atomic<uint32_t> state_;
  twist::twisted::Futex futex_{state_};
};

}  // namespace solutions
