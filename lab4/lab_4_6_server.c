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
6. Создать именованный канал (любым способом). 
Убедиться, что канал создан. 
Рассмотреть особенности работы с именованным каналом (открыть файл с установленным и не установленным флагом O_NDELAY). 
Использовать его для обмена информацией с процессом, созданным другим пользователем (своим соседом).
*/

int main(int argc, char* argv[], char* argp[]) {
    unlink(FIFO);
    if (mkfifo(FIFO, S_IFIFO | 0755) < 0) {
        printf("Невозможно создать FIFO\n");
        print_err();
    }
    int writefd;

    sleep(5);
    if ((writefd = open(FIFO, O_WRONLY | O_NDELAY)) < 0) {
        printf("Невозможно открыть FIFO\n");
        print_err();
        exit(1);
    }
    char msg[MAXBUFF] = "Мама мыла раму\n";

    if (write(writefd, msg, strlen(msg)) != strlen(msg)) {
        printf("Ошибка записи\n");
        exit(1);
    }

    sleep(10);
    printf("Сервер: Завершение работы\n");
    close(writefd);
    unlink(FIFO);
    exit(0);
}
