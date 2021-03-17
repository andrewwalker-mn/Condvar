CC = g++
CFLAGS = -g -lrt --std=c++14
DEPS = TCB.h uthread.h uthread_private.h Lock.h CondVar.h SpinLock.h async_io.h
OBJ = TCB.o uthread.o Lock.o CondVar.o SpinLock.o async_io.o
MAIN_OBJ = main.o
TESTLOCK = TCB.o uthread.o Lock.o testLockBetter.o
BROADCAST = broadcasttest.o
ASYNC = testAsyncBetter.o
ASYNCPERFORMANCE = asynctest.o
NONASYNC = nonasynctest.o
TESTSPIN = TCB.o uthread.o SpinLock.o testSpinBetter.o
TESTLOCKPERFORMANCE = TCB.o uthread.o Lock.o testLock.o
TESTSPINPERFORMANCE = TCB.o uthread.o SpinLock.o testSpin.o 
TESTCONTROL = TCB.o uthread.o testControl.o
TESTPRIO = priotest.o
TESTSLOCK = TCB.o uthread.o Lock.o shortTestLock.o
TESTSSPIN = TCB.o uthread.o SpinLock.o shortTestSpin.o
TESTSCONTROL = TCB.o uthread.o shortTestControl.o
SASYNC = sAsyncTest.o
SNONASYNC = sNonAsyncTest.o
TASYNC = tAsyncTest.o
TNONASYNC = tNonAsyncTest.o
TASYNCMW = tAsyncTestMoreWork.o
TNONASYNCMW = tNonAsyncTestMoreWork.o

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

uthread-sync-demo: $(OBJ) $(MAIN_OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

functionalitytests: locktest broadcasttest asynctest spintest priotest uthread-sync-demo

alltests: uthread-sync-demo locktest broadcasttest asynctest spintest priotest controltest sControlTest sLockTest sSpinTest sAsyncTest sNonAsyncTest tAsyncTest tNonAsyncTest asyncTestPerformance tNonAsyncTestMoreWork tAsyncTestMoreWork testLockPerformance testSpinPerformance 

broadcasttest: $(BROADCAST) $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

locktest: $(TESTLOCK)
	$(CC) -o $@ $^ $(CFLAGS)

asynctest: $(OBJ) $(ASYNC)
	$(CC) -o $@ $^ $(CFLAGS)

asyncTestPerformance: $(OBJ) $(ASYNCPERFORMANCE)
	$(CC) -o $@ $^ $(CFLAGS)

nonasynctest: $(OBJ) $(NONASYNC)
	$(CC) -o $@ $^ $(CFLAGS)

spintest: $(TESTSPIN)
	$(CC) -o $@ $^ $(CFLAGS)

priotest: $(OBJ) $(TESTPRIO)
	$(CC) -o $@ $^ $(CFLAGS)

controltest: $(OBJ) $(TESTCONTROL)
	$(CC) -o $@ $^ $(CFLAGS)

sControlTest: $(OBJ) $(TESTSCONTROL)
	$(CC) -o $@ $^ $(CFLAGS)

sLockTest: $(OBJ) $(TESTSLOCK)
	$(CC) -o $@ $^ $(CFLAGS)

sSpinTest: $(OBJ) $(TESTSSPIN)
	$(CC) -o $@ $^ $(CFLAGS)

sAsyncTest: $(OBJ) $(SASYNC)
	$(CC) -o $@ $^ $(CFLAGS)

sNonAsyncTest: $(OBJ) $(SNONASYNC)
	$(CC) -o $@ $^ $(CFLAGS)

tNonAsyncTest: $(OBJ) $(TNONASYNC)
	$(CC) -o $@ $^ $(CFLAGS)

tAsyncTest: $(OBJ) $(TASYNC)
	$(CC) -o $@ $^ $(CFLAGS)
	
tAsyncTestMoreWork: $(OBJ) $(TASYNCMW)
	$(CC) -o $@ $^ $(CFLAGS)
	
tNonAsyncTestMoreWork: $(OBJ) $(TNONASYNCMW)
	$(CC) -o $@ $^ $(CFLAGS)
	
testLockPerformance: $(OBJ) $(TESTLOCKPERFORMANCE)
	$(CC) -o $@ $^ $(CFLAGS)
	
testSpinPerformance: $(OBJ) $(TESTSPINPERFORMANCE)
	$(CC) -o $@ $^ $(CFLAGS)
	
.PHONY: clean

clean:
	rm -f *.o uthread-sync-demo locktest spintest priotest controltest sControlTest sLockTest sSpinTest sAsyncTest sNonAsyncTest tAsyncTest tNonAsyncTest asyncTestPerformance tNonAsyncTestMoreWork tAsyncTestMoreWork testLockPerformance testSpinPerformance
