#define MAX_STR_SIZE 1024
#define TOK_SIZE 64
#define TOK_DELIM " \t\r\n\a"

typedef unsigned int FileDescriptor;

char **_getArg(char *line);
char *_readLine();
char *_getCommand(char *str);
void getArgument(char *str, char *arg1, char *arg2);
void timeShell();
void cls();
int cp(char *src, char *dest);
void ls();
void helpShell();
int Shell_create(char *arg1);
int Shell_mount(char *arg1);
FileDescriptor Shell_open(char *arg1);
void Shell_close(FileDescriptor fd);







//int formatCommand(char *str);