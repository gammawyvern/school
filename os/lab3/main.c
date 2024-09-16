#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#define READ 0
#define WRITE 1

void sigHandler(int);

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

  // Signal Handlers
  signal(SIGUSR1, sigHandler);

  if(pid == 0) {
    while(1) {
      sleep(3);
      kill(getppid(), SIGUSR1);
    }
  } else {
    while(1) {
      pause();
    }
  }
}

void sigHandler(int sigNum) {
  printf("%d\n", sigNum);
}

