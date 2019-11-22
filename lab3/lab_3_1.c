//
// Created by Vladislav Shipugin on 01.11.19.
//

/*
1. Изменить реакцию процесса на некоторый сигнал при помощи signal. 

В функции обработчике переустановить реакцию на сигнал на значение по умолчанию. 
Запустить программу и объяснить наблюдаемый результат.
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/errno.h>
#include <signal.h>
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
    signal(SIGINT, SIG_DFL);
}

int main(int argc, char *argv[], char *argp[]) {
    signal(SIGINT, signal_handler);
    while (1) {
        pause();
    }

    return 0;
}
