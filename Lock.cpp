#include "Lock.h"
#include "uthread_private.h"

using namespace std;

#define BUSY 1
#define FREE 0

Lock::Lock() {
  value = FREE;
}

Lock::~Lock() {
}

void Lock::lock() {
  disableInterrupts();
  if (value == FREE) {
    running->increaseLockCount();
    value = BUSY;
  }
  else {
    lockQueue.push(running);
    switchThreads();
  }
  enableInterrupts();
}

void Lock::unlock() {
  disableInterrupts();
  _unlock();
  enableInterrupts();
  return;
}

void Lock::_unlock() {
  if (savedSignal) {
    TCB* temp = savedSignal;
    savedSignal = nullptr;
    running->decreaseLockCount();
    temp->increaseLockCount();

    addToReady(running);
    switchToThread(temp);
  }
  else if (!lockQueue.empty()) {
    TCB *next = lockQueue.front();
    running->decreaseLockCount();
    next->increaseLockCount();
    lockQueue.pop();
    addToReady(next);
  }
  else {
    running->decreaseLockCount();
    value = FREE;
  }
  return;
}

void Lock::_signal(TCB *tcb) {
  savedSignal = tcb;
  return;
}
