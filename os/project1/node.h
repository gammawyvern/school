#pragma once

#define READ 0
#define WRITE 1

void createNodeRing();
void addRingNodes(int);

void intHandler(int);
void stopHandler(int);

extern int id;
extern int circleSize;
extern int nodePipe[2];

