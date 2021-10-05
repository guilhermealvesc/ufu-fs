#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "./ufufs.h"
// usa o fd do SO
int penFd = -1;
// todas rotinas devem verificar se o fs foi montado

// vetor de file descriptors, lib gerencia arquivos abertor pelos fd's

struct fileDesc {
  int inode;
  int qntBlocks;
  int* blocosArquivo;
  unsigned int offset;
};
// int       int    int         vet    vet
// [MAGIC_N, BYTES, BLOCK_SIZE, FILES, FAT]
int ufufs_mount(const char* filePath) {
  //tenta abrir o pen drive 
  //atribui o fd na abertura para var global
  penFd = open(filePath, O_RDWR);
  if(penFd < 0) return 0; //adicionar errno
  read(penFd, );
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
  // tabela de metadados tem a quantidade de bytes do arquivo
  // sabendo disso e do tamanho do block size, alocar vetor com tamanho GET_BLOCKS(bytes_file) para comportar arquivo
  // retorna um inteiro (chave) para acessar arquivo
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
  // entra na estrutura do fd gerenciado pela lib
  // desloca o offset em 'offset' bytes do parametro
}

int ufufs_close(FileDescriptor fd) {
  // pega a estrutura do fd e salva usando a FAT
  // apaga as estruturas bonitim
}