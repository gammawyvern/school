#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>

#define KEY 123
#define SIZE 1024
#define DATAOFFSET 1 

int shm_id;
char* shm_addr;

void shutdown();
void int_handler(int);

int main() {

  key_t key = ftok("./writer.c", KEY);
  if(key == -1) {
    perror("Failed to create key.");
    exit(1);
  }

  shm_id = shmget(key, SIZE, S_IRUSR|S_IWUSR);
  if(shm_id < 0) {
    perror("Failed to create shared memory.");
    exit(1);
  }

  shm_addr = shmat(shm_id, 0, 0);
  if(shm_addr == (void*)-1) {
    perror("Failed to attach memory.");
    exit(1);
  }

  while(1) {
    while(*shm_addr == 0);

    printf("Process pid[%d] read: %s", getpid(), shm_addr+DATAOFFSET);
    (*shm_addr)++;

    if(strcmp(shm_addr+DATAOFFSET, "quit\n") == 0) { 
      int_handler(0);
    }

    while(*shm_addr != 0);
  }
}

void int_handler(int sig_num) {
  if(shmdt(shm_addr) < 0) {
    perror("Failed to detach memory.");
    exit(1);
  }

  exit(0);
}

