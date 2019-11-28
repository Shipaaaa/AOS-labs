//
// Created by Vladislav Shipugin on 23.11.19.
//

/*
4. Повторить п. 2, создавая параллельные процессы с помощью связки fork - exec. 

Как передать дескрипторы канала в порожденный процесс?
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
    int status, numBytes, file_descriptor;
    char buffer, msg[255];
    strcpy(msg, "");

    int pipe_file_descriptors[2];

    if (pipe(pipe_file_descriptors) < 0) {
        checkPerror("Error of creating channel");
        exit(EXIT_FAILURE);
    }

    int read_file_descriptor = pipe_file_descriptors[0];
    int write_file_descriptor = pipe_file_descriptors[1];

    pid = fork();
    if (pid < 0) {
        checkPerror("Error of forking");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        printf("Дочерний: Выполнение дочернего процесса\n");
        printf("Дочерний: Параметры дочернего pid=%d ppid=%d grp=%d\n", getpid(), getppid(), getpgrp());

        printf("pipefd1[0] = %d\npipefd1[1] = %d\n", read_file_descriptor, write_file_descriptor);

        close(read_file_descriptor);

        /* Делаем dup, чтобы занести открытый дескриптор pipe на запись в таблицу открытых дескрипторов процесса в позицию file_descriptor */
        /* После exec сможем обращаться к дескриптору file_descriptor и писать в него */
        file_descriptor = dup(write_file_descriptor);
        if (file_descriptor < 0) {
            checkPerror("Error of dup");
            exit(EXIT_FAILURE);
        }

        printf("\nДочерний: file_descriptor = %d", file_descriptor);
        close(write_file_descriptor);

        char file_descriptor_string[10] = "";
        sprintf(file_descriptor_string, "%d", file_descriptor);

        if (execl("lab_4_4_child", "lab_4_4_child", file_descriptor_string, NULL) < 0) {
            checkPerror("Error of forking");
            exit(EXIT_FAILURE);
        }
    } else {
        printf("Родитель: Выполнение родительского процесса\n");
        printf("Родитель: Параметры родительского pid=%d ppid=%d grp=%d\n\n", getpid(), getppid(), getpgrp());

        close(write_file_descriptor);

        while ((numBytes = read(read_file_descriptor, &buffer, 1)) != 0) {
            if (numBytes < 0) {
                checkPerror("Error of reading");
                break;
            }
            write(STDOUT_FILENO, &buffer, 1);
        }

        close(read_file_descriptor);
        
        printf("\nРодительский: ожидаем\n");
        waitpid(pid, &status, 0);

        if (WIFEXITED(status))
            printf("\nДочерний процесс завершен успешно. st=%d pid=%d\n", WEXITSTATUS(status), pid);  //Ожидаем изменения статуса конкретного процесса
        else if (WIFSIGNALED(status))
            printf("\nДочерний был убит сигналом. Номер сигнала=%d pid=%d\n", WTERMSIG(status), pid);
        else if (WIFSTOPPED(status))
            printf("\nДочерний был остановлен сигналом. Номер сигнала=%d pid=%d\n", WSTOPSIG(status), pid);
        else
            printf("\nДочерний процесс завершен неуспешно. pid=%d\n", pid);

        exit(EXIT_SUCCESS);
    }
}