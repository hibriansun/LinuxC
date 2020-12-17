#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    pid_t mypid, ret_from_fork;

    mypid = getpid();
    printf("Before:my pid is %d\n", mypid);

    ret_from_fork = fork();

    sleep(1);

    printf("After: my pid is %d, fork() said %d\n", getpid(), ret_from_fork);    
}