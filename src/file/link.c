#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void link_file(char name[200], char target[200]) {

  int re;
  if(access(name, F_OK) != -1) {
    re = remove(name);
    if(re!=0) {
      printf("Error: can't remove file: %s\n", name);
      exit(1);
    }
  }
  symlink(target, name);
  if(re!=0) {
    printf("Error: can't create symlink: %s -> %s\n", name, target);
    exit(1);
  }
}
