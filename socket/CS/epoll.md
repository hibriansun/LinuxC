## 创建一个epoll instance

```c
#include <sys/epoll.h>

// Method1
int epoll_create(int size);
// Method2
int epoll_create1(int flags);
```

#### `int epoll_create(int size);`

**参数 size**：旧内核创建epoll instance所需文件描述符数量，值必须大于0，新内核不需要这个参数，动态分配，但为了与旧内核做到兼容，仍需填入一个大于0的数值。

**返回值**：epoll instance的文件描述符

当不需要该epoll instance时，使用close(efd)，当所有指向某个epoll instance的我文件描述符被关闭后，内核将自动销毁实例并使相关资源可再利用。

#### `int epoll_create1(int flags) `

​       If flags is 0, then, other than(除了) the fact that the obsolete(过时的) size argument is dropped(被删除), epoll_create1() is the **same**  as  **epoll_create()**.
​       The  following  value  can be included in flags to obtain different
​       behavior:

   `EPOLL_CLOEXEC`
          Set the close-on-exec (FD_CLOEXEC) flag on the new file  de‐
          scriptor.   See  the  description  of  the O_CLOEXEC flag in
          open(2) for reasons why this may be useful.

> 考虑一种情况：
>
> 父进程监听一个端口后，fork出一个子进程，然后kill掉父进程，再重启父进程，这个时候提示端口占用，用netstat查看，子进程占用了父进程监听的端口。
>
>  原理其实很简单，子进程在fork出来的时候，使用了[写时复制（COW，Copy-On-Write）方式](https://www.cnblogs.com/biyeymyhjob/archive/2012/07/20/2601655.html)获得父进程的数据空间、 堆和栈副本，这其中也包括文件描述符。刚刚fork成功时，父子进程中相同的文件描述符指向系统文件表中的同一项（这也意味着他们共享同一文件偏移量）。这其中当然也包含父进程创建的socket。
>
>  接着，一般我们会调用exec执行另一个程序，此时会用全新的程序替换子进程的正文，数据，堆和栈等。此时保存文件描述符的变量当然也不存在了，我们就无法关闭无用的文件描述符了。所以通常我们会fork子进程后在子进程中直接执行close关掉无用的文件描述符，然后再执行exec。
>
>  但是在复杂系统中，有时我们fork子进程时已经不知道打开了多少个文件描述符（包括socket句柄等），这此时进行逐一清理确实有很大难度。我们期望的是能在fork子进程前打开某个文件句柄时就指定好：“这个句柄我在fork子进程后执行exec时就关闭”。其实时有这样的方法的：即所谓 的 close-on-exec。
>
> 
>
> ```cpp
> #ifdef WIN32
> 
> 	SOCKET ss = ::socket(PF_INET, SOCK_STREAM, 0);
> 
> #else
> 
> 	SOCKET ss = ::socket(PF_INET, SOCK_STREAM | SOCK_CLOEXEC, 0);
> 
> #endif
> ```
>
>  当然，其他的文件描述符也有类似的功能，例如文件，可以在打开的时候使用O_CLOEXEC标识（linux 2.6.23才开始支持此标记），达到和上面一样的效果。或者使用系统的fcntl函数设置FD_CLOEXEC即可。
>
> ```cpp
> //方案A
> int fd = open(“foo.txt”,O_RDONLY);
> 
> int flags = fcntl(fd, F_GETFD);
> 
> flags |= FD_CLOEXEC;
> 
> fcntl(fd, F_SETFD, flags);
> 
> //方案B，linux 2.6.23后支持
> 
> int fd = open(“foo.txt”,O_RDONLY | O_CLOEXEC);
> ```
>





## 控制[某个epoll instance监控的文件描述符上的]事件：注册、修改、删除

#### `int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);`

```c
#include <sys/epoll.h>

int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);

```

**参数：**

`int epfd`：epoll_create生成epoll instance的文件描述符

`op`：		表示动作，用3个宏来表示：
	`EPOLL_CTL_ADD` (注册新的fd到epfd)，       ----      给epfd指向的红黑树树根对应的红黑树添加节点fd
	`EPOLL_CTL_MOD` (修改已经注册的fd的监听事件)，
	`EPOLL_CTL_DEL` (从epfd删除一个fd)；

`event`：	告诉内核需要监听的事件（创建好填好后传入地址）

```c
struct epoll_event {
    __uint32_t events; /* Epoll events */
    epoll_data_t data; /* User data variable */
};
typedef union epoll_data {
    void *ptr;		// 可以设置函数指针，形成回调函数
    int fd;			// 与函数参数中的fd应一致，epoll实例检测到某个文件有事件发生，直接返回是哪个事件，就是通过返回这个文件描述符
    uint32_t u32;
    uint64_t u64;
} epoll_data_t;

// __uint32_t events设置
EPOLLIN ：	表示对应的文件描述符可以读（包括对端SOCKET正常关闭）
EPOLLOUT：	表示对应的文件描述符可以写
EPOLLPRI：	表示对应的文件描述符有紧急的数据可读（这里应该表示有带外数据到来）
EPOLLERR：	表示对应的文件描述符发生错误
EPOLLHUP：	表示对应的文件描述符被挂断；
EPOLLET： 	将EPOLL设为边缘触发(Edge Triggered)模式，这是相对于水平触发(Level Triggered)而言的
EPOLLONESHOT：只监听一次事件，当监听完这次事件之后，如果还需要继续监听这个socket的话，需要再次把这个socket加入到EPOLL队列里
```

**返回值：**

成功0，失败-1



## 等待所监控文件描述符上有事件的产生，类似于select()调用

#### `int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout)`

```c
#include <sys/epoll.h>

int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout)
```

参数

`events`：		是个数组，用作传出参数，用来存内核得到事件的集合，
`maxevents`：	告之内核这个events有多大，这个maxevents的值不能大于创建epoll_create()时的size，
`timeout`：	超时时间(毫秒)
-1：	无限长时间阻塞

0：	立即返回，非阻塞

\>0：	指定毫秒（将要被阻塞的时间）



**返回值：**

成功返回有多少文件描述符I/O就绪，时间到时返回0，出错返回-1