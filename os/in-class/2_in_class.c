#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int msg_size = 500 * sizeof(char);

int main() {
    int fd[2];

    if(pipe(fd) < 0) {
        perror("Failed to create pipe");
        exit(1);
    }

    pid_t pid = fork();

    if(pid < 0) {
        perror("Failed to fork");
        exit(1);
    } else if(pid == 0) {
        char user_input[msg_size];
        puts("Enter message:");
        fgets(user_input, msg_size, stdin);
        write(fd[1], user_input, msg_size);
        printf("[%d] child is sending string:\n\t%s\n", getpid(), user_input);
        exit(0);
    } else {
        int status;
        waitpid(pid, &status, 0);

        char user_input[msg_size];
        read(fd[0], user_input, msg_size);
        printf("[%d] parent recieved string:\n\t%s", getpid(), user_input);
    }

    return 0;
}

