#include <sys/types.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
  int size = 0;
  // int size = 2147483600;
  int shm_id;

  while(1) {
    size += 1024 * 1024;
    // size++;

    shm_id = shmget(IPC_PRIVATE, size, IPC_CREAT | S_IRUSR | S_IWUSR);
    if (shm_id < 0) {
      perror("failed to create shared memory");
      printf("Maximum shared memory size reached: %d\n", size - 1);
      break;
    }

    if (shmctl(shm_id, IPC_RMID, 0) < 0) {
      perror("failed to deallocate shared memory");
      exit(1);
    }
  }
}

