//
// Created by Vladislav Shipugin on 02.11.19.
//

/*
10. Написать программу, иллюстрирующую способ блокирования 
дополнительных сигналов на время работы обработчика сигнала. 

Что произойдет, если во время обработки некоторого сигнала 
в процесс поступит несколько однотипных заблокированных сигналов.
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

void signal_handler(int signal_id) {
    printf("Запустили обработчик, signal_id: %d\n", signal_id);
    if (signal_id == SIGUSR1) sleep(30);
    printf("\n");
}

int main(int argc, char *argv[], char *argp[]) {
    struct sigaction action, old_action;
    sigset_t set, old_set;

    action.sa_handler = signal_handler;

    sigaction(SIGUSR1, &action, &old_action);
    sigaction(SIGUSR2, &action, &old_action);
    sigaction(SIGALRM, &action, &old_action);
    sigaction(SIGCONT, &action, &old_action);
    sigaction(SIGINT, &action, &old_action);
    sigfillset(&set);
    sigdelset(&set, SIGUSR1);
    sigprocmask(SIG_SETMASK, &set, &old_set);

    kill(getpid(), SIGUSR1);
    sigprocmask(SIG_SETMASK, &old_set, 0);

    return 0;
}
