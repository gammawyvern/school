#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  DIR *dirPtr;
  struct stat statBuf;
  struct dirent *entryPtr;

  if (argc < 2) {
    printf("Usage: argument (filename) expected\n");
    exit(1);
  }

  dirPtr = opendir(argv[1]);
  if (dirPtr == 0) {
    perror("Invalid directory");
    exit(1);
  }

  while((entryPtr = readdir(dirPtr))) {
    if (strcmp(entryPtr->d_name, ".") == 0) {
      continue;
    }

    if (strcmp(entryPtr->d_name, "..") == 0) {
      continue;
    }

    stat(entryPtr->d_name, &statBuf);
    char* name = entryPtr->d_name;
    uid_t uid = statBuf.st_uid;
    gid_t gid = statBuf.st_gid;
    ino_t ino = statBuf.st_ino;

    printf("%u %u %8u %s\n", uid, gid, ino, name);
  }

  closedir (dirPtr);
  return 0;
}

