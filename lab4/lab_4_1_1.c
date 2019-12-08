//
// Created by Vladislav Shipugin on 23.11.19.
//

/*
1. Создать параллельный процесс, используя только fork. 
Организовать обмен информацией между процессами через каналы. 
Предусмотреть однонаправленную и двунаправленную связь. 
При создании двунаправленной связи исключить возможность появления «смертельных объятий».
*/

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

int main(int argc, char *argv[], char *argp[]) {
    pid_t pid;
    int wait_status;
    int pipe_file_descriptors[2];

    if (argc != 2) {
        printf("Usage: %s <string>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (pipe(pipe_file_descriptors) < 0) {
        checkPerror("Error of creating channel");
        exit(EXIT_FAILURE);
    }

    int read_file_descriptor = pipe_file_descriptors[0];
    int write_file_descriptor = pipe_file_descriptors[1];

    pid = fork();
    if (pid < 0) {
        checkPerror("Error of forking");
        exit(EXIT_FAILURE);
    }

    if (pid != 0) {
        printf("Выполнение родительского процесса\n");
        printf("Параметры родительского pid=%d ppid=%d grp=%d\n\n", getpid(), getppid(), getpgrp());

        close(read_file_descriptor);

        write(write_file_descriptor, argv[1], strlen(argv[1]));
        close(write_file_descriptor);

        waitpid(pid, &wait_status, 0);

        exit(EXIT_SUCCESS);
    } else {
        printf("Выполнение дочернего процесса\n");
        printf("Параметры дочернего pid=%d ppid=%d grp=%d\n\n", getpid(), getppid(), getpgrp());

        close(write_file_descriptor);

        char buffer;
        while (read(read_file_descriptor, &buffer, 1) > 0) {
            write(STDOUT_FILENO, &buffer, 1);
        }

        write(STDOUT_FILENO, "\n", 1);
        close(read_file_descriptor);

        exit(EXIT_SUCCESS);
    }
}
