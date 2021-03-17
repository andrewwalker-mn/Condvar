**Names: David Ma (maxxx818) and Andrew Walker (walk0655)**

# Running and Testing the Code:
A simple make command compiles everything, as does 'make alltests'. From there, specific tests are specified below.

We have 3 tests: locktest, broadcasttest, and the provided uthread-sync-demo-solution.

# Functionality Tests:
Performance tests are described further down in the README.

### ./locktest
Source code in testLockBetter.cpp.
This test shows functionality of locks, including their lock and unlock functions. It demonstrates the same code, first without locks and then with locks. Each thread simply prints their ID's and some numbers. The first printouts - without locks - clearly show less organization, while once locks are included the printouts print neatly. 

### ./broadcasttest
Source code in broadcasttest.cpp.
This test shows the functionality of the condvar broadcast() function. It simply creates 10 threads that call wait before an 11th thread calls broadcast, which causes the first 
10 threads to finish one after the other, before finally returning to the 11th thread and completing. Each thread simply prints a static int that gets incremented over time.

### ./uthread-sync-demo-solution
Source code given in main.cpp.
This is the given demo file, and shows functionalities of Lock, as well as Condvar wait() and signal(). As described, it has certain threads that produce input into a bounded buffer and certain threads that consume said input. The synchronicity is to make sure that none of the buffer invariants are violated; we see that the program can run for an indefinite amount of time, which means that everything is working. 

### ./spintest
Source code given in testSpinBetter.cpp.
Testing spinlocks is a little pointless in this environment, so the test is very similar to locktest; it demonstrates the same code with spinlocks and without spinlocks. Each thread simply prints their ID's and some numbers. The first printouts - without locks - clearly show less organization, while once locks are included the printouts print neatly. 

### ./priotest
Tests priority inversion; see description below.

### ./asynctest
Source code given in testAsyncBetter.cpp. Tests Asynchronous I/O, specifically the functions async_read() and async_write(). It runs three threads: a reading thread, a writing thread, and a normal thread. The reading and writing threads respectively call async_read() and async_write() and run first, and then the normal one prints numbers. The reading and writing threads show printouts after reading and writing is finished, and we can see that said printouts appear after the normal thread has already gotten some work done. Because the writing thread is writing more, it finishes after the normal thread has gotten more work done.

# Performance Evaluation
The performance evaluations, tests, and write-ups for **Lock vs SpinLock** and **Synchronous vs Asynchronous I/O** can be found in the file writeup.pdf.

## Priority Inversion:
### Implementation
We chose to implement priority boosting, which periodically boosts the priority of low-priority threads holding locks. Specifically, we add a static counter to the main uthread scheduler that counts the number of scheduler interrupts. Every x interrupts (currently we have x set to 20), the signal handler will iterate through the low priority queues and increase the priority the first one that it finds holding a lock. As part of this implementation, we also had to make some slight adjustments to Lock and Condvar so that we could track the number of locks that a thread is holding. 

### Testing
The test file is priotest.cpp, which compiles into ./priotest. 
The test runs three threads, each with their own respective functions. They're split into the three priorities - high, normal, and low. The first thread is low priority, but it also acquires the lock first, which means that it prevents the high priority thread from running. The test uses printouts to show which thread is running at any given point in time. Notice that without priority boosting (which can be simulated by commenting out the priority_boost function call in timeHandler() in uthread.cpp), the low priority thread must wait for the normal priority thread to finish before it can finish, which means that the high priority thread finishes only after both the normal and low priority threads have finished. With priority boosting, the low thread gets to finish before the normal thread, which means the high thread gets to finish faster as well. 
