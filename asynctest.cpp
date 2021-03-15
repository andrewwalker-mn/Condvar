#include "uthread.h"
#include "async_io.h"
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <math.h>

using namespace std;

#define UTHREAD_TIME_QUANTUM 1000

void* tester(void *arg) {
  for (int i=0; i<2000000; i++) {
    if (i%100000 == 0) {
      cout << i/100000 << " ";
    }
    else
    {
      double j = pow(i,3.0);
      j += j-pow(i,.7);
    }
  }
  cout << endl;
  return nullptr;
}

void * reads(void * arg) {
  cout << "in reads" << endl;
  FILE * file1 = fopen("infile.txt", "r");
  int fd = fileno(file1);

  char read_to[10000];
  size_t length = 10000;

  ssize_t ret = async_read(fd, read_to, length, 0);
  fclose(file1);
  // cout << read_to << " was read in " << endl;

  return nullptr;
}

void * writes(void * arg) {
  cout << "in writes" << endl;
  remove("outfile.txt");
  ofstream file {"outfile.txt"};

  FILE * file2 = fopen("outfile.txt", "w");
  int fd = fileno(file2);

  // char write_from[] = "asdfa";
  std::string write_from(100000000, '*');
  size_t length = 100000000;

  ssize_t ret = async_write(fd, &write_from[0], length, 0);
  fclose(file2);

  // std::ifstream f("outfile.txt");

  // if (f.is_open())
  //   std::cout << f.rdbuf() << " was written in" << endl;;

  return nullptr;
}

int main(int argc, char*argv[]) {
  int ret = uthread_init(UTHREAD_TIME_QUANTUM);
  if (ret != 0) {
    cerr << "Error: uthread_init" << endl;
    exit(1);
  }

  int thread1 = uthread_create(reads, nullptr);
  int thread2 = uthread_create(writes, nullptr);
  int thread3 = uthread_create(tester, nullptr);

  uthread_join(thread1, nullptr);
  uthread_join(thread2, nullptr);
  uthread_join(thread3, nullptr);

  return 0;
}
