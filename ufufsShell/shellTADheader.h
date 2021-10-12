
typedef unsigned int FileDescriptor;

int formatCommand(char *str);
void getCommand(char *str, char *command);
void getArgument(char *str, char *arg1, char *arg2);
void timeShell();
void cls();
void helpShell();
void Donut();
int Shell_create(char *arg1);
int Shell_mount(char *arg1);
FileDescriptor Shell_open(char *arg1);