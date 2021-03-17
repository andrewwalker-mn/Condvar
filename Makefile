CC = g++
CFLAGS = -g -lrt --std=c++14
DEPS = TCB.h uthread.h uthread_private.h Lock.h CondVar.h SpinLock.h async_io.h
OBJ = TCB.o uthread.o Lock.o CondVar.o SpinLock.o async_io.o
MAIN_OBJ = main.o
TESTLOCK = TCB.o uthread.o Lock.o testLockBetter.o
BROADCAST = broadcasttest.o
ASYNC = asynctest.o
NONASYNC = nonasynctest.o
TESTSPIN = TCB.o uthread.o SpinLock.o testSpin.o
TESTCONTROL = TCB.o uthread.o testControl.o
TESTPRIO = priotest.o
TESTSLOCK = TCB.o uthread.o Lock.o shortTestLock.o
TESTSSPIN = TCB.o uthread.o SpinLock.o shortTestSpin.o
TESTSCONTROL = TCB.o uthread.o shortTestControl.o
SASYNC = sAsyncTest.o
SNONASYNC = sNonAsyncTest.o
TASYNC = tAsyncTest.o
TNONASYNC = tNonAsyncTest.o

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

uthread-sync-demo: $(OBJ) $(MAIN_OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

alltests: locktest broadcasttest asynctest spintest priotest controltest sControlTest sLockTest sSpinTest sAsyncTest sNonAsyncTest tAsyncTest tNonAsyncTest

broadcasttest: $(BROADCAST) $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

locktest: $(TESTLOCK)
	$(CC) -o $@ $^ $(CFLAGS)

asynctest: $(OBJ) $(ASYNC)
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

.PHONY: clean

clean:
	rm -f *.o uthread-sync-demo locktest spintest priotest controltest sControlTest sLockTest sSpinTest sAsyncTest sNonAsyncTest tAsyncTest tNonAsyncTest
