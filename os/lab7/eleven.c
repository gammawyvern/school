#include <stdio.h>
#include <time.h>

int main() {
  struct timespec res;

  int result = clock_getres(CLOCK_REALTIME, &res);

  if (result != 0) {
    perror("failed to get clock resolution");
    return 1;
  }

  double s = (double)res.tv_sec;
  double ns = (double)res.tv_nsec;
  double ms = (s * 1000) + (ns / 1000000);
  printf("Clock resolution: %f milliseconds\n", ms);

  return 0;
}

