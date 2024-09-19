#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#include "node.h"
#include "message.h"

int id = 0;
int circleSize = 0;
int rootPipe[2];
int lastPipe[2];
int nextPipe[2];
int apple = 0;  

void createNodeRing() {
  signal(SIGINT, intHandler);

  // Obtain size of node circle
  while(circleSize <= 1) {
    printf("Enter valid number of nodes in circle: ");
    scanf("%d", &circleSize);
    while(getchar() != '\n');
  }

  // Setup pipe per node 
  if(pipe(lastPipe) != 0) {
    perror("failed to create pipe");
    exit(1);
  }
  rootPipe[READ] = lastPipe[READ];
  rootPipe[WRITE] = lastPipe[WRITE];

  addRingNodes();
  close(lastPipe[WRITE]);
  close(nextPipe[READ]);

  // Init root node
  if(id == 0) {
    data = createMessage();
    write(nextPipe[WRITE], &data, sizeof(struct message));
    write(nextPipe[WRITE], data.text, data.len);
  }
}

void addRingNodes() {
  // Setup pipe per node 
  if(pipe(nextPipe) != 0) {
    perror("failed to create pipe");
    exit(1);
  }

  // Fork new circle node
  pid_t pid = fork();
  if(pid < 0) {
    perror("failed to fork");
    exit(1);
  }

  if(pid == 0) {
    id++;
    // TODO make these print not annoyingly
    // printf("[%d] | Child pid[%d] created\n", id, getpid());

    lastPipe[READ] = nextPipe[READ];
    lastPipe[WRITE] = nextPipe[WRITE];
    
    if(id == circleSize - 1) {
      nextPipe[READ] = rootPipe[READ];
      nextPipe[WRITE] = rootPipe[WRITE];
    } else {
      addRingNodes();
    }
  }
}

void intHandler(int sigNum) {
  int status;
  wait(&status);

  free(data.text);
  close(lastPipe[READ]);
  close(nextPipe[WRITE]);
  printf("\n[%d]\t| Process pid[%d] shutting down...", id, getpid());
  if(id == 0) { printf("\n"); }
  exit(0);
}
