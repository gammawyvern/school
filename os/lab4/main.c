#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define MAX_FILENAME_LEN 100

void* dispatch(void*);
void* worker(void*);

int main() {
  srand(time(NULL));

  pthread_t thread;
  void* result;
  int thread_status;

  pthread_create(&thread, NULL, dispatch, NULL);
  if(thread_status != 0) {
    perror("Failed to create dispatcher");
  }

  return 0;
}

void* dispatch(void* argument) {
  char filename[MAX_FILENAME_LEN];

  while(1) {
    pthread_t thread;
    void* result;
    int thread_status;

    fgets(filename, MAX_FILENAME_LEN, stdin);
    pthread_create(&thread, NULL, worker, filename);
    if(thread_status != 0) {
      perror("Failed to process request");
    }
  }
}

void* worker(void* argument) {
  char* file_path = argument; 
  printf("%s", file_path);

  return NULL;
}

