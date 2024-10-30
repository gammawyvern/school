#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main() {
  char *data1;
  size_t size = 0;
  printf ("Please input username: ");
  getline(&data1, &size, stdin);
  // This does include trailing '\n' though
  // Not sure if this messes with freeing after
  data1[strcspn(data1, "\n")] = '\0';
  printf ("You entered: [%s]\n", data1);
  free (data1);
  return 0;
}

