#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#include "node.h"
#include "message.h"

int id = 0;
int circleSize = 0;
int nodePipe[2];

void createNodeRing() {
  signal(SIGINT, intHandler);

  // Obtain size of node circle
  while(circleSize <= 1) {
    printf("Enter number of nodes in circle: ");
    scanf("%d", &circleSize);
    while(getchar() != '\n');
  }

  // Setup pipe per node 
  if(pipe(nodePipe) != 0) {
    perror("failed to create pipe");
    exit(1);
  }

  addRingNodes(nodePipe[WRITE]);

  // Init root node
  if(id == 0) {
    // Wait for last node to be created
    read(nodePipe[READ], &data, sizeof(struct message));
    data = createMessage();
    write(nodePipe[WRITE], &data, sizeof(struct message));
    write(nodePipe[WRITE], data.text, data.len);
    free(data.text);
  }
}

void addRingNodes(int rootWrite) {
  int lastRead = nodePipe[READ];
  if(pipe(nodePipe) != 0) {
    perror("failed to create pipe");
    exit(1);
  }

  pid_t pid = fork();
  if(pid < 0) {
    perror("failed to fork");
    exit(1);
  }

  if(pid == 0) {
    id++;

    printf("[%d]\t| Child process pid[%d] created\n", id, getpid());
    if(id == circleSize - 1) {
      nodePipe[WRITE] = rootWrite;
      // Let root wait so prints out in order
      write(nodePipe[WRITE], &data, sizeof(struct message));
    } else {
      addRingNodes(rootWrite);
    }
  } else {
    nodePipe[READ] = lastRead;
  }
}

void intHandler(int sigNum) {
  int status;
  wait(&status);

  close(nodePipe[READ]);
  close(nodePipe[WRITE]);
  printf("\n[%d]\t| Process pid[%d] shutting down...", id, getpid());
  if(id == 0) { printf("\n"); }
  exit(0);
}

