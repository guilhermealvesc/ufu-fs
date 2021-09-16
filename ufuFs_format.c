#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <fcntl.h>
#include <errno.h>
#include "./blockmanager/types.h"
#include "./faloc/types.h"


int main (int argc, char** argv) {
  printf("\e[1;1H\e[2J");
  if(argc != 2) return 1;
  
  printf("\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\n");
  printf("       UFU-FS - FORMATTER      \n");
  printf("\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\n");
  int penFd;
  if((penFd = open(argv[1], O_RDWR)) == -1) {
    printf("\e[1;1H\e[2J");
    printf("Couldn't open file at path...\n");
    printf("ERR: '%s'\n", strerror(errno));
    return 1;
  }

  off_t VOL_SIZE;
  // get volume size
  VOL_SIZE = lseek(penFd, 0, SEEK_END);
  printf("VOL_SIZE: %.1lfGB\n", VOL_SIZE/1000000000.0);
  printf("BLOCKS: %.0lf\n", ceil((double)VOL_SIZE/BLOCK_SIZE));
  // CRIAR ESTRUTURAS DO PEN DRIVE NA MEMORIA RESERVADA
  FAT f = {ceil((double)VOL_SIZE/BLOCK_SIZE), NULL};
  f.FAT = malloc(f.BLOCKS * sizeof(int));
  if(!f.FAT) {
    printf("\e[1;1H\e[2J");
    printf("ERR: '%s'\n", strerror(errno));
  }
  // saber qual tamanho que será reservado, e reservá-lo no FAT
  for()
  return 0;
}