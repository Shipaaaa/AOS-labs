//
// Created by Vladislav Shipugin on 19.10.19.
//

/*
3. Написать программу, в которой процесс порождает новый 
и позволяет порожденному процессу завершиться. 
Оба процесса должны выводить свои идентификаторы 
(процесса, родительского процесса, группы процессов).

Проверить идентификатор и статус завершения порожденного процесса.

Выполнить данную программу и посмотреть ее результаты.
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/errno.h>
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
    int pid = fork();

    if (pid == -1) {
        checkPerror("Ошибка выполнения fork");
        exit(0);
    }

    if (pid == 0) {
        printf("Child: fork returned = %d\n", pid);
        printf("Child: PID = %d\n", getpid());
        printf("Child: PPID = %d\n", getppid());
        printf("Child: PGID = %d\n", getpgid(pid));
        printf("Child: Process group = %d\n", getpgrp());
    } else {
        printf("Parent: fork returned = %d\n", pid);
        printf("Parent: PID = %d\n", getpid());
        printf("Parent: PPID = %d\n", getppid());
        printf("Parent: PGID = %d\n", getpgid(pid));
        printf("Parent: Process group = %d\n", getpgrp());
        printf("Parent: Ожидаем завершения дочернего процесса\n");
        
        int code;
        if (wait(&code) == -1) {
            checkPerror("Parent: wait error");
        } else {
            printf("Parent: Дочерний процесс завершён! Статус завершения: %d\n", code);
        }
    }

    return 0;
}
