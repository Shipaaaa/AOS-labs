//
// Created by Vladislav Shipugin on 02.11.19.
//

/*
3. Написать программу, исключающую появление зомби для завершающихся порожденных процессов.
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

void signal_handler(int signal_id, struct __siginfo *sig_info, void *context) {
    printf("Запустили обработчик, signal_id: %d\n", signal_id);
    printf("si_pid: %d, si_status %d\n", sig_info->si_pid, sig_info->si_status);
    printf("Выходим из обработчика\n");
}

int main(int argc, char *argv[], char *argp[]) {
    errno = 0;

    struct sigaction action, old_action;

    action.sa_sigaction = signal_handler;
    action.sa_flags = SA_RESETHAND | SA_NODEFER;

    sigaction(SIGUSR1, &action, &old_action);
    checkPerror("sigaction error");

    int pid = fork();
    int child_pid;

    if (pid == 0) {
        printf("Child: Это сообщение выведено из дочернего процесса\n");
        printf("Child: fork returned = %d\n", pid);
        printf("Child: Process group = %d\n", getpgrp());
        printf("Child: PID = %d\n", child_pid = getpid());
        printf("Child: PPID = %d\n", getppid());
        pause();
        printf("Child: Завершает работу\n");

        return 1;

    } else {
        printf("Parent: Это сообщение выведено из родительского процесса\n");
        printf("Parent: fork returned = %d\n", pid);
        printf("Parent: Process group = %d\n", getpgrp());
        printf("Parent: PID = %d\n", getpid());
        printf("Parent: PPID = %d\n", getppid());
        printf("Parent: Ожидаем завершения дочернего процесса\n");
        sleep(2);
        kill(child_pid, SIGUSR1);
        pause();
        printf("Parent: Завершает работу\n");

        return 1;
    }

    return 0;
}
