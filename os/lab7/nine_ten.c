#include <sys/resource.h>
#include <stdio.h>

int main() {
  struct rlimit limits;

  int result = getrlimit(RLIMIT_NOFILE, &limits);

  if (result != 0) {
    perror("failed to get limits");
    return 1;
  }

  printf("Max open files (soft limit): %lu\n", limits.rlim_cur);
  printf("Max open files (hard limit): %lu\n", limits.rlim_max);

  return 0;
}

