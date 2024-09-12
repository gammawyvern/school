#include <stdio.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
    // Setup interactive prompt
    system("clear");

    // TODO not sure if there is some better way than an arbitrary size char array 
    char buffer[100];
    char* command_buffer[20];

    // Start interactive shell
    while(1) {
        printf("shell> ");
        fflush(stdout);
        fgets(buffer, 100, stdin);

        // TODO doesn't seem to work
        if(strcmp(buffer, "quit\n") == 0) {
            exit(0);
        }

        // Remove pain in the ass newline char
        // Not sure if there is a way to avoid it?
        char* newline_pos = strchr(buffer, '\n');
        if(newline_pos) {
            *newline_pos = '\0';
        }

        char* token = strtok(buffer, " ");
        int token_count = 0;
        while(token != NULL) {
            command_buffer[token_count] = token;
            token = strtok(NULL, " ");
            token_count++;
        }
        command_buffer[token_count] = NULL;

        // Execute command under child process
        pid_t child_pid = fork();
        if(child_pid < 0) {
            perror("fork() failed");
            exit(1);
        } else if(child_pid == 0) {
            execvp(command_buffer[0], command_buffer);
            perror("execvp() failed");
            exit(1);
        } else {
            int status;
            struct rusage child_usage;

            // This was the only way I found to print each child usage per child
            wait4(child_pid, &status, 0, &child_usage);
            printf("CPU time used: %ld.%lds\n", child_usage.ru_utime.tv_sec, child_usage.ru_utime.tv_usec);
            printf("Involuntary context switches: %ld\n", child_usage.ru_nivcsw);
        }
    }
}

