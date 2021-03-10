#include "Lock.h"
#include "uthread_private.h"

using namespace std;
// TODO

Lock::Lock() {
}

Lock::~Lock() {
}

void Lock::lock() {
  disableInterrupts();
  if (lockQueue.empty()) {
    lockQueue.push(running);
  }
  else {
    lockQueue.push(running);
    enableInterrupts();
    uthread_suspend(running->getId());
  }
  enableInterrupts();
}

void Lock::unlock() {
  disableInterrupts();
  _unlock();
  enableInterrupts();
}

void Lock::_unlock() {
  lockQueue.pop();
  if (savedSignal) {
    uthread_resume(savedSignal->getId());
  }
  if (!lockQueue.empty()) {
    uthread_resume(lockQueue.front()->getId());
  }
}

void Lock::_signal(TCB *tcb) {
  return;
}
