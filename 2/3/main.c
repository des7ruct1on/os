#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    printf("main process id = %d\n", getpid());
    fork();
    printf("Hi my id = %d, my parent id = %d\n", getpid(), getppid());
    fork();
    printf("Hi my id = %d, my parent id = %d\n", getpid(), getppid());
    return 0;
}