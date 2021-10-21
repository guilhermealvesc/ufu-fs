#include <math.h>
#define BLOCK_SIZE 4096 // 4KB logical block size
#define GET_BLOCKS(b) ceil((double)b / BLOCK_SIZE)