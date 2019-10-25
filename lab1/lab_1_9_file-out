//
// Created by Vladislav Shipugin on 12.10.19.
//

/*
9. Изменить программу п. 8 таким образом, 
чтобы функция копирования использовала стандартные ввод и вывод, 
а вызывающая программа переназначала стандартные ввод и вывод 
на указанные в аргументах командной строки файлы.
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

    int file_descriptor_for_redirect_std_in;
    int file_descriptor_for_redirect_std_out;

    if (argc < 3) {
        change_printf_to_error();
        printf("Вы не указали файлы для ввода/вывода.\n");
        change_printf_to_log();
        return -1;
    }

    const char *file_name_for_redirect_std_in = argv[1];
    const char *file_name_for_redirect_std_out = argv[2];

    file_descriptor_for_redirect_std_in = open(file_name_for_redirect_std_in, O_RDONLY);
    dup2(file_descriptor_for_redirect_std_in, STD_IN);
    if (errno != 0) {
        perror("Возникла ошибка при открытии файла для переопределения std in.");
        return -1;
    }

    close(STD_OUT);
    file_descriptor_for_redirect_std_out = open(file_name_for_redirect_std_out, O_WRONLY | O_CREAT, 0755);
    // dup2(file_descriptor_for_redirect_std_out, STD_OUT);
    if (errno != 0) {
        perror("Возникла ошибка при открытии файла для переопределения std out.");
        return -1;
    }

    copyFile(STD_IN, STD_OUT);

    close(file_descriptor_for_redirect_std_in);
    if (errno != 0) perror("Возникла ошибка при закрытии файла 1");

    close(file_descriptor_for_redirect_std_out);
    if (errno != 0) perror("Возникла ошибка при закрытии файла 2");

    return 0;
}
