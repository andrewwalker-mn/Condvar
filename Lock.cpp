#include "Lock.h"
#include "uthread_private.h"

using namespace std;
// TODO

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
    value = BUSY;
  }
  else {
    lockQueue.push(running);
    // change status to waiting? remove from ready?
    switchThreads();
    // addToReady(running);
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
  if (savedSignal) {
    TCB* temp = savedSignal;
    savedSignal = nullptr;
    // addToReady(temp);
    // cout << "unlocking and returning control to " << temp->getId() << endl;
    addToReady(running);
    switchToThread(temp);
  }
  else if (!lockQueue.empty()) {
    TCB *next = lockQueue.front();
    lockQueue.pop();
    // next->setState
    addToReady(next);
    // switchToThread(next);
  }
  else {
    value = FREE;
  }
  // if (savedSignal) {
    // TCB* temp = savedSignal;
    // savedSignal = nullptr;
    // addToReady(temp);
    // switchToThread(temp);
    // uthread_resume(savedSignal->getId());
  // }
  // if (!lockQueue.empty()) {
    // addToReady(lockQueue.front());
    // switchToThread(lockQueue.front());
    // uthread_resume(lockQueue.front()->getId());
  return;
}

void Lock::_signal(TCB *tcb) {
  // disableInterrupts();
  savedSignal = tcb;
  // enableInterrupts();
  return;
}
