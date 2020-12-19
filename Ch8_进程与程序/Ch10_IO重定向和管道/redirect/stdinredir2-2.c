/**
 * 可以想象当有一通电话打来，此时你在楼上使用座机接听，突然你需要下楼在楼下书房里查找资料，此时你让楼下的人打开分机，
 * 你在楼上挂断，但是此时通话并未结束，楼下依然有一台分机保持通话，前往楼下可使用楼下分机继续刚刚的通话
 * 
 * 这就是文件重定向的第二种方案：
 * 使用dup建立指向已经存在的文件描述符的第二个连接
 * 
 * redirect METHOD2: 
 * fd = open[new] -- close[STDIN_FILENO] -- dup[new fd]<redirect to STDIN_FILENO> -- close[new fd]
 * 
*/


/*
#include<unistd.h>

int dup（int fd）；

int dup2（int fd1，int fd2）;   // 让fd2指向fd1指向的文件

两个均为复制一个现存的文件的描述

两个函数的返回：若成功为新的文件描述，若出错为-1；



当调用dup函数时，内核在进程中创建一个新的文件描述符，此描述符是当前可用文件描述符的最小数值，这个文件描述符指向fd所拥有的文件表项



dup2则可以用fd2参数指定新的描述符数值。fd1 = oldfd, fd2 = newfd

如果fd2已经打开，则先关闭。

若fd1=fd2，则dup2返回fd2，而不关闭它。

dup2函数返回的新文件描述符同样与参数fd1共享同一文件表项。

通常使用这两个系统调用来重定向一个打开的文件描述符。
*/

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{

    // Test Part - read from standard input then print on standard output
    char line[100];
    fgets(line, 100, stdin);    printf("%s\n", line);
    fgets(line, 100, stdin);    printf("%s\n", line);
    fgets(line, 100, stdin);    printf("%s\n", line);


    /* redirect input */
    // new fd
    int fd = open("/etc/passwd", O_RDONLY);

    int newfd;
    /*******************************************************************/
    #ifdef CLOSE_DUP
        close(0);
        newfd = dup(fd);        // copy open fd to 0
    #else
        newfd = dup2(fd, 0);    // close 0, dup fd to 0     // 较为简单些，二合一了
    #endif

    /*******************************************************************/

    // close fd
    close(fd);

    // read from "/etc/passwd" then print on standard output
    fgets(line, 100, stdin);    printf("%s\n", line);
    fgets(line, 100, stdin);    printf("%s\n", line);
    fgets(line, 100, stdin);    printf("%s\n", line);
}