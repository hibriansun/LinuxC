#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// server 向writefifo写 client从writefifo读
// server 从readfifo读 client向readfifo写
#define READFIFO "readfifo"
#define WRITEFIFO "writefifo"

int main()
{
    umask(0);
    // 写入管道
    if(mkfifo(WRITEFIFO, S_IFIFO|0666)){
        perror("mkfifo");
        exit(1);
    }

    int wfd;
    umask(0);
    // 只读方式打开写入管道
    if((wfd = open(WRITEFIFO, O_WRONLY)) == -1){
        perror("open");
        exit(1);
    }

    int rfd;
    // 打开读入管道 -- 阻塞等待读入管道可被打开
    while((rfd = open(READFIFO, O_RDONLY)) == -1){
        printf("Waiting\n");
        sleep(1);
    }

    char buf[BUFSIZ];

    while(1){
        printf("Msg send to client:");
        // 向client写入>>"向writefifo写"
        fgets(buf, BUFSIZ, stdin);      // fgets会读入换行再加'\0'
        buf[strlen(buf)-1] = '\0';
        if(strcmp(buf, "quit") == 0){
            close(wfd);
            unlink(WRITEFIFO);      // 删除一个文件
            close(rfd);
            exit(0);
        }
        write(wfd, buf, strlen(buf));

        // 从client的"readfifo"读入
        read(rfd, buf, BUFSIZ);
        if(strlen(buf) != 0){
            printf("Msg from Client:%s\n", buf);
        }
    }
}