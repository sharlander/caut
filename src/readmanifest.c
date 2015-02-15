#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "structs.h"

int manifest_getentry (char line[1024], char savechar[200]);
int manifest_removeticks (char *string);
int fpcounter=0;
FILE *fp;
FILE *fp2[100];

int readmanifests(int verbose, int debug, char *path) {

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
  while(set_manifest_fp(buffer, path) == 1) {

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
      strcpy(file.target , "0");
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
        if (strstr(buffer, "target") != 0)
          manifest_getentry(buffer, file.target);
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

int set_manifest_fp(char buffer[1024], char *path) {

  char manifestname[512];
  char manifestpath[512];
  int count;
  int i;

  if(strstr(buffer, "include") != 0) {
    /* read manifestname */
    for(count=0;; count++) {
      if(buffer[count] == 34 || buffer[count] == 39)
        break;
    }
    count++;
    for(i=0;; i++, count++) {
      if(buffer[count] == 34 || buffer[count] == 39)
        break;
      manifestname[i] = buffer[count];
    }
    manifestname[i] = '\0';

    fpcounter++;

    fp2[fpcounter] = fp;

    sprintf(manifestpath, "%s/manifests/%s.pp", path, manifestname);

    fp = fopen(manifestpath, "r");
    if(fp==NULL) {
      printf("Error: can't open manifest %s\n", manifestpath);
      exit(1);
    }
    return 1;
  }
  else {
    if(feof(fp) == 1) {
      if(fpcounter>0) {
        fp = fp2[fpcounter];
        fpcounter--;
        return 1;
      }
      return 0;
    }
    else {
      return 1;
    }
  }
  return 0;
}
