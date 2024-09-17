#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#define READ 0
#define WRITE 1

struct message {
  int src; // Source node ID
  int dst; // Destination node ID
  char* text;
};

void addRingNodes();
void createNodeRing();
void communicate();

int id = 0;
int circleSize = 0;
int rootPipe[2];
int lastPipe[2];
int nextPipe[2];
int apple = 0;  

struct message* data;
char messageText[100];

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
      int dstID = -1;
      printf("Enter message to send: ");
      fgets(messageText, sizeof(messageText), stdin);

      // Obtain size of node circle
      while(dstID < 0 || dstID >= circleSize) {
        printf("Enter destination node id:\n\t");
        scanf("%d", &dstID);
        while(getchar() != '\n');
      }
      
      struct message send = {id, dstID, messageText};

      write(nextPipe[WRITE], &send, sizeof(send));
      // printf("[%d] sent: %d\n", id, data);
      apple = 0;
    } else {
      struct message recv;
      read(lastPipe[READ], &recv, sizeof(recv));
      apple = 1;
    }
  }
}

void sendMessage();

void intHandler(int sigNum) {
  free(data); 
}

