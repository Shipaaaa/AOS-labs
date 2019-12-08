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

int main(int argc, char **argv) {
    int readfd, n;
    char buff[MAXBUFF];

    if ((readfd = open(FIFO, O_RDONLY | O_NDELAY)) < 0) {
        printf("Невозможно открыть FIFO\n");
        exit(1);
    }

    sleep(5);
    while ((n = read(readfd, buff, MAXBUFF)) > 0)
        if (write(1, buff, n) != n) {
            printf("Ошибка вывода\n");
            exit(1);
        }
    sleep(5);

    printf("Клиент: Завершение работы\n");
    close(readfd);
    exit(0);
}
