//
// Created by Vladislav Shipugin on 23.11.19.
//

/*
5. С помощью системных вызовов pipe и dup реализовать конвейер: who | wc -l.
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

#define STD_IN 0
#define STD_OUT 1

int main(int argc, char *argv[], char *argp[]) {
    pid_t pid;

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

    if (pid != 0) {
        dup2(write_file_descriptor, STD_OUT);

        close(write_file_descriptor);
        close(read_file_descriptor);

        execlp("who", "who", 0);
    } else {
        dup2(read_file_descriptor, STD_IN);

        close(read_file_descriptor);
        close(write_file_descriptor);

        execl("/usr/bin/wc", "wc", "-l", 0);
    }
}