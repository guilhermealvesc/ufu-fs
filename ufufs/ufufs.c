#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include "../blockmanager/blockmanager.h"
#include "./ufufs.h"
#include "./ftree/types.h"
#define MAX_FDS 30

// usa o fd do SO
typedef struct{
  size_t inode;
  off_t qntBytes;
  void *blocks; //[block, block, block]
  off_t offset;
} FD;

typedef struct{
  int penFd;
  MBR MBRI;
  //const int MAGIC_N;
  //const int BYTES;
  //const int BLOCKS;
  //FILES FILES_TABLE;
  //FAT FAT;
  FD *fds[MAX_FDS];
} MountData;

//[0]
MountData md = {-1, NULL};
// todas rotinas devem verificar se o fs foi montado
// [MAGIC_N, BYTES, BLOCK_SIZE, FILES, FAT]
int ufufs_mount(const char *filePath){
  md.penFd = open(filePath, O_RDWR);
  if (md.penFd < 0)
    return 0; //adicionar errno
              //read(penFd,);

  if (read(md.penFd, &md.MBRI.MAGIC_N, sizeof(md.MBRI.MAGIC_N)) > 0){
    printf("MAGIC_N: %d\n", md.MBRI.MAGIC_N);
  }

  if (md.MBRI.MAGIC_N != MAGIC_NUM) return 0;

  if (read(md.penFd, &md.MBRI.BYTES, sizeof(md.MBRI.BYTES)) > 0){
    printf("BYTES: %ld\n", md.MBRI.BYTES);
  }

  if (read(md.penFd, &md.MBRI.BLOCKS, sizeof(md.MBRI.BLOCKS)) > 0){
    printf("BLOCKS: %ld\n",md.MBRI.BLOCKS);
  }

  if (!(md.MBRI.FILES_TABLE = (FILES) malloc(sizeof(struct file) * md.MBRI.BLOCKS)))
    printf("Erro ao alocar files_table\n");

  if(read(md.penFd, md.MBRI.FILES_TABLE, sizeof(struct file) * md.MBRI.BLOCKS) < 0){
    printf("Erro ao ler files_table do pen drive\n");
  } else {
    //essa parte podemos apagar dps, é só para teste-------------------------------------
    for(int i = 0; i < md.MBRI.BLOCKS; i++){
      printf("iblock: %zu\n", md.MBRI.FILES_TABLE[i].iblock);
      printf("Name: %s\n", md.MBRI.FILES_TABLE[i]->name);
      printf("Create Date: %d/%m/%Y, %H:%M:%S\n", md.MBRI.FILES_TABLE[i].create_date);
      printf("Last access: %d/%m/%Y, %H:%M:%S\n", md.MBRI.FILES_TABLE[i].last_access);
      printf("Size in bytes: %zu\n", md.MBRI.FILES_TABLE[i].bytes);
    }
    //-----------------------------------------------------------------------------------
  }

  if (!(md.MBRI.FAT = (size_t *)malloc(sizeof(size_t) * md.MBRI.BLOCKS)))
   printf("Couldn't alloc FAT...");

  if(read(md.penFd, md.MBRI.FAT, sizeof(size_t) * md.MBRI.BLOCKS) < 0){
    printf("Erro ao ler FAT");
  }

  return 1;
}

// file descriptor (int) -> usado para ler byte a byte do arquivo aberto
// quando o arquivo for aberto, preciso criar para esse file descriptor
// cada file descriptor:
// vetor de blocos do arquivo
// deslocamento em bytes

FileDescriptor ufufs_open(const char *filename){
  // olha na tabela de metadados se o arquivo existe
  int i;
  for(i = 0; i < md.MBRI.BLOCKS; i++){
    if(strcmp(md.MBRI.FILES_TABLE[i].name, filename) == 0)
      break;
    else
      i = -1; //se i != -1 existe
  }
  // existindo, criar o ponteiro para arquivo inicializado com deslocamento em bytes zerado (offset),
  if(i != -1){
    int fd;
    for (fd = 0; fd < MAX_FDS; fd++){
      if (!md.fds[fd])
        break;
      }
    // tabela de metadados tem a quantidade de bytes do arquivo
    // sabendo disso e do tamanho do block size, alocar vetor com tamanho GET_BLOCKS(bytes_file) para comportar arquivo
    
    if(!(md.fds[fd]->blocks = (int *) malloc(GET_BLOCKS(md.MBRI.FILES_TABLE.bytes))))
      return NULL;
    md.fds[fd]->inode = md.MBRI.FILES_TABLE.iblock;    
    md.fds[fd]->qntBytes = md.MBRI.FILES_TABLE.bytes;
    //md.fds[fd]->blocks = ?? não sei de onde puxar essas informacoes
    //md.fds[fd]->offset = ??
    // retorna um inteiro (chave) para acessar arquivo
    return fd;
  } else return NULL;
}

int ufufs_read(FileDescriptor fd, void *buf, size_t count)
{
  // invalid params
  if (md.penFd == -1 || fd < 0 || fd > MAX_FDS || !md.fds[fd] || !buf || count < 0)
    return -1;
  size_t bytesOffset =
      bytesOffset + count > md.fds[fd]->qntBytes
          ? md.fds[fd]->qntBytes
          : md.fds[fd]->offset + count;
  // desloca byte a byte ((char*) md.fds[fd]->blocks) + bytesOffset
  memcpy(buf, ((char *)md.fds[fd]->blocks) + bytesOffset, count);
  // se estiver inicializado retorna os count bytes no buffer, vamos trazer o arquivo inteiro para memória?
  // senão, usa o penFd para, inode (indice de inicio do arquivo na fat)
  // buscando o bloco usando as funções da fat
  // retornado, retorna isso no buffer
}

int ufufs_write(FileDescriptor fd, void *buf, size_t count)
{
  // invalid params
  if (md.penFd == -1 || fd < 0 || fd > MAX_FDS || !md.fds[fd] || !buf || count < 0)
    return -1;
  size_t bytesOffset = md.fds[fd]->offset;
  if (bytesOffset + count > md.fds[fd]->qntBytes)
  {
    // alocar mais espaço para o arquivo ou dar erro?
  }
  // desloca byte a byte ((char*) md.fds[fd]->blocks) + bytesOffset
  memcpy(((char *)md.fds[fd]->blocks) + bytesOffset, buf, count);
}

// DONE
off_t ufufs_seek(FileDescriptor fd, size_t offset)
{
  // invalid params
  if (md.penFd == -1 || fd < 0 || fd > MAX_FDS || !md.fds[fd])
    return -1;
  off_t bytesOffset = md.fds[fd]->offset + offset;
  // invalid offset
  if (bytesOffset < 0 || bytesOffset > md.fds[fd]->qntBytes)
    return -1;
  return md.fds[fd]->offset = bytesOffset;
}

int ufufs_close(FileDescriptor fd)
{
  // pega a estrutura do fd e salva usando a FAT
  // apaga as estruturas bonitim
}