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


int main(){
    char str[STR_MAX_SIZE], command[STR_MAX_SIZE];
    printf("Bem vindo ao mini-shell do UFU file system\n");
    printf("Digite Help nos comandos em caso de duvida\nExit para fechar\n");
    while(1){
        printf("Digite seu comando -> ");
        setbuf(stdin, NULL);
        scanf("%[^\n]s", str);

        if( !formatCommand(str) ){
            printf("O argumento colocado parece invalido\n Tente novamente\n");
            continue;
        }
        getCommand(str,command);
        
        if(command == "HELP"){  
            helpShell();
        } else if(command == "EXIT"){
            break;
        } else if(command == "TIME"){
            timeShell();
        } else if(command == "CLS"){
            cls();
        } else if(command == "DONUT"){
            Donut();
        } else if(command == "CREATE"){
         //----------------------------------------------------
            char arg1[STR_MAX_SIZE];
            getArgument(str,arg1,NULL);
            int verifica = Shell_create(arg1);
            if(verifica == -1){
                printf("Parametros invalidos\n");
                continue;
            } else if(verifica == 0){
                printf("Tabela FAT cheia\n");
                continue;
            } else if(verifica == 1){
                printf("Sucesso ao criar arquivo\n");
            }
        //----------------------------------------------------
        } else if (command == "OPEN"){
            char arg1[STR_MAX_SIZE];
            getArgument(str,arg1,NULL);
            Shell_open(arg1); //fazer os tratamentos de erros, etc
        } else if(command == "READ"){
            //n sei implementar o read, write e seek aki, precisamos reunir
        }

    }
    return 0;
}
