//
// Created by Vladislav Shipugin on 23.11.19.
//

#include <fcntl.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void change_printf_to_error() {
    printf("\033[0;31m");
}

void change_printf_to_log() {
    printf("\033[0m");
}

void showMessage(char *message) {
    printf("%s", message);
}

void showError(char *errorMessage) {
    change_printf_to_error();
    printf("%s", errorMessage);
    change_printf_to_log();
}

void checkPerror(char *errorMessage) {
    if (errno != 0) perror(errorMessage);
}

int main(int argc, char **argv) {
    printf("\nНачало нового исполняемого кода, загруженного в дочернем процессе\n");
    printf("\nПараметры дочернего процесса в новом коде pid=%d ppid=%d grp=%d\n", getpid(), getppid(), getpgrp());

    if (argc < 2) {
        printf("\nargc < 2\n");
        exit(EXIT_FAILURE);
    }

    int i;
    char buffer;

    for (i = 0; i < argc; i++) printf("\nargv[%d] = %s\n", i, argv[i]);

    int fildes = atoi(argv[1]);

    printf("\nБыл открыт переданный дескриптор pipe на запись: %d\n", fildes);

    //! Если мы открываем с O_NONBLOCK, и родительский завершается, закрывает канал на чтение, то при записи в этот канал получим SIGPIPE
    while (read(0, &buffer, 1) > 0) {
        write(fildes, &buffer, 1);
    }

    close(fildes);
    exit(EXIT_SUCCESS);
}