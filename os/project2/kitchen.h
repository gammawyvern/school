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
  const char* name;
  sem_t* location;
} Ingredient;

typedef struct {
  const char* name;
  Ingredient ingredients[10];
  int num_of_ingredients;
} Recipe;

void setup_kitchen(Kitchen* kitchen);
void cleanup_kitchen(Kitchen* kitchen);

