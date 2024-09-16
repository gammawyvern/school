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
    signal(SIGINT, sigHandler);

    if(pid == 0) {
       
    } else {
      kill(pid, SIGUSR1);
    }
}

void sigHandler(int sigNum) {
    printf("%d\n", sigNum);
}
