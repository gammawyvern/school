#pragma once

#define READ 0
#define WRITE 1

void addRingNodes();
void createNodeRing();

void intHandler(int);
void stopHandler(int);

extern int id;
extern int circleSize;
extern int rootPipe[2];
extern int lastPipe[2];
extern int nextPipe[2];
extern int apple;  

