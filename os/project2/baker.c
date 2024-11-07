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
  printf("\033[38;2;%d;%d;%dm", baker->color[0], baker->color[1], baker->color[2]);
  printf("\033[48;2;%d;%d;%dm", 5, 5, 5);
  printf("[%lu] %s", baker->id, message);
  printf("\033[0m\n");
}

void* run_baker_thread(void* arg) {
  Baker* baker = (Baker*)arg;

  Ingredient cookies[4] = {
    (Ingredient) {.name = "Flour", .location = &baker->kitchen->pantry},
    (Ingredient) {.name = "Eggs", .location = &baker->kitchen->refrigerator},
    (Ingredient) {.name = "Sugar", .location = &baker->kitchen->pantry},
    (Ingredient) {.name = "Milk", .location = &baker->kitchen->refrigerator}
  };

  for(int ing=0; ing<4; ing++) {
    char message[100];
    sprintf(message, "Grabbing %s", cookies[ing].name);

    sem_wait(cookies[ing].location);
    print_baker_message(baker, message);
    sleep(2);
    sem_post(cookies[ing].location);
  }

  return NULL;
}

