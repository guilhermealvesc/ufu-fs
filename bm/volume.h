

#define BLOCK_SIZE 4000 // 4KB logical block size

typedef struct {
  const int VOL_SIZE; // vol_size initialized upon MBR alloc 
  const int sectors;
} MBR;
// read from cache, reading from block if not cached
int read_cache(int pen_fd, int block_num, void* buf);
