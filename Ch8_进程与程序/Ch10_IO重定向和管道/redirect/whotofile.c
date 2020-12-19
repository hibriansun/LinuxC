/**
 * show hoe to redirect output for another program
 * 
 * 利用fork到exec这段时间来重新给子进程分配文件描述符实现另外程序的文件重定向
 * 
 * idea: fork, then in the child, redirect output, then exec
 * 
*/
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main()
{
    pid_t pid;
    pid = fork();

    switch (pid)
    {
        case 0:
            close(1);
            int fd = creat("userlist", 0644);       // 创建并打开文件，为文件分配最小的文件描述符(1)
            execlp("who", "who", NULL);

            perror("execlp");
            exit(1);
        
        default:
            wait(NULL);
            printf("Done running who. result in userlist\n");
    }
}