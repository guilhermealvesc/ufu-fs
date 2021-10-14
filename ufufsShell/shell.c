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
/*
    A ideia do algoritmo é: ler o input, procurar palavras definidas na tabelahash do shell
    utilizando uma busca, para facilitar vou reutilizar o código do nosso tra2 de aed2
    se reconhecer alguma palavra executar função dela
*/

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
        else if (!strcmp(command, "DONUT"))
        {
            Donut();
        }
        else if (!strcmp(command, "CREATE"))
        {
            //----------------------------------------------------
            arg = _getArg(str);
            int verifica = Shell_create(arg[1]);
            if (verifica == -1)
            {
                printf("Parametros invalidos\n");
                continue;
            }
            else if (verifica == 0)
            {
                printf("Tabela FAT cheia\n");
                continue;
            }
            else if (verifica == 1)
            {
                printf("Sucesso ao criar arquivo\n");
            }
            //----------------------------------------------------
        }
        else if (!strcmp(command, "OPEN"))
        {
            arg = _getArg(str);
            Shell_open(arg[1]); //fazer os tratamentos de erros, etc
        }
        else if (!strcmp(command, "MOUNT"))
        {
            arg = _getArg(str);
            if (!Shell_mount(arg[1]))
            {
                printf("Nao foi possivel montar ufu fs em %s\n", arg[1]);
            }
        }
        else
        {
            printf("Comando desconhecido!\n");
            printf("Digite Help nos comandos em caso de duvida\nExit para fechar\n");
        }
        printf("\n");
        for (int i = 0; i < STR_MAX_SIZE; i++) //pra q isso ?
        {
            str[i] = '\0';
            command[i] = '\0';
        }
    }
    return 0;
}
