#include <stdio.h>
#include <unistd.h>

int main() {
  long page_size;

  page_size = sysconf(_SC_PAGESIZE);
  printf("Page size: %ld bytes\n", page_size);

  return 0;
}

