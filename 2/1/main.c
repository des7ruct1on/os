#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    // id of current process
    pid_t cur_pid = getpid();
    printf("current process id: %d\n", cur_pid);
    // id of procces that summoned current procces
    pid_t cur_ppid = getppid();
    printf("current parent process id: %d\n", cur_ppid);
    // id of current group id
    gid_t group_id = getpgrp();
    printf("group procces id: %d\n", group_id);
    // real id of current user
    uid_t uid = getuid();
    printf("real user id: %d\n", uid);
    // real id of current group of users
    gid_t ugroup_id = getgid();
    printf("real group id: %d\n", ugroup_id);
    // effective id of current user
    uid_t euid = geteuid();
    // effective id of current group of users
    gid_t egid = getegid();
    printf("effective user id: %d\n", euid);
    printf("effective group id: %d\n", egid);
    return 0;
}