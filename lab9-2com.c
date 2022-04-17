#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define ERROR_WAIT -1
#define ERROR_FORK -1
#define ERROR 1
#define SUCCESSFUL_END 0
#define CHILD 0

/*
    fork
    Процесс, сделавший вызов fork(2) называется родительским, а вновь созданный процесс — дочерним. 
    Новый процесс является точной копией породившего его процесса. Как это ни удивительно, но новый процесс имеет 
те же инструкции и данные, что и его родитель. Более того, выполнение родительского и дочернего процесса 
начнется с одной и той же инструкции, следующей за fork(2). Единственно, чем они различаются — это 
идентификатором процесса PID. Каждый процесс имеет одного родителя, но может иметь несколько дочерних процессов.

    execlp
    Замещает дочерний процесс новой программой из файла. Комбинация функций fork и exec – это своего
рода двухступенчатый системный вызов, порождающий новый процесс. В UNIX эти два этапа выделены в самостоятельные функции.
*/

int main() {
    int pid = fork(); // Возвращает 0 в дочернем процессе, идентификатор дочернего процесса – в родительском процессе, –1 в случае ошибки
    if (pid == ERROR_FORK) {
        perror("error fork");
        return ERROR;
    }
    if (pid == CHILD) {
        execlp("cat", "cat", "lab9.txt", NULL);
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
