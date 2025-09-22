#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int attributes_print(){
    printf("ATTRIBUTES OF THE CURRENT PROCESS\n\n");

    printf("PID: %d\n", getpid());
    printf("PPID: %d\n", getppid());
    printf("PGID: %d\n", getpgrp());
    printf("UID: %d\n", getuid());
    printf("GID: %d\n", getgid());
    printf("EUID: %d\n", geteuid());
    printf("EGID: %d\n", getegid());

    return 0;
}

int main() {
    if (attributes_print()) {return 1;};
    return 0;
}