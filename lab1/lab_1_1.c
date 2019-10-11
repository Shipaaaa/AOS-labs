//
// Created by Vladislav Shipugin on 05.10.19.
//

/*
1. Написать программу, которая выполняет некоторый системный вызов, 
например open, и затем выводит возвращаемую ошибку. 

Использовать в программе разные способы вывода сообщения об ошибке: 
    вывод значения errno, 
    вывод соответствующего сообщения из системного массива сообщений об ошибках sys_errlist[], 
    вызов библиотечной функции perror.
*/

#include <stdio.h>
#include <sys/errno.h>
#include <sys/fcntl.h>
#include <unistd.h>

#define FILE_PATH "my_first_file.txt"

void change_printf_to_error() {
    printf("\033[0;31m");
}

void change_printf_to_log() {
    printf("\033[0m");
}

int main() {
    errno = 0;

    int file_descriptor = open(FILE_PATH, O_RDONLY);

    if (errno == ENOENT) {
        change_printf_to_error();
        printf("errno_code: %d\n", errno);
        perror("perror");
        printf("sys_errlist: %s\n", sys_errlist[errno]);
        change_printf_to_log();
    }

    if (file_descriptor) {
        printf("file_descriptor exist\n");
        close(file_descriptor);
    }

    if (errno == EBADF) {
        change_printf_to_error();
        printf("errno_code: %d\n", errno);
        perror("perror");
        printf("sys_errlist: %s\n", sys_errlist[errno]);
        change_printf_to_log();
    }
    return 0;
}
