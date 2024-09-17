#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#define READ 0
#define WRITE 1

int id;
int circleSize = 4;
int rootPipe[2];
int lastPipe[2];
int nextPipe[2];

void addRingNodes();
void createNodeRing();

int main() {
  id = 0;

  createNodeRing();

  close(lastPipe[WRITE]);
  close(nextPipe[READ]);

  printf("[%d]: [%d, %d]\n", id, lastPipe[READ], lastPipe[WRITE]);
  printf("[%d]: [%d, %d]\n", id, nextPipe[READ], nextPipe[WRITE]);
  sleep(1);

  return 0;
}

void createNodeRing() {
  // Create initial pipe for root node to last child
  if(pipe(lastPipe) != 0) {
    perror("failed to create pipe");
    exit(1);
  }

  // TODO check?
  rootPipe[WRITE] = lastPipe[WRITE];
  rootPipe[READ] = lastPipe[READ];

  addRingNodes();

  // Finish ring connection
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
    // TODO Does this make sense to do this?
    // Should I use dup() or something?
    lastPipe[READ] = nextPipe[READ];
    lastPipe[WRITE] = nextPipe[WRITE];
    
    if(id == circleSize - 1) {
      // TODO check?
      nextPipe[READ] = rootPipe[READ];
      nextPipe[WRITE] = rootPipe[WRITE];
    } else {
      addRingNodes();
    }

  }

  if(pid > 0){
    // All parent processes
  }
}

