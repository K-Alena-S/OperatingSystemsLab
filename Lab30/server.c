#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <ctype.h>

#define ERROR -1
#define COMPLETION_ERROR 1

#define MAX_LEN 1
#define SUCCESSFUL_END 0
#define MAX_BUF 256 
#define DEFAULT_PROTOCOL 0
#define END_OF_FILE 0

int to_upper_case(int file_descriptor) {
    int read_result;
    char message[MAX_BUF];

    while ((read_result = read(file_descriptor, message, MAX_BUF)) != END_OF_FILE) {
        if (read_result == ERROR) {
            perror("couldn't read");
            return COMPLETION_ERROR;
        }
        for (int i = 0; i < read_result; i++) {
            message[i] = toupper(message[i]);
            printf("%c", message[i]);
        }
    }
    return SUCCESSFUL_END;
}

int main() {
    struct sockaddr_un addr;

    unlink("server");

    memset(&addr, 0, sizeof(addr));

    int server_fd = socket(AF_LOCAL, SOCK_STREAM, DEFAULT_PROTOCOL);

    if (server_fd == ERROR) {
        perror("couldn't get socket");
        return COMPLETION_ERROR;
    }

    addr.sun_family = AF_LOCAL;
    strcpy(addr.sun_path, "server");

    int bind_result = bind(server_fd, (const struct sockaddr*)&addr, sizeof(addr));
    if (bind_result == ERROR) {
        perror("couldn't bind");
        close(server_fd);
        return COMPLETION_ERROR;
    }

    int listen_result = listen(server_fd, MAX_LEN);
    if (listen_result == ERROR) {
        perror("couldn't listen");
        close(server_fd);
        return COMPLETION_ERROR;
    }

    int client_fd = accept(server_fd, NULL, NULL);
    if (client_fd == ERROR) {
        perror("couldn't accept");
        close(server_fd);
        return COMPLETION_ERROR;
    }

    int close_result = close(server_fd);
    if (close_result == ERROR) {
        perror("couldn't close");
        close(client_fd);
        return COMPLETION_ERROR;
    }

    int read_result = to_upper_case(client_fd);
    if (read_result == COMPLETION_ERROR) {
        close(client_fd);
        return COMPLETION_ERROR;
    }

    close_result = close(client_fd);
    if (close_result == ERROR) {
        perror("couldn't close");
        return COMPLETION_ERROR;
    }

    unlink("server");
    return SUCCESSFUL_END;
}
