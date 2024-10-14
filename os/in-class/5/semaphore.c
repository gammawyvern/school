#include <sys/stat.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <stdio.h>

#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

struct sembuf p = {0, -1, SEM_UNDO};
struct sembuf v = {0, +1, SEM_UNDO};

int main() {
  int sem_id;
  int num = 2;

  if ((sem_id = semget(IPC_PRIVATE, 1, S_IRUSR | S_IWUSR)) == -1) {
    perror("semget: semget failed");
    exit(1);
  }

  if(semctl(sem_id, 0, SETVAL, num) == -1) {
    perror("semctl: init failed");
    exit(1);
  }

  fork();

  semop(sem_id, &p, 1);
  // If waiting for resource, print "doing stuff" every second 
  printf("[%d]: Using resource\n", getpid());
  sleep(10);
  printf("[%d]: Freeing resource\n", getpid());
  semop(sem_id, &v, 1);

  return 0;
}
