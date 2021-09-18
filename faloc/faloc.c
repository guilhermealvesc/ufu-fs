#include "faloc.h"

int fat_flag_block(int * fat, int block, int flag) {
  if(!fat) return 0;
  fat[block] = flag;
  return 1;
}