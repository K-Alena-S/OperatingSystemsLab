#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define ERROR_WAIT -1
#define ERROR_FORK -1
#define ERROR 1
#define SUCCESSFUL_END 0
#define CHILD 0


int main(int argc, char *argv[]) {
    int pid = fork(); 
    if (pid == ERROR_FORK) {
        perror("error fork");
        return ERROR;
    }
    if (pid == CHILD) {
        execlp("cat", "cat", argv[1], NULL);
        perror("execlp");
        return ERROR;
    }
    int er_wait = wait(NULL);
    if (er_wait == ERROR_WAIT){
        perror("error wait");
        return ERROR;
    }
    printf("The line is printed by the ininial parent process\n");
    return SUCCESSFUL_END;
}
