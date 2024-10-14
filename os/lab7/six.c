#include <stdio.h>
#include <unistd.h>

int main() {
  long phys_pages;

  phys_pages = sysconf(_SC_PHYS_PAGES);
  printf("Number of physical pages: %ld pages\n", phys_pages);

  return 0;
}

