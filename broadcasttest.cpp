
#include "uthread.h"
#include "Lock.h"
#include "CondVar.h"
#include <cassert>
#include <cstdlib>
#include <iostream>

using namespace std;

static Lock lock;
static CondVar cv;

#define UTHREAD_TIME_QUANTUM 100000000

static int count = 0;

void* waits(void * arg) {
  lock.lock();

  cout << count << endl;
  cv.wait(lock);

  cout << count << endl;
  count += 1;


  lock.unlock();

  return nullptr;
}

void* signals(void * arg) {
  lock.lock();

  cout << "broadcasting" << endl;
  cv.broadcast();
  cout << "aaaaaaaand we're back in signals" << endl;

  lock.unlock();

  return nullptr;
}

int main(int argc, char*argv[]) {

  int ret = uthread_init(UTHREAD_TIME_QUANTUM);
  if (ret != 0) {
    cerr << "Error: uthread_init" << endl;
    exit(1);
  }

  // Create producer threads
  int *wait_threads = new int[10];
  for (int i = 0; i < 10; i++) {
    wait_threads[i] = uthread_create(waits, nullptr);
    if (wait_threads[i] < 0) {
      cerr << "Error: uthread_create waits" << endl;
    }
  }

  int signal_thread = uthread_create(signals, nullptr);

  for (int i = 0; i < 10; i++) {
    int result = uthread_join(wait_threads[i], nullptr);
    if (result < 0) {
      cerr << "Error: uthread_join waits" << endl;
    }
  }

  uthread_join(signal_thread, nullptr);

  delete[] wait_threads;

  return 0;
}
