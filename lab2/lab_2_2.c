//
// Created by Vladislav Shipugin on 19.10.19.
//

/*
2. Кто выполняется первым после fork: отец или сын? 
Написать программу, которую можно использовать для проверки данной ситуации.
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>

int main(int argc, char *argv[], char *argp[]) {
    int pid = fork();

    if (pid == -1) perror("[fork]");

    if (pid == 0) {
        printf("Дочерний процесс\n");
    } else {
        printf("Родительский процесс\n");
    }
    return 0;
}
