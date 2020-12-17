/**
 * fork()之后是父进程先执行还是子进程先执行是不确定的，这取决于内核的调度算法
*/

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    pid_t fork_rv;
    printf("Before: my pid is %d\n", getpid());

    fork_rv = fork();

    if(fork_rv == -1){
        perror("fork");
    } else if (fork_rv == 0){
        printf("I'm the child. my pid = %d\n", getpid());
    } else {    
        printf("I'm the parent. my pid = %d\n", getpid());
    }
}