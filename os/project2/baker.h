#pragma once

#include <semaphore.h>
#include <unistd.h>
#include <stdio.h>

#include "kitchen.h"

typedef struct {
  Kitchen* kitchen;
  unsigned long id;
} Baker;

void* create_baker(void* arg);

