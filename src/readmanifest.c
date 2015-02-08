#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int manifest_getentry (char line[1024], char savechar[200]);
int manifest_removeticks (char *string);

int readmanifests(int verbose, int debug, char *path) {

  struct data {
    char name[200];
    char ensure[200];
    char path[200];
    char owner[200];
    char group[200];
    char mode[200];
  } file[100];



  FILE *fp;

  char buffer[1024];
  int filecount=0;
  int i, count;

  char manifest[512];

  // fill all with a std value
  strcpy(file[100].name, "0");
  strcpy(file[100].ensure, "0");
  strcpy(file[100].path, "0");
  strcpy(file[100].owner, "0");
  strcpy(file[100].group, "0");

  for (count=0;count<100;count++)
    file[count] = file[100];

  sprintf(manifest, "%s/manifests/init.pp", path);

  fp = fopen(manifest, "r");
  if(fp==NULL) {
    printf("Error: can't read manifest\n");
    exit(1);
  }

  if(debug==0)
    printf("reading manifest: %s\n", manifest);

  while(feof(fp) != 1) {

    fgets(buffer, 1024, fp);

    if(buffer[0] == '#')
      continue;

    if(strstr(buffer, "file {") != 0) {

      ++filecount;

      for(count=0;; count++) {
        if(buffer[count] == 34 || buffer[count] == 39)
          break;
      }
      count++;
      for(i=0;; i++, count++) {
        if(buffer[count] == 34 || buffer[count] == 39)
          break;
        file[filecount].name[i] = buffer[count];
      }

      file[filecount].name[i] = '\0';

      if(debug==0)
        printf("found file ... %s\n", file[filecount].name);

      while (strstr(buffer, "}") == 0) {

        fgets(buffer, 1024, fp);

        if (strstr(buffer, "ensure") != 0)
          manifest_getentry(buffer, file[filecount].ensure);
        if (strstr(buffer, "path") != 0)
          manifest_getentry(buffer, file[filecount].path);
        if (strstr(buffer, "owner") != 0)
          manifest_getentry(buffer, file[filecount].owner);
        if (strstr(buffer, "group") != 0)
          manifest_getentry(buffer, file[filecount].group);
        if (strstr(buffer, "mode") != 0)
          manifest_getentry(buffer, file[filecount].mode);
      }
    }
  }

  if(debug==0)
    printf("closing manifest\n");

  for(i=1; i<=filecount; i++) {
    if(verbose==0)
      printf("creating ... %s\n", file[i].name);

    /* create file */
    parsetemplate(debug, path, file[i].name, file[i].path);

    /* set file attributes */
    /* user, group */
    if((strcmp(file[i].owner, "0")!=0) || (strcmp(file[i].group, "0")!=0))
      file_chown(debug, file[i].path, file[i].owner, file[i].group);

    /* mode */
    if(debug==0)
      printf("changing file mode to: %s\n", file[i].mode);
    if(strcmp(file[i].mode, "0")!=0)
      chmod(file[i].path, strtol(file[i].mode, 0, 8));
  }

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
