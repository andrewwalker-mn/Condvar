#ifndef LOCK_H
#define LOCK_H

#include "TCB.h"
#include <queue>

using namespace std;

// Synchronization lock
class Lock {
public:
  Lock();
  ~Lock();
  // Attempt to acquire lock. Grab lock if available, otherwise thread is
  // blocked until the lock becomes available
  void lock();

  // Unlock the lock. Wake up a blocked thread if any is waiting to acquire the
  // lock and hand off the lock
  void unlock();
private:
  // TODO - Add members as needed
  queue<TCB*> lockQueue;
  TCB* savedSignal;

  // Unlock the lock while interrupts have already been disabled
  // NOTE: Assumes interrupts are disabled
  void _unlock();

  // Let the lock know that it should switch to this thread after the lock has
  // been released (following Hoare semantics)
  // NOTE: Assumes interrupts are disabled
  void _signal(TCB *tcb);

  // Allow condition variable class access to Lock private members
  // NOTE: CondVar should only use _unlock() and _signal() private functions
  //       (should not access private variables directly)
  friend class CondVar;
};

#endif // LOCK_H
