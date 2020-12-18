/**
 * prompting shell version2 -- 带提示符的shell
 * fork() -- 
 * return -1: fork failed, 
 * return 0 -- fork child process successful, value return by child process invoke fork function, 
 * return others -- return child pid to parent process
 * 解决问题：命令只能输入执行一次
 * 需要改进：
 * 按下Ctrl+D / 输入exit退出
 * 在一行输入下所有参数
 * 严重的问题：当psh2执行一个程序，ctrl+c退出这个程序时，连带的psh2 shell也被退出  ----   由于键盘信号发送给所有连接进程
 * ---> psh3.c
 */

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_ARGVS 20
#define MAX_ARGLEN 100

char* mallocString(char* buf);

void excute(char* argv[]);

int main()
{
    char* argvs[MAX_ARGVS+1];       // 参数指针数组中，最后一个必须为NULL作为标识参数结束的标志
    char argv_buf[MAX_ARGLEN];
    int index_args = 0;

    while(index_args < MAX_ARGVS){         // Endless loop
        printf("Arg[%d]:", index_args);    // prompting part
        if(fgets(argv_buf, MAX_ARGLEN, stdin) && *argv_buf != '\n'){
            // store args
            argvs[index_args++] = mallocString(argv_buf);      
        }else{
            // excute command by args
            if(index_args > 0){     // prevent excute only "\n" in string command
                argvs[index_args] = NULL;       // replace "\n" with NULL to mark the end of args   
                index_args = 0;
                excute(argvs);
                index_args = 0;
            }
        }
    }
}


// 创建子进程来执行另一个程序
void excute(char* argv[]){
    pid_t pid = fork();
    int exit_status;

    switch (pid)
    {
    case 0:
        execvp(argv[0], argv);
        perror("fork failed");
        exit(1);
    
    case -1:
        perror("fork failed");
        exit(1);

    default:
        while( wait(&exit_status) != pid);
        printf("child process exited with status %d, %d\n", exit_status>>8, exit_status&0377);
    }
}

char* mallocString(char* buf){
    buf[strlen(buf) - 1] = '\0';    // replace '\n' at end of string with '\0'
    char* rtn = (char*)malloc(strlen(buf) + 1);
    
    if(rtn == NULL){
        fprintf(stderr, "No memory to store args\n");
    }

    strcpy(rtn, buf);

    return rtn;
}