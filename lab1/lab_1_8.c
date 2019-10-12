//
// Created by Vladislav Shipugin on 12.10.19.
//

/*
8. Написать программу копирования одного файла в другой. 
Имена файлов задаются в командной строке 
(первый аргумент - исходный файл, второй - результирующий). 
Если аргументы командной строки отсутствуют, 
использовать стандартный ввод и стандартный вывод. 
Копирование файлов выполнить с помощью функции.
*/

#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/errno.h>
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

void change_printf_to_error() {
    printf("\033[0;31m");
}

void change_printf_to_log() {
    printf("\033[0m");
}

// TODO Добавить while
int copyFile(int fileDescIn, int fileDescOut) {
    char source[10000] = "";
    int readByte = 0;
    readByte = read(fileDescIn, source, 10000);
    write(fileDescOut, source, readByte);

    return 0;
}

int main(const int argc, const char *argv[]) {
    errno = 0;

    int file_descriptor_for_copy;
    int file_descriptor_result;

    if (argc < 3) {
        file_descriptor_for_copy = 0;
        file_descriptor_result = 1;
    } else {
        const char *file_name_for_copy = argv[1];
        const char *file_name_result = argv[2];

        file_descriptor_for_copy = open(file_name_for_copy, O_RDONLY);
        if (errno != 0) {
            perror("Возникла ошибка при открытии файла.");
            return -1;
        }

        file_descriptor_result = open(file_name_result, O_WRONLY| O_CREAT, 0400);
        if (errno != 0) {
            perror("Возникла ошибка при открытии файла.");
            return -1;
        }
    }

    copyFile(file_descriptor_for_copy, file_descriptor_result);

    close(file_descriptor_for_copy);
    if (errno != 0) perror("Возникла ошибка при закрытии файла 1");

    close(file_descriptor_result);
    if (errno != 0) perror("Возникла ошибка при закрытии файла 2");

    return 0;
}
