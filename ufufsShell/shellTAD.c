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

int formatCommand(char *str)
{
    int i, j;
    for (i = 0; str[i] != '\0'; i++)
    {
        if (str[i] == 39)
        { // ' na tablea ascii
            for (j = i; str[j] != 39; j++)
            { //O que está entre aspas simples será os caminhos das pastas, não pode ser formatado
                if (str[j] == '\0')
                    return 0; //flag para caminho invalido
            }
        }
        else if (str[i] > 96 && str[i] < 123)
        {
            str[i] = str[i] - 32; //deixa as letras maiusculas
        }
    }
    return 1; //formatou com sucesso
}

void getCommand(char *str, char *command)
{
    int j = 0;
    for (int i = 0; str[i] != '\0' && str[i] != 39; i++)
    {
        if (str[i] != ' ')
        {
            command[j] = str[i];
            j++;
        }
    }
}

void getArgument(char *str, char *arg1, char *arg2)
{
    int i, j;
    arg1[0] = '\0';

    for (i = 0; str[i] != '\0'; i++)
    {
        if (str[i] == 39)
        {
            int u = 0;
            if (arg1[0] != '\0') //verifica se o primeiro já foi atribuido
                for (j = i; str[j] != 39; j++)
                {
                    arg1[u] = str[j]; //atribui o primeiro argumento
                }
            else if (arg2 != NULL) //se existe o segundo, o atribui
                for (j = i; str[j] != 39; j++)
                {
                    arg2[u] = str[j];
                }
            else
                break; //se não existe o segundo, nao tem pq continuar o loop
            i = j;     //faz com que 'i' acompanhe 'j' para o proximo loop
        }
    }
}

void timeShell()
{
    //Get current seconds in Epoch standard
    time_t now = time(NULL);
    struct tm timeFormat;
    char temp[9];
    //Get struct tm, which represents local time
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
    printf("Argumentos sempre entre ' ' \n");
    printf("Comandos:\n");
    printf("Time : mostra o tempo da maquina\n");
    printf("DONUT : hehe\n");
    //escrever todos depois
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

void Donut()
{ //nao testei ainda, cuidado
    float A = 0, B = 0;
    float i, j;
    int k;
    float z[1760];
    char b[1760];
    printf("\x1b[2J");
    for (;;)
    {
        memset(b, 32, 1760);
        memset(z, 0, 7040);
        for (j = 0; j < 6.28; j += 0.07)
        {
            for (i = 0; i < 6.28; i += 0.02)
            {
                float c = sin(i);
                float d = cos(j);
                float e = sin(A);
                float f = sin(j);
                float g = cos(A);
                float h = d + 2;
                float D = 1 / (c * h * e + f * g + 5);
                float l = cos(i);
                float m = cos(B);
                float n = sin(B);
                float t = c * h * g - f * e;
                int x = 40 + 30 * D * (l * h * m - t * n);
                int y = 12 + 15 * D * (l * h * n + t * m);
                int o = x + 80 * y;
                int N = 8 * ((f * e - c * d * g) * m - c * d * e - f * g - l * d * n);
                if (22 > y && y > 0 && x > 0 && 80 > x && D > z[o])
                {
                    z[o] = D;
                    b[o] = ".,-~:;=!*#$@"[N > 0 ? N : 0];
                }
            }
        }
        printf("\x1b[H");
        for (k = 0; k < 1761; k++)
        {
            putchar(k % 80 ? b[k] : 10);
            A += 0.00004;
            B += 0.00002;
        }
        usleep(30000);
    }
}
