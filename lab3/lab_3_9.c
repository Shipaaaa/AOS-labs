//
// Created by Vladislav Shipugin on 02.11.19.
//

/*
9. Включить в порожденный процесс вместо системного вызова kill 
системный вызов alarm с перехватом и без перехвата сигнала. Что изменится?

В лабораторной работе 3 в задании 9 ошибка. (узнал у Вавренюка)
В родительском процессе kill должен быть, а alarm() надо включать в порожденный. 
*/

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/errno.h>
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

int N = 20;

void signal_handler(int signal_id) {
    printf("Запустили обработчик, signal_id: %d\n", signal_id);
    signal(SIGINT, signal_handler);

    N = 1000;
}

int main(int argc, char *argv[], char *argp[]) {
    // signal(SIGINT, signal_handler);

    int pid = fork();
    int return_code = 0;

    if (pid == 0) {
        alarm(1);
        printf("Child: PID: %d\n", getpid());
        printf("Child: PPID: %d\n", getppid());
        printf("Child: PGID: %d\n", getpgid(pid));

        int i, j;
        for (i = 0; i < 1000; i++)
            for (j = 0; j < 100; j++)
                printf("Внешняя итерация: %d, внутренняя итерация: %d\n", i, j);

        printf("Child: Дичь какая-то PGID: %d\n", getpgid(pid));
        exit(6);
    } else {
        printf("Parent: PID: %d\n", getpid());
        printf("Parent: PPID: %d\n", getppid());
        printf("Parent: PGID: %d\n", getpgid(pid));
        printf("Parent: переходим в состояние wait\n");

        // kill(pid, SIGINT);
        wait(&return_code);

        printf("Parent: результат завершение wait: WEXITSTATUS:%d, WCOREDUMP:%d, WTERMSIG:%d\n", WEXITSTATUS(return_code), WCOREDUMP(return_code), WTERMSIG(return_code));
        printf("Parent: Завершаем.\n");

        exit(1);
    }
    return 0;
}
