#include "uthread.h"
#include "Lock.h"
#include <math.h>
#include "uthread_private.h"

using namespace std;

static Lock test_lock;

void* tester(void *arg) {
  test_lock.lock();
  cout << running->getId() << "\t";
  for (int i=0; i<3000000; i++) {
    if (i%1000000 == 0) {
      cout << i/1000000;
    }
    else
    {
      double j = pow(i,3.0);
      j += j-pow(i,.7);
    }
  }
  cout << endl;
  test_lock.unlock();
  test_lock.lock();
  cout << running->getId() << "\t";
  for (int i=3000000; i<6000000; i++) {
    if (i%1000000 == 0) {
      cout << i/1000000;
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

void* tester2(void *arg) {
  // test_lock.lock();
  cout << running->getId() << "\t";
  for (int i=0; i<3000000; i++) {
    if (i%1000000 == 0) {
      cout << i/1000000;
    }
    else
    {
      double j = pow(i,3.0);
      j += j-pow(i,.7);
    }
  }
  cout << endl;
  // test_lock.unlock();
  // test_lock.lock();
  cout << running->getId() << "\t";
  for (int i=3000000; i<6000000; i++) {
    if (i%1000000 == 0) {
      cout << i/1000000;
    }
    else
    {
      double j = pow(i,3.0);
      j += j-pow(i,.7);
    }
  }
  cout << endl;
  // test_lock.unlock();
  return nullptr;
}

int main(int argc, char *argv[]) {

  cout << "----------------- WITHOUT LOCKS ---------------" << endl;

  int numthreads = 5;
  if (uthread_init(1) != 0)
    cout << "Init problem!" << endl;
  int *tester_threads = new int[numthreads];
  for (int i = 0; i < numthreads; i++) {
    tester_threads[i] = uthread_create(tester2, nullptr);
    if (tester_threads[i] < 0) {
      cerr << "Error: uthread_create tester" << endl;
    }
  }

  for (int i = 0; i < numthreads; i++) {
    int result = uthread_join(tester_threads[i], nullptr);
    if (result < 0) {
      cerr << "Error: uthread_join tester" << endl;
    }
  }
  delete[] tester_threads;

  cout << "----------------- WITH LOCKS ---------------" << endl;

  numthreads = 5;

  tester_threads = new int[numthreads];
  for (int i = 0; i < numthreads; i++) {
    tester_threads[i] = uthread_create(tester, nullptr);
    if (tester_threads[i] < 0) {
      cerr << "Error: uthread_create tester" << endl;
    }
  }

  for (int i = 0; i < numthreads; i++) {
    int result = uthread_join(tester_threads[i], nullptr);
    if (result < 0) {
      cerr << "Error: uthread_join tester" << endl;
    }
  }
  delete[] tester_threads;



  return 0;
}
