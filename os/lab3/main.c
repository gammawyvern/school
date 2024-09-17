#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>

#define READ 0
#define WRITE 1

void sig1Handler(int);
void sig2Handler(int);
void intHandler(int);

int main() {
  int pid;
  int fd[2];

  if(pipe(fd) != 0) {
      perror("failed to create pipe");
      exit(1);
  }

  pid = fork();
  if(pid < 0) {
      perror("failed to fork");
      exit(1);
  }

  if(pid == 0) {

    srand(time(NULL));
    while(1) {
      int random_int = (rand() % 5) + 1;
      sleep(random_int);
      random_int = rand() % 2;
      if(random_int == 0) {
        kill(getppid(), SIGUSR1);
      } else {
        kill(getppid(), SIGUSR2);
      } 
    }
  } else {
    signal(SIGINT, intHandler);
    signal(SIGUSR1, sig1Handler);
    signal(SIGUSR2, sig2Handler);

    printf("Spawned child [%d]\n", pid);

    while(1) {
      printf("Waiting...\t");
      pause();
    }
  }
}

void sig1Handler(int sigNum) {
  printf("recieved a SIGUSR1 signal\n");
}

void sig2Handler(int sigNum) {
  printf("recieved a SIGUSR2 signal\n");
}

void intHandler(int sigNum) {
  printf("\nThat's it, you're getting shut down!\n"); 
  // Nothing to free in this program
  exit(0);
}
