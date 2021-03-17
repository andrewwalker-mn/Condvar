Names: David Ma (maxxx818) and Andrew Walker (walk0655)

# Running and Testing the Code:
A simple make command compiles everything, as does 'make alltests'. From there, specific tests are specified below.

We have 3 tests: locktest, broadcasttest, and the provided uthread-sync-demo-solution.

## Tests:
### ./locktest
Source code in testLock.cpp.
This test shows functionality of locks. If you comment out lines 12, 25, 26, and 39 (lock and unlock), you will notice the threads run at the same time. 
If you leave them uncommented, they finish their cout loops atomically. 

### ./broadcasttest
Source code in broadcasttest.cpp.
This test shows the functionality of the condvar broadcast function. It simply creates 10 threads that call wait before an 11th thread calls broadcast, which causes the first 
10 threads to finish one after the other, before finally returning to the 11th thread and completing. 

### ./uthread-sync-demo-solution
Source code given in main.cpp.
This is the given demo file, and shows functionality of Lock, as well as Broadcast wait and signal. As described, it has certain threads that produce input into a bounded buffer
and certain threads that consume said input. The synchronicity is to make sure that none of the buffer invariants are violated; we see that the program can run for an indefinite 
amount of time, which means that everything is working. 

## Priority Inversion:
### Implementation
We chose to implement priority boosting, which periodically boosts the priority of low-priority threads holding locks. Specifically, we add a static counter to the main uthread scheduler that counts the number of scheduler interrupts. Every x interrupts (currently we have x set to 20), the signal handler will iterate through the low priority queues and increase the priority the first one that it finds holding a lock. As part of this implementation, we also had to make some slight adjustments to Lock and Condvar so that we could track the number of locks that a thread is holding. 

### Testing
The test file is priotest.cpp, which compiles into ./priotest. 
The test runs three threads, each with their own respective functions. They're split into the three priorities - high, normal, and low. The first thread is low priority, but it also acquires the lock first, which means that it prevents the high priority thread from running. The test uses printouts to show which thread is running at any given point in time. Notice that without priority boosting (which can be simulated by commenting out the priority_boost function call in timeHandler() in uthread.cpp), the low priority thread must wait for the normal priority thread to finish before it can finish, which means that the high priority thread finishes only after both the normal and low priority threads have finished. With priority boosting, the low thread gets to finish before the normal thread, which means the high thread gets to finish faster as well. 
