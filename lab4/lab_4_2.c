//
// Created by Vladislav Shipugin on 23.11.19.
//

/*
2. Модифицировать программу п. 1 таким образом,
чтобы родительский процесс читал из канала в цикле, пока не встретится конец файла 
(порожденный процесс может записывать в канал информацию,
читаемую им из стандартного входного потока).

Как избежать бесконечного ожидания родительским процессом чтения из пустого канала?
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
    int wait_status, numBytes, choice;
    int pipe_file_descriptors[2];
    char msg[255];

    strcpy(msg, "1234");

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

    if (pid != 0) {
        printf("\nРодительский: Выполнение родительского процесса");
        printf("\nРодительский: Параметры родительского pid=%d ppid=%d grp=%d\n", getpid(), getppid(), getpgrp());

        close(write_file_descriptor);
        sleep(4);

        //Работает, в отличие от нижнего блока
        //Работает, в отличие от нижнего блока
        //Если read возвращает 0 - значит, что канал был закрыт, EOF
        char write_buffer;
        while ((numBytes = read(read_file_descriptor, &write_buffer, 1)) != 0) {
            if (numBytes < 0) {
                checkPerror("Error of reading");
                break;
                //exit(EXIT_FAILURE);
            }
            write(STDOUT_FILENO, &write_buffer, 1);
        }

        close(read_file_descriptor);

        printf("\nРодительский: ожидаем\n");

        //Ожидаем завершения дочернего. Должен завершиться от SIGPIPE
        waitpid(pid, &wait_status, 0);

        if (WIFEXITED(wait_status))
            printf("\nДочерний процесс завершен успешно. st=%d pid=%d\n", WEXITSTATUS(wait_status), pid);  //Ожидаем изменения статуса конкретного процесса
        else if (WIFSIGNALED(wait_status))
            printf("\nДочерний был убит сигналом. Номер сигнала=%d pid=%d\n", WTERMSIG(wait_status), pid);
        else if (WIFSTOPPED(wait_status))
            printf("\nДочерний был остановлен сигналом. Номер сигнала=%d pid=%d\n", WSTOPSIG(wait_status), pid);
        else
            printf("\nДочерний процесс завершен неуспешно. pid=%d\n", pid);

        exit(EXIT_SUCCESS);
    } else {
        printf("\nДочерний: Выполнение дочернего процесса");
        printf("\nДочерний: Параметры дочернего pid=%d ppid=%d grp=%d\n", getpid(), getppid(), getpgrp());

        close(read_file_descriptor);

        char write_buffer;
        //Бесконечно считываем
        //! Если мы открываем с O_NONBLOCK, и родительский завершается, закрывает канал на чтение, то при записи в этот канал получим SIGPIPE
        while (read(0, &write_buffer, 1) > 0) {
            write(write_file_descriptor, &write_buffer, 1);
        }

        close(write_file_descriptor);

        exit(EXIT_SUCCESS);
    }
}