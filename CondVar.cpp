#include "CondVar.h"
#include "uthread_private.h"

CondVar::CondVar() {

}

void CondVar::wait(Lock &lock) {
  disableInterrupts();
  setLock(&lock);
  lock._unlock();
  _waiting.push(running);
  running->setState(BLOCK);
  // cout << running->getId() << " start waiting" << endl;
  switchThreads();
  // cout << running->getId() << " finished waiting" << endl;
  enableInterrupts();
  // lock.lock();
}

void CondVar::signal() {
  if(!_waiting.empty()) {
    disableInterrupts();
    TCB *next = _waiting.front();
    _waiting.pop();
    // cout << "size of waiting is: " << _waiting.size() << endl;
    // cout << next->getId() << endl;
    // _lock->_unlock();
    _lock->_signal(running);
    running->decreaseLockCount();
    next->increaseLockCount();
    // cout << "signalled, switching to thread " << next->getId() << endl;
    switchToThread(next);
    enableInterrupts();
    // _lock->lock();
  }
  // _lock->_unlock();
}

void CondVar::broadcast() {
  while(!_waiting.empty()) {
    disableInterrupts();
    TCB *next = _waiting.front();
    _waiting.pop();
    // _lock->unlock();
    _lock->_signal(running);
    running->decreaseLockCount();
    next->increaseLockCount();
    switchToThread(next);
    enableInterrupts();
    // _lock->lock();
  }
}

void CondVar::setLock(Lock* lock) {
  _lock = lock;
}

int CondVar::getWaiting() {
    return _waiting.size();
  }

// TODO
