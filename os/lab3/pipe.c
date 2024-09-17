#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#define READ 0
#define WRITE 1

int main() {
  int fd[2];
  int pid;

  if(pipe(fd) < 0) {
    perror("failed to create pipe");
    exit(1);
  }

  pid = fork();
  if(pid < 0) {
    perror("failed to fork");
    exit(1);
  }

  char myStringOutput[] = "This is a test";
  char myStringInput[50];

  if(pid == 0) {
    close(fd[READ]);
    write(fd[WRITE], myStringOutput, sizeof(myStringOutput));
    exit(0);
  } else {
    close(fd[WRITE]);
    int status;
    waitpid(pid, &status, 0);
    long bytes = read(fd[READ], myStringInput, sizeof(myStringInput));
    puts(myStringInput);
    printf("Read [%ld] bytes\n", bytes);
  }
}
