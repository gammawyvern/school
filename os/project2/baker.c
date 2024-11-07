#include "baker.h"

void id_to_color(int id, unsigned char color[]) {
  // AI generated simple hash?
  // Apparently the number is related to the golden ratio?
  // We'll try it!
  // unsigned int hash = id * 2654435761u;
  unsigned int hash = id * 0x9e3779b9;

  color[0] = (hash & 0xFF0000) >> 16;
  color[1] = (hash & 0x00FF00) >> 8;
  color[2] = (hash & 0x0000FF);

  // Convert to range > 50 per color
  unsigned char min_value = 100;
  for(int channel=0; channel<3; channel++) {
    if(color[channel] == 0) {
      color[channel] = min_value;
      continue;
    }

    float percent = 255 / color[channel];
    color[channel] = min_value + ((255 - min_value) * percent);
  }
}

void print_baker_message(Baker* baker, char* message) {
  unsigned char color[] = {255, 255, 255};
  id_to_color(baker->id, color);
  unsigned char bg[] = {5, 5, 5};

  printf("\033[38;2;%d;%d;%dm", color[0], color[1], color[2]);
  printf("\033[48;2;%d;%d;%dm", bg[0], bg[1], bg[2]);
  printf("[%ld] - %s", baker->id, message);
  printf("\033[0m\n");
}

void* create_baker(void* arg) {
  Baker* baker = (Baker*)arg;
  
  // Setup needed recipes, not sure about a better way tbh
  Recipe cookies = {
    ingredients = [
      Ingredient {
        name = "Flour",
        location = baker->kitchen->pantry
      },
      Ingredient {
        name = "Sugar",
        location = baker->kitchen->pantry
      },
      Ingredient {
        name = "Milk",
        location = baker->kitchen->refridgerator
      },
      Ingredient {
        name = "Butter",
        location = baker->kitchen->refridgerator
      }
    ]
  };

  sem_wait(&(baker->kitchen->mixer));
  print_baker_message(baker, "Using mixer");
  // printf("[%lu] - Using mixer\n", baker->id);
  sleep(1);
  sem_post(&(baker->kitchen->mixer));

  return NULL;
}

