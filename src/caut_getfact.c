#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int getentry (char line[200], char savechar[200]);
int removeticks (char *string);

void caut_getfact(char *name, char *value) {

  FILE *facter, *commonfact;
  char infact[512];
  char incommonfact[512];

  sprintf(infact, "%s/facts/facter.ft", path);
  sprintf(incommonfact, "%s/facts/common.ft", path);

  /* open fact files */

  fact = fopen(infact, "r");
  if(fact==NULL) {
    printf("Error: can't open facts file\n");
    exit(1);
  }
  commonfact = fopen(incommonfact, "r");
  if(commonfact==NULL) {
    printf("Error: can't open common facts file\n");
    exit(1);
  }

  /* facter facts */
  do {
    fgets(allfacts, 200, fact);
    if (strstr(allfacts, factname) != 0) {
      getentry(allfacts, factvalue);
    }
  } while(feof(fact) != 1);

  /* common facts */
  do {
    fgets(allfacts, 200, commonfact);
    if (strstr(allfacts, factname) != 0)
      getentry(allfacts, factvalue);
  } while(feof(commonfact) != 1);

  fclose(facter);
  fclose(commonfact);
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
