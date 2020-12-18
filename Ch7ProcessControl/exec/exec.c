/**
 * http://akaedu.github.io/book/ch30s03.html#id2866732
 * 
 * #include <unistd.h>
 * int execl(const char *path, const char *arg, ...);
 * int execlp(const char *file, const char *arg, ...);
 * int execle(const char *path, const char *arg, ..., char *const envp[]);
 * int execv(const char *path, char *const argv[]);
 * int execvp(const char *file, char *const argv[]);
 * int execve(const char *path, char *const argv[], char *const envp[]);
 * 
 * exec函数组关系：http://akaedu.github.io/book/images/process.exec.png
 * 
 */

// exec函数族调用举例：
// char *const ps_argv[] ={"ps", "-o", "pid,ppid,pgrp,session,tpgid,comm", NULL};
// char *const ps_envp[] ={"PATH=/bin:/usr/bin", "TERM=console", NULL};
// execl("/bin/ps", "ps", "-o", "pid,ppid,pgrp,session,tpgid,comm", NULL);
// execv("/bin/ps", ps_argv);
// execle("/bin/ps", "ps", "-o", "pid,ppid,pgrp,session,tpgid,comm", NULL, ps_envp);
// execve("/bin/ps", ps_argv, ps_envp);
// execlp("ps", "ps", "-o", "pid,ppid,pgrp,session,tpgid,comm", NULL);
// execvp("ps", ps_argv);

/*
这些函数如果调用成功则加载新的程序从启动代码开始执行，不再返回，如果调用出错则返回-1，所以exec函数只有出错的返回值而没有成功的返回值。

这些函数原型看起来很容易混，但只要掌握了规律就很好记。不带字母p（表示path）的exec函数第一个参数必须是程序的相对路径或绝对路径，例如"/bin/ls"或"./a.out"，而不能是"ls"或"a.out"。对于带字母p的函数：

如果参数中包含/，则将其视为路径名。

否则视为不带路径的程序名，在PATH环境变量的目录列表中搜索这个程序。

=============================================================================================
带有字母l（表示list）的exec函数要求将新程序的每个命令行参数都当作一个参数传给它，命令行参数的个数是可变的，因此函数原型中有...，
...中的最后一个可变参数应该是NULL，起sentinel的作用。对于带有字母v（表示vector）的函数，则应该先构造一个指向各参数的指针数组，然后将该数组的首地址当作参数传给它，数组中的最后一个指针也应该是NULL，就像main函数的argv参数或者环境变量表一样。

对于以e（表示environment）结尾的exec函数，可以把一份新的环境变量表传给它，其他exec函数仍使用当前的环境变量表执行新程序。

v -- 参数存到一个数组中，传数组

p -- 有p表示第一个参数不是path是filename，filename里有'/'相当于路径名，没有函数则去环境变量里面找可执行文件，
无p表示第一个参数为path

*/
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
    pid_t pid = fork();

    switch (pid)
    {
    case -1:
        perror("Create process unsuccessfully\n");
        break;
    
    case 0:{
            printf("Child process is running, child pid: %u, parent pid: %u\n", getpid(), getppid());
            printf("uid: %u, gid: %u\n", getuid(), getgid());
            execv("./processimage", argv);
            printf("********The new process image has replaced old process image permanently. So this sentence will never be printed!********8\n");
            exit(0);
        }
    default:
        printf("Parent process is running.\n");
          break;
    }

    int status;
    wait(&status);
    exit(0);
}

/*
    Parent process is running.
    Child process is running, child pid: 22255, parent pid: 22254
    uid: 1000, gid: 1000
    I'm a process image from processimage
    My pid: 22255, parentid: 22254
    Uid: 1000, gid: 1000

    证明一点：
    旧进程内存映像会被替换，但是旧进程的pid进程号等不会被替换，换里不换表
*/