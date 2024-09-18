#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "node.h"
#include "message.h"

int id = 0;
int circleSize = 0;
int rootPipe[2];
int lastPipe[2];
int nextPipe[2];
int apple = 0;  

void createNodeRing() {
  // Obtain size of node circle
  while(circleSize <= 1) {
    printf("Enter valid number of nodes in circle: ");
    scanf("%d", &circleSize);
    while(getchar() != '\n');
  }

  // Create initial pipe for root node to last child
  if(pipe(lastPipe) != 0) {
    perror("failed to create pipe");
    exit(1);
  }

  rootPipe[WRITE] = lastPipe[WRITE];
  rootPipe[READ] = lastPipe[READ];

  addRingNodes();

  printf("New pipe: [%d %d]\n", nextPipe[READ], nextPipe[WRITE]);
  sleep(0.5);

  close(lastPipe[WRITE]);
  close(nextPipe[READ]);
  sleep(3);
  printf("id[%d] read[%d] write[%d]\n", id, lastPipe[READ], nextPipe[WRITE]);
  sleep(1);

  // Init root node
  if(id == 0) {
    apple = 1;
    data = createMessage();
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
