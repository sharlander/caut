#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

char *path;

int main (int argc, char *argv[])
{

// read command line agruments

  /* "bool" values for all possible options */
  int optionpath=1;
  int optionfacter=1;
  int optionverbose=1;
  int optiondebug=1;
  /* counter and argument variables */
  int counter, count, argposition[argc], maxarg=1, arg=1;

  /* set all argposition values to 0 */
  for (counter=1; counter<argc; counter++)
    argposition[counter] = 0;

  /* loop through the arguments and set the option values */
  for (counter=1;counter<argc;counter++) {
    /* loop through the short options */
    if ((argv[counter][0] == '-') && (argv[counter][1] != '-'))
      for (count = 1; count < strlen(argv[counter]); count++) {
        switch (argv[counter][count]) {
        case 'h':
          help(argv[0]);
          return 0;
          break;
        case 'f':
          optionfacter = 0;
          if (count == 1)
            maxarg += 1;
          break;
        case 'd':
          optiondebug = 0;
          if (count == 1)
            maxarg += 1;
          break;
        case 'v':
          optionverbose = 0;
          if (count == 1)
            maxarg += 1;
          break;
        case 'p':
          optionpath = 0;
          if (count == 1)
            maxarg += 1;
          maxarg += 1;
          path = argv[counter+1];
          argposition[counter+1] = 1;
          break;
        default:
            printf("Error: unknown option\n");
            return 1;
            break;
        }
        /* if there is an option, mark the argposition with 1 */
        argposition[counter] = 1;
      }

    /* for long options just compare the strings */
    if (strcmp(argv[counter], "--help") == 0) {
      help(argv[0]);
      return 0;
    }
    if (strcmp(argv[counter], "--version") == 0) {
      version();
      return 0;
    }
    if (strcmp(argv[counter], "--facter") == 0) {
      optionfacter = 0;
      maxarg += 1;
      argposition[counter] = 1;
    }
    if (strcmp(argv[counter], "--debug") == 0) {
      optiondebug = 0;
      maxarg += 1;
      argposition[counter] = 1;
    }
    if (strcmp(argv[counter], "--verbose") == 0) {
      optionverbose = 0;
      maxarg += 1;
      argposition[counter] = 1;
    }
    if (strcmp(argv[counter], "--path") == 0) {
      optionpath = 0;
      maxarg += 2;
      path = argv[counter+1];
      argposition[counter] = 1;
      argposition[counter+1] = 1;
    }
  }

  /* if there were too many/less arguments show usage and exit */
  if (argc > maxarg) {
    printf("Error: Too many arguments\nUsage: %s [OPTIONS] [ARGUMENTS]\n", argv[0]);
    return 1;
  }
  else if (argc < maxarg) {
    printf("Error: Argument expected\nUsage: %s [OPTIONS] [ARGUMENTS]\n", argv[0]);
    return 1;
  }

  /* /\* loop through the arguments and check which value is not marked as an option *\/ */
  /* for (counter=1; counter<argc; counter++) */
  /*   if (argposition[counter] == 0) */
  /*     arg = counter; */

  /* /\* if the argument starts with a dash its not valid *\/ */
  /* if (argv[arg][0] == '-') { */
  /*   printf("Error: unkown option\n"); */
  /*   return 1; */
  /* } */

  /* collect facts and exit */
  if(optionfacter==0) {
    factertofile(path);
    exit(0);
  }

  readmanifests(optionverbose, optiondebug, path);
  return 0;
}
