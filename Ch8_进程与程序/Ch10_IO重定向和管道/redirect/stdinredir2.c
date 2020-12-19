/**
 * 可以想象当有一通电话打来，此时你在楼上使用座机接听，突然你需要下楼在楼下书房里查找资料，此时你让楼下的人打开分机，
 * 你在楼上挂断，但是此时通话并未结束，楼下依然有一台分机保持通话，前往楼下可使用楼下分机继续刚刚的通话
 * 
 * 这就是文件重定向的第二种方案：
 * 使用dup建立指向已经存在的文件描述符的第二个连接
 * 
 * fd = open[newfile] -- close[STDIN_FILENO] -- dup[fd]<redirect fd(代表的通话) to STDIN_FILENO(楼下座机)> -- close[fd](挂断楼上座机)
 * 
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

    // close
    close(STDIN_FILENO);

    // redirect [dup]
    dup(fd);        // STDIN_FILENO(选用此刻最低文件描述符) -> fd所指文件

    // close fd
    close(fd);

    // read from "/etc/passwd" then print on standard output
    fgets(line, 100, stdin);    printf("%s\n", line);
    fgets(line, 100, stdin);    printf("%s\n", line);
    fgets(line, 100, stdin);    printf("%s\n", line);
}