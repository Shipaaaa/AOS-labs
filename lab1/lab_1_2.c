//
// Created by Vladislav Shipugin on 12.10.19.
//

/*
2. Создать новый файл с правом только на чтение 
(имя файла и права доступа ввести из командной строки), 
записать в него несколько строк, закрыть. 

Повторно открыть данный файл на чтение, прочитать из него информацию, 
вывести ее на экран, закрыть.

Еще раз открыть этот же файл на чтение и запись, 
проверить результат системного вызова open. 

Объяснить полученные результаты.
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

int main(const int argc, const char *argv[]) {
    errno = 0;

    if (argc < 3) {
        change_printf_to_error();
        printf("Введите имя файла и прова доступа.\n");
        change_printf_to_log();
    }

    const char *file_name = argv[1];
    const int permission_mode = strtol(argv[2], 0, 8);

    printf("Открывается файл %s, с правами доступа %o\n", file_name, permission_mode);

    int file_descriptor = open(file_name, O_WRONLY | O_TRUNC | O_CREAT, permission_mode);

    if (file_descriptor == -1) {
        change_printf_to_error();
        printf("Не удаётся открыть файл.\n");
        change_printf_to_log();
        return 0;
    }

    // TODO Сделать ввод текста с клавиатуры.
    char *test_string = "неск\n";
    int count = 3;

    for (int i = 0; i < count; i++) {
        int ssize_t = write(file_descriptor, test_string, strlen(test_string));
        printf("%d символов записано\n", ssize_t);
    }

    int close_result = close(file_descriptor);
    printf("Файла был закрыт с кодом %d\n", close_result);

    printf("\n");

    file_descriptor = open(file_name, O_RDONLY);

    if (file_descriptor == -1) {
        change_printf_to_error();
        printf("Не удаётся открыть файл.\n");
        change_printf_to_log();
        return 0;
    }

    int file_len = strlen(test_string) * count;
    char read_buffer[file_len];
    read(file_descriptor, read_buffer, file_len);
    printf("%s\n", read_buffer);
    close(file_descriptor);

    file_descriptor = open(file_name, O_RDWR);
    printf("file_descriptor: %d\n", file_descriptor);
    perror("Открытие файла");
    close(file_descriptor);

    return 0;
}
