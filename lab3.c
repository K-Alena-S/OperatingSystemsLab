#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SUCCESSFUL_END 0
#define ERROR 1
#define ERROR_CODE_SETUID -1


void print_id() {  // Печать реального и эффективного идентификатора пользователя
    printf("Real ID = %d\n", getuid());  //getuid - возвращает реальный идентификатор пользователя (кто запустил процесс).
    printf("Effective ID = %d\n", geteuid());  // geteuid - возвращает эффективный идентификатор пользователя (кто владеет программой).
}

int open_file() {
    FILE* file = fopen("text.txt", "r");
    if (file == NULL) { // NULL - код ошибки, если файл не удалось открыть
        perror("Opening a file");
        return ERROR;
    }
    printf("Fine\n");
    int close_res = fclose(file);
    if (close_res == EOF){ // EOF - код ошибки, если файл не удалось закрыть
        perror("Closing a file");
        return ERROR;
    }
    return SUCCESSFUL_END;
}


int main() {
    
    print_id(); // Печать эффективного и реального идентфикатора пользователя
    open_file(); // Открытие файла

    int setuid_res = setuid(getuid()); // Устанавливаем эффективный идентификатор равным реальному
    if (setuid_res == ERROR_CODE_SETUID) {
        perror("Setuid");
        return ERROR;
    }

    print_id();
    open_file(); // Второй раз пытаемся открыть файл, уже с измененным идентификатором

    return SUCCESSFUL_END;
}
