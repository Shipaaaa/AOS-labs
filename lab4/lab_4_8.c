#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdbool.h>
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
8. Добавить в один из процессов использование слабой блокировки. Что произойдет?
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
        struct flock lock;
        lock.l_type = F_RDLCK;
        lock.l_start = 0;
        lock.l_whence = SEEK_SET;
        lock.l_len = 0;
        fcntl(fd, F_SETLKW, lock);
        int i = 0;
        int val;
        char *c;
        do {
            if (write(fd, "1", 1) == -1) {
                perror("[Parent] write error");
            }
        } while (i++ < 100);
        lock.l_type = F_UNLCK;
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