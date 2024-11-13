#pragma once

#include <semaphore.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>

#include "kitchen.h"

typedef struct {
  Kitchen* kitchen;
  unsigned long id;
  unsigned char color[3];
} Baker;

Baker create_baker(Kitchen*, int);
void set_color(Baker*);
void print_baker_message(Baker*, char*);
void* run_baker_thread(void*);

