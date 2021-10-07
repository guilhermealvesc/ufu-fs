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
  short int MAGIC_N;
  size_t BYTES;
  size_t BLOCKS;
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
  if(!(MBRI.FILES_TABLE = (FILES) calloc(MBRI.BLOCKS, sizeof(struct file))))
    throw_e("Couldn't alloc Files Table...");
  // ALOCAÇÃO FAT
  if(!(MBRI.FAT = fat_init(MBRI.BLOCKS)))
    throw_e("Couldn't alloc FAT...");  
  // saber qual tamanho que será reservado, e reservá-lo no FAT
  // size_t table_size = sizeof(MBRI.FILES_TABLE) / sizeof(MBRI.FILES_TABLE[0]); //table_size terá o tamanho do vetor, se FILES_TABLE[3], table_size = 3;
  int R_AREA_SIZE = sizeof(short int) + sizeof(size_t) * 2 + (MBRI.BLOCKS * sizeof(struct file)) + (MBRI.BLOCKS * sizeof(size_t));
  int R_BLOCK_SIZE = GET_BLOCKS(R_AREA_SIZE);
  printf("R_AREA_SIZE: %.1lfMB\nR_BLOCK_SIZE: %d\nBLOCKS: %d\n", R_AREA_SIZE/1000000.0, R_BLOCK_SIZE, MBRI.BLOCKS);
  
  void* reserved_area;
  if((reserved_area = malloc(R_BLOCK_SIZE * BLOCK_SIZE)) == NULL) {
    throw_e("Couldn't alloc reserved area..");
  }
  size_t offset = 0;
  //convertemos para char para andar de byte a byte na memoria
  //writing MAGIC_N
  memcpy(((char*) reserved_area) + offset, &MBRI.MAGIC_N, sizeof(MBRI.MAGIC_N));
  offset += sizeof(MBRI.MAGIC_N);

  //writing BYTES
  memcpy(((char*) reserved_area) + offset, &MBRI.BYTES, sizeof(MBRI.BYTES));
  offset += sizeof(MBRI.BYTES);

  //writing BLOCKS
  memcpy(((char*) reserved_area) + offset, &MBRI.BLOCKS, sizeof(MBRI.BLOCKS));
  offset += sizeof(MBRI.BLOCKS);

  //writing FILES_TABLE
  memset(((char*) reserved_area) + offset, MBRI.FILES_TABLE, MBRI.BLOCKS * sizeof(struct file));
  offset += MBRI.BLOCKS * sizeof(struct file);

  for(int i = 0; i < MBRI.BLOCKS; i++) {
    //write_block(penFd, i, (&MBRI) + BLOCK_SIZE * i 
    fat_flag_block(MBRI.FAT, i, i < R_BLOCK_SIZE ? BLOCK_MBR : BLOCK_FREE );
    //write_block(penFD,i,);
    //fazer um loop para cada elemento da struct para escrever tudo
    //caso faça tudo neste for pode ocorrer de escrever mais de uma vez cada um
  }

  //writing FAT
  memcpy(((char*) reserved_area) + offset, MBRI.FILES_TABLE, MBRI.BLOCKS * sizeof(struct file));
  offset += MBRI.BLOCKS * sizeof(struct file);

  return 0;
}
