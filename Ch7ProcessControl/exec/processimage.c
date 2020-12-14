/*
  本程序为exec.c文件所使用来进行内存映像替换
*/

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char** argv)
{
    printf("I'm a process image from processimage\n");
    printf("My pid: %u, parentid: %u\n", getpid(), getppid());
    printf("Uid: %u, gid: %u\n", getuid(), getgid());


    printf("Args from main method paramter list:\n");
    for(int i = 0; i < argc; i++){
        printf("argv[%d]:%s\n", i, argv[i]);
    }
}