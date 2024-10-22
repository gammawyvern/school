#include <sys/resource.h>
#include <stdio.h>

int main() {
  struct rlimit limits;

  int result = getrlimit(RLIMIT_FSIZE, &limits);

  if (result != 0) {
    perror("failed to get limits");
    return 1;
  }

  printf("Max file size: %lu\n", limits.rlim_cur);

  return 0;
}

