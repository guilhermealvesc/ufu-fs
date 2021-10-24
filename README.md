# ufu-fs

## Sistema de arquivos (ufuFS) para ser usado em pen drives

- [Descrição do trabalho](https://ufubr.sharepoint.com/sites/GBC045-SistemasOperacionaisMaterial/Documentos%20Compartilhados/8%20-%20Unidade%20VIII%20(Estudo%20de%20Casos%20-%20TCDs)/TEMAS/TCD-G3.pdf)
- Alunos: Guilherme Alves Carvalho, Amauri Pietropaolo Tavares Júnior, Pedro Leale e João Batista de Sousa Paula.

## Compile

### ufuFs Shared Library

`gcc -c -fpic ./ufufs/ufufs.c ./faloc/faloc.c ./blockmanager/blockmanager.c && gcc -shared ufufs.o faloc.o blockmanager.o -o libufufs.so`

### ufufs_format

`gcc ufuFs_format.c utils/utils.c blockmanager/blockmanager.c faloc/faloc.c ufufs/ufufs.c -o ufuFs_format.out -lm`

### ufufs_shell

`gcc -L/home/valtim/Documentos/prog/ufu-fs ./ufufsShell/shell.c ./ufufsShell/shellTAD.c -o shell.out -lufufs -lm`

## Run

### shell.out

`sudo "LD_LIBRARY_PATH=/home/valtim/Documentos/prog/ufu-fs:$LD_LIBRARY_PATH" ./shell.out`
