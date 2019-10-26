//
// Created by Vladislav Shipugin on 19.10.19.
//

/*
1. Написать программу, которая добавляет новые строки в среду процесса.
*/

#include <stdio.h>
#include <stdlib.h>

#define OVERWRITE 1

void change_printf_to_error() {
    printf("\033[0;31m");
}

void change_printf_to_log() {
    printf("\033[0m");
}

void showMessage(char *message) {
    printf(message);
}

void showError(char *errorMessage) {
    change_printf_to_error();
    printf(errorMessage);
    change_printf_to_log();
}

int main(int argc, char *argv[], char *argp[]) {
    if (argc < 3) {
        showError("Вы не ввели название переменной и её значение.");
        return -1;
    }

    int result;

    char *name = argv[1];
    char *value = argv[2];

    char *env_value;

    result = setenv(name, value, OVERWRITE);

    if (result == -1) {
        perror("Ошибка установки env:");
    } else {
        printf("Переменная окружения добавлена! setenv = %d\n", result);

        env_value = getenv(name);
        printf("name: %s value: %s\n\n", name, env_value);

        printf("Вывод окружения с argp:\n");
        int i;
        for (i = 0; *(argp + i); i++) {
            printf("%s\n", *(argp + i));
        }
    }

    return 0;
}
