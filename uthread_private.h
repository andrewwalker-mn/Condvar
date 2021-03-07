// This file contains uthread library functionality that is exposed for other
// uthread library modules to access. This functionality should not be accessed
// directly by user code (user code should use the interface provided by the
// public uthread.h header file)

#ifndef UTHREAD_PRIVATE
#define UTHREAD_PRIVATE

#include "TCB.h"

extern TCB* running; // The "Running" thread

// Switch to the next thread on the ready queue
// NOTE: switchThreads does not move the running thread to another queue. This
//       is the responsibility of the caller. switchThreads does set the state
//       of the newly running thread to RUNNING
void switchThreads();

// Switch to the thread provided
// NOTE: Same note for switchThreads applies for switchToThread
void switchToThread(TCB *tcb);

// Add the provided thread to the ready queue
void addToReady(TCB* th);

// Disable/enable interrupts
void disableInterrupts();
void enableInterrupts();

#endif // UTHREAD_PRIVATE
