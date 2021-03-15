CC = g++
CFLAGS = -g -lrt --std=c++14
DEPS = TCB.h uthread.h uthread_private.h Lock.h CondVar.h SpinLock.h async_io.h
OBJ = TCB.o uthread.o Lock.o CondVar.o SpinLock.o async_io.o
MAIN_OBJ = main.o
TESTLOCK = TCB.o uthread.o Lock.o testLock.o
BROADCAST = broadcasttest.o
ASYNC = asynctest.o
TESTSPIN = TCB.o uthread.o SpinLock.o testSpin.o

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

uthread-sync-demo: $(OBJ) $(MAIN_OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

alltests: locktest broadcasttest

broadcasttest: $(BROADCAST) $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

locktest: $(TESTLOCK)
	$(CC) -o $@ $^ $(CFLAGS)

asynctest: $(ASYNC) $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

spintest: $(TESTSPIN)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o uthread-sync-demo locktest spintest
