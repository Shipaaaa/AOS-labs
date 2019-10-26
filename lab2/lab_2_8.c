//
// Created by Vladislav Shipugin on 25.10.19.
//

/*
8. Открыть файл (достаточно большого размера), доступный для чтения. 
Имя файла передается из командной строки. 
После открытия файла создать параллельный процесс с помощью только fork. 
В обоих процессах создать свои файлы для записи, 
читать информацию из общего файла и копировать ее 
в собственные выходные файлы (не на экран).

Вывести на экран содержимое полученных файлов 
из порожденного процесса по окончании записи в файл и из родительского процесса, 
дождавшись завершения порожденного процесса.

Посмотреть, что изменится, если читаемую процессами информацию сразу выводить на экран.
*/

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define CHILD_FILE_PATH "ChildFile.txt"
#define PARENT_FILE_PATH "ParentFile.txt"

void change_printf_to_error() {
    printf("\033[0;31m");
}

void change_printf_to_log() {
    printf("\033[0m");
}

void showMessage(char *message) {
    printf("%s", message);
}

void showError(char *errorMessage) {
    change_printf_to_error();
    printf("%s", errorMessage);
    change_printf_to_log();
}

void checkPerror(char *errorMessage) {
    if (errno != 0) perror(errorMessage);
}

int main(int argc, char *argv[], char *argp[]) {
    errno = 0;

    const char *file_name = argv[1];

    int input_file_descriptor = open(file_name, O_RDONLY);

    if (input_file_descriptor < 0) {
        checkPerror("Ошибка открытия файла");
        return -1;
    }

    int file_size;

    file_size = lseek(input_file_descriptor, 0, SEEK_END);
    lseek(input_file_descriptor, 0, SEEK_SET);

    char input_buffer[file_size], parent_buffer[file_size], child_buffer[file_size];

    if (fork() == 0) {
        read(input_file_descriptor, input_buffer, file_size);
        printf("Прочитали файл\n\n");

        printf("Пишем в дочернем процессе \n");
        int сhild_write_file_descriptor = open(CHILD_FILE_PATH, O_RDWR | O_TRUNC | O_CREAT, 0666);
        write(сhild_write_file_descriptor, input_buffer, file_size);
        close(сhild_write_file_descriptor);

        printf("Вывод данных из порожденного процесса\n");

        сhild_write_file_descriptor = open(CHILD_FILE_PATH, O_RDWR);
        int child_read_size = read(сhild_write_file_descriptor, child_buffer, file_size);
        printf("Child: %d bytes\n\n%s\n\n", child_read_size, child_buffer);

        close(сhild_write_file_descriptor);

        exit(0);

    } else {
        read(input_file_descriptor, input_buffer, file_size);
        printf("Прочитали файл\n\n");
        
        printf("Пишем в родительском процессе \n");
        int parent_file_descriptor = open(PARENT_FILE_PATH, O_RDWR | O_TRUNC | O_CREAT, 0666);
        write(parent_file_descriptor, input_buffer, file_size);
        close(parent_file_descriptor);

        printf("Ожидание вывода данных из порожденного процесса\n");
        wait(NULL);

        parent_file_descriptor = open(PARENT_FILE_PATH, O_RDWR);
        int parent_read_size = read(parent_file_descriptor, parent_buffer, file_size);
        printf("Parent: %d bytes\n\n%s\n\n", parent_read_size, parent_buffer);

        close(parent_file_descriptor);
    }

    close(input_file_descriptor);

    return 0;
}
