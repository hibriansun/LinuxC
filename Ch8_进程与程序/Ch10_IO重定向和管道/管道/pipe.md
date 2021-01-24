### 管道

一种文件，因此可以使用IO函数对管道读写

* 特点1：数据单向流动
* 特点2：仅能用于父子进程/兄弟进程间的通信



使用管道可以来连接一个进程的输出和另一个进程的输入



管道的工作原理：管道是内核中的一个单项的数据通道。管道有一个读取端和写入端。



管道有缺陷：效率不高，是一个队列，导致缓冲大小受限，如果出现多个进程读取数据必然会出现数据读取不完整(Unix/Linux编程实践教程P321)

[![rtSwwj.md.png](https://s3.ax1x.com/2020/12/18/rtSwwj.md.png)](https://imgchr.com/i/rtSwwj)

**1. 创建管道**

`#include <unistd.h>`

`result = pipe(int array[2]);`

`array[0]:读取端文件描述符  array[1]:写入端文件描述符`

`return 0->success   -1 -> error`

[![rtSqXD.png](https://s3.ax1x.com/2020/12/18/rtSqXD.png)](https://imgchr.com/i/rtSqXD)

**2. 使用管道**

* **Weird Demo -- 同一进程间的管道**

  ```c
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
      int aPipe[2];
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
  ```

* **使用pipe & fork在两个进程间共享管道**

  [![rNxwuD.md.png](https://s3.ax1x.com/2020/12/19/rNxwuD.md.png)](https://imgchr.com/i/rNxwuD)

```c
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

int main()
{
    int aPipe[2];
    char buf[BUFSIZ];

    if(pipe(aPipe) == -1){
        perror("Pipe creation failed.");
        exit(0);
    }

    // parent process read data from stdin, then write data into aPipe[1], after that, child process read data from aPipe[0] and print data on screen
    while(fgets(buf, BUFSIZ, stdin)){
        int len = strlen(buf);
        if(write(aPipe[1], buf, len) != len){
            perror("write into pipe");
            break;
        }
        printf("Write data into pipe by parent process (pid: %d) sucessfully.\n", getpid());

        pid_t pid = fork();

        switch (pid)
        {
        case 0:
            if((len = read(aPipe[0], buf, BUFSIZ)) == -1){
                perror("read from pipe");
                break;
            }

            printf("Read data from pipe by child process (pid: %d) successfully.\n", getpid());
            
            if(write(STDOUT_FILENO, buf, len) != len){
                perror("writing to stdout");
                break;
            }
            printf("Write data into stdout by child process (pid: %d) successfully.\n\n", getpid());
            
            break;
        
        case -1:
            perror("fork");
            exit(1);

        default:
            wait(NULL);
            break;
        }
    }
}
```

* **使用pipe、fork、exec**

>\#include<unistd.h>
>
>int dup（int fd）；
>
>int dup2（int fd1，int fd2）;    // 让fd2指向fd1指向的文件
>
>两个均为复制一个现存的文件的描述
>
>两个函数的返回：若成功为新的文件描述，若出错为-1；
>
>
>
>当调用dup函数时，内核在进程中创建一个新的文件描述符，此描述符是当前可用文件描述符的最小数值，这个文件描述符指向fd所拥有的文件表项
>
>
>
>dup2则可以用fd2参数指定新的描述符数值。fd1 = oldfd, fd2 = newfd
>
>1. 如果fd2已经打开，则先关闭。
>
>   若fd1=fd2，则dup2返回fd2，而不关闭它。
>
>2. dup2函数返回的新文件描述符f2同样与参数fd1共享同一文件表项。(f1, f2都指向同一文件项)                       
>
>   
>
>   通常使用这两个系统调用来重定向一个打开的文件描述符。
>
>   close(0);
>
>   newfd = dup(fd);
>
>   // 上面两句合二为一
>
>   newfd = dup2(fd, 0);	// 先尝试关闭0，再将fd所指文件关联到0上

[![rUA0zj.png](https://s3.ax1x.com/2020/12/19/rUA0zj.png)](https://imgchr.com/i/rUA0zj)](https://imgchr.com/i/rUAMJe)

[![rUAMJe.jpg](https://s3.ax1x.com/2020/12/19/rUAMJe.jpg)](https://imgchr.com/i/rUAMJe)

```c
/**
 * 两程序间共享管道
 * pipe fork exec
 * 实现类似这样的用法
 * who | sort
 * ==> pipedemo3 who sort
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char** argv)
{
    if(argc != 3){
        fprintf(stderr, "Usage: pipedemo3 cmd1 cmd2\n");
        exit(1);
    }

    int aPipe[2];
    
    if(pipe(aPipe) == -1){
        perror("pipe");
        exit(1);
    }

    pid_t pid = fork();

    switch (pid)
    {
    case 0:
        // 断掉子进程指向管道输出的文件描述符
        if(close(aPipe[0]) == -1){     // 子进程先执行替换成who，who输出信息，因此只需要将who的输出通到管道的输入就好了
            perror("close");
            exit(1);
        }

        // 重定向(STDOUT_FILENO指向从stdout到管道入口)
        dup2(aPipe[1], STDOUT_FILENO);
        
        // 父进程的aPipe两个文件描述符完成使命，使STDOUT_FILENO重定向完成
        if(close(aPipe[1]) == -1){
            perror("close");
            exit(1);
        }

        execlp(argv[1], argv[1], NULL);

    default:
        wait(NULL);
        close(aPipe[1]);
        dup2(aPipe[0], STDIN_FILENO);
        close(aPipe[0]);
        execlp(argv[2], argv[2], NULL);
    }
}
```

### 需要在对管道读写时关闭不必要的管道文件描述符

关闭未使用的管道文件描述符

- 关闭未使用的管道文件描述符可以确保进程不会耗尽其文件描述符限制，即节省文件描述符开销。（这不是最主要的原因）
- 因为从管道读取数据的进程会关闭其持有的管道写入描述符，所以当其他进程完成输出并关闭管道写入描述符后，读端进程就能看到文件结束，若是读取端进程没有关闭写入端，则在其他写入端进程关闭写入端后，**读取端也不会看到文件结束，即使其已经读取完管道中的数据**。此外读取端的read()函数会一直**阻塞**以等待数据到来，这是**因为内核还知道至少存在一个管道的写入描述符还打开着**，即读取进程自己打开的写入文件描述符，理论上讲这个进程仍然可以向管道写入数据，即使其已经被读取操作阻塞了。因为在真实的环境下，read()有可能会被一个向管道写入数据的信号处理器中断。
- 而写入进程关闭其读取文件描述符的原因则与读取进程不同。当一个进程试图向一个没有打开着的读取文件描述符发管道写入数据时，内核会向写入进程发送一个SIGPIPE信号。默认情况下，这个信号会杀死一个进程。但是进程可以捕获或忽略这信号，这样会导致管道的write（）操作因EPIPE错误而失败。（已损坏的管道）。此外如果写入进程没有关闭读取端，那么即使在其他进程已经关闭了管道的读取端之后，写入端进程仍然能够向管道写入数据，最后数据将充满整个管道，**后续的写入请求会被永远阻塞**。



### 有名管道

与匿名管道特性基本一致，除了有名管道可以在没有亲缘关系的进程间通信

创建有名管道

```c
#include <sys/types.h>
#include <sys/stat.h>

int mkfifo(const char *pathname, mode_t mode);


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int mknod(const char *pathname, mode_t mode, dev_t dev);


// mode: 文件类型|0XXX(权限类型)
```

#### 全双工有名管道应用

由于管道的单项流动性，我们需要建立两个管道来实现双向数据流通

```c
// server.c
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
```

```c
// client.c
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
```

