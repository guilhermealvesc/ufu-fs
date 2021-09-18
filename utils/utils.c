#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

void throw_e (const char* message) {
  printf("\e[1;1H\e[2J");
  printf("%s\n", message);
  printf("ERR: '%s'\n", strerror(errno));
  exit(errno);
}