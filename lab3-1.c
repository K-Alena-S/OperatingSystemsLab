#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#define SUCCESSFUL_END 0
#define ERROR 1
#define ERROR_CODE_SETUID -1

void print_id() {
    printf("\n Real ID = %d\n", getuid());
    printf(" Effective ID = %d\n", geteuid());
}

int open_file() {
    FILE* file = fopen("text.txt", "r");
    if (file == NULL) {
        perror("Opening a file");
        return ERROR;
    }
    printf(" File opened successfully\n");
    int close_res = fclose(file);
    if (close_res == EOF){
        perror("Closing a file");
        return ERROR;
    }
    return SUCCESSFUL_END;
}

int main() {

    print_id();
    int result = open_file();
    if (result != SUCCESSFUL_END) return result;

    int setuid_res = setuid(getuid());
    if (setuid_res == ERROR_CODE_SETUID) {
        perror("Setuid");
        return ERROR;
    }

    print_id();
    result =  open_file();
    if (result == SUCCESSFUL_END) return SUCCESSFUL_END;

    return result;
}
