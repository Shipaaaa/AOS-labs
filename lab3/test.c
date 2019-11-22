#include <errno.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

unsigned do_work() {
    unsigned num = 1;
    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < (1 << 24); j++) {
            num *= (num + 1);
            printf("Iter %d complete\n", i);
        }
        printf("Iter %d complete\n", i);
    }
    return num;
}

int main() {

    pid_t pid = fork();
    if (-1 == pid) {
        perror("fork");
        exit(1);
    }
    if (0 == pid) {
        alarm(1);
        do_work();
        exit(5);
    } else {
        int retcode = -1;
        pid_t childpid = wait(&retcode);
        printf("Forked: child %d; Finished: child %d with %d dumped %d signal %d \n", pid, childpid, WEXITSTATUS(retcode), WCOREDUMP(retcode), WTERMSIG(retcode));
    }

    return 0;
}