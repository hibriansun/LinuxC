/**
 * 如果某个子进程的父进程先于子进程结束，这个子进程被称为孤儿进程(orphan)，子进程将被init进程收养，init进程将成为这个进程的父进程
 * 注意观察子进程打印的ParentPID的变化
*/

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
     pid_t pid;

     pid = fork();
     switch (pid)
     {
     case 0:
        while(1){
            printf("Child Process is running, PID:%u, ParentPID:%u\n", getpid(), getppid());
            sleep(3);
        }
         break;
     case -1:
        perror("Process creation failed.\n");
        break;
     default:
        printf("Parent process is running, PID:%u\n", getpid());
        exit(0);
     }
}