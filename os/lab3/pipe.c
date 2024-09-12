#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

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

    char message[500];

    if(pid == 0) {
        strcpy(message, "secretpassword123!");
        write(fd[WRITE], message, sizeof(message));
    } else {
        read(fd[READ], message, sizeof(message));
        puts(message);
    }
}
