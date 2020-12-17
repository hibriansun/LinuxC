/**
 * wait用于处理fork出子进程后子进程执行结束后占着进程号和一定资源不放成为僵尸进程的情况
 * wait：父进程（调用的进程）暂停，等待他的一个子进程结束为止，再恢复父进程
 * 可以尝试注释掉wait语句看和有wait语句有什么区别
 * 
 * 子进程的exit就像发送一个信号给父进程以唤醒他
*/


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

#define DELAY 2

void child_code(int delay);

void parent_code(int childpid);

int main()
{
    pid_t newpid;

    printf("Before: mypid is %d\n", getpid());

    if((newpid = fork()) == -1)
        perror("fork");
    else if (newpid == 0)
        child_code(DELAY);
    else
        parent_code(newpid);
}

void child_code(int delay){
    printf("child %d here. will sleep for %d seconds\n", getpid(), delay);
    sleep(DELAY);
    printf("child done. about to exit.\n");
    exit(17);
} 

void parent_code(int childpid){
    printf("test\n");
    int wait_rv;
    wait_rv = wait(NULL);   // 再wait调用前之前代码该怎么执行就执行，不会等待子进程结束(前提是这次调用父进程先执行，再走到子进程)
    printf("done waiting for %d. Wait returned: %d\n", childpid, wait_rv);
}