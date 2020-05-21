#pragma once

#include <twist/strand/spin_wait.hpp>
#include <twist/stdlike/atomic.hpp>
#include <twist/twisted/futex.hpp>

namespace solutions {

using twist::strand::SpinWait;
using twist::twisted::Futex;

class Mutex {
 public:
  void Lock() {
    while (state_.exchange(1) != 0) {
      wake_count_.fetch_add(1);
      futex_.Wait(1);
      wake_count_.fetch_sub(1);
    }
  }

  void Unlock() {
    state_.store(0);
    if (wake_count_ > 0) {
      futex_.Wake(1);
    }
  }

 private:
  twist::stdlike::atomic<uint32_t> wake_count_{0};
  twist::stdlike::atomic<uint32_t> state_{0};
  Futex futex_{state_};
};

}  // namespace solutions
