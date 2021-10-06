#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "./ufufs.h"
#define MAX_FDS 30

// usa o fd do SO
typedef struct {
  int inode;
  int qntBlocks;
  int* blocosArquivo; //[block, block, block]
  unsigned int offset;
} FD;

typedef struct {
  int penFd;
  // int BYTES;
  // int BLOCKS;
  FD* fds[MAX_FDS];
} MountData;

//[0]
MountData md = {-1, NULL};
// todas rotinas devem verificar se o fs foi montado

// vetor de file descriptors, lib gerencia arquivos abertor pelos fd's


// int       int    int         vet    vet
// [MAGIC_N, BYTES, BLOCK_SIZE, FILES, FAT]
int ufufs_mount(const char* filePath) {
  //tenta abrir o pen drive 
  //atribui o fd na abertura para var global
  md.penFd = open(filePath, O_RDWR);
  if(penFd < 0) return 0; //adicionar errno
  // read(penFd, );
  //puxar os arquivos importantes, FAT, METADADOS, INFORMAÇÕES

  //coloca fat, metadados, informações na área compartilhada
  //retorna se deu erro ou não
}

// file descriptor (int) -> usado para ler byte a byte do arquivo aberto
// quando o arquivo for aberto, preciso criar para esse file descriptor
// cada file descriptor:
// vetor de blocos do arquivo
// deslocamento em bytes

FileDescriptor ufufs_open(const char* filename) {
  // olha na tabela de metadados se o arquivo existe 
  
  // existindo, criar o ponteiro para arquivo inicializado com deslocamento em bytes zerado (offset),
  int fd;
  for(fd = 0; fd < MAX_FDS; fd++) {
    if(!md.fds[fd]) break;
  }

  // tabela de metadados tem a quantidade de bytes do arquivo
  // sabendo disso e do tamanho do block size, alocar vetor com tamanho GET_BLOCKS(bytes_file) para comportar arquivo
  // retorna um inteiro (chave) para acessar arquivo
  return fd;
}

int ufufs_read(FileDescriptor fd, void *buf, size_t count) {
  // entra na estrutura do fd gerenciado pela lib
  // tenta ler count bytes do vetor de blocos de arquivo
  // se estiver inicializado retorna os count bytes no buffer
  // senão, usa o penFd para, inode (indice de inicio do arquivo na fat) 
  // buscando o bloco usando as funções da fat
  // retornado, retorna isso no buffer
}

int ufufs_write(FileDescriptor fd, void *buf, size_t count) {
  // escreve na estrutura do file descriptor (vetor n blocos - representa o arquivo)
}


int ufufs_seek(FileDescriptor fd, int offset) {

  // validar entradas
  md.fds[fd]->offset += offset;
  // entra na estrutura do fd gerenciado pela lib
  // desloca o offset em 'offset' bytes do parametro
  return ;
}

int ufufs_close(FileDescriptor fd) {
  // pega a estrutura do fd e salva usando a FAT
  // apaga as estruturas bonitim
}