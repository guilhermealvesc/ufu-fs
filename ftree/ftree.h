#include <time.h>
#include "types.h"
#define MAX_LEN_TIME_STR 21
// dia[1-31]/mês[1-12]/[1900-2999], hh:mm:ss
#define TIME_FORMAT "%d/%m/%Y, %H:%M:%S"

// struct table {
//   Table curr; // current/atual, equivalente a "."
//   Table prev; // previous/anterior, equivalente a".."
//   File table[H_TABLE_SIZE];
// };

// Directory, File, Link

// single file/folder struct in file tree/table
// struct file {
//   union {
//     unsigned int iblock;
//     Table next_table;
//   };
//   const char name[11];
//   const time_t create_date;
//   time_t last_access;
//   unsigned int bytes;
//   enum F_TYPES ftype;
// };
