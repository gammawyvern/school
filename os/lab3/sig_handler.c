#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void sigHandler(int);

int main() {
    puts("before signal");
    sleep(4);
    signal(SIGINT, sigHandler);
    puts("waiting");
    pause();
    return 0;
}

void sigHandler(int sigNum) {
    puts("recieved interrupt");
    sleep(2);
    puts("time to exit");
    exit(0);
}
