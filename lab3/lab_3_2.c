//
// Created by Vladislav Shipugin on 02.11.19.
//

/*
2. Модифицировать программу п.1 так, чтобы вместо signal использовать sigaction.
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
    sleep(10);
    printf("Выходим\n");
}

int main(int argc, char *argv[], char *argp[]) {
    struct sigaction action, old_action;

    action.sa_handler = signal_handler;
    action.sa_flags = SA_RESETHAND | SA_NODEFER;

    sigaction(SIGINT, &action, &old_action);
    
    while (1) {
        pause();
    }

    return 0;
}
