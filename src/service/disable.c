#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void disable_service(int debug, char *name, char *path, int provider) {

  int re;
  char command[512];

  switch(provider) {
  case 1:
    if(debug==0)
      printf("disabling service ... %s\n", name);
    sprintf(command, "/usr/sbin/chkconfig %s ", name);
    re = system(command);
    if(re != 0) {
      printf("Error: disabling service failed\n");
      exit(1);
    }
    break;
  case 2:
    if(debug==0)
      printf("disabling service ... %s\n", name);
    sprintf(command, "/usr/bin/systemctl disable %s.service &>/dev/null", name);
    re = system(command);
    if(re != 0) {
      printf("Error: disabling service failed\n");
      exit(1);
    }
    break;
  default:
    printf("Error: unknown provider given\n");
    exit(1);
    break;
  }
}
