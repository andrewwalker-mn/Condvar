#include "SpinLock.h"

#define BUSY 1
#define FREE 0

using namespace std;

SpinLock::SpinLock() {}

// SpinLock::~SpinLock() {}

// Acquire the lock. Spin until the lock is acquired if the lock is already
// held
void SpinLock::lock() {
  while (atomic_value.test_and_set(std::memory_order_acquire));
}

// Unlock the lock
void SpinLock::unlock() {
  atomic_value.clear(std::memory_order_release);
}
