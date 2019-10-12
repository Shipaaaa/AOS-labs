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

#define FILE_PATH "my_first_file.txt"

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
    const int permission_mode = atoi(argv[2]);

    printf("Открывается файл %s, с правами доступа %d\n", file_name, permission_mode);

    int file_descriptor = open(file_name, O_WRONLY | O_CREAT);
    printf("%d\n", errno);
    perror("Открыли 1 раз");

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

    for (int i = 0; i < 3; i++) {
        int ssize_t = write(file_descriptor, test_string, strlen(test_string));
        printf("%d символов записано\n", ssize_t);
        printf("%d\n", errno);
        perror("Записали строку");
    }

    int close_result = close(file_descriptor);
    printf("Файла был закрыт с кодом %d\n", close_result);
    printf("%d\n", errno);
    perror("Закрыли 1 раз");

    if (chmod(file_name, permission_mode) < 0) perror("Ошибка выполнения chmod:");

    printf("\n");

    file_descriptor = open(file_name, O_RDONLY);
    printf("%d\n", errno);
    perror("Открыли 2 раз");

    if (file_descriptor == -1) {
        change_printf_to_error();
        printf("Не удаётся открыть файл.\n");
        change_printf_to_log();
        return 0;
    }

    char read_buffer[file_len];
    read(file_descriptor, read_buffer, file_len);
    printf("%s\n", read_buffer);
    printf("%d\n", errno);
    perror("Читаем файл");

    close(file_descriptor);
    printf("%d\n", errno);
    perror("Закрыли 2 раз");

    file_descriptor = open(file_name, O_RDWR);
    printf("%d\n", file_descriptor);
    perror("Открыли 3 раз");
    close(file_descriptor);
    printf("%d\n", errno);
    perror("Закрыли 4 раз");

    return 0;
}
