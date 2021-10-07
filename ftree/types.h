#include <time.h>


typedef struct file* FILES;
struct file {
  size_t iblock;
  const char name[11];
  const time_t create_date;
  time_t last_access;
  size_t bytes;
};