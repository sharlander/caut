#include <stdio.h>

void help(char *progname) {

  printf("Usage: %s [OPTIONS]\n"
         "-f, --facter\n"
         "Import facts from the facter program\n"
         "\n"
         "-p, --path <argument>\n"
         "Set the module path (default is pwd)\n"
         "\n"
         "-d, --debug\n"
         "Show debug messages\n"
         "\n"
         "-v, --verbose\n"
         "Set verbose mode\n"
         "\n"
         "--version\n"
         "Show program version\n"
         "\n"
         "-h, --help\n"
         "Display this help\n", progname);
}
