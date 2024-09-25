#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <time.h>

#define MAX_FILENAME_LEN 100

void* dispatch(void*);
void* worker(void*);
void intHandler(int);

int file_requests = 0;
int file_accesses = 0;
int active_threads = 0;
int accepting_requests = 1;

int main() {
  signal(SIGINT, intHandler);
  srand(time(NULL));

  pthread_t thread;
  void* result;
  int thread_status;

  thread_status = pthread_create(&thread, NULL, dispatch, NULL);
  if(thread_status != 0) {
    perror("Failed to create dispatcher");
  }

  int join_status = pthread_join(thread, &result);
  free(result);

  return 0;
}

void* dispatch(void* argument) {
  char file_name[MAX_FILENAME_LEN];
  pthread_t thread;
  void* result;
  int thread_status;

  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

  while(accepting_requests != 0) {
    printf("Please enter next file to search for...\n");
    fgets(file_name, MAX_FILENAME_LEN, stdin);

    if(accepting_requests == 0) { break; }
    file_name[strcspn(file_name, "\n")] = '\0';
    char* file_name_arg = malloc(strlen(file_name) + 1); 
    strcpy(file_name_arg, file_name);


    thread_status = pthread_create(&thread, &attr, worker, file_name_arg);
    if(thread_status != 0) {
      perror("Failed to process request");
    } else {
      active_threads++;
      file_requests++;
    }
  }

  return NULL;
}

void* worker(void* argument) {
  char* file_name = argument; 

  int sleep_time = 1;
  int found_file = rand() % 10;
  if(found_file >= 8) {
    sleep_time = 7 + (rand() % 4);
  }

  sleep(sleep_time);
  printf("Accessed: %s\n", file_name);
  file_accesses++;
  free(argument);
  active_threads--;

  return NULL;
}

void intHandler(int sigNum) {
  if(accepting_requests != 0) {
    printf("\nShutting down. Waiting for current outgoing requests...\n");
    accepting_requests = 0;
    while(active_threads > 0);

    printf("\nTotal number of file requests: %d\n", file_requests);
    printf("Actual number of files accessed: %d\n", file_accesses);
  }
}
