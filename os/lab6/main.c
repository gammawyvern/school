#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/stat.h>

#define SIZE 16

int main(int argc, char *argv[]) {
  int status;
  long int i, loop, temp, *sharedMemoryPointer;
  int sharedMemoryID, semId;
  pid_t pid;
  struct sembuf sbuf;

  loop = atoi(argv[1]);

  sharedMemoryID = shmget(IPC_PRIVATE, SIZE, IPC_CREAT | S_IRUSR | S_IWUSR);
  if (sharedMemoryID < 0) {
    perror("Unable to obtain shared memory\n");
    exit(1);
  }
  sharedMemoryPointer = shmat(sharedMemoryID, 0, 0);
  if (sharedMemoryPointer == (void*) -1) {
    perror("Unable to attach\n");
    exit(1);
  }

  sharedMemoryPointer[0] = 0;
  sharedMemoryPointer[1] = 1;

  semId = semget(IPC_PRIVATE, 1, 00600);
  if (semId < 0) {
    perror("Unable to create semaphore\n");
    exit(1);
  }

  semctl(semId, 0, SETVAL, 1);

  pid = fork();
  if (pid < 0) {
    printf("Fork failed\n");
    exit(1);
  }

  if (pid == 0) {
    for (i = 0; i < loop; i++) {
      sbuf.sem_num = 0;
      sbuf.sem_op = -1;
      sbuf.sem_flg = 0;
      semop(semId, &sbuf, 1);

      temp = sharedMemoryPointer[0];
      sharedMemoryPointer[0] = sharedMemoryPointer[1];
      sharedMemoryPointer[1] = temp;

      sbuf.sem_op = 1;
      semop(semId, &sbuf, 1);
    }

    if (shmdt(sharedMemoryPointer) < 0) {
      perror("Unable to detach\n");
      exit(1);
    }

    exit(0);
  } else { 
    for (i = 0; i < loop; i++) {
      sbuf.sem_num = 0;
      sbuf.sem_op = -1;
      sbuf.sem_flg = 0;
      semop(semId, &sbuf, 1);

      temp = sharedMemoryPointer[0];
      sharedMemoryPointer[0] = sharedMemoryPointer[1];
      sharedMemoryPointer[1] = temp;

      sbuf.sem_op = 1;
      semop(semId, &sbuf, 1);
    }

    wait(&status);

    printf("Values: %li\t%li\n", sharedMemoryPointer[0], sharedMemoryPointer[1]);

    if (shmdt(sharedMemoryPointer) < 0) {
      perror("Unable to detach\n");
      exit(1);
    }

    if (shmctl(sharedMemoryID, IPC_RMID, 0) < 0) {
      perror("Unable to deallocate\n");
      exit(1);
    }

    semctl(semId, 0, IPC_RMID);
  }

  return 0;
}

