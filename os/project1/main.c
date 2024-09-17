#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#define READ 0
#define WRITE 1

struct message {
  int src; // Source node ID
  int dst; // Destination node ID
  char* message;
}

int id = 0;
int circleSize = 0;
int rootPipe[2];
int lastPipe[2];
int nextPipe[2];
int apple = 0;  

struct message* data;

void addRingNodes();
void createNodeRing();

int main() {
  // Obtain size of node circle
  while(circleSize <= 1) {
    puts("Enter valid number of nodes in circle");
    scanf("%d", &circleSize);
    while(getchar() != '\n');
  }

  createNodeRing();
  close(lastPipe[WRITE]);
  close(nextPipe[READ]);

  communicate();

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

  if(id == 0) {
    apple = 1;
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

void communicate() {
  while(1) {
    if(apple == 1) {
      sleep(1); // TODO temp to slowdown output
      data++;
      write(nextPipe[WRITE], &data, sizeof(data));
      printf("[%d] sent: %d\n", id, data);
      apple = 0;
    } else {
      read(lastPipe[READ], &data, sizeof(data));
      printf("[%d] recieved: %d\n", id, data);
      apple = 1;
    }
  }
}

void intHandler(int sigNum) {

}

