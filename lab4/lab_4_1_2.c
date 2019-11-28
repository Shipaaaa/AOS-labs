//
// Created by Vladislav Shipugin on 23.11.19.
//

/*
1. Создать параллельный процесс, используя только fork. 
Организовать обмен информацией между процессами через каналы. 
Предусмотреть однонаправленную и двунаправленную связь. 
При создании двунаправленной связи исключить возможность появления «смертельных объятий».
*/

#include <fcntl.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

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
    pid_t pid;
    int status, numBytes, lengthStr = 0;
    int first_pipe_file_descriptors[2], second_pipe_file_descriptors[2];
    char buffer, msg[255], string[255];

    if (argc != 2) {
        printf("Usage: %s <string>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    strcpy(string, argv[1]);

    if (pipe(first_pipe_file_descriptors) < 0 || pipe(second_pipe_file_descriptors) < 0) {
        checkPerror("Error of creating channel");
        exit(EXIT_FAILURE);
    }

    int first_read_file_descriptor = first_pipe_file_descriptors[0];
    int first_write_file_descriptor = first_pipe_file_descriptors[1];

    int second_read_file_descriptor = second_pipe_file_descriptors[0];
    int second_write_file_descriptor = second_pipe_file_descriptors[1];

    pid = fork();
    if (pid < 0) {
        checkPerror("Error of forking");
        exit(EXIT_FAILURE);
    }

    if (pid != 0) {
        printf("Родитель: Выполнение родительского процесса\n");
        printf("Родитель: Параметры родительского pid=%d ppid=%d grp=%d\n\n", getpid(), getppid(), getpgrp());

        close(first_read_file_descriptor);
        close(second_write_file_descriptor);

        printf("Родитель: Записываем в first_write_file_descriptor сообщение для дочернего: %s\n\n", string);
        write(first_write_file_descriptor, string, strlen(string));
        
        // Получаем размер сообщения в буфере для чтения
        if ((numBytes = read(second_read_file_descriptor, msg, 10000)) < 0) {
            checkPerror("Error of reading size");
            exit(EXIT_FAILURE);
        } else if (numBytes < 10000) {
            printf("\nРодитель: Сообщение в канале second_read_file_descriptor от дочернего имеет размер: %d", numBytes);
            //Повторно считываем из канала, зная размер сообщения
            if (read(second_read_file_descriptor, msg, numBytes) < 0) {
                checkPerror("Error of reading msg");
                exit(EXIT_FAILURE);
            }
            printf("\nРодитель: Прочитано из канала second_read_file_descriptor сообщение от дочернего: %s\n", msg);
        }

        close(first_write_file_descriptor);
        close(second_read_file_descriptor);

        waitpid(pid, &status, 0);
        
        if (WIFEXITED(status))
            printf("\nРодитель: Дочерний процесс завершен успешно. st=%d pid=%d\n", WEXITSTATUS(status), pid);
        else if (WIFSIGNALED(status))
            printf("\nРодитель: Дочерний был убит сигналом. Номер сигнала=%d pid=%d\n", WTERMSIG(status), pid);
        else if (WIFSTOPPED(status))
            printf("\nРодитель: Дочерний был остановлен сигналом. Номер сигнала=%d pid=%d\n", WSTOPSIG(status), pid);
        else
            printf("\nРодитель: Дочерний процесс завершен неуспешно. pid=%d\n", pid);

        exit(EXIT_SUCCESS);
    } else {
        printf("\nДочерний: Выполнение дочернего процесса");
        printf("\nДочерний: Параметры дочернего pid=%d ppid=%d grp=%d\n", getpid(), getppid(), getpgrp());

        close(first_write_file_descriptor);     
        close(second_read_file_descriptor); 

        /* while (read(first_read_file_descriptor, &buffer, 1) > 0)	//Считываем переданный отцом аргумент
            write(STDOUT_FILENO, &buffer, 1); */

        if (read(first_read_file_descriptor, msg, strlen(string)) < 0) {
            checkPerror("Error of string");
            exit(EXIT_FAILURE);
        }
        printf("\nДочерний: Прочитано из канала first_read_file_descriptor сообщение от родителя: %s", msg);

        strcat(msg, " успешно получено");
        printf("\nДочерний: Записываем в first_write_file_descriptor сообщение родительскому: %s\n", msg);
        write(second_write_file_descriptor, msg, strlen(msg));  //Передаем в канал отцу свое сообщение

        close(first_read_file_descriptor);
        close(second_write_file_descriptor);

        exit(EXIT_SUCCESS);
    }
}