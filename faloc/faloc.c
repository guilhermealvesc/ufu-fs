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

int fat_getf_block(int pen_fd, size_t *fat, size_t FE, unsigned int index, void *buf)
{
  printf("md.penFd: %d, md.MBRI.FAT: %p, md.MBRI.FILES_TABLE[i].fat_entry: %ld, i: %d, (char *)md.fds[fd]->blocks + (BLOCK_SIZE * i): %p\n",
         pen_fd,
         fat,
         FE,
         index,
         buf);
  printf("fat[FE]: %ld\n", fat[FE]);
  printf("pen_fd < 0: %d || !fat: %d || FE < 0: %d || fat[FE] < BLOCK_END: %d || index < 0: %d || buf == NULL: %d\n",
         pen_fd < 0, !fat, FE < 0, fat[FE] < BLOCK_END, index < 0, buf == NULL);
  if (pen_fd < 0 || !fat || FE < 0 || fat[FE] == BLOCK_MBR || fat[FE] == BLOCK_FREE || index < 0 || buf == NULL)
  {
    printf("paramentos invalidos\n");
    return 0;
  }
  unsigned int i = 0;
  int block = FE;
  printf("entra iteração fat\n");
  while (i < index && fat[block] > BLOCK_END)
  {
    block = fat[block];
    i++;
  }
  // foi para memória livre ou mbr
  printf("sai iteração fat\n");
  if (block < BLOCK_END)
    return 0;
  printf("not block end\n");

  // block == BLOCK_END
  return read_block(pen_fd, block, buf);
}

int fat_increase_blocks(int pen_fd, size_t *fat, int blocksFat, size_t FE, int blocks)
{
  if (pen_fd < 0 || !fat || FE < 0 || fat[FE] < BLOCK_END || blocksFat < 0 || blocks < 0)
  {
    printf("paramentros invalidos\n");
    return 0;
  }
  int block = FE, i;
  while (fat[block] > BLOCK_END)
  {
    block = fat[block];
  }
  // block == indice na fat do fim do arquivo antes de aumentar
  for (i = 0; i < blocksFat; i++)
  {
    if (fat[i] == BLOCK_FREE)
    {
      blocks--;
      fat[block] = i;
      fat[i] = BLOCK_END;
      block = i;
      if (blocks == 0)
        break;
    }
  }
  // printf("Cheguei final da fat increase blocks\n");
  return 1;
}

int fat_writef_block(int pen_fd, size_t *fat, size_t FE, unsigned int index, void *buf)
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

int fat_delete_file(int pen_fd, size_t *fat, size_t FE)
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
