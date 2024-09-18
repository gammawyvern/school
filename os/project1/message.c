#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "node.h"
#include "message.h"

struct message* data = NULL;
char* messageText = NULL;

void communicate() {
  while(1) {
    if(apple == 1) {
      if(data->dst == id) {
        printf("[%d] recieved message: %s", id, data->text);

        free(data->text);
        free(data);
        data = createMessage();

        write(nextPipe[WRITE], data, sizeof(struct message));
      } else {
        write(nextPipe[WRITE], data, sizeof(struct message));
      }

      apple = 0;
    } else {
      read(lastPipe[READ], data, sizeof(struct message));
      apple = 1;
    }
  }
}

struct message* createMessage() {
  int dstID = -1;
  size_t msgLen;

  printf("Enter message to send: ");
  getline(&messageText, &msgLen, stdin);

  while(dstID < 0 || dstID >= circleSize) {
    printf("Enter destination node id: ");
    scanf("%d", &dstID);
    while(getchar() != '\n');
  }

  struct message* message = malloc(sizeof(struct message));
  message->src = id;
  message->dst = dstID;
  message->text = malloc(msgLen + 1);
  strcpy(message->text, messageText);
  message->len = msgLen;

  return message;
}

void intHandler(int sigNum) {
  free(data); 
}

