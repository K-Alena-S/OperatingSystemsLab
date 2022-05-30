#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#define ERROR -1
#define COMPLETION_ERROR 1
#define SUCCESSFUL_END 0
#define FGETS_ERROR NULL

#define MAX_BUF 256
#define DEFAULT_PROTOCOL 0

void sig_processing(int sig) {
    fprintf(stderr, "Server unreachable\n");
}

int get_socket() {
    int file_descriptor = socket(AF_LOCAL, SOCK_STREAM, DEFAULT_PROTOCOL);
    if (file_descriptor == ERROR) {
        perror("Couldn't create socket");
        return COMPLETION_ERROR;
    }
    return file_descriptor;
}

int write_in_socket(int file_descriptor) {
    char message[MAX_BUF];
    char* fgets_result = NULL;
    while ((fgets_result = fgets(message, MAX_BUF, stdin)) != FGETS_ERROR) {
        int write_result = write(file_descriptor, message, strlen(message));
        if (write_result == ERROR) {
            perror("Couldn't write");
            return COMPLETION_ERROR;
        }
    }
    return SUCCESSFUL_END;
}

int main() {
    struct sockaddr_un address;

    void* sigset_result = sigset(SIGPIPE, sig_processing);
    if (sigset_result == SIG_ERR) {
        perror("sigset");
        return COMPLETION_ERROR;
    }
    memset(&address, 0, sizeof(address));
    address.sun_family = AF_LOCAL;

    int file_descriptor = get_socket();
    if (file_descriptor == COMPLETION_ERROR) return COMPLETION_ERROR;

    strcpy(address.sun_path, "server");

    int connect_result = connect(file_descriptor, (struct sockaddr*)&address, sizeof(address));

    if (connect_result == ERROR) {
        close(file_descriptor);
        fprintf(stderr, "Server unreachable\n");
        return COMPLETION_ERROR;
    }
    int write_result = write_in_socket(file_descriptor);
    if (write_result == ERROR) {
        close(file_descriptor);
        return COMPLETION_ERROR;
    }

    int close_result = close(file_descriptor);
    if (close_result == ERROR) {
        perror("Couldn't close");
        return COMPLETION_ERROR;
    }
    return SUCCESSFUL_END;
}
