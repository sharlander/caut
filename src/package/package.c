#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "../structs.h"

void providepackage(int debug, char *path) {

  if((strcmp(package.ensure, "present") == 0 ) || (strcmp(package.ensure, "installed") == 0 )) {
    /* install package */
    if(debug==0)
      printf("installing package... %s\n", package.name);
    install_package(debug, package.name, path);
  }
  else if((strcmp(package.ensure, "absent") == 0 ) || (strcmp(package.ensure, "uninstalled") == 0 )) {
    /* uninstall package */
    if(debug==0)
      printf("uninstalling package... %s\n", package.name);
    uninstall_package(debug, package.name, path);
  }
  else {
    printf("Unknown ensure on package: %s => %s\n", package.name, package.ensure);
    exit(1);
  }
}
