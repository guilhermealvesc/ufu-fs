#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "blockmanager.h"

// dummy code
// int main() {
//   int penFd = open("/dev/sdc1", O_RDONLY);
//   if(!penFd) {
//     printf("deu zebra\n");
//     return -1;
//   }
//   printf("deu bão\n");
//   return 0;
// }

/* 
4000 -> 5000
5000 - 4000 = 1000
block_num * BLOCK_SIZE - block_actual_size * BLOCK_SIZE 
*/

int read_block(int pen_fd, int block_num, void* buf) {
  // go to block
  if(lseek(pen_fd, (off_t) block_num * BLOCK_SIZE, SEEK_SET))
    return -1;
  // reads from pen_fd and writes to buf 'BLOCK_SIZE' bytes
  return read(pen_fd, buf, BLOCK_SIZE);
}

int write_block(int pen_fd, int block_num, void* buf) {
  // go to block
  if(lseek(pen_fd, (off_t) block_num * BLOCK_SIZE, SEEK_SET))
    return -1;
  // reads from buf and writes to pen_fd 'BLOCK_SIZE' bytes
  return write(pen_fd, buf, BLOCK_SIZE);
}