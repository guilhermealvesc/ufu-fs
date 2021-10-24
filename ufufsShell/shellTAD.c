#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <time.h>
#include <dirent.h>
//#include <sys/syscall.h>
#include "shellTADheader.h"
#include "../ufufs/ufufs.h"

char **_getArg(char *line)
{
  int bufsize = TOK_SIZE, j = 0;
  char **tokens = malloc(bufsize * sizeof(char *));
  char *token;

  if (!tokens)
  {
    printf("allocation error\n");
    return NULL;
  }

  token = strtok(line, TOK_DELIM);
  while (token != NULL)
  {
    tokens[j] = token;
    j++;

    if (j >= bufsize)
    {
      bufsize += TOK_SIZE;
      tokens = realloc(tokens, bufsize * sizeof(char *));
      if (!tokens)
      {
        printf("Allocation error\n");
        return NULL;
      }
    }

    token = strtok(NULL, TOK_DELIM);
  }
  tokens[j] = NULL;
  return tokens;
}

char *_getCommand(char *str)
{
  int j = 0;
  int bufsize = strlen(str);
  char *command = malloc(sizeof(char) * bufsize);
  if (!command)
    return NULL;

  for (int i = 0; str[i] != '\0'; i++)
  {
    if (str[i] != ' ')
    {
      command[j] = str[i];
      command[j] = toupper(command[j]);
      j++;
      if (str[i + 1] == ' ')
        break;
    }
  }
  command[j] = '\0';
  return command;
}

char *_readLine()
{
  int bufsize = MAX_STR_SIZE;
  int j = 0;
  char *buffer = malloc(sizeof(char) * bufsize);
  int c;

  if (!buffer)
  {
    printf("Allocation error\n");
    return NULL;
  }

  while (1)
  {
    // le um caractere
    c = getchar();

    // se chegarmos a EOF, troca para \0 e retorna
    if (c == EOF || c == '\n')
    {
      buffer[j] = '\0';
      return buffer;
    }
    else
    {
      buffer[j] = c;
    }
    j++;

    // realoca se excedemos o tamanho do buffer
    if (j >= bufsize)
    {
      bufsize += MAX_STR_SIZE;
      buffer = realloc(buffer, bufsize);
      if (!buffer)
      {
        printf("Allocation error\n");
        return NULL;
      }
    }
  }
}

void timeShell()
{
  // Get current seconds in Epoch standard
  time_t now = time(NULL);
  struct tm timeFormat;
  char temp[9];
  // Get struct tm, which represents local time
  timeFormat = *localtime(&now);
  /*
  Convert time, storing conversion in 'temp',
  passing it's size, the format desired and
  tm structured representing local time
  */
  strftime(temp, sizeof(temp), "%H:%M:%S", &timeFormat);
  printf("%s\n", temp);
}

void cls()
{
  printf("\e[1;1H\e[2J"); /*
  //Writes "\e[1;1H\e[2J" to stdout, giving an effect of cleared screen
  char regex[13] = "\e[1;1H\e[2J";
  if (!syscall(SYS_write, STDIN_FILENO, regex, sizeof(char) * 13)){
    err("syscall gerou erro!");
  }*/
}

void helpShell()
{
  printf("Comandos e argumentos separados por espaços \n");
  printf("Comandos:\n");
  printf("Time: mostra o tempo da maquina\n");
  printf("CLS: limpa a tela\n");
  printf("Create nome do arquivo: cria arquivo com nome especificado, erro caso ja exista, sem espacos em branco no nome, use _ caso necessario\n");
  printf("LS: lista todos os arquivos no diretorio\n");
  printf("Mount: realiza a montagem do pendrive\n");
  // escrever todos depois
}

int Shell_create(char *arg1)
{
  return ufufs_create(arg1);
}

int Shell_mount(char *arg1)
{
  return ufufs_mount(arg1);
}

FileDescriptor Shell_open(char *arg1)
{
  return ufufs_open(arg1);
}

// int Shell_read(char *arg1, char *arg2){
//     return ufufs_read(md.penFd, arg1, arg2);
// }

void ls()
{
  return ufufs_ls();
}

int cp(char *src, char *dest)
{
  if (src[0] == '#' && dest[0] != '#')
  {
    // olha se já existe
    // se não existe, cria metadados
    // ufufs_open
    // ufufs_write no file descriptor e bala
    // copia do nosso pen drive para dest
  }
  else if (dest[0] == '#' && src[0] != '#')
  {
    // copia do so para o pen drive
    // mesma coisa só que com fopen e fwrite
  }
  else
    return 0;
  return 1;
}

void Shell_close(FileDescriptor fd)
{
  ufufs_close(fd);
}
