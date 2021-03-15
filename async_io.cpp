#include "async_io.h"
#include "uthread.h"
#include <aio.h>
#include <errno.h>
#include <iostream>
#include <cstring>
#include <cstdlib>

using namespace std;

// TODO
// look into aio_read, aio_write, aio_error, and aio_return()
// These functions will initiate an I/O request with the operating system and then switch to another uthread
// thread until successfully polling for completion. This means that the asynchronous I/O functions will be
// synchronous with respect to the calling thread but asynchronous in terms of letting other threads complete
// work in the meantime. The aio_read(), aio_write(), aio_error(), and aio_return() system calls (refer to
// their man pages) will be helpful for carrying out asynchronous I/O requests.

ssize_t async_read(int fd, void *buf, size_t count, int offset) {
  struct aiocb aiocb = {0};

  aiocb.aio_fildes = fd;
  aiocb.aio_offset = offset;
  aiocb.aio_buf = buf;
  aiocb.aio_nbytes = count;

  int ret = aio_read(&aiocb);

  if (ret == -1) {
    cout << "error w/ aioread";
    return -1;
  }

  int error;

  while ((error = aio_error(&aiocb)) == EINPROGRESS){
      uthread_yield();
  };

  error = aio_error(&aiocb);
  ssize_t returned = aio_return(&aiocb);

  if (error != 0) {
    cout << "error 2 w/ aioread: " << strerror(error) << endl;
    return -1;
  }

  return returned;
};


ssize_t async_write(int fd, void *buf, size_t count, int offset) {
  struct aiocb aiocb = {0};

  aiocb.aio_fildes = fd;
  aiocb.aio_offset = offset;
  aiocb.aio_buf = buf;
  aiocb.aio_nbytes = count;

  int ret = aio_write(&aiocb);

  if (ret == -1) {
    cout << "error w/ aiowrite";
    return -1;
  }

  int error;

  while ((error = aio_error(&aiocb)) == EINPROGRESS){
    cout << "not done yet, yielding" << endl;
    uthread_yield();
  };
  cout << "finished writing" << endl;

  error = aio_error(&aiocb);
  ssize_t returned = aio_return(&aiocb);

  if (error != 0) {
    cout << "error 2 w/ aiowrite: " << strerror(error) << endl;
    return -1;
  }

  return returned;
};
