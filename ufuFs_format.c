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

typedef struct
{
  short int MAGIC_N;
  off_t BYTES;
  size_t BLOCKS;
  FILES FILES_TABLE;
  FAT FAT;
} MBR;

void throw_e(const char *message);

int main(int argc, char **argv)
{
  printf("\e[1;1H\e[2J");
  if (argc != 2)
    return 1;

  printf("\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\n");
  printf("       UFU-FS - FORMATTER      \n");
  printf("\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\n");
  int penFd;
  if ((penFd = open(argv[1], O_RDWR)) == -1)
    throw_e("Couldn't open file at path...");

  off_t VOL_SIZE = lseek(penFd, 0, SEEK_END);

  // MBR initialization
  MBR MBRI = {MAGIC_NUM, VOL_SIZE, GET_BLOCKS(VOL_SIZE), NULL, NULL};

  // ALOCAR FILES
  size_t bytesFiles = MBRI.BLOCKS * sizeof(struct file);
  if (!(MBRI.FILES_TABLE = (FILES)malloc(bytesFiles)))
    throw_e("Couldn't alloc Files Table...");

  // ALOCAÇÃO FAT
  size_t bytesFat = MBRI.BLOCKS * sizeof(size_t);
  if (!(MBRI.FAT = fat_init(MBRI.BLOCKS)))
    throw_e("Couldn't alloc FAT...");

  off_t R_AREA_SIZE = sizeof(short int) + sizeof(off_t) + sizeof(size_t) + bytesFiles + bytesFat;
  size_t R_BLOCK_SIZE = GET_BLOCKS(R_AREA_SIZE);

  printf("R_AREA_SIZE: %.1lfMB\nR_BLOCK_SIZE: %ld\nBLOCKS: %ld\n", R_AREA_SIZE / 1000000.0, R_BLOCK_SIZE, MBRI.BLOCKS);

  // reserverd mem allocation proc
  void *reserved_area;
  if ((reserved_area = calloc(BLOCK_SIZE, R_BLOCK_SIZE)) == NULL)
  {
    throw_e("Couldn't alloc reserved area..");
  }
  size_t offset = 0;
  //writing MAGIC_N
  //convertemos para char para andar de byte a byte na memoria
  memcpy(((char *)reserved_area) + offset, &MBRI.MAGIC_N, sizeof(MBRI.MAGIC_N));
  offset += sizeof(MBRI.MAGIC_N);

  //writing BYTES
  memcpy(((char *)reserved_area) + offset, &MBRI.BYTES, sizeof(MBRI.BYTES));
  offset += sizeof(MBRI.BYTES);

  //writing BLOCKS
  memcpy(((char *)reserved_area) + offset, &MBRI.BLOCKS, sizeof(MBRI.BLOCKS));
  offset += sizeof(MBRI.BLOCKS);

  //writing FILES_TABLE - unecessary
  // memset(((char *)reserved_area) + offset, MBRI.FILES_TABLE, bytesFiles);
  offset += bytesFiles;

  for (int i = 0; i < R_BLOCK_SIZE; i++)
  {
    fat_flag_block(MBRI.FAT, i, BLOCK_MBR);
  }

  //writing FAT
  memcpy(((char *)reserved_area) + offset, MBRI.FILES_TABLE, bytesFat);
  offset += bytesFat;

  // memcpy(&MBRI.MAGIC_N, reserved_area, sizeof(MBRI.MAGIC_N));

  // printf("MAGIC_N: %d\n", MBRI.MAGIC_N);

  for (int i = 0; i < R_BLOCK_SIZE; i++)
  {
    if (write_block(penFd, i, ((char *)reserved_area) + (BLOCK_SIZE * i)) < 0)
    {
      throw_e("Erro na formatacao!!");
    }
  }
  close(penFd);
  ufufs_mount(argv[1]);
  return 0;
}
