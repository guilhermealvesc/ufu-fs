#define BLOCK_END -1
#define BLOCK_FREE -2
#define BLOCK_MBR -3

// FAT fs, pointer vector 'BLOCKS' sized
typedef struct {
  const int BLOCKS;
  int* FAT;
} FAT;
