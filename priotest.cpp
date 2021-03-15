#include "uthread.h"
#include "async_io.h"
#include "Lock.h"
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <math.h>
#include "uthread_private.h"

using namespace std;

#define UTHREAD_TIME_QUANTUM 10000

static Lock test_lock;

void* low_tester(void *arg) {
  test_lock.lock();
  uthread_decrease_priority(running->getId());
  for (int i=0; i<20000000; i++) {
    if (i%100000 == 0) {
      // cout << i/100000 << " ";
      cout << "low ";
    }
    else
    {
      double j = pow(i,3.0);
      j += j-pow(i,.7);
    }
  }
  cout << endl;
  test_lock.unlock();
  return nullptr;
}

void* high_tester(void *arg) {
  test_lock.lock();
  for (int i=0; i<20000000; i++) {
    if (i%100000 == 0) {
      // cout << i/100000 << " ";
      cout << "high ";
    }
    else
    {
      double j = pow(i,3.0);
      j += j-pow(i,.7);
    }
  }
  cout << endl;
  test_lock.unlock();
  return nullptr;
}

void* tester(void * arg) {
  for (int i=0; i<20000000; i++) {
    if (i%100000 == 0) {
      // cout << i/100000 << " ";
      cout << "norm ";
    }
    else
    {
      double j = pow(i,3.0);
      j += j-pow(i,.7);
    }
  }
  cout << endl;
  return nullptr;
}

int main(int argc, char *argv[]) {
  if (uthread_init(UTHREAD_TIME_QUANTUM) != 0)
    cout << "Init problem!" << endl;

  int low = uthread_create(low_tester, nullptr);
  int high = uthread_create(high_tester, nullptr);
  uthread_increase_priority(high);
  int norm = uthread_create(tester, nullptr);

  // int *tester_threads = new int[3];
  // for (int i = 0; i < 3; i++) {
  //   tester_threads[i] = uthread_create(tester, nullptr);
  //   if (tester_threads[i] < 0) {
  //     cerr << "Error: uthread_create tester" << endl;
  //   }
  // }

  uthread_join(low, nullptr);
  uthread_join(high, nullptr);
  uthread_join(norm, nullptr);
  cout << "got here" << endl;
  // for (int i = 0; i < 3; i++) {
  //   int result = uthread_join(tester_threads[i], nullptr);
  //   if (result < 0) {
  //     cerr << "Error: uthread_join tester" << endl;
  //   }
  // }
  // delete[] tester_threads;

  return 0;
}
