#include <math.h>
#define BLOCK_SIZE 4000 // 4KB logical block size, should it be 4096?
#define GET_BLOCKS(b) ceil((double)b / BLOCK_SIZE)