#include <stdio.h>
#include <unistd.h>

int main() {
  long process_max;

  process_max = sysconf(_SC_CHILD_MAX);
  printf("Max number of processes per user: %ld processes\n", process_max);

  return 0;
}

