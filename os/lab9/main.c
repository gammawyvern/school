#include <stdio.h>
#include <stdlib.h>

// #define COEFFICIENT 2
#define COEFFICIENT 24
#define KB 1024
#define LOOP 3

int main() {
  int count, *intPtr;
  long int i, j, dim = COEFFICIENT * KB;

  intPtr = malloc(dim * dim * sizeof(int));
  if (intPtr == 0) {
    perror ("totally out of space");
    exit (1);
  }

  for (count=1; count<=LOOP; count++)
    for (i=0; i<dim; i++)
      for (j=0; j<dim; j++)
        intPtr[i * dim + j] = (i + j) % count;

  free (intPtr);
  return 0;
}

