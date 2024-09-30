#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define FOO 4096

int main () {
  int shmId;
  char *sharedMemoryPtr;

  shmId = shmget(IPC_PRIVATE, FOO, IPC_CREAT|S_IRUSR|S_IWUSR);
  if(shmId < 0) {
    perror("Unable to get shared memory\n");
    exit(1);
  }

  sharedMemoryPtr = shmat(shmId, 0, 0);
  if(sharedMemoryPtr == (void*)-1) {
    perror("Unable to attach\n");
    exit(1);
  }

  printf("Value a: %p\t Value b: %p\n", (void *)sharedMemoryPtr, (void *)sharedMemoryPtr + FOO);
  
  struct shmid_ds shm_info;
  shmctl(shmId, IPC_STAT, &shm_info);
  printf("Amount of memory allocated: %d\n", shm_info.shm_segsz);
  printf("ID: %d\n", shmId);
  sleep(10);

  if(shmdt(sharedMemoryPtr) < 0) {
    perror("Unable to detach\n");
    exit(1);
  }

  if(shmctl(shmId, IPC_RMID, 0) < 0) {
    perror("Unable to deallocate\n");
    exit(1);
  }

  return 0;
}

