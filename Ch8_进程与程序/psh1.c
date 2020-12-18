/**
 * prompting shell version1 -- 带提示符的shell
 * Prompts for the command and its arguments
 * Builds the argument vector for the call to execvp
 * Uses execvp(), and never returns
 * psh1.c的问题 -- 只能输入执行一次指令 --> 解决方案psh2.c 建立一个新进程来启动程序
 */

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

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

void excute(char* argv[]){
    execvp(argv[0], argv);

    perror("execvp failed");
    exit(1);
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