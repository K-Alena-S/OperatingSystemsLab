#include <sys/types.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define error_time -1
#define true_setenv 0
#define error NULL

extern char *tzname[];

int main(){
  time_t now;
  struct tm *sp;
  char *Ctime;
  
  int result_time = time(&now);
  if (result_time == error_time){
    perror("Error");
    return 1;
  }
  
  int result_setenv = setenv("TZ", "PST8PDT", 1);
  if (result_setenv != true_setenv){
    perror("Error");
    return 1;
  }
  
  if ((Ctime = ctime(&now)) == error){
    perror("Error");
    return 1;
  }
  printf("%s", Ctime);
  
  if ((sp = localtime(&now)) == error){
    perror("Error");
    return 1;
  }
  
  printf("%02d/%d/%02d %02d:%02d:%02d %s\n",
         sp->tm_mon + 1 , sp->tm_mday,
         sp->tm_year+1900, sp->tm_hour,
         sp->tm_min, sp->tm_sec, tzname[sp->tm_isdst]);
  return 0;
}
