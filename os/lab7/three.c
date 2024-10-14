#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
  int sem_id;
  int max_value = 0;
  struct sembuf s_buff = {0, 1, 0};

  sem_id = semget(IPC_PRIVATE, 1, IPC_CREAT | S_IRUSR | S_IWUSR);
  if (sem_id == -1) {
      perror("failed to create semaphore");
      return 1;
  }

  if (semctl(sem_id, 0, SETVAL, 0) == -1) {
      perror("failed to setup semaphore");
      return 1;
  }

  while (1) {
    max_value++;

    if (semctl(sem_id, 0, SETVAL, max_value) == -1) {
      perror("failed to increase semaphore");
      printf("Maximum counting semaphore value: %d\n", max_value - 1);
      break;
    }
  }

  if (semctl(sem_id, 0, IPC_RMID) == -1) {
      perror("failed to free semaphore");
      return 1;
  }

  return 0;
}

