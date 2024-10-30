#include <stdio.h> // Shared C libraries
#include <stdlib.h>

int main();
int one();
int two();

int initialized_1 = 50; // Initialized data
int initialized_2 = 20; // Initialized data

int uninitialized_1; // Static data
int uninitialized_2; // Static data

int main() {
  int stack_1;
  int stack_2;

  printf("Program Text:\n");
  printf("main:\t\t%p\n", &main);
  printf("function one:\t%p\n", &one);
  printf("function two:\t%p\n", &two);
  printf("\n");

  printf("Initialized Data:\n");
  printf("one:\t%p\n", &initialized_1);
  printf("two:\t%p\n", &initialized_2);
  printf("\n");

  printf("Uninitialized Data:\n");
  printf("one:\t%p\n", &uninitialized_1);
  printf("two:\t%p\n", &uninitialized_2);
  printf("\n");

  void* run_time = malloc(20);
  void* run_time_2 = malloc(10);

  printf("Run Time:\n");
  printf("one:\t%p\n", run_time);
  printf("two:\t%p\n", run_time_2);
  printf("\n");

  printf("Stack Data:\n");
  printf("one:\t%p\n", &stack_1);
  printf("two:\t%p\n", &stack_2);
  printf("\n");

  printf("Shared Library Data:\n");
  printf("one:\t%p\n", &printf);
  printf("two:\t%p\n", &malloc);
  printf("\n");

  return 0;
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

