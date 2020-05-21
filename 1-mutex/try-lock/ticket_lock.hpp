#pragma once

#include <twist/stdlike/atomic.hpp>
#include <twist/strand/spin_wait.hpp>

namespace solutions {

using twist::strand::SpinWait;

class TicketLock {
 public:
  // Don't change this method
  void Lock() {
    const size_t this_thread_ticket = next_free_ticket_.fetch_add(1);

    SpinWait spin_wait;
    while (this_thread_ticket != owner_ticket_.load()) {
      spin_wait();
    }
  }

  bool TryLock() {
    size_t current_thread_ticket = owner_ticket_.load();  // non const
    // if next_free = curr -> next_free = curr + 1 and ret true, else ret false
    return next_free_ticket_.compare_exchange_strong(current_thread_ticket,
                                                     current_thread_ticket + 1);
  }

  // Don't change this method
  void Unlock() {
    owner_ticket_.store(owner_ticket_.load() + 1);
  }

 private:
  twist::stdlike::atomic<size_t> next_free_ticket_{0};
  twist::stdlike::atomic<size_t> owner_ticket_{0};
};

}  // namespace solutions
