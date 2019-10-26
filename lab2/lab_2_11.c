//
// Created by Vladislav Shipugin on 25.10.19.
//

/*
11. Выполнить из программы на Си какую-либо команду Shell (cp или ls): 
с помощью вызовов fork-exec, с помощью библиотечной функции system.

Необходимые для команды данные передать через аргументы командной строки.
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
    if (argc < 4) {
        showError("Не переданы необходиые параметры: имя файла, новое имя файла, флаг для ls\n");
        return -1;
    }

    if (fork() != 0) {
        printf("Parent START\n");
        wait(NULL);
        printf("Parent STOP\n");
    } else {
        printf("Child START\n");
        // execl("/bin/cp", "cp", argv[1], argv[2], NULL);
        
        execl("/bin/ls", "ls", argv[3], NULL);

        printf("Child STOP\n");
    }
    
    return 0;
}
