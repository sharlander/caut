#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void fill_var(int debug, char buffer[512], FILE *fact, FILE *commonfact);
int parsetemplate(int debug, char *path, char filename[200], char filepath[200]);
int getentry (char line[200], char savechar[200]);
int removeticks (char *string);

int parsetemplate(int debug, char *path, char filename[200], char filepath[200]) {

  FILE *fact, *commonfact, *templ, *out;

  char infact[512];
  char incommonfact[512];
  char intempl[512];

  sprintf(infact, "%s/facts/%s.ft", path, filename);
  sprintf(incommonfact, "%s/facts/common.ft", path);
  sprintf(intempl, "%s/template/%s", path, filename);

  templ = fopen(intempl, "r");
  if(templ==NULL) {
    printf("Error: can't open template file\n");
    exit(1);
  }
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

  out = fopen(filepath, "w");
  if(commonfact==NULL) {
    printf("Error: can't open target file: %s\n", filepath);
    exit(1);
  }

  char buffer[512];

  fgets(buffer, 512, templ);

  do {

    if(strstr(buffer, "<%$") != 0) {
      fill_var(debug, buffer, fact, commonfact);
      fputs(buffer, out);
    }
    else {
      fputs(buffer, out);
    }

    fgets(buffer, 512, templ);

  } while(feof(templ) != 1);

  fclose(out);
  fclose(templ);
  fclose(fact);
  fclose(commonfact);

  return 0;
}

void fill_var(int debug, char buffer[512], FILE *fact, FILE *commonfact) {

  int i,n,count;
  char tmpbuffer[512];
  char allfacts[200];
  char factname[200];
  char factvalue[200];
  int counter;

  // clear buffer
  for(i=0;i<=512;i++)
    tmpbuffer[i] = 0;

  // loop buffer
  for(i=0,n=0;i<=strlen(buffer);i++,n++) {

    if(buffer[i] != '<') {
      tmpbuffer[n] = buffer[i];
      continue;
    }
    if(buffer[++i] != '%') {
      tmpbuffer[n++] = '<';
      tmpbuffer[n] = buffer[i];
      continue;
    }
    if(buffer[++i] != '$') {
      tmpbuffer[n++] = '<';
      tmpbuffer[n++] = '%';
      tmpbuffer[n] = buffer[i];
      continue;
    }

    if(debug==0)
      printf("found fact ... looking for factname\n");

    // get factname
    for(counter=0; buffer[++i]!='%'; counter++) {
      factname[counter] = buffer[i];
    }
    factname[counter] = '\0';

    if(debug==0) {
      printf("found factname: %s\n", factname);
      printf("looking for factvalue\n");
    }

    factname[counter++] = '=';
    factname[counter] = '\0';

    // get fact value
    // common facts
    rewind(commonfact);
    do {
      fgets(allfacts, 200, commonfact);
      if (strstr(allfacts, factname) != 0)
        getentry(allfacts, factvalue);
    } while(feof(commonfact) != 1);
    // file specific facts
    rewind(fact);
    do {
      fgets(allfacts, 200, fact);
      if (strstr(allfacts, factname) != 0) {
        getentry(allfacts, factvalue);
      }
    } while(feof(fact) != 1);

    if(debug==0) {
      printf("found factvalue: %s\n", factvalue);
      printf("adding fact to buffer\n");
    }

    // add fact to buffer
    strcat(tmpbuffer, factvalue);

    if(debug==0)
      printf("current buffer: %s\n", tmpbuffer);

    // set n after fact
    n = strlen(tmpbuffer)-1;

    // jump over the closing '>'
    i++;
  }

  // copy tmpbuffer to common buffer
  strcpy(buffer, tmpbuffer);

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
