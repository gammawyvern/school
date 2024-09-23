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
    free(data.text);
    data = createMessage();
  }

  printf("[%d]\t| Forwarding recieved message headed to [%d]\n", id, data.dst);
  write(nodePipe[WRITE], &data, sizeof(struct message));
  write(nodePipe[WRITE], data.text, data.len);
  free(data.text);

  communicate();
}

struct message createMessage() {
  struct message msg;
  msg.src = id;

  size_t bufferSize = 0;
  printf("[%d]\t| Enter message: ", id);
  msg.len = getline(&msg.text, &bufferSize, stdin);
  if(msg.text[msg.len - 1] == '\n') {
    msg.text[msg.len - 1] = '\0';
    msg.len--;
  }

  msg.dst = -1;
  while(msg.dst < 0 || msg.dst >= circleSize) {
    printf("[%d]\t| Enter destination node id: ", id);
    scanf("%d", &msg.dst);
    while(getchar() != '\n');
  }

  return msg;
}

