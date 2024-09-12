#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    int file = open("output.txt", O_RDWR | O_CREAT, 0644);
    /*dup2(file, STDOUT_FILENO);*/

    if(fork() == 0) {
        char* command[] = {"ls", "-l", NULL};
        sleep(2);
        execvp(command[0], command);
    } else {
        dup2(file, STDOUT_FILENO);
    }

    close(file);
}
