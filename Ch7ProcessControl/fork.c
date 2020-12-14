/**
 * 由父进程fork出一个子进程，子进程继承了父进程几乎所有的资源
 * fork函数会有两个返回值，在fork子进程成功的前提下，一个是父进程调用fork函数后的返回值，是刚创建子进程的id
 * 另一个是子进程中fork函数的返回值，为0
 * 
 * 创建子进程失败则只返回-1
 * 
 * 借此，我们可以区分成功创建子进程情况下，用返回值来区分父子进程
 * 
 * 创建一个新子进程后，父进程和子进程会抢夺CPU，抢到CPU者执行，另一个挂起等待
*/


// getpid(): 获得当前进程的pid
// getppid(): 获得父进程的pid

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    pid_t pid;

    pid = fork();
    switch(pid){
        case 0:
            printf("Child process is running, ChildProcess return %u, ParentPid is %u\n", pid, getppid());
            break;

        case -1:
            printf("Create Process fail\n");
            break;

        default:
            printf("Parent process is running, ChildPid is %u, ParentPid(CurrentPid) is %u\n", pid, getpid());
            break;
    }

}