#include <stdio.h>

void install_package(int debug, char *name, char *path) {
  /* this could be used to call different package providers */
  /* right now i only support yum so i call yum directly */
  yuminstall(debug, name, path);
}
