#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "kitchen.h"
#include "baker.h"

size_t get_winner(time_t* finish_times, size_t size);

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
    bakers[id] = create_baker(&kitchen, id);

    int result = pthread_create(&baker_threads[id], NULL, run_baker_thread, &bakers[id]);
    if (result != 0) {
      perror("Failed to create thread");
      return 1;
    }
  }

  time_t finish_times[num_of_bakers];
  for (unsigned long id=0; id<num_of_bakers; id++) {
    pthread_join(baker_threads[id], (void*)&finish_times[id]);
  }

  size_t winner = get_winner(finish_times, num_of_bakers);
  printf("Baker [%lu] won!\n", winner);

  cleanup_kitchen(&kitchen);
  return 0;
}

size_t get_winner(time_t* finish_times, size_t size) {
  size_t winner_index = 0;
  for (size_t i=0; i<size; i++) {
    if (finish_times[i] < finish_times[winner_index]) {
      winner_index = i;
    }
  }

  return winner_index;
}

