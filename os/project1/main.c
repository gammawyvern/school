#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

#define READ 0
#define WRITE 1

struct message {
  int src; // Source node ID
  int dst; // Destination node ID
  char* text;
  size_t len;
};

void addRingNodes();
void createNodeRing();
void communicate();
struct message* createMessage();

int id = 0;
int circleSize = 0;
int rootPipe[2];
int lastPipe[2];
int nextPipe[2];
int apple = 0;  

struct message* data = NULL;
char* messageText;

int main() {
  // Obtain size of node circle
  while(circleSize <= 1) {
    printf("Enter valid number of nodes in circle: ");
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

  rootPipe[WRITE] = lastPipe[WRITE];
  rootPipe[READ] = lastPipe[READ];

  addRingNodes();

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

void communicate() {
  while(1) {
    if(apple == 1) {
      if(data->dst == id) {
        printf("[%d] recieved message: %s", id, data->text);

        free(data->text);
        free(data);
        data = createMessage();

        write(nextPipe[WRITE], data, sizeof(struct message));
      } else {
        write(nextPipe[WRITE], data, sizeof(struct message));
      }

      apple = 0;
    } else {
      read(lastPipe[READ], data, sizeof(struct message));
      apple = 1;
    }
  }
}

struct message* createMessage() {
  int dstID = -1;
  size_t msgLen;

  printf("Enter message to send: ");
  getline(&messageText, &msgLen, stdin);

  while(dstID < 0 || dstID >= circleSize) {
    printf("Enter destination node id: ");
    scanf("%d", &dstID);
    while(getchar() != '\n');
  }

  struct message* message = malloc(sizeof(struct message));
  message->src = id;
  message->dst = dstID;
  message->text = malloc(msgLen + 1);
  strcpy(message->text, messageText);
  message->len = msgLen;

  return message;
}

void intHandler(int sigNum) {
  free(data); 
}

