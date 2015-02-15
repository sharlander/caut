#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "../structs.h"

void providefile(int debug, char *path);

void providefile(int debug, char *path) {

  if((strcmp(file.ensure, "present") == 0) || (strcmp(file.ensure, "file") == 0)) {
    /* create file */
    parsetemplate(debug, path, file.name, file.path);

    /* set file attributes */
    /* user, group */
    if((strcmp(file.owner, "0")!=0) || (strcmp(file.group, "0")!=0))
      file_chown(debug, file.path, file.owner, file.group);

    /* mode */
    if(debug==0)
      printf("changing file mode to: %s\n", file.mode);
    if(strcmp(file.mode, "0")!=0)
      chmod(file.path, strtol(file.mode, 0, 8));
  }
  else if(strcmp(file.ensure, "directory") == 0) {
    /* create directory */
    mkdir_file(file.path);

    /* set file attributes */
    /* user, group */
    if((strcmp(file.owner, "0")!=0) || (strcmp(file.group, "0")!=0))
      file_chown(debug, file.path, file.owner, file.group);

    /* mode */
    if(debug==0)
      printf("changing file mode to: %s\n", file.mode);
    if(strcmp(file.mode, "0")!=0)
      chmod(file.path, strtol(file.mode, 0, 8));
  }
  else if(strcmp(file.ensure, "absent") == 0) {
    /* delete file */
    if(debug==0)
      printf("deleting file ... %s\n", file.path);
    delete_file(file.path);
  }
  else {
    printf("Unknown ensure on file: %s => %s\n", file.name, file.ensure);
    exit(1);
  }
}
