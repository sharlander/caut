#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int factertofile(char path[200]) {

  char buffer[1024];

  char infile[512], outfile[512];
  FILE *infp, *outfp;

  sprintf(infile, "%s/facts/.common.ft", path);
  sprintf(outfile, "%s/facts/common.ft", path);

  infp = fopen(infile, "w");
  if(infp==NULL) {
    printf("Error: can't write to facts directory\n");
    exit(1);
  }
  outfp = fopen(outfile, "w");
  if(infp==NULL) {
    printf("Error: can't write to facts directory\n");
    exit(1);
  }

  fclose(infp);
  fclose(outfp);

  int fd;

  /* if((fd = open(infile, O_RDWR | O_CREAT))==-1){ /\*open the file *\/ */
  if((fd = open(infile, O_RDWR | O_CREAT | O_SYNC, 0600))==-1){ /*open the file */
    perror("open");
    return 1;
  }

  int pid;

  pid = fork();

  if(pid == 0) {
  dup2(fd,STDOUT_FILENO); /*copy the file descriptor fd into standard output*/
  close(fd); /* close the file descriptor as we don't need it more  */

  execl( "/usr/bin/facter", (char *) 0 );
  }
  else
    wait(pid);

  infp = fopen(infile, "r");
  outfp = fopen(outfile, "w");

  char *ptr;

  fgets(buffer, 1024, infp);

  do {
    ptr = strtok(buffer, " => ");
    fputs(ptr, outfp);
    fputc('=', outfp);
    ptr = strtok(NULL, " => ");
    fputs(ptr, outfp);

    fgets(buffer, 1024, infp);
  } while(feof(infp) != 1);

  fclose(infp);
  fclose(outfp);

  remove(infile);
}
