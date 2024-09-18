#pragma once

#include <sys/types.h>

#define READ 0
#define WRITE 1

struct message {
  int src; // Source node ID
  int dst; // Destination node ID
};

void communicate();
struct message createMessage();

extern struct message data;

