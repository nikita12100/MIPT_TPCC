#include <twist/test_framework/test_framework.hpp>

#include <twist/fiber/core/api.hpp>
#include <twist/fiber/sync/mutex.hpp>

using twist::fiber::DeadlockDetected;
using twist::fiber::Mutex;
using twist::fiber::RunScheduler;
using twist::fiber::Spawn;
using twist::fiber::Yield;

TEST_SUITE(Deadlock) {
  // Deadlock with one fiber and one mutex
  SIMPLE_TEST(OneFiber) {
    Mutex mutex;

    auto fiber = [&]() {
      mutex.Lock();
      mutex.Lock();
    };

    // RunScheduler(fiber) should throw DeadlockDetected exception
    ASSERT_THROW(RunScheduler(fiber), DeadlockDetected);
  }

  // Deadlock with two fibers
  SIMPLE_TEST(TwoFibers) {
    Mutex mutex_1;
    Mutex mutex_2;
    auto finn = [&]() {
      mutex_1.Lock();
      Yield();
      mutex_2.Lock();
      mutex_1.Unlock();
      mutex_2.Unlock();
    };

    auto jake = [&]() {
      mutex_2.Lock();
      mutex_1.Lock();
      Yield();
      mutex_1.Unlock();
      mutex_2.Unlock();
    };

    auto adventure = [&]() {
      // Run two cooperative fibers
      Spawn(finn);
      Spawn(jake);
    };

    RunScheduler(finn);
    RunScheduler(finn);

    RunScheduler(jake);
    RunScheduler(jake);

    // Finn and Jake should block each other
    ASSERT_THROW(RunScheduler(adventure), DeadlockDetected);
  }
}

RUN_ALL_TESTS()
