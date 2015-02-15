#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void delete_file(char name[200]) {

  int re;
  re = remove(name);
  if(re!=0) {
    perror("Error: cant remove file");
    exit(1);
  }
}
