#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "faloc.h"

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
  FAT fat = (FAT) malloc(blocks * sizeof(int));
  if(fat) {
    for(i = 0; i < blocks; i++) 
      fat_flag_block(fat, i, BLOCK_FREE);
  }
  return fat;
}