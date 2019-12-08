#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
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
7.Создать два параллельных процесса, в каждом из которых осуществляется работа с одним 
и тем же файлом. Каждый процесс открывает файл 
(имя файла передается через список аргументов командной строки). Один процесс пишет в файл, другой - читает из него. Что произойдет без включения блокировок?
*/
int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s filename", argv[0]);
        exit(1);
    }
    int pid;
    if ((pid = fork()) == -1) {
        perror("fork error");
        exit(1);
    }

    if (pid) {
        int fd;
        if ((fd = open(argv[1], O_WRONLY | O_CREAT, 0755)) == -1) {
            perror("[Parent] open error");
            exit(1);
        }
        int i = 0;
        int val;
        char *c;
        do {
            if (write(fd, "1", 1) == -1) {
                perror("[Parent] write error");
            }
        } while (i++ < 100);
        close(fd);
    } else {
        int fd;
        if ((fd = open(argv[1], O_RDONLY)) == -1) {
            perror("[Child] open error");
            exit(1);
        }
        int i = 0;
        int val;
        char *c;
        do {
            if (read(fd, &c, 1) == -1) {
                perror("[Child] write error");
            }
            write(1, &c, 1);
        } while (i++ < 100);
        close(fd);
    }
    return 0;
}