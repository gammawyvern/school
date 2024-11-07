#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#include "kitchen.h"
#include "baker.h"

int main(int argc, char* argv[]) {
  if (argc != 2) {
    printf("Usage: ./a.out num_of_bakers\n");
    return 1;
  }

  int num_of_bakers = atoi(argv[1]);
  if (num_of_bakers < 2) {
    printf("\nMust have at least 2 bakers\n");
    return 1;
  }

  Kitchen kitchen;
  setup_kitchen(&kitchen);

  pthread_t baker_threads[num_of_bakers];
  Baker bakers[num_of_bakers];

  for (unsigned long id=0; id<num_of_bakers; id++) {
    bakers[id].id = id;
    bakers[id].kitchen = &kitchen;

    int result = pthread_create(&baker_threads[id], NULL, create_baker, &bakers[id]);
    if (result != 0) {
      perror("Failed to create thread");
      return 1;
      }
  }

  for (unsigned long id=0; id<num_of_bakers; id++) {
    pthread_join(baker_threads[id], NULL);
  }

  cleanup_kitchen(&kitchen);
  return 0;
}

