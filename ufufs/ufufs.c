#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include "./ufufs.h"

int getDate(time_t *t_epoch, char *str)
{
  if (!t_epoch || !str)
    return 0;
  struct tm *tm = localtime(t_epoch);
  if (!tm)
    return 0;
  return strftime(str, MAX_LEN_TIME_STR, TIME_FORMAT, tm);
}

/*
descomentar aki e apagar as mudanças que fiz a partir da linha 30 de ufufs.h em caso de erro
// usa o fd do SO
typedef struct
{
  size_t inode;
  off_t qntBytes;
  void *blocks;
  off_t offset;
} FD;
*/

MountData md = {-1, "", {-1, -1, -1, NULL, NULL}, NULL};

int updateMBR()
{
  size_t bytesFiles = md.MBRI.BLOCKS * sizeof(struct file);
  size_t bytesFat = md.MBRI.BLOCKS * sizeof(size_t);
  off_t R_AREA_SIZE = sizeof(short int) + sizeof(off_t) + sizeof(size_t) + bytesFiles + bytesFat;
  size_t R_BLOCK_SIZE = GET_BLOCKS(R_AREA_SIZE);

  void *reserved_area;
  if ((reserved_area = calloc(BLOCK_SIZE, R_BLOCK_SIZE)) == NULL)
  {
    return 0;
  }
  size_t offset = 0;
  // writing MAGIC_N
  memcpy(((char *)reserved_area) + offset, &md.MBRI.MAGIC_N, sizeof(md.MBRI.MAGIC_N));
  offset += sizeof(md.MBRI.MAGIC_N);

  // writing BYTES
  memcpy(((char *)reserved_area) + offset, &md.MBRI.BYTES, sizeof(md.MBRI.BYTES));
  offset += sizeof(md.MBRI.BYTES);

  // writing BLOCKS
  memcpy(((char *)reserved_area) + offset, &md.MBRI.BLOCKS, sizeof(md.MBRI.BLOCKS));
  offset += sizeof(md.MBRI.BLOCKS);

  // writing FILES_TABLE
  memcpy(((char *)reserved_area) + offset, md.MBRI.FILES_TABLE, bytesFiles);
  offset += bytesFiles;

  // writing FAT
  memcpy(((char *)reserved_area) + offset, md.MBRI.FAT, bytesFat);

  for (int i = 0; i < R_BLOCK_SIZE; i++)
  {
    if (write_block(md.penFd, i, ((char *)reserved_area) + (BLOCK_SIZE * i)) < 0)
    {
      return 0;
    }
  }
  close(md.penFd);
  md.penFd = open(md.filePath, O_RDWR);
  free(reserved_area);
  return 1;
}

int ufufs_mount(const char *filePath)
{
  md.penFd = open(filePath, O_RDWR);
  if (md.penFd < 0)
    return 0;

  if (read(md.penFd, &md.MBRI.MAGIC_N, sizeof(md.MBRI.MAGIC_N)) <= 0)
    return 0;

  if (md.MBRI.MAGIC_N != MAGIC_NUM)
    return 0;

  if (read(md.penFd, &md.MBRI.BYTES, sizeof(md.MBRI.BYTES)) <= 0)
    return 0;

  if (read(md.penFd, &md.MBRI.BLOCKS, sizeof(md.MBRI.BLOCKS)) <= 0)
    return 0;

  if (!(md.MBRI.FILES_TABLE = (FILES)malloc(sizeof(struct file) * md.MBRI.BLOCKS)))
    return 0;

  if (read(md.penFd, md.MBRI.FILES_TABLE, sizeof(struct file) * md.MBRI.BLOCKS) <= 0)
    return 0;

  if (!(md.MBRI.FAT = (size_t *)malloc(sizeof(size_t) * md.MBRI.BLOCKS)))
    return 0;

  if (read(md.penFd, md.MBRI.FAT, sizeof(size_t) * md.MBRI.BLOCKS) <= 0)
    return 0;

  strcpy(md.filePath, filePath);
  return 1;
}

void ufufs_ls()
{
  if (md.penFd == -1)
  {
    printf("Pen Drive nao montado!\n");
    return;
  }
  printf("Arquivos no pen drive montado:\n");
  for (int i = 0; i < md.MBRI.BLOCKS; i++)
  {
    if (strcmp(md.MBRI.FILES_TABLE[i].name, "")) //#define TIME_FORMAT "%d/%m/%Y, %H:%M:%S"
    {
      char create_date[MAX_LEN_TIME_STR];
      char last_access[MAX_LEN_TIME_STR];
      getDate(&md.MBRI.FILES_TABLE[i].create_date, create_date);
      getDate(&md.MBRI.FILES_TABLE[i].last_access, last_access);
      printf("%s, %s create date, %s last access, %ld bytes\n", md.MBRI.FILES_TABLE[i].name, create_date, last_access, md.MBRI.FILES_TABLE[i].bytes);
    }
  }
}

int ufufs_create(const char *fname)
{
  // invalid params----------------------------------------------
  if (md.penFd == -1 || !fname || strlen(fname) > 10)
    return -1;
  size_t pos = -1;
  size_t i;
  for (i = 0; i < md.MBRI.BLOCKS; i++)
  {
    if (!strcmp(md.MBRI.FILES_TABLE[i].name, ""))
      pos = i;
    else if (!strcmp(md.MBRI.FILES_TABLE[i].name, fname))
      return -1;
  }
  // FILES_TABLE FULL
  if (pos == -1)
    return -1;
  for (i = 0; i < md.MBRI.BLOCKS; i++)
  {
    if (md.MBRI.FAT[i] == BLOCK_FREE)
      break;
  }
  // FAT FULL
  if (i == md.MBRI.BLOCKS)
    return 0;
  //--------------------------------------------------------------
  time_t t_time;
  time(&t_time);
  fat_flag_block(md.MBRI.FAT, i, BLOCK_END);

  md.MBRI.FILES_TABLE[pos].fat_entry = i;
  strcpy(md.MBRI.FILES_TABLE[pos].name, fname);
  md.MBRI.FILES_TABLE[pos].create_date = t_time;
  md.MBRI.FILES_TABLE[pos].last_access = t_time;
  md.MBRI.FILES_TABLE[pos].bytes = 0;
  return updateMBR();
}

// file descriptor (int) -> usado para ler byte a byte do arquivo aberto
// quando o arquivo for aberto, preciso criar para esse file descriptor
// cada file descriptor:
// vetor de blocos do arquivo
// deslocamento em bytes

FileDescriptor ufufs_open(const char *filename)
{
  // olha na tabela de metadados se o arquivo existe
  if (md.penFd == -1)
    return -1;
  size_t file_entry;
  for (file_entry = 0; file_entry < md.MBRI.BLOCKS; file_entry++)
  {
    if (strcmp(md.MBRI.FILES_TABLE[file_entry].name, filename) == 0)
      break;
  }
  if (file_entry >= md.MBRI.BLOCKS)
    return -1;
  int fd;
  for (fd = 0; fd < MAX_FDS; fd++)
  {
    if (!md.fds[fd])
      break;
  }
  // printf("entra aloc fd\n");
  if ((md.fds[fd] = (FD *)malloc(sizeof(FD))) == NULL)
    return -1;
  // printf("sai aloc fd\n");
  // printf("entra aloc block\n");
  size_t blocosArquivo = GET_BLOCKS(md.MBRI.FILES_TABLE[file_entry].bytes);
  if (blocosArquivo == 0)
    blocosArquivo = 1;
  if (!(md.fds[fd]->blocks = (void *)malloc(blocosArquivo * BLOCK_SIZE)))
    return -1;
  // printf("sai aloc block\n");

  size_t fat_entry = md.MBRI.FILES_TABLE[file_entry].fat_entry;
  
  for (int i = 0; i < blocosArquivo; i++)
  {
    int resGetf = fat_getf_block(
        md.penFd,
        md.MBRI.FAT,
        fat_entry,
        i,
        (char *)md.fds[fd]->blocks + (BLOCK_SIZE * i));
    if (resGetf <= 0)
    {
      return -1;
    }
  }

  md.fds[fd]->file_entry = file_entry;
  md.fds[fd]->qntBytes = md.MBRI.FILES_TABLE[file_entry].bytes;
  md.fds[fd]->offset = 0;
  // retorna um inteiro (chave) para acessar arquivo
  return fd;
}

int ufufs_read(FileDescriptor fd, void *buf, size_t count)
{
  // invalid params
  if (md.penFd == -1 || fd < 0 || fd > MAX_FDS || !md.fds[fd] || !buf || count < 0)
    return 0;

  if (md.fds[fd]->qntBytes < md.fds[fd]->offset)
    return EOF;

  size_t bytesOffset = md.fds[fd]->offset;
  if (bytesOffset + count > md.fds[fd]->qntBytes)
    return EOF;

  // desloca byte a byte ((char*) md.fds[fd]->blocks) + bytesOffset
  memcpy(buf, ((char *)md.fds[fd]->blocks) + bytesOffset, count);
  md.fds[fd]->offset += count;
  return 1;
}

int ufufs_write(FileDescriptor fd, void *buf, size_t count)
{
  // invalid params
  if (md.penFd == -1 || fd < 0 || fd > MAX_FDS || !md.fds[fd] || !buf || count < 0)
  {
    printf("parametros ufu_fs_write invalidos\n");
    return 0;
  }

  if (md.fds[fd]->qntBytes < md.fds[fd]->offset)
  {
    printf("erro EOF\n");
    return EOF;
  }

  size_t bytesOffset = md.fds[fd]->offset;
  if (bytesOffset + count > md.fds[fd]->qntBytes)
  {
    size_t newQntBlocs = GET_BLOCKS(md.fds[fd]->qntBytes + count) == 0 ? 1 : GET_BLOCKS(md.fds[fd]->qntBytes + count);
    void *newBlocks = reallocarray(md.fds[fd]->blocks, newQntBlocs, BLOCK_SIZE);
    if (!newBlocks)
    {
      
      return 0;
    }
    md.fds[fd]->blocks = newBlocks;
    md.fds[fd]->qntBytes += count;
  }
  // desloca byte a byte ((char*) md.fds[fd]->blocks) + bytesOffset
  memcpy(((char *)md.fds[fd]->blocks) + bytesOffset, buf, count);
  return 1;
}

// DONE
off_t ufufs_seek(FileDescriptor fd, size_t offset)
{
  // invalid params
  if (md.penFd == -1 || fd < 0 || fd > MAX_FDS || !md.fds[fd])
    return 0;

  off_t bytesOffset = md.fds[fd]->offset + offset;
  // invalid offset
  if (bytesOffset < 0 || bytesOffset > md.fds[fd]->qntBytes)
    return 0;
  return md.fds[fd]->offset = bytesOffset;
}

int ufufs_close(FileDescriptor fd)
{
  if (md.penFd == -1 || fd < 0 || fd > MAX_FDS || !md.fds[fd])
    return 0;
  // pega a estrutura do fd e salva usando a FAT
  int i;
  size_t file_entry = md.fds[fd]->file_entry;
  size_t fat_entry = md.MBRI.FILES_TABLE[file_entry].fat_entry;
  // salvar md.fds[fd]->blocks (blocos do arquivo alterado)
  if (md.MBRI.FILES_TABLE[file_entry].bytes < md.fds[fd]->qntBytes)
  {
    
    size_t extraBlocks = GET_BLOCKS(md.fds[fd]->qntBytes) - GET_BLOCKS(md.MBRI.FILES_TABLE[file_entry].bytes);

    // aumentar o arquivo na fat -> alterações na fat
    fat_increase_blocks(md.penFd, md.MBRI.FAT, md.MBRI.BLOCKS, fat_entry, extraBlocks);
  }

  for (i = 0; i < GET_BLOCKS(md.fds[fd]->qntBytes); i++)
  {
    if (fat_writef_block(md.penFd, md.MBRI.FAT, fat_entry, i, (char *)md.fds[fd]->blocks + (BLOCK_SIZE * i)))
    {
      return 0;
    }
  }

  time_t t_time;
  time(&t_time);
  md.MBRI.FILES_TABLE[file_entry].bytes = md.fds[fd]->qntBytes;
  md.MBRI.FILES_TABLE[file_entry].last_access = t_time;

  free(md.fds[fd]->blocks);
  free(md.fds[fd]);
  md.fds[fd] = NULL;
  updateMBR();
  return 1;
}

size_t ufufs_get_size(FileDescriptor fd)
{
  if (md.penFd == -1 || fd < 0 || fd > MAX_FDS || !md.fds[fd])
    return -1;
  return md.fds[fd]->qntBytes;
}