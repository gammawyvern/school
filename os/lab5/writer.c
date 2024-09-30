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

void shutdown();
void int_handler(int);

int shm_id;
char* shm_addr;

int main() {
  signal(SIGINT, int_handler);

  key_t key = ftok("./writer.c", KEY);
  if(key == -1) {
    perror("Failed to create key.");
    exit(1);
  }

  shm_id = shmget(key, SIZE, IPC_CREAT|S_IRUSR|S_IWUSR);
  if(shm_id < 0) {
    perror("Failed to create shared memory.");
    exit(1);
  }

  shm_addr = shmat(shm_id, 0, 0);
  if(shm_addr == (void*)-1) {
    perror("Failed to attach memory.");
    exit(1);
  }

  *shm_addr = 0;

  while(1) {
    printf("Enter message to write...\n");
    fgets(shm_addr+DATAOFFSET, SIZE-DATAOFFSET, stdin);

    *shm_addr = 1;
    if(strcmp(shm_addr+DATAOFFSET, "quit\n") == 0) { shutdown(); }
    printf("Waiting for 2 processes to read it...\n");

    while(*shm_addr != 3);
    *shm_addr = 0;
  }
}

void int_handler(int sig_num) {
  strcpy(shm_addr+DATAOFFSET, "quit\n");
  *shm_addr = 1;

  shutdown();
}

void shutdown() {
  if(shmdt(shm_addr) < 0) {
    perror("Failed to detach memory.");
    exit(1);
  }

  if(shmctl(shm_id, IPC_RMID, 0) < 0) {
    perror("Failed to deallocate memory.");
    exit(1);
  }

  exit(0);
}

