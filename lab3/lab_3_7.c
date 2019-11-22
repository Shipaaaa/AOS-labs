//
// Created by Vladislav Shipugin on 02.11.19.
//

/*
7. Повторить выполнение предыдущих пунктов задания, 
используя в порожденном процессе вместо вложенных циклов системный вызов pause. 

Что изменится? Как завершить процесс в случае выполнения с pause п. 4?
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

int main(int argc, char *argv[], char *argp[]) {
    int pid = fork();
    int return_code;

    if (pid == 0) {
        printf("Child: PID: %d\n", getpid());
        printf("Child: PPID: %d\n", getppid());
        printf("Child: PGID: %d\n", getpgid(pid));

        pause();

        exit(5);
    } else {
        printf("Parent: PID: %d\n", getpid());
        printf("Parent: PPID: %d\n", getppid());
        printf("Parent: PGID: %d\n", getpgid(pid));
        printf("Parent: переходим в состояние wait\n");

        sleep(3);
        kill(pid, SIGINT);
        wait(&return_code);

        printf("Parent: результат завершение wait: %d\n", WEXITSTATUS(return_code));
        printf("Parent: Завершаем.\n");

        exit(1);
    }
    return 0;
}
