#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#define SUCCESSFUL_END 0
#define ERROR 1
#define ERROR_CODE_SETUID -1

/*
    Реальные и эффективные идентификаторы для процесса первоначально совпадают. Эффективный идентификатор пользователя
    и список групп доступа (показываемый getgroups(2)) используются для определения прав доступа к файлам.
    Владелец любого файла, созданного процессом, определяется эффективным идентификатором пользователя, а группа файла - эффективным
    идентификатором группы. Единственный способ выполнения программы - не ее владельцем - это
    запустить программу (a.out) с установленным битом установки идентификатора пользователя или группы. Если бит установлен, тогда
    эффективный идентификатор пользователя и/или группы становится таким же, как и у владельца и/или группы данной программы. Благодаря этому
    механизму можно стать "заместителем" или "представителем" привилегированной группы. Бит установки идентификатора пользователя
    или группы устанавливается с помощью chmod().  И если до запуска такого исполняемого файла этот бит установлен, то эффективный идентификатор 
    пользователя процесса становится таким же, как и у владельца.
    
    Используя SUID-бит можно получить доступ к недостижимому файлу, если владелец этого файла установит этот бит.
*/

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
    printf("File opened successfully\n");
    int close_res = fclose(file);
    if (close_res == EOF){ // EOF - код ошибки, если файл не удалось закрыть
        perror("Closing a file");
        return ERROR;
    }
    return SUCCESSFUL_END;
}


int main() {
    
    print_id(); // Печать эффективного и реального идентфикатора пользователя
    int result = open_file(); // Открытие файла
    if (result == SUCCESSFUL_END) return SUCCESSFUL_END;

    int setuid_res = setuid(getuid()); // Устанавливаем эффективный идентификатор равным реальному
    if (setuid_res == ERROR_CODE_SETUID) {
        perror("Setuid");
        return ERROR;
    }

    print_id();
    result = open_file(); // Второй раз пытаемся открыть файл, уже с измененным идентификатором
    if (result == SUCCESSFUL_END) return SUCCESSFUL_END;
    
    return result;
}
