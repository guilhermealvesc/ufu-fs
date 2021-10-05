
// SHARED LIB
typedef unsigned int FileDescriptor;

#define MAGIC_NUM -1534
int ufufs_mount(const char* filePath); 

FileDescriptor ufufs_open(const char* filename);

int ufufs_read(FileDescriptor fd, void *buf, size_t count);

int ufufs_write(FileDescriptor fd, void *buf, size_t count);

int ufufs_close(FileDescriptor fd);

int ufufs_seek(FileDescriptor fd, int offset);

// typedef struct {
//   const int MAGIC_N;
//   const int BYTES;
//   const int BLOCKS;
//   FILES FILES_TABLE;
//   FAT FAT;
// } MBR;