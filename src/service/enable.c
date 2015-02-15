#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void enable_service(int debug, char *name, char *path, int provider) {

  int re;
  char command[512];

  switch(provider) {
  case 1:
    if(debug==0)
      printf("enabling service ... %s\n", name);
    sprintf(command, "/usr/sbin/chkconfig %s on", name);
    re = system(command);
    if(re != 0) {
      printf("Error: enabling service failed\n");
      exit(1);
    }
    break;
  case 2:
    if(debug==0)
      printf("enabling service ... %s\n", name);
    sprintf(command, "/usr/bin/systemctl enable %s.service &>/dev/null", name);
    re = system(command);
    if(re != 0) {
      printf("Error: enabling service failed\n");
      exit(1);
    }
    break;
  default:
    printf("Error: unknown provider given\n");
    exit(1);
    break;
  }
}
