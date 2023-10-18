#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    int pid;
    pid=fork();
    printf("%d\n", pid);
    return 0;
}