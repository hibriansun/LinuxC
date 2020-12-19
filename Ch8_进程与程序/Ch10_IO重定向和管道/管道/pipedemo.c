/**
 * * Demostrates: how to create and use a pipe
 * * Effect: create a pipe, <process perspective> write into writing end, then runs around and reading from
 *   reading end. A litte weird, but demostrates the idea.
 *  
 *   result = pipe(int array[2]);
 *   array[0]:读取端文件描述符  array[1]:写入端文件描述符
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    int aPipe[2];       // 管道两端文件描述符指针数组
    char buf[BUFSIZ];

    int result;

    // create a pipe
    if(result = pipe(aPipe)){
        perror("Create pipe fail.\n");
        exit(1);
    }

    printf("Create pipe sucessfully! It's file descriptor: {%d, %d}\n", aPipe[0], aPipe[1]);

    // illustrate
    // read from stdin, write into pipe, read from pipe, and print on screen(stdout)
    // https://s3.ax1x.com/2020/12/19/rNXBHs.png this pic shows how data transports
    while(fgets(buf, BUFSIZ, stdin)){
        int len = strlen(buf);
        // write buf into pipe
        if( write(aPipe[1], buf, len) != len){
            perror("write into pipe");
            break;
        }

        // read data from pipe
        len = read(aPipe[0], buf, len);
        if(len == -1){
            perror("reading from pipe");
            break;
        }

        // print on screen (write data into stdout)
        if(write(STDOUT_FILENO, buf, len) != len){
            perror("writing to stdout");
            break;
        }
    }

}