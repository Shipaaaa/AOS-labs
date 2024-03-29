#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
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
11. Повторить выполнение п. 10, включив в процессы для синхронизации работы 
с терминалом использование режима слабой блокировки. 
Как изменится поведение процессов?
*/
int main(int argc, char* argv[]) {
    int pid;
    if ((pid = fork()) == -1) {
        perror("fork error");
        exit(1);
    }

    char buf[100];
    struct flock wl, rl;
    if (pid) {
        int n;
        wl.l_type = F_WRLCK;
        wl.l_whence = SEEK_CUR;
        wl.l_start = 0;
        wl.l_len = 0;
        rl.l_type = F_RDLCK;
        rl.l_whence = SEEK_CUR;
        rl.l_start = 0;
        rl.l_len = 0;

        while (1) {
            wl.l_type = F_WRLCK;
            rl.l_type = F_RDLCK;
            fcntl(0, F_SETLKW, &rl);
            fcntl(1, F_SETLKW, &wl);
            if ((n = read(0, &buf, sizeof(buf))) == -1) {
                perror("[P] read error");
            }
            write(1, "[P] ", 4);
            if (write(1, buf, n) == -1) {
                perror("[P] write error");
            }
            wl.l_type = F_UNLCK;
            rl.l_type = F_UNLCK;
            fcntl(0, F_SETLK, &rl);
            fcntl(1, F_SETLK, &wl);
        }
    } else {
        wl.l_type = F_WRLCK;
        wl.l_whence = SEEK_CUR;
        wl.l_start = 0;
        wl.l_len = 0;
        rl.l_type = F_RDLCK;
        rl.l_whence = SEEK_CUR;
        rl.l_start = 0;
        rl.l_len = 0;

        printf("[C] Child process started. PID = %d\n", getpid());
        int n;
        while (1) {
            wl.l_type = F_WRLCK;
            rl.l_type = F_RDLCK;
            fcntl(0, F_SETLKW, &rl);
            fcntl(1, F_SETLKW, &wl);
            if ((n = read(0, &buf, sizeof(buf))) == -1) {
                perror("read error");
            }
            write(1, "[C] ", 4);
            if (write(1, buf, n) == -1) {
                perror("write error");
            }
            // unlock
            wl.l_type = F_UNLCK;
            rl.l_type = F_UNLCK;
            fcntl(0, F_SETLK, &rl);
            fcntl(1, F_SETLK, &wl);
        }
    }
    return 0;
}