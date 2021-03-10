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
    switchThreads();
    // switchToThread(lockQueue.front());
    // uthread_suspend(running->getId()); //suspend or switch?
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
  lockQueue.pop();
  if (savedSignal) {
    TCB* temp = savedSignal;
    savedSignal = nullptr;
    addToReady(temp);
    // switchToThread(temp);
    // uthread_resume(savedSignal->getId());
  }
  if (!lockQueue.empty()) {
    addToReady(lockQueue.front());
    // switchToThread(lockQueue.front());
    // uthread_resume(lockQueue.front()->getId());
  }
  return;
}

void Lock::_signal(TCB *tcb) {
  savedSignal = tcb;
  return;
}


