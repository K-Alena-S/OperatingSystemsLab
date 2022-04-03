#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SUCCESSFUL_END 0
#define ERROR 1
#define ERROR_CODE_SETUID -1

void print_id() {
    printf("Real ID = %d\n", getuid());
    printf("Effective ID = %d\n", geteuid());
}

int open_file() {
    FILE* file = fopen("text.txt", "r");
    if (file == NULL) {
        perror("Opening file");
        return ERROR;
    }
    printf("Fine\n");
    int close_res = fclose(file);
    if (close_res == EOF) return ERROR;
    return SUCCESSFUL_END;
}


int main() {
    print_id();
    open_file();

    int setuid_res = setuid(getuid());
    if (setuid_res == ERROR_CODE_SETUID) {
        perror("Setuid");
        return ERROR;
    }

    print_id();
    open_file();

    return SUCCESSFUL_END;
}
~
