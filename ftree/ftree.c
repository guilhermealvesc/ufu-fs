#include "ftree.h"
#include <stdlib.h>
#include <string.h>

//http://www.cse.yorku.ca/~oz/hash.html By Dan Bernstein
unsigned long djb2Hash(unsigned char *str);
int sondagemLinear(int pos, int i);

int getDate(time_t *t_epoch, char *str)
{
  if (!t_epoch || !str)
    return 0;
  struct tm *tm = localtime(t_epoch);
  if (!tm)
    return 0;
  return strftime(str, MAX_LEN_TIME_STR, TIME_FORMAT, tm);
}

// Table makedir(Table caller) {
//   Table tb = (Table) malloc(sizeof(struct table));
//   int i;
//   if(tb) {
//     tb->curr = tb;
//     tb->prev = caller ? caller : tb;
//     for(i = 0; i < H_TABLE_SIZE; i++) {
//       // negative bytes means file wans't created at this position
//       tb->table[i].bytes = -1;
//     }
//   }
//   return tb;
// }

// // int get_file(Table , const char* path) {

// // }
// int get_file(Table tb, const char* fname, File** file) {
//   if(!tb || !fname) return 0;
//   int i, newPos;
//   int pos = djb2Hash(fname);
//   for(i = 0; i < H_TABLE_SIZE; i++) {
//     newPos = sondagemLinear(pos, i);
//     if(tb->table[newPos].bytes == -1)
//       return 0;
//     if(!strcmp(tb->table[newPos].name, fname)) {
//       *file = &tb->table[newPos];
//       return 1;
//     }
//   }
// }

// int set_file(Table tb, File* file) {
//   if(!tb || !file) return 0;
//   int i, newPos;
//   int pos = djb2Hash(file->name);
//   for(i = 0; i < H_TABLE_SIZE; i++) {
//     newPos = sondagemLinear(pos, i);
//     if(tb->table[newPos].bytes == -1) {
//       memcpy(&tb->table[newPos], file, sizeof(File));
//       // set file needs much things
//       // fat_set_file()
//       return 1;
//     }
//   }
// }

// int valid_file(File* file){ //1 = valido, 0 = invalido
//   int i;
//   if(file->name == NULL) return 0;
//     else for(i = 0; file->name[i] != '\0'; i++)
//             if(file->name[i] == '.') return 0; //nosso padrão não terá '.' em nome de pastas (podemos mudar dps)
//   if(file->bytes == NULL || file->bytes < 0) return 0;
//     else if(file->create_date == NULL) return 0;
//             else if(file->last_access != file->create_date) return 0; //na hora da criacao eles devem ser iguais
//                   else if(file->ftype == NULL) return 0;
//     return 1;
//     //dentro do pdf a data é especificada para estar nos intervalos de dia[1-31]/mês[1-12]/[1900-2999]
//     //porém como é uma string que varia, deveriam ser feitas muitas verificações para saber se está neste período
//     //mas só de verificar se é NULL acho o suficiente pq a função strftime não costuma falhar
// }

// unsigned long djb2Hash(unsigned char *str) {
//   unsigned long hash = H_TABLE_SIZE;
//   int c;
//   while (c = *str++)
//     hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
//   return hash;
// }

// int sondagemLinear(int pos, int i) {
//   return ((pos + i) & 0x7FFFFFFF) % H_TABLE_SIZE;
// }