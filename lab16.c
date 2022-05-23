#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#define ERROR -1
#define SUCCESSFUL_END 0

int ColorSelection(int fd) {
    printf("What is your favorite color: white(w), blue(b), green(g), red(r) or lilac(l)?\n");
    char color;
    ssize_t readd = read(fd, &color, 1);
    if (readd == ERROR) {
        perror("error read");
        return ERROR;
    }
    switch (color) {
    case 'w':
        printf("You chose white!\n");
        break;
    case 'b':
        printf("You chose blue!\n");
        break;
    case 'g':
        printf("You chose green!\n");
        break;
    case 'r':
        printf("You chose red!\n");
        break;
    case 'l':
        printf("You chose lilac!\n");
        break;
    default:
        printf("Incorrect input (%c)\n", color);
        break;
    }
    return SUCCESSFUL_END;
}

int main(int argc, char* argv[]) {
    int fd, tcs, tcg;

    struct termios tty, savetty;
    fd = open("/dev/tty", O_RDWR);
    if (fd == ERROR) {
        perror("error open");
        return ERROR;
    }

    tcg = tcgetattr(fd, &tty);
    if (tcg == ERROR) {
        perror("error tcgetattr");
        return ERROR;
    }

    savetty = tty;
    tty.c_cc[VMIN] = 1;
    tty.c_lflag &= ~(ECHO | ICANON);

    tcs = tcsetattr(fd, TCSANOW, &tty);
    if (tcs == ERROR) {
        perror("error tcgetattr");
        return ERROR;
    }
    
    int res = ColorSelection(fd);
    if (res == ERROR) return ERROR;

    tcs = tcsetattr(fd, TCSANOW, &savetty);
    if (tcs == ERROR) {
        perror("error tcsetattr");
        return ERROR;
    }
    int close_f = close(fd);
    if (close_f == ERROR) {
        perror("error close");
        return ERROR;
    }
    return SUCCESSFUL_END;
}
