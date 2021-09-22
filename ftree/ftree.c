#include "ftree.h"
#include <stdlib.h>
#include <string.h>
#include "./faloc/faloc.h"

//http://www.cse.yorku.ca/~oz/hash.html By Dan Bernstein
unsigned long djb2Hash(unsigned char *str);
int sondagemLinear(int pos, int i);

int getDate(time_t* t_epoch, char* str) {
  if(!t_epoch || !str) return 0;
  struct tm* tm = localtime(t_epoch);
  if(!tm) return 0;
  return strftime(str, MAX_LEN_TIME_STR, TIME_FORMAT, tm);
} 

Table makedir(Table caller) {
  Table tb = (Table) malloc(sizeof(struct table));
  int i;
  if(tb) {
    tb->curr = tb;
    tb->prev = caller ? caller : tb;
    for(i = 0; i < H_TABLE_SIZE; i++) {
      // negative bytes means file wans't created at this position
      tb->table[i].bytes = -1;
    }
  }
  return tb;
}

// int get_file(Table , const char* path) {

// }
int get_file(Table tb, const char* fname, File** file) {
  if(!tb || !fname) return 0;
  int i, newPos;
  int pos = djb2Hash(fname);
  for(i = 0; i < H_TABLE_SIZE; i++) {
    newPos = sondagemLinear(pos, i);
    if(tb->table[newPos].bytes == -1)
      return 0;
    if(!strcmp(tb->table[newPos].name, fname)) {
      *file = &tb->table[newPos];
      return 1;
    }
  }
}

int set_file(Table tb, File* file) {
  if(!tb || !file) return 0;
  int i, newPos;
  int pos = djb2Hash(file->name);
  for(i = 0; i < H_TABLE_SIZE; i++) {
    newPos = sondagemLinear(pos, i);
    if(tb->table[newPos].bytes == -1) {
      memcpy(&tb->table[newPos], file, sizeof(File));
      // set file needs much things
      // fat_set_file()
      return 1;
    }
  }
}


unsigned long djb2Hash(unsigned char *str) {
  unsigned long hash = H_TABLE_SIZE;
  int c;
  while (c = *str++)
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
  return hash;
}

int sondagemLinear(int pos, int i) {
  return ((pos + i) & 0x7FFFFFFF) % H_TABLE_SIZE;
}