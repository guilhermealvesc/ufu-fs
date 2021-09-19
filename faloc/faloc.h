#include "types.h"

FAT fat_init(int blocks);
int fat_show(int * fat, int blocks);
int fat_flag_block(int* fat, int block, int flag);


// NEED IMPLEMENTATION
// traverse fat getting blocks and copy it to buf ('file_size' buffer)
int fat_get_file(int pen_fd, int* fat, int flie_size, int block_ini, void* buf);

// set file on fat copying it's data from buf
int fat_set_file(int pen_fd, int* fat, int file_size, void* buf);

int fat_delete_file(int pen_fd, int* fat, int block_ini);