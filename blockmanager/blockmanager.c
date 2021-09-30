#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "blockmanager.h"

int read_block(int pen_fd, int block_num, void* buf) {
  // go to block
  off_t actual = lseek(pen_fd, 0, SEEK_CUR);
  if(lseek(pen_fd, (off_t) (block_num * BLOCK_SIZE) - (actual * BLOCK_SIZE), SEEK_SET))
    return -1;
  // reads from pen_fd and writes to buf 'BLOCK_SIZE' bytes
  return read(pen_fd, buf, BLOCK_SIZE);
}

int write_block(int pen_fd, int block_num, void* buf) {
  // go to block
  off_t actual = lseek(pen_fd, 0, SEEK_CUR);
  if(lseek(pen_fd, (off_t) (block_num * BLOCK_SIZE) - (actual * BLOCK_SIZE), SEEK_SET))
    return -1;
  // reads from buf and writes to pen_fd 'BLOCK_SIZE' bytes
  return write(pen_fd, buf, BLOCK_SIZE);
}