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
#define READFIFO "writefifo"
#define WRITEFIFO "readfifo"

int main()
{
    umask(0);
    // 建立管道client向中写入，server从中读取
    if(mkfifo(WRITEFIFO, S_IFIFO|0666)){
        perror("mkfifo");
        exit(1);
    }


    umask(0);
    int rfd;
    // client从writefifo中读取
    while((rfd = open(READFIFO, O_RDONLY)) == -1){
        printf("Waiting\n");
        sleep(1);
    }

    int wfd;
    // 只读方式打开要写的文件
    umask(0);
    if((wfd = open(WRITEFIFO, O_WRONLY)) == -1){
        perror("open");
        exit(1);
    }


    char buf[BUFSIZ];
    while(1){
        // 从管道中读取信息
        int len = read(rfd, buf, BUFSIZ);
        if(len != 0){

            printf("Msg from server:%s\n", buf);
        }
        
        // send msg to WRITEFIFO
        printf("Msg send to server:");
        fgets(buf, BUFSIZ, stdin);
        buf[strlen(buf)-1] = '\0';
        if(strcmp(buf, "quit") == 0){
            close(wfd);
            unlink(WRITEFIFO);      // 删除一个文件
            close(rfd);
            exit(0);
        }
        write(wfd, buf, strlen(buf));
    }
}