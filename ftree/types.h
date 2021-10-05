#include <time.h>


typedef struct file* FILES;
struct file {
  unsigned int iblock;
  const char name[11];
  const time_t create_date;
  time_t last_access;
  unsigned int bytes;
};