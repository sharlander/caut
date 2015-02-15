#include <stdio.h>

void uninstall_package(int debug, char *name, char *path) {
  /* this could be used to call different package providers */
  /* right now i only support yum so i call yum directly */
  yumuninstall(debug, name, path);
}
