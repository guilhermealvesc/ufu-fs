# ufu-fs

## Sistema de arquivos (ufuFS) para ser usado em pen drives

- [Descrição do trabalho](https://ufubr.sharepoint.com/sites/GBC045-SistemasOperacionaisMaterial/Documentos%20Compartilhados/8%20-%20Unidade%20VIII%20(Estudo%20de%20Casos%20-%20TCDs)/TEMAS/TCD-G3.pdf)
- Alunos: Guilherme Alves Carvalho, Amauri Pietropaolo Tavares Júnior, Pedro Leale e João Batista de Sousa Paula.

## Compile

### ufufs_format

`gcc ufuFs_format.c utils/utils.c blockmanager/blockmanager.c faloc/faloc.c ufufs/ufufs.c -o main.out -lm`

### ufufs_shell

`gcc ./ufufsShell/shell.c ./ufufsShell/shellTAD.c ./ufufs/ufufs.c ./faloc/faloc.c ./blockmanager/blockmanager.c -lm -o shell.out`