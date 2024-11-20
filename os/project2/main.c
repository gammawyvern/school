#define _GNU_SOURCE

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "kitchen.h"
#include "baker.h"

int compare_time(const void* a, const void* b, void* finish_times);
size_t* get_finish_order(struct timespec** finish_times, size_t size);

size_t ramsy;

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

  srand(time(NULL));
  ramsy = rand() % num_of_bakers;

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

  struct timespec* finish_times[num_of_bakers];
  for (unsigned long id=0; id<num_of_bakers; id++) {
    void* return_time;
    pthread_join(baker_threads[id], &return_time);
    finish_times[id] = (struct timespec*)return_time;
  }

  size_t* finish_order = get_finish_order(finish_times, num_of_bakers);
  printf("\nBaker [%lu] won!\n", finish_order[0]);
  printf("Finish order: ");
  for (size_t id=0; id<num_of_bakers; id++) {
    printf("%lu ", finish_order[id]);
    free(finish_times[id]);
  } printf("\n");

  free(finish_order);
  cleanup_kitchen(&kitchen);

  return 0;
}

int compare_time(const void* a, const void* b, void* finish_times) {
  size_t index_a = *(size_t*)a;
  size_t index_b = *(size_t*)b;
  struct timespec** times = (struct timespec**)finish_times; 

  size_t sec_diff = times[index_a]->tv_sec - times[index_b]->tv_sec;
  size_t nsec_diff = times[index_a]->tv_nsec - times[index_b]->tv_nsec;

  if (sec_diff == 0) {
    return nsec_diff; 
  }

  return sec_diff;
}

size_t* get_finish_order(struct timespec** finish_times, size_t size) {
  size_t* order = malloc(size * sizeof(size_t));
  for (size_t id=0; id<size; id++) {
    order[id] = id;
  }

  qsort_r(order, size, sizeof(size_t), compare_time, finish_times);

  return order;
}

