#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

#define FIFO "fifo.l"
#define MAXBUFF 80

void print_err() {
    printf("\n\n*****print_err()*****");

    printf("\nError code: %d", errno);
    perror("\nError message from perror(): ");

    printf("\n*****End print_err()*****\n");
}

/*
10. Создать два параллельных процесса, разделяющие ввод с клавиатуры и вывод на экран. 
Что получится при исполнении этих процессов?
*/
int main(int argc, char* argv[]) {
    int pid;
    if ((pid = fork()) == -1) {
        perror("fork error");
        exit(1);
    }

    char buf[100];
    if (pid) {
        int n;
        while (1) {
            if ((n = read(0, &buf, sizeof(buf))) == -1) {
                perror("[Parent] read error");
            }
            write(1, "[Parent] ", 4);
            if (write(1, buf, n) == -1) {
                perror("[Parent] write error");
            }
        }
    } else {
        printf("[Child] Child process started. PID = %d\n", getpid());
        int n;
        while (1) {
            if ((n = read(0, &buf, sizeof(buf))) == -1) {
                perror("read error");
            }
            write(1, "[Child] ", 4);
            if (write(1, buf, n) == -1) {
                perror("write error");
            }
        }
    }
    return 0;
}