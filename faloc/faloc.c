#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "faloc.h"
#include "../blockmanager/blockmanager.h"

int fat_flag_block(int* fat, int block, int flag) {
  if(!fat) return 0;
  fat[block] = flag;
  return 1;
}

int fat_show(int * fat, int blocks) {
  int i;
  char str[11];
  printf("FAT:");
  for(i = 0; i < blocks; i++) {
    switch (fat[i]) {
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
        str[1] =  '\0';
        break;
    }
    printf(" [%s]", str);
  }
  printf("\n");
}

FAT fat_init(int blocks) {
  int i;
  if(blocks < 0) return NULL;
  FAT fat = (FAT) malloc(blocks * sizeof(int));
  if(fat) {
    for(i = 0; i < blocks; i++) 
      fat_flag_block(fat, i, BLOCK_FREE);
  }
  return fat;
}

int fat_get_file(int pen_fd, int* fat, int block_ini, void* buf) {
  if(!fat || block_ini < 0 || fat[block_ini] < BLOCK_END || !buf )
    return 0;
  int block, i;
  for(i = 0, block = block_ini; block != BLOCK_END; i++, block = fat[block]) {
    if(read_block(pen_fd, block, buf + (i * BLOCK_SIZE)) == -1) {
      return 0;
    }
  }
  return 1;
}

int fat_set_file(int pen_fd, int* fat, int BLOCKS, int file_size, void* buf) {
  if(!fat || BLOCKS < 0 || file_size < 0 || !buf)
    return 0;
  int i, saved, BLOCKS_FILE = GET_BLOCKS(file_size), last;

  for(i = 0, saved = 0; i < BLOCKS && saved < BLOCKS_FILE; i++) {
    if(fat[i] == BLOCK_FREE) {
      if(write_block(pen_fd, i, buf + (saved * BLOCK_SIZE)) != -1) {
        if(saved) fat[last] = i;
        last = i;
        saved++;
      } else return 0;
    }
  }

  return 1;
}


int fat_delete_file(int pen_fd, int* fat, int block_ini) {
  if(!fat || block_ini < 0 || fat[block_ini] < BLOCK_END)
    return 0;
  int block, block_aux;
  for(block = block_ini; block != BLOCK_END; block = block_aux) {
    block_aux = fat[block];
    fat[block] = BLOCK_FREE;
  }
}
