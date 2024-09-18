#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "node.h"
#include "message.h"

struct message data;

void communicate() {
  while(1) {
    if(apple == 1) {
      if(data.dst == id) {
        printf("[%d] recieved message from [%d]\n", id, data.src);
        data = createMessage();
      }

      printf("[%d] forwarding message to [%d]\n", id, data.dst);
      write(nextPipe[WRITE], &data, sizeof(struct message));
      apple = 0;
    } else {
      read(lastPipe[READ], &data, sizeof(struct message));
      apple = 1;
    }
  }
}

struct message createMessage() {
  struct message msg;

  printf("Enter message: ");
  getline();

  int dstID = -1;
  while(dstID < 0 || dstID >= circleSize) {
    printf("[%d] Enter destination node id: ", id);
    scanf("%d", &dstID);
    while(getchar() != '\n');
  }

  msg.src = id;
  msg.dst = dstID;

  return msg;
}

