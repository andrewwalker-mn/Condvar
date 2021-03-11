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
