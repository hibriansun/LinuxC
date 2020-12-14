/**
 * 创建一个新子进程后，父进程和子进程会抢夺CPU，抢到CPU者执行，另一个挂起等待
 * 一般来说fork一个子进程后，子进程先执行还是父进程先执行是不确定的，取决于系统内核调度算法，一般来讲父子进程会**交替执行**
 * 因为操作系统一般让所有进程都享有同等执行权，除非某进程优先级比其他高
*/

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    pid_t pid;
    char* msg;
    int cnt;

    pid = fork();
    switch (pid)
    {
    case 0:
        cnt = 3;
        msg = "Child process is running.";
        break;

    case -1:
        perror("Process creation failed.\n");
        break;
    default:
        cnt = 5;
        msg = "Parent process is running.";
        break;
    }

    while(cnt){
        puts(msg);
        sleep(1);       // 1s
        cnt--;
    }
}