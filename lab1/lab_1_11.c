//
// Created by Vladislav Shipugin on 19.10.19.
//

/*
11. Написать программу, которая определяет, какой файл из перечисленных в командной строке 
имеет наибольшую длину. Вывести длину файла в байтах.

*/

#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/errno.h>
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#define STD_IN 0
#define STD_OUT 1

void change_printf_to_error() {
    printf("\033[0;31m");
}

void change_printf_to_log() {
    printf("\033[0m");
}

int main(const int argc, const char *argv[]) {
    errno = 0;

    if (argc == 1) {
        change_printf_to_error();
        printf("Вы не указали файлы для сравнения размера.\n");
        change_printf_to_log();
        return -1;
    }

    struct stat stat_buffer;
    int result_with_error = 0;

    int max_size_of_file = -1;
    int index_of_file_with_max_size = 0;

    for (int i = 1; i < argc; i++) {
        result_with_error = stat(argv[i], &stat_buffer);

        if (!result_with_error) {
            if (stat_buffer.st_size > max_size_of_file) {
                index_of_file_with_max_size = i;
                max_size_of_file = stat_buffer.st_size;
            }
        } else {
            printf("Ошибка при открытии файла %s: %s\n", argv[i], strerror(errno));
        }
    }

    printf("Самый длинный файл: %s\nЕго размер %d байт.\n", argv[index_of_file_with_max_size], max_size_of_file);

    return 0;
}
