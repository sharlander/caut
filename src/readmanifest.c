#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "structs.h"

int manifest_getentry (char line[1024], char savechar[200]);
int manifest_removeticks (char *string);

int readmanifests(int verbose, int debug, char *path) {

  FILE *fp;
  char buffer[1024];
  int i, count;

  char manifest[512];

  /* struct file_data file; */

  sprintf(manifest, "%s/manifests/init.pp", path);

  fp = fopen(manifest, "r");
  if(fp==NULL) {
    printf("Error: can't read manifest\n");
    exit(1);
  }

  if(debug==0)
    printf("reading manifest: %s\n", manifest);

  /* loop init manifest */
  while(feof(fp) != 1) {

    /* read a line of the manifest */
    fgets(buffer, 1024, fp);

    /* skip comments */
    if(buffer[0] == '#')
      continue;

    /* start file */
    if(strstr(buffer, "file {") != 0) {

      /* clean file struct for new file */
      strcpy(file.name , "0");
      strcpy(file.ensure , "0");
      strcpy(file.path , "0");
      strcpy(file.owner , "0");
      strcpy(file.group , "0");
      strcpy(file.mode , "0");

      /* read filename */
      for(count=0;; count++) {
        if(buffer[count] == 34 || buffer[count] == 39)
          break;
      }
      count++;
      for(i=0;; i++, count++) {
        if(buffer[count] == 34 || buffer[count] == 39)
          break;
        file.name[i] = buffer[count];
      }

      file.name[i] = '\0';

      if(debug==0)
        printf("found file ... %s\n", file.name);

      /* read file attributes */
      while (strstr(buffer, "}") == 0) {

        fgets(buffer, 1024, fp);

        if (strstr(buffer, "ensure") != 0)
          manifest_getentry(buffer, file.ensure);
        if (strstr(buffer, "path") != 0)
          manifest_getentry(buffer, file.path);
        if (strstr(buffer, "owner") != 0)
          manifest_getentry(buffer, file.owner);
        if (strstr(buffer, "group") != 0)
          manifest_getentry(buffer, file.group);
        if (strstr(buffer, "mode") != 0)
          manifest_getentry(buffer, file.mode);
      }
      /* done reading file resource, create file */
      if(verbose==0)
        printf("ensure file ... %s\n", file.name);
      providefile(debug, path);
    }

    /* start package */
    if(strstr(buffer, "package {") != 0) {

      /* clean package struct for new package */
      strcpy(package.name , "0");
      strcpy(package.ensure , "0");

      /* read packagename */
      for(count=0;; count++) {
        if(buffer[count] == 34 || buffer[count] == 39)
          break;
      }
      count++;
      for(i=0;; i++, count++) {
        if(buffer[count] == 34 || buffer[count] == 39)
          break;
        package.name[i] = buffer[count];
      }

      package.name[i] = '\0';

      if(debug==0)
        printf("found package ... %s\n", package.name);

      /* read package attributes */
      while (strstr(buffer, "}") == 0) {

        fgets(buffer, 1024, fp);

        if (strstr(buffer, "ensure") != 0)
          manifest_getentry(buffer, package.ensure);
      }
      /* done reading file resource, create file */
      if(verbose==0)
        printf("ensure package ... %s\n", package.name);
      providepackage(debug, path);
    }
    /* start service */
    if(strstr(buffer, "service {") != 0) {

      /* clean service struct for new service */
      strcpy(service.name , "0");
      strcpy(service.ensure , "0");

      /* read servicename */
      for(count=0;; count++) {
        if(buffer[count] == 34 || buffer[count] == 39)
          break;
      }
      count++;
      for(i=0;; i++, count++) {
        if(buffer[count] == 34 || buffer[count] == 39)
          break;
        service.name[i] = buffer[count];
      }

      service.name[i] = '\0';

      if(debug==0)
        printf("found service ... %s\n", service.name);

      /* read service attributes */
      while (strstr(buffer, "}") == 0) {

        fgets(buffer, 1024, fp);

        if (strstr(buffer, "ensure") != 0)
          manifest_getentry(buffer, service.ensure);
        if (strstr(buffer, "enable") != 0)
          manifest_getentry(buffer, service.enable);
      }
      /* done reading file resource, create file */
      if(verbose==0)
        printf("ensure service ... %s\n", service.name);
      provideservice(debug, path);
    }
  }

  if(debug==0)
    printf("closing manifest\n");

  fclose(fp);

  return 0;
}

int manifest_getentry (char line[1024], char savechar[200]) {
  char *value;
  value = strtok(line, " => ");
  value = strtok(NULL, " => ");
  value[(strlen(value)-1)] = 0;
  manifest_removeticks(value);
  strcpy(savechar, value);
  return 0;
}

int manifest_removeticks (char *string) {
  if ((string[0] == 39) || (string[0] == 34))
    memmove(string, string+1, strlen(string));
  if ((string[(strlen(string)-1)] == 39) || (string[(strlen(string)-1)] == 34))
    string[(strlen(string)-1)] = 0;
  else if ((string[(strlen(string)-2)] == 39) || (string[(strlen(string)-2)] == 34)) {
    string[(strlen(string)-1)] = 0;
    string[(strlen(string)-1)] = 0;
   }
  else if ((string[(strlen(string)-1)] == ',') &&
           (string[(strlen(string)-2)] != 34) &&
           (string[(strlen(string)-2)] != 39)
          )
    string[(strlen(string)-1)] = 0;

  return 0;
}
