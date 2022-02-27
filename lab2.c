#include <sys/types.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

extern char *tzname[];

int main(){
  time_t now;
  struct tm *sp;
  char *Ctime;
  
  int res1 = time(&now);
  if (res1 == -1){
    perror("Error");
    return 1;
  }
  
  int res2 = setenv("TZ", "PST8PDT", 1);
  if (res2 != 0){
    perror("Error");
    return 1;
  }
  
  if ((Ctime = ctime(&now)) == NULL){
    perror("Error");
    return 1;
  }
  printf("%s", Ctime);
  
  if ((sp = localtime(&now)) == NULL){
    perror("Error");
    return 1;
  }
  
  printf("%02d/%d/%02d %02d:%02d:%02d %s\n",
         sp->tm_mon + 1 , sp->tm_mday,
         sp->tm_year+1900, sp->tm_hour,
         sp->tm_min, sp->tm_sec, tzname[sp->tm_isdst]);
  return 0;
}
