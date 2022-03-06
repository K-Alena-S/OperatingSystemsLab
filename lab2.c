#include <sys/types.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define ERROR_TIME -1
#define TRUE_SETENV 0
#define ERROR 1
#define SUCCESSFUL_END 0

extern char *tzname[];

int main(){
  time_t now;
  struct tm *struct_pointer; 
  char *Ctime;
  
  int result_time = time(&now);
  if (result_time == ERROR_TIME){
    perror("Error");
    return ERROR;
  }
  
  int result_setenv = setenv("TZ", "PST8PDT", 1);
  if (result_setenv != TRUE_SETENV){
    perror("Error");
    return ERROR;
  }
  
  Ctime = ctime(&now);
  if (Ctime == NULL){
    perror("Error");
    return ERROR;
  }
  printf("%s", Ctime);
  
  struct_pointer = localtime(&now);
  if (struct_pointer == NULL){
    perror("Error");
    return ERROR;
  }
  
  printf("%02d/%d/%02d %02d:%02d:%02d %s\n",
         struct_pointer->tm_mon + 1 , struct_pointer->tm_mday,
         struct_pointer->tm_year+1900, struct_pointer->tm_hour,
         struct_pointer->tm_min, struct_pointer->tm_sec, tzname[struct_pointer->tm_isdst]);
  return SUCCESSFUL_END;
}
