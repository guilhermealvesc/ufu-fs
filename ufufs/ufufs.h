#include "../faloc/faloc.h"
#include "../ftree/ftree.h"
#define MAX_FDS 30
// SHARED LIB
typedef unsigned int FileDescriptor;

#define MAGIC_NUM -1534
int ufufs_mount(const char *filePath);

FileDescriptor ufufs_open(const char *fname);

int ufufs_create(const char *fname);

void ufufs_ls();

int ufufs_read(FileDescriptor fd, void *buf, size_t count);

int ufufs_write(FileDescriptor fd, void *buf, size_t count);

void ufufs_close(FileDescriptor fd);

off_t ufufs_seek(FileDescriptor fd, size_t offset);

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
  size_t inode;
  off_t qntBytes;
  void *blocks;
  off_t offset;
} FD;

typedef struct
{
  int penFd;
  MBR MBRI;
  FD *fds[MAX_FDS];
} MountData;

// MountData md = {-1, {}, NULL};