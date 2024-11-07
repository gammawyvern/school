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
  unsigned int hash = baker->id * 0x9e3779b9;

  color[0] = (hash & 0xFF0000) >> 16;
  color[1] = (hash & 0x00FF00) >> 8;
  color[2] = (hash & 0x0000FF);

  unsigned char min_value = 100;
  for(int channel=0; channel<3; channel++) {
    if(color[channel] == 0) {
      color[channel] = min_value;
      continue;
    }

    float percent = 255 / color[channel];
    color[channel] = min_value + ((255 - min_value) * percent);
    baker->color[channel] = color[channel];
  }
}

void print_baker_message(Baker* baker, char* message) {
  printf("\033[38;2;%d;%d;%dm", baker->color[0], baker->color[1], baker->color[2]);
  printf("\033[48;2;%d;%d;%dm", 5, 5, 5);
  printf("[%ld] %s", baker->id, message);
  printf("\033[0m\n");
}

void* run_baker_thread(void* arg) {
  Baker* baker = (Baker*)arg;

  sem_wait(&(baker->kitchen->mixer));
  print_baker_message(baker, "Using mixer");
  sleep(1);
  sem_post(&(baker->kitchen->mixer));

  return NULL;
}

