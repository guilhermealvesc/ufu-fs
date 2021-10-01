#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <fcntl.h>
#include "./utils/utils.h"
#include "./blockmanager/blockmanager.h"
#include "./faloc/faloc.h"
#define MAGIC_NUM -1534
typedef struct {
  const int MAGIC_N;
  const int BYTES;
  FAT* FA_TABLE;
} MBR;

void throw_e (const char* message);

int main (int argc, char** argv) {
  printf("\e[1;1H\e[2J");
  if(argc != 2) return 1;
  
  printf("\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\n");
  printf("       UFU-FS - FORMATTER      \n");
  printf("\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\n");
  int penFd;
  if((penFd = open(argv[1], O_RDWR)) == -1)
    throw_e("Couldn't open file at path...");
  // get volume size
  off_t VOL_SIZE = lseek(penFd, 0, SEEK_END);
  // printf("VOL_SIZE: %.1lfGB\n", VOL_SIZE/1000000000.0);
  // printf("BLOCKS: %.0lf\n", ceil((double)VOL_SIZE/BLOCK_SIZE));
  // CRIAR ESTRUTURAS DO PEN DRIVE NA MEMORIA RESERVADA
  FAT fat_str = {GET_BLOCKS(VOL_SIZE), NULL};
  if(!(fat_str.fat = fat_init(fat_str.BLOCKS)))
    throw_e("Coundn't alloc FAT...");  
  
  MBR MBRI = {MAGIC_NUM, VOL_SIZE, &fat_str};
  // saber qual tamanho que será reservado, e reservá-lo no FAT
  int R_AREA_SIZE =  /* + SIZE_TABLES */ +  (MBRI.FA_TABLE->BLOCKS * sizeof(int));
  int R_BLOCK_SIZE = GET_BLOCKS(R_AREA_SIZE);

  int i;
  for(i = 0; i < GET_BLOCKS(sizeof(MBR)); i++) {
    write_block(penFd, i, (&MBRI) + (BLOCK_SIZE * i));
    fat_flag_block(MBRI.FA_TABLE->fat, i, BLOCK_MBR);
  }
  for(i = 0; i < R_BLOCK_SIZE; i++) {

    write_block(penFd, i, (&MBRI) + BLOCK_SIZE * i);
    // if()
    fat_flag_block(MBRI.FA_TABLE->fat, i, BLOCK_MBR);
  }
  return 0;
}
