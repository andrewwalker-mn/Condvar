#include "CondVar.h"
#include "uthread_private.h"

CondVar::CondVar() {

}

void CondVar::wait(Lock &lock) {
  setLock(lock);
  lock._unlock();
  _waiting.push(running);
  uthread_suspend(running->getId());
  lock.lock();
}

void CondVar::signal() {
  if(!_waiting.empty()) {
    TCB* next = _waiting.front();
    _waiting.pop();
    _lock._unlock();
    _lock._signal(running);
    switchToThread(next); // we move this to _signal?
    _lock.lock();
  }
}

void CondVar::broadcast() {
  while(!_waiting.empty()) {
    TCB* next = _waiting.front();
    _waiting.pop();
    _lock._unlock();
    _lock._signal(running);
    switchToThread(next); // we move this to _signal?
    _lock.lock();
  }
}

void CondVar::setLock(Lock &lock) {
  _lock = lock;
}

// TODO
