#include "baker.h"

Baker create_baker(Kitchen* kitchen, int id) {
  Baker baker = {
    .kitchen = kitchen,
    .id = id,
    .color = {255, 255, 255}
  };

  set_color(&baker);

  return baker;
}

void set_color(Baker* baker) {
  unsigned char color[] = {255, 255, 255};
  unsigned int hash = baker->id * 0x517cc1b7;

  color[0] = (hash & 0xFF0000) >> 16;
  color[1] = (hash & 0x00FF00) >> 8;
  color[2] = (hash & 0x0000FF);

  unsigned char min_value = 50;
  for(int channel=0; channel<3; channel++) {
    if(color[channel] == 0) {
      color[channel] = min_value;
      continue;
    }

    float percent = (float)color[channel] / 255.0f;
    color[channel] = min_value + ((255 - min_value) * percent);
    baker->color[channel] = color[channel];
  }
}

void print_baker_message(Baker* baker, char* message) {
  char formatted_message[124];
  snprintf(
    formatted_message, sizeof(formatted_message),
    "\033[38;2;%d;%d;%dm\033[48;2;5;5;5m[%lu] %s\033[0m\n",
    baker->color[0], baker->color[1], baker->color[2],
    baker->id, message
  );

  printf("%s", formatted_message);
}

void* run_baker_thread(void* arg) {
  Baker* baker = (Baker*)arg;

  Recipe recipes[] = {
    (Recipe) {
      .name = "Cookies",
      .num_of_ingredients = 4,
      .ingredients = {
        {.name = "Flour", .location = &baker->kitchen->pantry},
        {.name = "Sugar", .location = &baker->kitchen->pantry},
        {.name = "Eggs", .location = &baker->kitchen->refrigerator},
        {.name = "Milk", .location = &baker->kitchen->refrigerator}
      }
    },
    (Recipe) {
      .name = "Pancakes",
      .num_of_ingredients = 7,
      .ingredients = {
        {.name = "Flour", .location = &baker->kitchen->pantry},
        {.name = "Sugar", .location = &baker->kitchen->pantry},
        {.name = "Baking Soda", .location = &baker->kitchen->pantry},
        {.name = "Salt", .location = &baker->kitchen->pantry},
        {.name = "Eggs", .location = &baker->kitchen->refrigerator},
        {.name = "Milk", .location = &baker->kitchen->refrigerator},
        {.name = "Butter", .location = &baker->kitchen->refrigerator}
      }
    },
    (Recipe) {
      .name = "Pizza Dough",
      .num_of_ingredients = 3,
      .ingredients = {
        {.name = "Yeast", .location = &baker->kitchen->pantry},
        {.name = "Sugar", .location = &baker->kitchen->pantry},
        {.name = "Salt", .location = &baker->kitchen->pantry}
      }
    },
    (Recipe) {
      .name = "Soft Pretzels",
      .num_of_ingredients = 6,
      .ingredients = {
        {.name = "Flour", .location = &baker->kitchen->pantry},
        {.name = "Sugar", .location = &baker->kitchen->pantry},
        {.name = "Salt", .location = &baker->kitchen->pantry},
        {.name = "Yeast", .location = &baker->kitchen->pantry},
        {.name = "Baking Soda", .location = &baker->kitchen->pantry},
        {.name = "Eggs", .location = &baker->kitchen->refrigerator},
      }
    },
    (Recipe) {
      .name = "Cinnamon Rolls",
      .num_of_ingredients = 6,
      .ingredients = {
        {.name = "Flour", .location = &baker->kitchen->pantry},
        {.name = "Sugar", .location = &baker->kitchen->pantry},
        {.name = "Salt", .location = &baker->kitchen->pantry},
        {.name = "Butter", .location = &baker->kitchen->refrigerator},
        {.name = "Eggs", .location = &baker->kitchen->refrigerator},
        {.name = "Cinnamon", .location = &baker->kitchen->pantry}
      }
    },
  };

  char message[100];
  for(int rec=0; rec<5; rec++) {
    for(int ing=0; ing<recipes[rec].num_of_ingredients; ing++) {
      sprintf(message, "Grabbing %s", recipes[rec].ingredients[ing].name);

      sem_wait(recipes[rec].ingredients[ing].location);
      print_baker_message(baker, message);
      sem_post(recipes[rec].ingredients[ing].location);
    }

    sem_wait(&baker->kitchen->spoon);
    print_baker_message(baker, "Grabbing spoon");

    sem_wait(&baker->kitchen->bowl);
    print_baker_message(baker, "Grabbing bowl");

    sem_wait(&baker->kitchen->mixer);
    print_baker_message(baker, "Grabbing mixer");

    print_baker_message(baker, "Mixed ingredients");

    sem_wait(&baker->kitchen->oven);
    print_baker_message(baker, "Using oven");

    // TODO should spoon/mixer move above?
    sem_post(&baker->kitchen->spoon);
    sem_post(&baker->kitchen->bowl);
    sem_post(&baker->kitchen->mixer);
    sem_post(&baker->kitchen->oven);

    sprintf(message, "Finished creating %s", recipes[rec].name);
    print_baker_message(baker, message);
  }

  print_baker_message(baker, "FINISHED");

  return NULL;
}

