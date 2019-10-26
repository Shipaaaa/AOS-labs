//
// Created by Vladislav Shipugin on 25.10.19.
//

/*
10. Создать (с помощью связки fork - exec) параллельный процесс. 

Имя исполняемого файла для exec передается с помощью аргумента командной строки. 
Передать в порожденный процесс некоторую информацию через список параметров 
(список аргументов в функции main). 

Каждый процесс (и родительский, и порожденный) должен вывести на экран 
список переданных ему аргументов и свое окружение.
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
    if (argc < 2) {
        showError("Необходимо ввести 4 парамметра - 1 выполняемый файл\n");
        return -1;
    }

    if (fork() != 0) {
        printf("Parent\n");
        wait(NULL);

        int i;
        for (i = 0; i < argc; i++) printf("Родительский аргумент %d: %s\n", i, argv[i]);

        // showError("Вывод родительского окружения с argp:\n");

        for (i = 0; *(argp + i); i++) {
            // printf("%s\n", *(argp + i));
        }

    } else {
        printf("Child\n");

        int i;
        for (i = 0; i < argc; i++) printf("Дочерний аргумент %d: %s\n", i, argv[i]);

        // showError("Вывод дочернего окружения с argp:\n");

        for (i = 0; *(argp + i); i++) {
            // printf("%s\n", *(argp + i));
        }

        execl(argv[1], argv[2], NULL);
        showError("ОЧЕНЬ ВАЖНЫЙ ТЕКСТ\n");

        exit(0);
    }
    return 0;
}
