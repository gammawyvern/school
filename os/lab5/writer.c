#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>

#define SIZE 1024 
#define KEY 123

int main() {
  int shm_id;
  char* shm_addr;

  key_t key = ftok("./writer.c", KEY);
  if(key == -1) {
    perror("Failed to create key.");
    exit(1);
  }

  shm_id = shmget(key, SIZE, IPC_CREAT|S_IRUSR|S_IWUSR);
  if(shm_id < 0) {
    perror("Failed to create shared memory.");
    exit(1);
  }

  shm_addr = shmat(shm_id, 0, 0);
  if(shm_addr == (void*)-1) {
    perror("Failed to attach memory.");
    exit(1);
  }

  if(shmdt(shm_addr) < 0) {
    perror("Failed to detach memory.");
    exit(1);
  }

  if(shmctl(shm_id, IPC_RMID, 0) < 0) {
    perror("Failed to deallocate memory.");
    exit(1);
  }
}

