#include "CondVar.h"
#include "uthread_private.h"

CondVar::CondVar() {

}

void CondVar::wait(Lock &lock) {
  setLock(lock);
  disableInterrupts();
  lock._unlock();
  _waiting.push(running);
  running->setState(BLOCK);
  switchThreads();
  enableInterrupts();
  lock.lock();
}

void CondVar::signal() {
  if(!_waiting.empty()) {
    disableInterrupts();
    TCB *next = _waiting.front();
    _waiting.pop();
    _lock._unlock();
    _lock._signal(running);
    switchToThread(next);
    enableInterrupts();
    _lock.lock();
  }
}

void CondVar::broadcast() {
  while(!_waiting.empty()) {
    disableInterrupts();
    TCB *next = _waiting.front();
    _waiting.pop();
    _lock._unlock();
    _lock._signal(running);
    switchToThread(next); // we move this to _signal?
    enableInterrupts();
    _lock.lock();
  }
}

void CondVar::setLock(Lock &lock) {
  _lock = lock;
}

// TODO
