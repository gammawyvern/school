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
        printf("[%d] recieved message from [%d]: %s\n", id, data.src, data.text);
        free(data.text);

        data = createMessage();
      }

      printf("[%d] forwarding message to [%d]\n", id, data.dst);
      write(nextPipe[WRITE], &data, sizeof(struct message));
      write(nextPipe[WRITE], data.text, data.len);
      apple = 0;
    } else {
      read(lastPipe[READ], &data, sizeof(struct message));
      data.text = malloc(data.len);
      read(lastPipe[READ], data.text, data.len);
      apple = 1;
    }
  }
}

struct message createMessage() {
  struct message msg;

  char* tmpText = NULL;
  size_t bufferSize = 0;
  printf("[%d] Enter message: ", id);
  size_t textLen = getline(&tmpText, &bufferSize, stdin);

  if(tmpText[textLen - 1] == '\n') {
    tmpText[textLen - 1] = '\0';
    textLen--;
  }

  int dstID = -1;
  while(dstID < 0 || dstID >= circleSize) {
    printf("[%d] Enter destination node id: ", id);
    scanf("%d", &dstID);
    while(getchar() != '\n');
  }

  msg.src = id;
  msg.dst = dstID;
  msg.len = textLen;
  msg.text = malloc(msg.len); 
  strcpy(msg.text, tmpText);

  return msg;
}

