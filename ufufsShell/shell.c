#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <time.h>
#include <dirent.h>
#include "shellTADheader.h"
#define clear() printf("\033[H\033[J")
#define STR_MAX_SIZE 1024

int main()
{
    cls();
    char *str, *command, **arg;
    printf("Bem vindo ao mini-shell do UFU file system\n");
    printf("Digite Help nos comandos em caso de duvida\nExit para fechar\n");
    while (1)
    {
        printf("Digite seu comando -> ");
        str = _readLine();

        command = _getCommand(str);

        if (!strcmp(command, "HELP"))
        {
            helpShell();
        }
        else if (!strcmp(command, "EXIT"))
        {
            break;
        }
        else if (!strcmp(command, "TIME"))
        {
            timeShell();
        }
        else if (!strcmp(command, "CLS"))
        {
            cls();
        }
        else if (!strcmp(command, "CP"))
        {
            arg = _getArg(str);
            if (cp(arg[1], arg[2]))
            {
                printf("Arquivo copiado de '%s' para '%s'\n", arg[1], arg[2]);
            }
            else
            {
                printf("Nao foi possivel copiar\n");
            }
        }
        else if (!strcmp(command, "CREATE"))
        {

            arg = _getArg(str);
            int v = Shell_create(arg[1]);
            if (v == -1)
                printf("Falha ao criar!\n");
            else if (v == 0)
            {
                printf("Pen Drive cheio\n");
            }
        }
        else if (!strcmp(command, "CLOSE"))
        {
            // achar o filedescriptor e jogar na ufufs_close();
        }
        else if (!strcmp(command, "LS"))
        {
            ls();
        }
        else if (!strcmp(command, "MOUNT"))
        {
            if (getuid() != 0)
            {
                printf("Sem permissoes sudo para utilizar mount!\n");
            }
            else
            {
                arg = _getArg(str);
                if (!Shell_mount(arg[1]))
                {
                    printf("Nao foi possivel montar ufu fs em %s\n", arg[1]);
                }
                else
                {
                    printf("Pen drive montado com sucesso!\n");
                }
                free(arg);
            }
        } //----------- ERROU COMANDO ---------------------------------------------
        else
        {
            printf("Comando desconhecido!\n");
            printf("Digite Help nos comandos em caso de duvida\nExit para fechar\n");
        } //----------- ERROU COMANDO ---------------------------------------------
        printf("\n");
        free(str);
        free(command);
    }
    return 0;
}
