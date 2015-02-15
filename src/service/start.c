#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void start_service(int debug, char *name, char *path, int provider) {

  int re=0;
  char command[512];

  switch(provider) {
  case 1:
    sprintf(command, "/etc/init.d/%s status >/dev/null", name);
    re = system(command);
    if(re != 0) {
      if(debug==0)
        printf("starting service ... %s\n", name);
      sprintf(command, "/etc/init.d/%s start >/dev/null", name);
      re = system(command);
      if(re!=0) {
        printf("Error: couldn't start service: %s\n", name);
        exit(1);
      }
    }
    if(debug==0)
      printf("%s ... already running\n", name);
    break;
  case 2:
    sprintf(command, "/usr/bin/systemctl status %s.service &>/dev/null", name);
    re = system(command);
    if(re != 0) {
      if(debug==0)
        printf("starting service ... %s\n", name);
      sprintf(command, "/usr/bin/systemctl start %s.service &>/dev/null", name);
      re = system(command);
      if(re!=0) {
        printf("Error: couldn't start service: %s\n", name);
        exit(1);
      }
    }
    else
      if(debug==0)
        printf("%s ... already running\n", name);
    break;
  default:
    printf("Error: unknown provider given\n");
    exit(1);
    break;
  }
}
