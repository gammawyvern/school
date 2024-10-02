#include <sys/types.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

struct myStruct {
  int num;
  char string[40];
};

int main() {
  key_t key = ftok("/home/keag/Github/school/os/in-class/producer.c", 0);

  int sharedMemoryID = shmget(key, sizeof(struct myStruct), S_IWUSR|S_IRUSR|S_IRGRP|S_IWGRP|IPC_CREAT);
  if(sharedMemoryID < 0) {
    perror("Failed to create shared memory");
    exit(EXIT_FAILURE);
  }

  void* sharedMemoryPointer = shmat(sharedMemoryID, 0, 0);
  if(sharedMemoryPointer == (void*)-1) {
    perror("Failed to attach memory");
    exit(EXIT_FAILURE);
  }

  printf("Number in memory: %d\n", ((struct myStruct*)sharedMemoryPointer)->num);
  printf("String in memory: %s\n", ((struct myStruct*)sharedMemoryPointer)->string);

  if(shmdt(sharedMemoryPointer) < 0) {
    perror("Failed to detach memory");
    exit(EXIT_FAILURE);
  }

  if(shmctl(sharedMemoryID, IPC_RMID, NULL) < 0) {
    perror("Failed free shared memory");
    exit(EXIT_FAILURE);
  }

  return 0;
}

