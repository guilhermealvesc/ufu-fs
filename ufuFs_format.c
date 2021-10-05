#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <fcntl.h>
#include "./utils/utils.h"
#include "./blockmanager/blockmanager.h"
#include "./faloc/faloc.h"
#include "./ftree/ftree.h"
#include "./ufufs/ufufs.h"
#include "./ftree/types.h"


// [MAGIC_N, BYTES, BLOCK_SIZE, FILES, FAT]

typedef struct {
  const int MAGIC_N;
  const int BYTES;
  const int BLOCKS;
  FILES FILES_TABLE;
  FAT FAT;
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
  MBR MBRI = {MAGIC_NUM, VOL_SIZE, GET_BLOCKS(VOL_SIZE), NULL, NULL};
  
  // ALOCAR FILES 
  // [R R R R F F F F F F F F]
  if(!(MBRI.FILES_TABLE = malloc(MBRI.BLOCKS * sizeof(struct file)))) //struct file = 40 bytes
    throw_e("Couldn't alloc Files Table...");
  // ALOCAÇÃO FAT
  if(!(MBRI.FAT = fat_init(MBRI.BLOCKS)))
    throw_e("Couldn't alloc FAT...");  
  // saber qual tamanho que será reservado, e reservá-lo no FAT
  // size_t table_size = sizeof(MBRI.FILES_TABLE) / sizeof(MBRI.FILES_TABLE[0]); //table_size terá o tamanho do vetor, se FILES_TABLE[3], table_size = 3;
  int R_AREA_SIZE = sizeof(const int) * 3 + (MBRI.BLOCKS * sizeof(struct file)) + (MBRI.BLOCKS * sizeof(int));
  int R_BLOCK_SIZE = GET_BLOCKS(R_AREA_SIZE);
  printf("R_AREA_SIZE: %.1lfMB\nR_BLOCK_SIZE: %d\nBLOCKS: %d\n", R_AREA_SIZE/1000000.0, R_BLOCK_SIZE, MBRI.BLOCKS);
  int i;
  // ESCREVER OS 3 INTEIROS E OS 2 VETORES
  // for(i = 0; i < R_BLOCK_SIZE; i++) {
  //   //write_block(penFd, i, (&MBRI) + BLOCK_SIZE * i);

  //   fat_flag_block(MBRI.FAT, i, BLOCK_MBR);
  //   write_block(penFD,i,)
  //   //fazer um loop para cada elemento da struct para escrever tudo
  //   //caso faça tudo neste for pode ocorrer de escrever mais de uma vez cada um
  // }
  return 0;
}
