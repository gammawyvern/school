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
      }

      printf("Forwarding data");
      write(nextPipe[WRITE], data, sizeof(struct message));
      // write(nextPipe[WRITE], data->text, data->len);

      apple = 0;
    } else {
      read(lastPipe[READ], data, sizeof(struct message));
      // data->text = malloc(data->len + 1); // add +1 for \0
      // read(lastPipe[READ], data->text, data->len);
      // data->text[data->len] = '\0';

      apple = 1;
    }
  }
}

struct message* createMessage() {
  struct message* msg = malloc(sizeof(struct message));

  char* tmpText = NULL;
  int dstID = -1;
  size_t bufferSize = 0;
  size_t msgLen = 0;

  printf("Enter message to send: ");
  msgLen = getline(&tmpText, &bufferSize, stdin);
  // Remove newline from message
  if(tmpText[msgLen - 1] == '\n') {
    tmpText[msgLen - 1] == '\0';
    msgLen--;
  }

  while(dstID < 0 || dstID >= circleSize) {
    printf("Enter destination node id: ");
    scanf("%d", &dstID);
    while(getchar() != '\n');
  }

  msg->src = id;
  msg->dst = dstID;
  msg->len = msgLen;
  msg->text = malloc(msgLen + 1);
  strcpy(msg->text, tmpText);

  return msg;
}

void intHandler(int sigNum) {
  free(data); 
}

