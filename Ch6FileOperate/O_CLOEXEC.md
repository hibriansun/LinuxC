考虑一种情况：

父进程监听一个端口后，fork出一个子进程，然后kill掉父进程，再重启父进程，这个时候提示端口占用，用netstat查看，子进程占用了父进程监听的端口。

 原理其实很简单，子进程在fork出来的时候，使用了[写时复制（COW，Copy-On-Write）方式](https://www.cnblogs.com/biyeymyhjob/archive/2012/07/20/2601655.html)获得父进程的数据空间、 堆和栈副本，这其中也包括文件描述符。刚刚fork成功时，父子进程中相同的文件描述符指向系统文件表中的同一项（这也意味着他们共享同一文件偏移量）。这其中当然也包含父进程创建的socket。

 接着，一般我们会调用exec执行另一个程序，此时会用全新的程序替换子进程的正文，数据，堆和栈等。此时保存文件描述符的变量当然也不存在了，我们就无法关闭无用的文件描述符了。所以通常我们会fork子进程后在子进程中直接执行close关掉无用的文件描述符，然后再执行exec。

 但是在复杂系统中，有时我们fork子进程时已经不知道打开了多少个文件描述符（包括socket句柄等），这此时进行逐一清理确实有很大难度。我们期望的是能在fork子进程前打开某个文件句柄时就指定好：“这个句柄我在fork子进程后执行exec时就关闭”。其实时有这样的方法的：即所谓 的 close-on-exec。



```cpp
#ifdef WIN32

	SOCKET ss = ::socket(PF_INET, SOCK_STREAM, 0);

#else

	SOCKET ss = ::socket(PF_INET, SOCK_STREAM | SOCK_CLOEXEC, 0);

#endif
```

 当然，其他的文件描述符也有类似的功能，例如文件，可以在打开的时候使用O_CLOEXEC标识（linux 2.6.23才开始支持此标记），达到和上面一样的效果。或者使用系统的fcntl函数设置FD_CLOEXEC即可。

```cpp
//方案A
int fd = open(“foo.txt”,O_RDONLY);

int flags = fcntl(fd, F_GETFD);

flags |= FD_CLOEXEC;

fcntl(fd, F_SETFD, flags);

//方案B，linux 2.6.23后支持

int fd = open(“foo.txt”,O_RDONLY | O_CLOEXEC);
```

