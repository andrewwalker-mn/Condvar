#include "uthread.h"
#include "async_io.h"
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <math.h>

using namespace std;

#define UTHREAD_TIME_QUANTUM 10000

void* tester(void *arg) {
  for (int i=0; i<5000000; i++) {
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
  FILE * file1 = fopen("infile.txt", "r");
  int fd = fileno(file1);

  char read_to[11];
  size_t length = 10;
  for (int i=0; i<1000000; i++) {
    if (i%100000==0)
      cout << "in reads" << endl;

    ssize_t ret = fread(read_to, 1, length, file1);
    // cout << read_to << " was read in " << endl;
  }
  fclose(file1);
  return nullptr;
}

void * writes(void * arg) {
  remove("outfile.txt");
  ofstream file {"outfile.txt"};
  FILE * file2 = fopen("outfile.txt", "w");
  std::string write_from(10, '*');
  size_t length = 10;
  int fd = fileno(file2);
  for (int i=0; i<1000000; i++) {
    if (i%100000 == 0)  
      cout << "in writes" << endl;

    ssize_t ret = fwrite(&write_from[0], 1, length, file2);

    // std::ifstream f("outfile.txt");

    // if (f.is_open())
    //   std::cout << f.rdbuf() << " was written in" << endl;;
  }
  fclose(file2);
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
