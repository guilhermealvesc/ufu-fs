#include "types.h"
#define validBlock(fat, block) (fat && block >= 0)

size_t *fat_init(int blocks);
int fat_show(size_t *fat, int blocks);
int fat_flag_block(size_t *fat, int block, int flag);

// get block from file
int fat_getf_block(int pen_fd, size_t *fat, int inode, unsigned int index, void *buf);

// write block to file
int fat_writef_block(int pen_fd, size_t *fat, int inode, unsigned int index, void *buf);

// delete file from fat
int fat_delete_file(int pen_fd, size_t *fat, int inode);