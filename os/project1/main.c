#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#define READ 0
#define WRITE 1

int id;
pid_t rootPID;
int circleSize;
int fd[2];
int data;

int apple = 0;

void addCircleNode();

int main() {
  id = 0;
  rootPID = getpid();

  // Obtain size of node circle
  while(circleSize <= 1) {
    puts("Enter valid number of nodes in circle");
    scanf("%d", &circleSize);
    while(getchar() != '\n');
  }

  // Setup circle node tree
  addCircleNode();

  if(getpid() == rootPID) {
    apple = 1;
    data = 200;
  }

  while(1) {
    if(apple == 1) {
      write(fd[WRITE], &data, sizeof(data));
      apple = 0;
    } else {
      read(fd[READ], &data, sizeof(data));
      printf("[%d]: %d\n", getpid(), data);
      apple = 1;
    }
  }

  printf("[%d]\n", getpid());
  return 0;
}

void addCircleNode() {
  int old_write_fd = fd[WRITE];
  int old_read_fd = fd[READ];

  // Setup pipe per node 
  if(pipe(fd) != 0) {
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
    // close(fd[WRITE]);

    id++;
    if(id == circleSize - 1) {
      // TODO loop pipe back to rootPID
      // dup2(, ); // something like this?
    } else {
      addCircleNode();
    }

    // I really don't know
    // dup2();
  } else {
    // close(fd[READ]);
  }
}

