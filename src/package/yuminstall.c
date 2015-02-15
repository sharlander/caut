#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void yuminstall(int debug, char *name, char *path);

void yuminstall(int debug, char *name, char *path) {

  char buffer[512];
  int pid;
  char checkcmd[512];
  int re;

  FILE *fp;
  char packagelist[200];
  sprintf(packagelist, "%s/facts/%s.pg", path, name);

  fp = fopen(packagelist, "r");
  if(fp==NULL) {
    printf("Error: can't open package file for package: %s\n", name);
    exit(1);
  }

  if(debug==0)
    printf("reading package file ... %s\n", packagelist);

  /* loop package file */
  while(feof(fp) != 1) {

    /* read a line of the file */
    fgets(buffer, 512, fp);

    /* skip comments */
    if(buffer[0] == '#')
      continue;

    /* remove newline on line end */
    if(buffer[(strlen(buffer)-1)]=='\n')
      buffer[(strlen(buffer)-1)] = '\0';

    if(debug==0)
      printf("checking package: %s\n", buffer);

    sprintf(checkcmd, "/usr/bin/rpm -q --quiet %s", buffer);
    re = system(checkcmd);

    if(re != 0) {
      if(debug==0)
        printf("installing package: %s\n", buffer);
        char args[512];
        sprintf(args, "/usr/bin/yum install -y --quiet %s", buffer);
        re=system(args);

      if(re != 0) {
        printf("Error: installation of package %s failed\n", buffer);
        exit(1);
      }
    }
    else
      if(debug==0)
        printf("%s ... already installed\n", buffer);
  }
}