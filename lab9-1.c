#include <stdio.h>
#include <unistd.h>

#define ERROR_FORK -1
#define ERROR 1
#define SUCCESSFUL_END 0
#define CHILD 0

int main() {
    int pid = fork();
    if (pid == ERROR_FORK) {
        perror("error fork");
        return ERROR;
    }
    if (pid == CHILD) {
        execlp("cat", "cat", "text.txt", NULL);
        perror("execlp");
        return ERROR;
    }
    printf("The line is printed by the ininial parent process\n");
    return SUCCESSFUL_END;
}
