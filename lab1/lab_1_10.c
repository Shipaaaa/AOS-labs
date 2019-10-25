//
// Created by Vladislav Shipugin on 19.10.19.
//

/*
10. Написать программу, которая выводит на экран содержимое файла в обратном порядке: 
сначала выводится последний символ файла, затем предпоследний и так далее до первого. 
Использовать lseek и read.
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

    if (argc < 2) {
        change_printf_to_error();
        printf("Вы не указали файл для вывода в обратном порядке.\n");
        change_printf_to_log();
        return -1;
    }

    const char *file_name = argv[1];
    int file_descriptor = open(file_name, O_RDONLY);

    if (errno != 0) {
        perror("Возникла ошибка при открытии файла.");
        return -1;
    }

    char read_buffer;
    const int step = 1;
    int read_bytes = 0;

    lseek(file_descriptor, 0, SEEK_END);

    do {
        read_bytes = read(file_descriptor, &read_buffer, step);
        write(STD_OUT, &read_buffer, read_bytes);
    } while (lseek(file_descriptor, (long)-2, SEEK_CUR) >= 0);

    printf("\n");

    close(file_descriptor);

    return 0;
}
