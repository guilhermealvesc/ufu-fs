#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "faloc.h"

int fat_flag_block(size_t *fat, int block, int flag)
{
  if (!validBlock(fat, block) || flag < BLOCK_MBR)
    return 0;
  fat[block] = flag;
  return 1;
}

int fat_show(size_t *fat, int blocks)
{
  int i;
  char str[11];
  printf("FAT:");
  for (i = 0; i < blocks; i++)
  {
    switch (fat[i])
    {
    case BLOCK_END:
      strcpy(str, "BLOCK_END");
      break;
    case BLOCK_FREE:
      strcpy(str, "BLOCK_FREE");
      break;
    case BLOCK_MBR:
      strcpy(str, "BLOCK_MBR");
      break;
    default:
      str[0] = fat[i] + '0';
      str[1] = '\0';
      break;
    }
    printf("\n[%s]", str);
  }
  printf("\n");
}

size_t *fat_init(int blocks)
{
  int i;
  if (blocks < 0)
    return NULL;
  size_t *fat = (size_t *)malloc(blocks * sizeof(size_t));
  if (fat)
  {
    for (i = 0; i < blocks; i++)
      fat_flag_block(fat, i, BLOCK_FREE);
  }
  return fat;
}

int fat_getf_block(int pen_fd, size_t *fat, int FE, unsigned int index, void *buf)
{
  if (pen_fd < 0 || !fat || FE < 0 || fat[FE] < BLOCK_END || index < 0 || buf == NULL)
    return 0;
  unsigned int i = 0;
  int block = FE;
  while (i < index && fat[block] > BLOCK_END)
  {
    block = fat[block];
    i++;
  }
  // foi para memória livre ou mbr
  if (block < BLOCK_END)
    return 0;
  // block == BLOCK_END
  return read_block(pen_fd, block, buf);
}

int fat_writef_block(int pen_fd, size_t *fat, int FE, unsigned int index, void *buf)
{
  if (pen_fd < 0 || !fat || FE < 0 || fat[FE] < BLOCK_END || index < 0 || buf == NULL)
    return 0;
  unsigned int i = 0;
  int block = FE;
  while (i < index && fat[block] > BLOCK_END)
  {
    block = fat[block];
    i++;
  }
  // foi para memória livre ou mbr
  if (block < BLOCK_END)
    return 0;
  // block == BLOCK_END
  return write_block(pen_fd, block, buf);
}

int fat_delete_file(int pen_fd, size_t *fat, int FE)
{
  if (pen_fd < 0 || !fat || FE < 0 || fat[FE] < BLOCK_END)
    return 0;
  int block, block_aux;
  for (block = FE; fat[block] != BLOCK_END; block = block_aux)
  {
    block_aux = fat[block];
    fat[block] = BLOCK_FREE;
  }
  fat[block] = BLOCK_FREE;
  return 1;
}
