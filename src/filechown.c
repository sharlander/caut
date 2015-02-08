#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <pwd.h>
#include <grp.h>

int file_chown(int debug, char filename[200], char owner[200], char group[200]) {

  int uid, gid;

  if(strcmp(owner, "0")==0)
    uid = getuid();
  else
    uid = getuserid(owner);

  if(strcmp(group, "0")==0)
    gid = getgid();
  else
    gid = getgroupid(group);

  if(debug==0) {
    printf("changing file owner to: %d\n", uid);
    printf("changing file group to: %d\n", gid);
  }

  if(chown(filename, uid, gid)) {
    perror("Error: chown failed");
    exit(1);
  }
}


int getgroupid(char *string) {

  struct group *grp;
  short int    lp;

  if (NULL == (grp = getgrnam(string))) {
    perror("getgrnam() error.");
    exit(1);
  }

  return grp->gr_gid;
}

int getuserid(char *string) {

  struct passwd pwd;
  struct passwd *result;
  char *buf;
  size_t bufsize;
  int s;

  bufsize = sysconf(_SC_GETPW_R_SIZE_MAX);
  if (bufsize == -1)          /* Value was indeterminate */
    bufsize = 16384;        /* Should be more than enough */

  buf = malloc(bufsize);
  if (buf == NULL) {
    perror("malloc");
    exit(1);
  }

  s = getpwnam_r(string, &pwd, buf, bufsize, &result);
  if (result == NULL) {
    if (s == 0)
      printf("Not found\n");
    else {
      errno = s;
      perror("getpwnam_r");
    }
    exit(1);
  }

  return (int)pwd.pw_uid;
}
