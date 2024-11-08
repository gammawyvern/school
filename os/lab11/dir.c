#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>

int main() {
  DIR *dirPtr;
  struct stat statBuf;
  struct dirent *entryPtr;

  dirPtr = opendir("./");
  while((entryPtr = readdir(dirPtr))) {
    if (strcmp(entryPtr->d_name, ".") == 0) {
      continue;
    }

    if (strcmp(entryPtr->d_name, "..") == 0) {
      continue;
    }

    stat(entryPtr->d_name, &statBuf);
    printf("%-10s(%d bytes)\n", entryPtr->d_name, statBuf.st_size);
  }

  closedir (dirPtr);
  return 0;
}

