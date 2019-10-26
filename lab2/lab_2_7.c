//
// Created by Vladislav Shipugin on 25.10.19.
//

/*
7. Модифицировать программу, 
включив в нее setpgrp в порожденный процесс до того, как он будет выполнять pause.

Повлияет ли на порожденный процесс нажатие клавиши прерывания в тот момент, 
когда родительский процесс "спит"?

Будет ли показан порожденный процесс в выводе команды ps?
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/errno.h>
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
    int pid = fork();

    if (pid == -1) {
        checkPerror("Ошибка выполнения fork");
        exit(0);
    }

    if (pid == 0) {
        printf("Child: fork returned = %d\n", pid);
        printf("Child: PID = %d\n", getpid());
        printf("Child: PPID = %d\n", getppid());
        printf("Child: PGID = %d\n", getpgid(pid));
        printf("Child: Process group = %d\n", getpgrp());
        
        setpgrp();
        
        printf("Child: new fork returned = %d\n", pid);
        printf("Child: new PID = %d\n", getpid());
        printf("Child: new PPID = %d\n", getppid());
        printf("Child: new PGID = %d\n", getpgid(pid));
        printf("Child: new Process group = %d\n", getpgrp());

        printf("Child: Останавниваем процесс\n");
        pause();
        printf("Child: Выходим.");
    } else {
        printf("Parent: fork returned = %d\n", pid);
        printf("Parent: PID = %d\n", getpid());
        printf("Parent: PPID = %d\n", getppid());
        printf("Parent: PGID = %d\n", getpgid(pid));
        printf("Parent: Process group = %d\n", getpgrp());
        
        printf("Parent: Останавниваем процесс\n");
        pause();
        printf("Child: Выходим.");
    }

    return 0;
}
