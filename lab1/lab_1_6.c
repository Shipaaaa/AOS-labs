//
// Created by Vladislav Shipugin on 12.10.19.
//

/*
6. Модифицировать программу п. 4 для создания разреженного файла.
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

    printf("Открывается файл %s, с правами доступа %o\n\n", file_name, permission_mode);

    int file_descriptor = open(file_name, O_WRONLY | O_CREAT, permission_mode);
    perror("Открыли файл 1 раз");

    if (file_descriptor == -1) {
        change_printf_to_error();
        printf("Не удаётся открыть файл.\n");
        change_printf_to_log();
        return 0;
    }

    // TODO Сделать ввод текста с клавиатуры.
    char *test_string = "несколько строк\n";
    int count = 3;
    int file_len = strlen(test_string) * count;

    lseek(file_descriptor, 1000, SEEK_SET);
    int ssize_t = write(file_descriptor, test_string, strlen(test_string));
    printf("%d символов записано\n", ssize_t);
    perror("Записали в файл");

    lseek(file_descriptor, 27, SEEK_END);
    ssize_t = write(file_descriptor, test_string, strlen(test_string));
    printf("%d символов записано\n", ssize_t);
    perror("Записали в файл");

    lseek(file_descriptor, 10, SEEK_CUR);
    ssize_t = write(file_descriptor, test_string, strlen(test_string));
    printf("%d символов записано\n", ssize_t);
    perror("Записали в файл");

    int close_result = close(file_descriptor);
    printf("Файла был закрыт с кодом %d\n", close_result);
    perror("Закрыли файл 1 раз");

    printf("\n");

    file_descriptor = open(file_name, O_RDONLY);
    perror("Открыли файл 2 раз");

    if (file_descriptor == -1) {
        change_printf_to_error();
        printf("Не удаётся открыть файл.\n");
        change_printf_to_log();
        return 0;
    }

    char read_buffer[255];
    lseek(file_descriptor, 0, SEEK_SET);
    while (read(file_descriptor, read_buffer, 255) > 0)
        printf("%s\n", read_buffer);
    perror("Прочитали из файла");

    close(file_descriptor);
    perror("Закрыли файл 2 раз");

    file_descriptor = open(file_name, O_RDWR);
    perror("Открыли файл 3 раз");

    printf("file_descriptor: %d\n", file_descriptor);

    close(file_descriptor);
    perror("Закрыли файл 3 раз");

    return 0;
}
