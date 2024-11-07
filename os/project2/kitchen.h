#pragma once

#include <semaphore.h>

typedef struct {
  sem_t mixer;
  sem_t pantry;
  sem_t refrigerator;
  sem_t bowl;
  sem_t spoon;
  sem_t oven;
} Kitchen;

typedef struct {
  char* name;
  sem_t* location;
  int obtained;
} Ingredient;

typedef struct {
  Ingredient* ingredients;
} Recipe;

void setup_kitchen(Kitchen* kitchen);
void cleanup_kitchen(Kitchen* kitchen);

