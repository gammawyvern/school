#include <stdio.h> // Shared C libraries
#include <stdlib.h> // Shared C libraries

int main();
long offset();
void recurse();
int one();
int two();

int initialized_1 = 50; // Initialized data
int initialized_2 = 20; // Initialized data

int uninitialized_1; // Static data
int uninitialized_2; // Static data

int main() {
  // Get relative distance from main
  void *main_addr = (void*)&main;

  printf("=== Program Memory Map ===\n\n");

  // Program Text
  printf("Program Text:\n");
  printf("main:\t%ld\n", offset(main_addr, &main));
  printf("one:\t%ld\n", offset(main_addr, &one));
  printf("two:\t%ld\n", offset(main_addr, &two));
  printf("\n");

  // Initialized Data
  printf("Initialized Data:\n");
  printf("one:\t%ld\n", offset(main_addr, &initialized_1));
  printf("two:\t%ld\n", offset(main_addr, &initialized_2));
  printf("\n");

  // Uninitialized Data
  printf("Uninitialized Data:\n");
  printf("one:\t%ld\n", offset(main_addr, &uninitialized_1));
  printf("two:\t%ld\n", offset(main_addr, &uninitialized_2));
  printf("\n");

  // Run-Time Data (Heap)
  void* run_time_1 = malloc(20);
  void* run_time_2 = malloc(10);
  printf("Run-Time Data (Heap):\n");
  printf("one:\t%ld\n", offset(main_addr, &run_time_1));
  printf("two:\t%ld\n", offset(main_addr, &run_time_2));
  printf("\n");

  // Stack Data
  printf("Stack Data:\n");
  recurse(5, main_addr);
  printf("\n");

  // Shared Library Data
  printf("Shared Library Functions:\n");
  printf("one:\t%ld\n", offset(main_addr, &printf));
  printf("two:\t%ld\n", offset(main_addr, &malloc));
  printf("\n");

  free(run_time_1);
  free(run_time_2);

  return 0;
}

long offset(void* main_addr, void* next_addr) {
  return (long)(next_addr - main_addr);
}

void recurse(int num, void* last_addr) {
  if (num <= 0) {
    return;
  }

  int x;
  printf("Next:\t%ld\n", offset(last_addr, &x));
  recurse(--num, last_addr);
}

// Program text ?
int one() {
  int x = 2 + 2;
  return x;
}

// Program text ?
int two() {
  int y = 4 * 4;
  return y;
}
