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

#define STD_IN 0
#define STD_OUT 1

void change_printf_to_error() {
    printf("\033[0;31m");
}

void change_printf_to_log() {
    printf("\033[0m");
}

int copyFile(int source, int destination) {
    const int buffer_size = 255;
    int read_bytes = 0;

    char read_buffer[buffer_size] = "";

    do {
        read_bytes = read(source, read_buffer, buffer_size);
        write(destination, read_buffer, read_bytes);
    } while (read_bytes == buffer_size);
    
    return 0;
}

int main(const int argc, const char *argv[]) {
    errno = 0;

    int file_descriptor_for_copy;
    int file_descriptor_result;

    if (argc < 3) {
        file_descriptor_for_copy = STD_IN;
        file_descriptor_result = STD_OUT;
    } else {
        const char *file_name_for_copy = argv[1];
        const char *file_name_result = argv[2];

        file_descriptor_for_copy = open(file_name_for_copy, O_RDONLY);
        if (errno != 0) {
            perror("Возникла ошибка при открытии файла.");
            return -1;
        }

        file_descriptor_result = open(file_name_result, O_WRONLY | O_CREAT, 0755);
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
