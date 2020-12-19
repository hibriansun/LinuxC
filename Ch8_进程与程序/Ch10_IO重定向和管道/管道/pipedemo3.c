/**
 * 两程序间共享管道
 * pipe fork exec
 * 实现类似这样的用法
 * who | sort
 * ==> pipedemo3 who sort
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char** argv)
{
    if(argc != 3){
        fprintf(stderr, "Usage: pipedemo3 cmd1 cmd2\n");
        exit(1);
    }

    int aPipe[2];
    
    if(pipe(aPipe) == -1){
        perror("pipe");
        exit(1);
    }

    pid_t pid = fork();

    switch (pid)
    {
    case 0:
        // 断掉子进程指向管道输出的文件描述符
        if(close(aPipe[0]) == -1){     // 子进程先执行替换成who，who输出信息，因此只需要将who的输出通到管道的输入就好了
            perror("close");
            exit(1);
        }

        // 重定向(STDOUT_FILENO指向从stdout到管道入口)
        dup2(aPipe[1], STDOUT_FILENO);
        
        // 父进程的aPipe两个文件描述符完成使命，使STDOUT_FILENO重定向完成
        if(close(aPipe[1]) == -1){
            perror("close");
            exit(1);
        }

        execlp(argv[1], argv[1], NULL);

    default:
        wait(NULL);
        close(aPipe[1]);
        dup2(aPipe[0], STDIN_FILENO);
        close(aPipe[0]);
        execlp(argv[2], argv[2], NULL);
    }
}