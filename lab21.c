#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>

#define BEL '\a'
#define ERROR 1
#define SUCCESSFUL_END 0
#define FLAG_EXIT 0

int flag = 1;
int count_sig = 0;

void sigcatch(int sig){
        if (sig == SIGQUIT) {
                flag = FLAG_EXIT;
                return;
        }

        count_sig++;
        printf("%c", BEL);
}

int main(){
        void* signals = sigset(SIGINT, sigcatch);
        if(signals == SIG_ERR){
                perror("sigset");
                return ERROR;
        }
        signals = sigset(SIGQUIT, sigcatch);
        if(signals == SIG_ERR){
                perror("sigset");
                return ERROR;
        }

        while(flag){
                pause();
        }
        printf("\n%d signals were executed \n", count_sig);
        return SUCCESSFUL_END;
}
