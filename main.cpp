#include "uthread.h"
#include "Lock.h"
#include "CondVar.h"
#include <cassert>
#include <cstdlib>
#include <iostream>

using namespace std;

#define UTHREAD_TIME_QUANTUM 10000
#define SHARED_BUFFER_SIZE 10
#define PRINT_FREQUENCY 100000
#define RANDOM_YIELD_PERCENT 50

// Shared buffer
static int buffer[SHARED_BUFFER_SIZE];
static int head = 0;
static int tail = 0;
static int item_count = 0;

// Shared buffer synchronization
static Lock buffer_lock;
static CondVar need_space_cv;
static CondVar need_item_cv;

// Bookkeeping
static int produced_count = 0;
static int consumed_count = 0;
static bool producer_in_critical_section = false;
static bool consumer_in_critical_section = false;

// Verify the buffer is in a good state
void assert_buffer_invariants() {
  assert(item_count <= SHARED_BUFFER_SIZE);
  assert(item_count >= 0);
  assert(head < SHARED_BUFFER_SIZE);
  assert(head >= 0);
  assert(tail < SHARED_BUFFER_SIZE);
  assert(tail >= 0);

  if (head > tail) {
    assert((head - tail) == item_count);
  }
  else if (head < tail) {
    assert(((SHARED_BUFFER_SIZE - tail) + head) == item_count);
  }
  else {
    assert((item_count == SHARED_BUFFER_SIZE) || (item_count == 0));
  }

  assert(produced_count >= consumed_count);
}

void* producer(void *arg) {
  while (true) {
    buffer_lock.lock();

    // Wait for room in the buffer if needed
    // NOTE: Assuming Hoare semantics
    if (item_count == SHARED_BUFFER_SIZE) {
      need_space_cv.wait(buffer_lock);
    }

    // Make sure synchronization is working correctly
    assert(!producer_in_critical_section);
    producer_in_critical_section = true;
    assert_buffer_invariants();

    // Place an item in the buffer
    buffer[head] = uthread_self();
    head = (head + 1) % SHARED_BUFFER_SIZE;
    item_count++;
    produced_count++;

    // Signal that there is now an item in the buffer
    need_item_cv.signal();

    producer_in_critical_section = false;
    buffer_lock.unlock();

    // Randomly give another thread a chance
    if ((rand() % 100) < RANDOM_YIELD_PERCENT) {
      uthread_yield();
    }
  }

  return nullptr;
}

void* consumer(void *arg) {
  while (true) {
    buffer_lock.lock();

    // Wait for an item in the buffer if needed
    // NOTE: Assuming Hoare semantics
    if (item_count == 0) {
      need_item_cv.wait(buffer_lock);
    }

    // Make sure synchronization is working correctly
    assert(!consumer_in_critical_section);
    consumer_in_critical_section = true;
    assert_buffer_invariants();

    // Grab an item from the buffer
    int item = buffer[tail];
    tail = (tail + 1) % SHARED_BUFFER_SIZE;
    item_count--;
    consumed_count++;

    // Print an update periodically
    if ((consumed_count % PRINT_FREQUENCY) == 0) {
      cout << "Consumed " << consumed_count << " items" << endl;
    }

    // Signal that there is now room in the buffer
    need_space_cv.signal();

    consumer_in_critical_section = false;
    buffer_lock.unlock();

    // Randomly give another thread a chance
    if ((rand() % 100) < RANDOM_YIELD_PERCENT) {
      uthread_yield();
    }
  }

  return nullptr;
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    cerr << "Usage: ./uthread-sync-demo <num_producer> <num_consumer>" << endl;
    cerr << "Example: ./uthread-sync-demo 20 20" << endl;
    exit(1);
  }

  int producer_count = atoi(argv[1]);
  int consumer_count = atoi(argv[2]);

  if ((producer_count + consumer_count) > 99) {
    cerr << "Error: <num_producer> + <num_consumer> must be <= 99" << endl;
    exit(1);
  }

  // Init user thread library
  int ret = uthread_init(UTHREAD_TIME_QUANTUM);
  if (ret != 0) {
    cerr << "Error: uthread_init" << endl;
    exit(1);
  }

  // Create producer threads
  int *producer_threads = new int[producer_count];
  for (int i = 0; i < producer_count; i++) {
    producer_threads[i] = uthread_create(producer, nullptr);
    if (producer_threads[i] < 0) {
      cerr << "Error: uthread_create producer" << endl;
    }
  }

  // Create consumer threads
  int *consumer_threads = new int[consumer_count];
  for (int i = 0; i < consumer_count; i++) {
    consumer_threads[i] = uthread_create(consumer, nullptr);
    if (consumer_threads[i] < 0) {
      cerr << "Error: uthread_create consumer" << endl;
    }
  }

  // NOTE: Producers and consumers run until killed but if we wanted to
  //       join on them do the following

  // Wait for all producers to complete
  for (int i = 0; i < producer_count; i++) {
    int result = uthread_join(producer_threads[i], nullptr);
    if (result < 0) {
      cerr << "Error: uthread_join producer" << endl;
    }
  }

  // Wait for all consumers to complete
  for (int i = 0; i < consumer_count; i++) {
    int result = uthread_join(consumer_threads[i], nullptr);
    if (result < 0) {
      cerr << "Error: uthread_join consumer" << endl;
    }
  }

  delete[] producer_threads;
  delete[] consumer_threads;

  return 0;
}
