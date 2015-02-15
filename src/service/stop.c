#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void stop_service(int debug, char *name, char *path, int provider) {

  int re;
  char command[512];

  switch(provider) {
  case 1:
    if(debug==0)
      printf("stopping service ... %s\n", name);
    sprintf(command, "/etc/init.d/%s stop >/dev/null", name);
    re = system(command);
    if(re != 0) {
      printf("Error: stopping service failed\n");
      exit(1);
    }
    break;
  case 2:
    if(debug==0)
      printf("stopping service ... %s\n", name);
    sprintf(command, "/usr/bin/systemctl stop %s.service", name);
    re = system(command);
    if(re != 0) {
      printf("Error: stopping service failed\n");
      exit(1);
    }
    break;
  default:
    printf("Error: unknown provider given\n");
    exit(1);
    break;
  }
}
