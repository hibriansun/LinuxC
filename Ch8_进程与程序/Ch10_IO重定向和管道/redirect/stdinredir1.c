/**
 * purpose:show how to redirect standard input by replacing file
 *  descriptor 0 with a connecting to a file
 * 
 * action:reads three lines from standard input, then closed fd0, opens a disk
 *  file, then reads in three more lines from standard input
 * 
*/

// redirect file descriptor -> METHOD1: "CLOSE - THEN - OPEN" strategy

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    int fd;
    char line[100];

    // read from standard input then print on standard output
    fgets(line, 100, stdin);    printf("%s\n", line);
    fgets(line, 100, stdin);    printf("%s\n", line);
    fgets(line, 100, stdin);    printf("%s\n", line);

    // CLOSE
    close(STDIN_FILENO);        // 关闭0号文件描述符对应的文件<standard input>，此时0号文件描述符空闲起来了可以被使用

    // OPEN
    fd = open("/etc/passwd", O_RDONLY);     // 占用文件描述符STDIN_FILENO(0号)

    // Illustrate
    fgets(line, 100, stdin);    printf("%s\n", line);
    fgets(line, 100, stdin);    printf("%s\n", line);
    fgets(line, 100, stdin);    printf("%s\n", line);
}