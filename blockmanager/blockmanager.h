#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "./types.h"


// reads block BLOCK_SIZE from pen_fd at position 'block_num' and copies it to 'buf'
int read_block(int pen_fd, int block_num, void* buf);
// reads block from buf and copies it to 'pen_fd' at position 'block_num' 
int write_block(int pen_fd, int block_num, void* buf);
