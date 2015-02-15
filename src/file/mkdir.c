#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void mkdir_file(char name[200]) {

  int re;
  re = mkdir(name);
  if(re>0) {
    perror("Error: cant create dir");
    exit(1);
  }
}
