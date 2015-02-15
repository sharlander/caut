#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int getentry (char line[200], char savechar[200]);
int removeticks (char *string);
extern char *path;

void caut_getfact(char *name, char *value) {

  FILE *fact;
  char infact[512];
  char buffer[1024];

  /* open facter file */

  sprintf(infact, "%s/facts/facter.ft", path);
  fact = fopen(infact, "r");

  if(fact!=NULL) {
    do {
      fgets(buffer, 200, fact);
      if (strstr(buffer, name) != 0) {
        getentry(buffer, value);
      }
    } while(feof(fact) != 1);

    fclose(fact);
  }

  /* open common facts file */

  sprintf(infact, "%s/facts/common.ft", path);
  fact = fopen(infact, "r");

  if(fact!=NULL) {
    do {
      fgets(buffer, 200, fact);
      if (strstr(buffer, name) != 0) {
        getentry(buffer, value);
      }
    } while(feof(fact) != 1);

    fclose(fact);
  }
}

int getentry (char line[200], char savechar[200]) {
  char *value;
  value = strtok(line, "=");
  value = strtok(NULL, "=");
  value[(strlen(value)-1)] = 0;
  removeticks(value);
  strcpy(savechar, value);
  return 0;
}

int removeticks (char *string) {
  if ((string[0] == 39) || (string[0] == 34))
    memmove(string, string+1, strlen(string));
  if ((string[(strlen(string)-1)] == 39) || (string[(strlen(string)-1)] == 34))
    string[(strlen(string)-1)] = 0;
  return 0;
}
