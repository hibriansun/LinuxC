/**
 * 像shell一样，使用一个程序调用运行另一个程序
 * exec1.c -- show how easy it is for a program to run a program
*/

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    char* arglist[3];
    arglist[0] = "ls";
    arglist[1] = "-l";
    arglist[2] = NULL;
    printf("*** About to exec ls -l\n");
    execvp("ls", arglist);
    printf("*** ls is done. bye\n");        // 这条消息没打印，去哪儿了？
}