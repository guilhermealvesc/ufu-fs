#include "../faloc/faloc.h"
#define MAX_LEN_TIME_STR 21
// dia[1-31]/mês[1-12]/[1900-2999], hh:mm:ss
#define TIME_FORMAT "%d/%m/%Y, %H:%M:%S"
#define MAX_FDS 30

// SHARED LIB
typedef unsigned int FileDescriptor;

#define MAGIC_NUM -1534
extern int ufufs_mount(const char *filePath);

extern FileDescriptor ufufs_open(const char *fname);

extern int ufufs_create(const char *fname);

extern void ufufs_ls();

extern int ufufs_read(FileDescriptor fd, void *buf, size_t count);

extern int ufufs_write(FileDescriptor fd, void *buf, size_t count);

extern void ufufs_close(FileDescriptor fd);

extern off_t ufufs_seek(FileDescriptor fd, size_t offset);

extern size_t ufufs_get_size(FileDescriptor fd);

typedef struct file *FILES;
struct file
{
  size_t fat_entry;
  char name[11];
  time_t create_date;
  time_t last_access;
  size_t bytes;
};

typedef struct
{
  short int MAGIC_N;
  off_t BYTES;
  size_t BLOCKS;
  FILES FILES_TABLE;
  FAT FAT;
} MBR;

typedef struct
{
  size_t file_entry;
  off_t qntBytes;
  void *blocks;
  off_t offset;
} FD;

typedef struct
{
  int penFd;
  char filePath[50];
  MBR MBRI;
  FD *fds[MAX_FDS];
} MountData;