#include "uthread.h"
#include "Lock.h"
#include <math.h>

using namespace std;


// lockQueue.push(new TCB(0, ORANGE, NULL, NULL, READY));

// Shared buffer synchronization
static Lock test_lock;

void* tester(void *arg) {
  // test_lock.lock();
  for (int i=0; i<1000000; i++) {
    if (i%100000 == 0) {
      cout << i/100000;
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
  if (uthread_init(1) != 0)
    cout << "Init problem!" << endl;
  int *tester_threads = new int[3];
  for (int i = 0; i < 3; i++) {
    tester_threads[i] = uthread_create(tester, nullptr);
    if (tester_threads[i] < 0) {
      cerr << "Error: uthread_create tester" << endl;
    }
  }
  
  for (int i = 0; i < 3; i++) {
    int result = uthread_join(tester_threads[i], nullptr);
    if (result < 0) {
      cerr << "Error: uthread_join tester" << endl;
    }
  }
  delete[] tester_threads;
  return 0;
}
