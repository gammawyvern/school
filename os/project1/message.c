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
  char tmpText[120];
  msg.src = id;

  printf("[%d]\t| Enter message: ", id);
  fgets(tmpText, sizeof(tmpText), stdin);

  msg.len = strlen(tmpText);
  if(msg.len > 0 && tmpText[msg.len - 1] == '\n') {
    tmpText[msg.len - 1] = '\0';
    msg.len--;
  }

  msg.dst = -1;
  while(msg.dst < 0 || msg.dst >= circleSize) {
    printf("[%d]\t| Enter destination node id: ", id);
    scanf("%d", &msg.dst);
    while(getchar() != '\n');
  }

  msg.text = malloc(msg.len);
  strcpy(msg.text, tmpText);

  return msg;
}

