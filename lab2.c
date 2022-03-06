#include <sys/types.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define ERROR_TIME -1
#define ERROR_SETENV -1
#define ERROR 1
#define SUCCESSFUL_END 0

extern char *tzname[];

int main(){
  time_t now;
  struct tm *struct_pointer; 
  char *Ctime;
  
  int result_time = time(&now); // текущее календарное значение времени в секундах сохраняется по адресу now, отсчет секунд с 01.01.1970 по Гринвичу (время Лондона)
  if (result_time == ERROR_TIME){
    perror("Error");
    return ERROR;
  }
  
  int result_setenv = setenv("TZ", "PST8PDT", 1); // копирует переменную окружения из оболочки bash и меняет часовой пояс на Калифорнию, 
  if (result_setenv == ERROR_SETENV){                                                                      // основная (откуда копируются) переменная не изменяется 
    perror("Error");
    return ERROR;
  }
  
  Ctime = ctime(&now); // преобразует время в секундах, в дату в понятной форме
  if (Ctime == NULL){
    perror("Error");
    return ERROR;
  }
  printf("%s", Ctime); // пример вывода: Sun Mar  6 04:15:48 2022 (день недели/ месяц/ день месяца/ время/ год)
  
  struct_pointer = localtime(&now); //  Время указанное в now используется для заполнения структуры tm значениями текущей даты и времени.
  if (struct_pointer == NULL){
    perror("Error");
    return ERROR;
  }
  
  printf("%02d/%d/%02d %02d:%02d:%02d %s\n", // пример вывода: 03/6/2022 04:15:48 PST (месяц/ день/ год/ время/ часовой пояс)
         struct_pointer->tm_mon + 1 , struct_pointer->tm_mday,
         struct_pointer->tm_year+1900, struct_pointer->tm_hour,
         struct_pointer->tm_min, struct_pointer->tm_sec, tzname[struct_pointer->tm_isdst]);
  return SUCCESSFUL_END;
}
