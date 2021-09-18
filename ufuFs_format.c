#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <fcntl.h>
#include <errno.h>
#include "./blockmanager/blockmanager.h"
#include "./faloc/faloc.h"
#define GET_BLOCKS(b) ceil((double)b/BLOCK_SIZE)

// int fat_flag_block(int * fat, int block, int flag) {
//   if(!fat) return 0;
//   fat[block] = flag;
//   return 1;
// }

typedef struct {
  const int BYTES;
  const int BLOCKS;
  // struct tabelas de arquivos
  FAT FA_TABLE;
} MBR;

void throw_e (const char* message);
FAT fat_init(int blocks);

int main (int argc, char** argv) {
  printf("\e[1;1H\e[2J");
  if(argc != 2) return 1;
  
  printf("\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\n");
  printf("       UFU-FS - FORMATTER      \n");
  printf("\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\n");
  int penFd;
  if((penFd = open(argv[1], O_RDWR)) == -1)
    throw_e("Couldn't open file at path...");
  void* BUF = malloc(BLOCK_SIZE);
  read_block(penFd, 0, BUF);
  // get volume size
  off_t VOL_SIZE = lseek(penFd, 0, SEEK_END);
  // printf("VOL_SIZE: %.1lfGB\n", VOL_SIZE/1000000000.0);
  // printf("BLOCKS: %.0lf\n", ceil((double)VOL_SIZE/BLOCK_SIZE));
  // CRIAR ESTRUTURAS DO PEN DRIVE NA MEMORIA RESERVADA
  MBR MBRI = {VOL_SIZE, GET_BLOCKS(VOL_SIZE), NULL};
  if(!(MBRI.FA_TABLE = fat_init(MBRI.BLOCKS)))
    throw_e("Coundn't alloc FAT...");  
  
  // saber qual tamanho que será reservado, e reservá-lo no FAT
  int R_AREA_SIZE = sizeof(MBR) /* + SIZE_TABLES */ +  (MBRI.BLOCKS * sizeof(int));
  int R_BLOCK_SIZE = GET_BLOCKS(R_AREA_SIZE);
  int i;
  for(i = 0; i < R_BLOCK_SIZE; i++)
    fat_flag_block(MBRI.FA_TABLE, i, BLOCK_MBR);
  return 0;
}

void throw_e (const char* message) {
  printf("\e[1;1H\e[2J");
  printf("%s\n", message);
  printf("ERR: '%s'\n", strerror(errno));
  exit(errno);
}

FAT fat_init(int blocks) {
  int i;
  FAT fat = (FAT) malloc(blocks * sizeof(int));
  if(fat) {
    for(i = 0; i < blocks; i++) 
      fat_flag_block(fat, i, BLOCK_FREE);
  }
  return fat;
}