#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int id;
pid_t rootPID;
int circleSize;
int fd[2];

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

    int received;
    read(fd[0], &received, sizeof(int));

    return 0;
}

void addCircleNode() {
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
    } else if(pid == 0) {
        id++;

        if(id == circleSize - 1) {
            // TODO loop pipe back to rootPID
        } else {
            addCircleNode();
        }
    } else {
        write(fd[1], &id, sizeof(int));
    }
}

