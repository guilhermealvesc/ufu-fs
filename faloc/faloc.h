#include "types.h"
#define validBlock(fat, block) (fat && block >= 0)

int* fat_init(int blocks);
int fat_show(int * fat, int blocks);
int fat_flag_block(int* fat, int block, int flag);


// NEED IMPLEMENTATION
// traverse fat getting blocks and copy it to buf ('file_size' buffer)
int fat_get_file(int pen_fd, int* fat, int block_ini, void* buf);

// set file on fat copying it's data from buf
int fat_set_file(int pen_fd, int* fat, int BLOCKS, int file_size, void* buf);

// delete file from fat
int fat_delete_file(int pen_fd, int* fat, int block_ini);