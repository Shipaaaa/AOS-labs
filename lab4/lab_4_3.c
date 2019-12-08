//
// Created by Vladislav Shipugin on 23.11.19.
//

/*
3. Модифицировать программу п. 2 таким образом, чтобы процессы 
реализовали двунаправленную связь (диалог).
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
    int child_pid, pid, ppid, pgid, pid_wait, status, num_of_bytes;

    int first_pipe_file_descriptors[2];
    int second_pipe_file_descriptors[2];

    char first_buffer[30] = "abcdef999";
    char second_buffer[30] = "qwerty";

    char tmp;
    char tmpbuff[30] = "";

    if (pipe(first_pipe_file_descriptors) == -1 || pipe(second_pipe_file_descriptors) == -1) {
        checkPerror("pipe");
    }

    int first_read_file_descriptor = first_pipe_file_descriptors[0];
    int first_write_file_descriptor = first_pipe_file_descriptors[1];

    int second_read_file_descriptor = second_pipe_file_descriptors[0];
    int second_write_file_descriptor = second_pipe_file_descriptors[1];

    if ((child_pid = fork()) > 0) {
        printf("ParentPid:%d\n", pid = getpid());
        printf("ParentPpid:%d\n", ppid = getppid());
        printf("ParentGpid:%d\n", pgid = getpgrp());

        close(first_read_file_descriptor);
        close(second_write_file_descriptor);

        //printf("Type message for child process: ");
        read(0, &tmpbuff, 30);

        write(first_write_file_descriptor, &tmpbuff, strlen(tmpbuff));
        close(first_write_file_descriptor);

        printf("Reading from the pipe in the Parent process:");
        while ((num_of_bytes = read(second_read_file_descriptor, &tmp, 1)) > 0) {
            putchar(tmp);
        }
        printf("\n");

        close(second_read_file_descriptor);
        exit(0);
    } else {
        printf("ChildPid:%d\n", pid = getpid());
        printf("ChildPpid:%d\n", ppid = getppid());
        printf("ChildGpid:%d\n", pgid = getpgrp());

        close(first_write_file_descriptor);
        close(second_read_file_descriptor);

        printf("Reading from the pipe in the Child process:");
        while ((num_of_bytes = read(first_read_file_descriptor, &tmp, 1)) > 0) {
            putchar(tmp);
        }
        printf("\n");
        close(first_read_file_descriptor);

        //чтение из стандартного потока и запись в канал
        //printf("Type message for parent process: ");
        read(0, &tmpbuff, 30);
        write(second_write_file_descriptor, &tmpbuff, strlen(tmpbuff));

        close(second_write_file_descriptor);
        exit(0);
    }
    return 0;
}