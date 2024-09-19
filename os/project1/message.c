#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "node.h"
#include "message.h"

struct message data;

void communicate() {
  read(nodePipe[READ], &data, sizeof(struct message));
  data.text = malloc(data.len);
  read(nodePipe[READ], data.text, data.len);

  if(data.dst == id) {
    printf("[%d]\t| Recieved message from [%d]: %s\n", id, data.src, data.text);
    char* oldText = data.text;
    data = createMessage();
    free(oldText);
  }

  printf("[%d]\t| Forwarding message headed to [%d]\n", id, data.dst);
  write(nodePipe[WRITE], &data, sizeof(struct message));
  write(nodePipe[WRITE], data.text, data.len);

  communicate();
}

struct message createMessage() {
  struct message msg;

  char* tmpText = NULL;
  size_t bufferSize = 0;
  printf("[%d]\t| Enter message: ", id);
  size_t textLen = getline(&tmpText, &bufferSize, stdin);

  if(tmpText[textLen - 1] == '\n') {
    tmpText[textLen - 1] = '\0';
    textLen--;
  }

  int dstID = -1;
  while(dstID < 0 || dstID >= circleSize) {
    printf("[%d]\t| Enter destination node id: ", id);
    scanf("%d", &dstID);
    while(getchar() != '\n');
  }

  msg.src = id;
  msg.dst = dstID;
  msg.len = textLen;
  msg.text = malloc(msg.len); 
  strcpy(msg.text, tmpText);
  // Needed??
  free(tmpText);

  return msg;
}

