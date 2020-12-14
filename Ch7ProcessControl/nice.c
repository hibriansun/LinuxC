/**
 * 调整进程优先级：
 * #include <unistd.h>
 * int nice(int increment);         // man 2 nice
 * 
       adds  increment  to the nice value for the **calling thread**.  (A higher
       nice value means a low priority.)

       The range of the nice value is +19 (low priority) to -20  (high  prior‐
       ity).   Attempts  to  set a nice value outside the range are clamped to
       the range.

       Traditionally, only a privileged process could  lower  the  nice  value
       (i.e., set a higher priority).  However, since Linux 2.6.12, an unpriv‐
       ileged process can decrease the nice value of a target process that has
       a suitable RLIMIT_NICE soft limit; see getrlimit(2) for details.


 * 
*/


// 本程序演示修改子进程优先级

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <sys/wait.h>

int main()
{
    
    pid_t pid = fork();
    switch (pid)
    {
    case 0:
        printf("Child process is running, Cur pid: %u, Parent pid: %u\n", pid, getppid());

        int oldPriority = getpriority(PRIO_PROCESS, 0);
        printf("Old Priority: %u\n", oldPriority);

        int newPriority = nice(5);

        // newPriority = nice(-2);      // sudo 执行nice(negative)是正常的，非sudo异常，具体看man 2 nice
        printf("New Priority: %u\n", newPriority);

        exit(0);
        
    case -1:
        perror("Process creation failed\n");
        break;

    default:
        printf("Parent process is running, Child process pid: %u, Parent process pid: %u\n", pid, getpid());
        break;
    }

    int status;
    // 父进程用来wait子进程结束
    wait(&status);

    exit(0);
}