#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include "volume.h"

// memory caching ? created where??
// void *cache = malloc(cached size);
// cache implementation?? map volume bytes into memory
// CACHE LESS BLOCKS IN MEMORY
// calc num blocks = VOL_SIZE / BLOCK_SIZE

// read_block sends to cache
int read_block(int pen_fd, int block_num, void* buf);

// dummy code
int main() {
  int penFd = open("/dev/sdc1", O_RDONLY);
  if(!penFd) {
    printf("deu zebra\n");
    return -1;
  }
  printf("deu bão\n");
  return 0;
}

// cache nneded?
int read_cache(int pen_fd, int block_num, void* buf) {
  if(not_in_cache) {
    //if not in cache, cache it
    read_block(pen_fd, block_num, buf);
  }
}

// read_block sends to cache
int read_block(int pen_fd, int block_num, void* buf) {
  // go to block
  if(lseek(pen_fd, (off_t) block_num * BLOCK_SIZE, SEEK_SET)) {
    return -1;
  }
  // write to buf 'BLOCK_SIZE' bytes
  read(pen_fd, buf, BLOCK_SIZE);
}