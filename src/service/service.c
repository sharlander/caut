#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "../structs.h"

void provideservice(int debug, char *path);

void provideservice(int debug, char *path) {

  int provider=0;
  char providername[200];

  caut_getfact("provider_service", providername);
  if(debug==0)
    printf("provider is: %s\n", providername);

  if(strcmp(providername, "init") == 0) {
  /* init */
  provider=1;
  }
  else if(strcmp(providername, "systemd") == 0) {
  /* systemd */
  provider=2;
  }
  else {
    printf("Unkown service provider, please set the provier_service fact either to init or systemd\n");
    exit(1);
  }

  if((strcmp(service.ensure, "present") == 0 ) || (strcmp(service.ensure, "running") == 0 )) {
    /* start service */
    start_service(debug, service.name, path, provider);
  }
  else if((strcmp(service.ensure, "absent") == 0 ) || (strcmp(service.ensure, "stopped") == 0 )) {
    /* stop service */
    stop_service(debug, service.name, path, provider);
  }
  else {
    printf("Unknown ensure on service: %s => %s\n", service.name, service.ensure);
    exit(1);
  }

  if(strcmp(service.enable, "0") != 0 ) {
    if(strcmp(service.enable, "true") == 0 ) {
      enable_service(debug, service.name, path, provider);
    }
    else if(strcmp(service.enable, "false") == 0 ) {
      disable_service(debug, service.name, path, provider);
    }
    else {
      printf("Unknown enable on service: %s => %s\n", service.name, service.enable);
      exit(1);
    }
  }
}
