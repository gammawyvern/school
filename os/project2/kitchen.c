#include "kitchen.h"

void setup_kitchen(Kitchen* kitchen) {
  sem_init(&kitchen->pantry, 0, 1);
  sem_init(&kitchen->refrigerator, 0, 2);
  sem_init(&kitchen->spoon, 0, 5);
  sem_init(&kitchen->bowl, 0, 3);
  sem_init(&kitchen->mixer, 0, 2);
  sem_init(&kitchen->oven, 0, 1);
}

void cleanup_kitchen(Kitchen* kitchen) {
  sem_destroy(&kitchen->pantry);
  sem_destroy(&kitchen->refrigerator);
  sem_destroy(&kitchen->spoon);
  sem_destroy(&kitchen->bowl);
  sem_destroy(&kitchen->mixer);
  sem_destroy(&kitchen->oven);
}

