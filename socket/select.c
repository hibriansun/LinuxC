/**
 * Why select?
 * 
 * 在一个CS模型中，Server会面临者多个Client的连接请求：
 * 1. 如果采用非阻塞socket，对所有socket进行轮番查询状态（有数据到达读取数据，没有则下一个）   ----   这种方法虽然简单，但是低效
 * 2. 如果让系统监视socket并阻塞他们，当有socket上发生事件时，系统通知服务器进程哪个socket上发生什么事件，服务器进程再去处理   --  高效许多，避免无事件发生时CPU轮番查询非阻塞socket
 * select函数就可以实现第二种阻塞式监视socket
 * 
 * `man select`
 * int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
 * 
 * select函数监视的时文件状态，socket也是文件，因此也适用
*/

#include <stdio.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

int main()
{
    fd_set readFdSet;
    struct timeval timeout;

    // 初始化 -- 清空文件描述符集合
    FD_ZERO(&readFdSet);
    // 将标准输入文件描述符放入readFdSet集合中
    FD_SET(0, &readFdSet);


    // 设置阻塞时间 -- 这段时间内socket上没有事件发生select则返回0
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;

    int rtn;

    while(FD_ISSET(STDIN_FILENO, &readFdSet)){
        
        rtn = select(1, &readFdSet, NULL, NULL, &timeout);

        if(rtn == -1){
            perror("select");
            exit(1);
        }else if(rtn != 0){
            getchar();
            printf("Data has been recived.\n");
        }else{
            printf("No Data approches.\n");
            exit(0);
        }
        
    }

    return 0;
}