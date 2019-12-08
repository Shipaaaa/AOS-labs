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

int main(int argc, char **argv) {
    pid_t pid;
    int status, numBytes, filedes;
    char buf, msg[255];
    strcpy(msg, "");

    int pipe_file_descriptors[2];

    if (pipe(pipe_file_descriptors) < 0) {
        checkPerror("pipe");
        exit(EXIT_FAILURE);
    }

    int read_file_descriptor = pipe_file_descriptors[0];
    int write_file_descriptor = pipe_file_descriptors[1];

    pid = fork();
    if (pid < 0) {
        checkPerror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        printf("\nДочерний: Выполнение дочернего процесса");
        printf("\nДочерний: Параметры дочернего pid=%d ppid=%d grp=%d\n", getpid(), getppid(), getpgrp());

        printf("\npipefd1[0] = %d\npipefd1[1] = %d\n", read_file_descriptor, write_file_descriptor);
        close(read_file_descriptor); 

        /* Делаем dup, чтобы занести открытый дескриптор pipe на запись в таблицу открытых дескрипторов процесса в позицию fildes */
        /* После exec сможем обращаться к дескриптору fildes и писать в него */
        filedes = dup(write_file_descriptor);
        if (filedes < 0) {
            checkPerror("dup");
            exit(EXIT_FAILURE);
        }

        printf("\nДочерний: filedes = %d", filedes);

        close(write_file_descriptor);

        char filedesStr[10] = "";
        sprintf(filedesStr, "%d", filedes);
        if (execl("lab_4_4_child", "lab_4_4_child", filedesStr, NULL) < 0) {
            checkPerror("exec");
            exit(EXIT_FAILURE);
        }
    } else {
        printf("\nРодительский: Выполнение родительского процесса");
        printf("\nРодительский: Параметры родительского pid=%d ppid=%d grp=%d\n", getpid(), getppid(), getpgrp());
        close(write_file_descriptor);

        //Работает, в отличие от нижнего блока
        //Если read возвращает 0 - значит, что канал был закрыт, EOF
        while ((numBytes = read(read_file_descriptor, &buf, 1)) != 0) {
            //Если открыли с помощью pipe2(pipe_file_descriptors, O_NONBLOCK), так можем отловить ошибку, когда в канале нет данных (EAGAIN)
            if (numBytes < 0) {
                checkPerror("read");
                break;
            }
            write(STDOUT_FILENO, &buf, 1);
        }

        close(read_file_descriptor);

        printf("\nРодительский: ожидаем\n");

        //Ожидаем завершения дочернего. Должен завершиться от SIGPIPE
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
