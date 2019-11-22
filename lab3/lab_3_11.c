//
// Created by Vladislav Shipugin on 02.11.19.
//

/*
11. Написать программу, позволяющую использовать sigaction 
для реализации примера синхронизации процессов. 

Выполнить эту программу и объяснить ее поведение. 
Использовать sigsuspend и sigprocmask.
*/

#include <fcntl.h>
#include <math.h>
#include <signal.h>
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

void signal_handler(int signal_id) {
    printf("Запустили обработчик, signal_id: %d\n", signal_id);
}

int main(int argc, char *argv[], char *argp[]) {
    struct sigaction new_action, old_action;

    sigset_t only_sigur1_mask;
    sigset_t all_except_sigur1_mask;
    
    memset(&new_action, 0, sizeof(new_action));

    sigemptyset(&only_sigur1_mask);
    sigaddset(&only_sigur1_mask, SIGUSR1);

    sigfillset(&all_except_sigur1_mask);
    sigdelset(&all_except_sigur1_mask, SIGUSR1);

    new_action.sa_mask = only_sigur1_mask;
    new_action.sa_handler = signal_handler;

    sigaction(SIGUSR1, &new_action, &old_action);

    sigprocmask(SIG_BLOCK, &only_sigur1_mask, NULL);

    int i, p, child_pid;
    int pid, ppid, pgid;

    if ((child_pid = fork()) > 0) {
        printf("Parent: PID: %d\n", getpid());
        printf("Parent: PPID: %d\n", getppid());
        printf("Parent: PGID: %d\n", getpgid(pid));

        for (i = 0; i < 10; i++) {
            p = i * sqrt(64);

            printf("Parent res: [i:%d]:%d\n", i, p);
            kill(child_pid, SIGUSR1);
            sigprocmask(SIG_UNBLOCK, &only_sigur1_mask, NULL);

            sigsuspend(&all_except_sigur1_mask);

            sigprocmask(SIG_BLOCK, &only_sigur1_mask, NULL);
        }

        exit(0);
    } else {
        printf("Child: PID: %d\n", pid = getpid());
        printf("Child: PPID: %d\n", ppid = getppid());
        printf("Child: PGID: %d\n", pgid = getpgrp());

        for (i = 0; i < 10; i++) {
            p = i * sqrt(4);

            printf("Child res: [i:%d]:%d\n", i, p);
            kill(ppid, SIGUSR1);
            sigprocmask(SIG_UNBLOCK, &only_sigur1_mask, NULL);

            sigsuspend(&all_except_sigur1_mask);

            sigprocmask(SIG_BLOCK, &only_sigur1_mask, NULL);
        }
        exit(0);
    }
    return 0;
}
