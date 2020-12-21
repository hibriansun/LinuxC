ref: https://www.bilibili.com/video/BV1kt411z7ND

## pthread_create

```c
#include <pthread.h>

// @param 
// thread: point to thread ID
// attr: pointer to thread attributes
// start_routine：函数指针，指向线程创建后要调用的函数
// arg：要传给线程线程函数的参数

int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
					void *(*start_routine) (void *), void *arg);

Compile and link with -pthread.
```



## pthread_join

```c
       int pthread_join(pthread_t thread, void **retval);
       
       Compile and link with -pthread.

DESCRIPTION
       The  pthread_join()  function  waits  for  the  thread specified by
       thread to terminate.  If that thread has already  terminated,  then
       pthread_join() returns immediately.  **The thread specified by thread
       must be joinable.**

       If retval is not NULL, then pthread_join() copies the  exit  status
       of  the  target thread (i.e., the value that the target thread sup‐
       plied to pthread_exit(3)) into the location pointed to  by  retval.
       If  the target thread was canceled, then PTHREAD_CANCELED is placed
       in the location pointed to by retval.

```

一个线程(example1中的th线程)仅允许一个线程(example1中的main线程)使用pthread_join()来等待他(th)的终止，在调用前，被等待的线程(th)应该是处于可join状态，调用后，主动调用另一线程的线程会被挂起等待被调用线程结束

一个可被join线程所占用的内存仅当对其执行了pthread_join()后才会释放



## example1.c

main函数这个线程开始执行，创建了一条子线程myfunc，但myfunc还没来得及执行完，main函数进程就结束了，因此看不到example1.c打印myfunc中的HelloWorld。

```c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void* myfunc(void* args){
        printf("Hello World\n");
        return NULL;
}


int main()
{
        pthread_t th;
        // 函数名也就是指针
        pthread_create(&th, NULL, myfunc, NULL);        // 创建一条子线程th，执行myfunc函数

        return 0;
}

```



解决方法：使用join进行等待，等待所有子线程结束后再结束main线程

```c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void* myfunc(void* args){
        printf("Hello World\n");
        return NULL;
}


int main()
{
        pthread_t th;
        // 函数名也就是指针
        pthread_create(&th, NULL, myfunc, NULL);        // 创建一条子线程th，执行myfunc函数
        pthread_join(th, NULL);
        return 0;
}

```

## example3.c

本例看出两条线程运行速度不一样

```c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void* myfunc(void* args){
        int i;
        for(i = 1; i <= 50; i++){
                printf("%d\n", i);
        }
        return NULL;
}


int main()
{
        pthread_t th1;
        pthread_t th2;

        pthread_create(&th1, NULL, myfunc, NULL);       // 创建一条子线程th，执行myfunc函数
        pthread_create(&th2, NULL, myfunc, NULL);

        pthread_join(th1, NULL);
        pthread_join(th2, NULL);

        return 0;
}

```

## example4.c

为了更加详细的区分不同的数字是谁打印出来的，这里我们给myfunc函数传参

```c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void* myfunc(void* args){
        int i;
        for(i = 1; i <= 50; i++){
                printf("%s: %d\n", (char*)args, i);
        }
        return NULL;
}


int main()
{
        pthread_t th1;
        pthread_t th2;

        // 第四个参数是往myfunc中传参的参数
        pthread_create(&th1, NULL, myfunc, " th1");     // 创建一条子线程th，执行myfunc函数
        pthread_create(&th2, NULL, myfunc, " th2");

        pthread_join(th1, NULL);
        pthread_join(th2, NULL);

        return 0;
}

```

## example5.c

把一个5000个随机数的数组中数字加起来by两条线程，前2500个数字使用th1加起来，后2500个数字用th2加起来，最终在main函数中将两条线程执行结果相加    

```c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
        int first;
        int last;
        int result;
}MY_ARGS;

int arr[5000];

int s1 = 0, s2 = 0;

void* myfunc(void* args){
        int s;
        for(int i = ((MY_ARGS*)args)->first; i < ((MY_ARGS*)args)->last; i++){
                s += arr[i];
        }
        ((MY_ARGS*)args)->result = s;

        return NULL;
}


int main()
{
        for(int i = 0 ; i < 5000; i++){
                arr[i] = rand() % 50;
        }

        /*
        for(int i = 0; i < 5000; i++){
                printf("arr[%d]: %d\n",i ,arr[i]);
        }
        */

        pthread_t th1;
        pthread_t th2;

        MY_ARGS my_args1 = {0, 2500, 0};
        MY_ARGS my_args2 = {2500, 5000, 0};

        // 第四个参数是往myfunc中传参的参数
        pthread_create(&th1, NULL, myfunc, &my_args1);  // 创建一条子线程th，执行myfunc函数
        pthread_create(&th2, NULL, myfunc, &my_args2);

        pthread_join(th1, NULL);
        pthread_join(th2, NULL);


        s1 = my_args1.result;
        s2 = my_args2.result;
        printf("s1: %d\n", s1);
        printf("s2: %d\n", s2);
        printf("s1 + s2: %d\n", s1 + s2);


        return 0;

}

```

## example6.c  --  线程同步问题

当多个线程同时运行时，线程的调度由操作系统决定，程序本身无法决定。因此，任何一个线程都有可能在任何指令处被操作系统暂停，然后在某个时间段后继续执行。

这个时候，有个单线程模型下不存在的问题就来了：如果多个线程同时读写共享变量，会出现数据不一致的问题。

**如果两条线程往同一个全局变量里加？ -- Race Condition** :

**A race condition is an undesirable(不良的) situation that occurs when a device or system attempts to perform two or more operations <u>at the same time</u>**

```c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
        int first;
        int last;
}MY_ARGS;

int arr[5000];
int s = 0;

void* myfunc(void* args){
        for(int i = ((MY_ARGS*)args)->first; i < ((MY_ARGS*)args)->last; i++){
                s += arr[i];
        }

        return NULL;
}


int main()
{
        for(int i = 0 ; i < 5000; i++){
                arr[i] = rand() % 50;
        }

        /*
        for(int i = 0; i < 5000; i++){
                printf("arr[%d]: %d\n",i ,arr[i]);
        }
        */

        pthread_t th1;
        pthread_t th2;

        MY_ARGS my_args1 = {0, 2500};
        MY_ARGS my_args2 = {2500, 5000};

        // 第四个参数是往myfunc中传参的参数
        pthread_create(&th1, NULL, myfunc, &my_args1);  // 创建一条子线程th，执行myfunc函数
        pthread_create(&th2, NULL, myfunc, &my_args2);

        pthread_join(th1, NULL);
        pthread_join(th2, NULL);


        printf("s: %d\n", s);


        return 0;

}

```

结果好像与example5.c一致，但是当线程加和数字量一大呢？



## example7.c  -- race condition

```c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int s = 0;

void* myfunc(void* args){
        int i = 0;
        for(i = 0; i < 1000000; i++){
                s++;
        }
}

int main()
{
        pthread_t th1;
        pthread_t th2;

        pthread_create(&th1, NULL, myfunc, NULL);
        pthread_create(&th2, NULL, myfunc, NULL);

        pthread_join(th1, NULL);
        pthread_join(th2, NULL);

        printf("s理想值应为2000000\n");
        printf("%d\n", s);
}

```

输出：

```c
[briansun@briansun multithreading]$ gcc example7.c -lpthread -o example7
[briansun@briansun multithreading]$ ./example7 
s理想值应为2000000
1117439
[briansun@briansun multithreading]$ ./example7 
s理想值应为2000000
1116611
[briansun@briansun multithreading]$ ./example7 
s理想值应为2000000
1083574
[briansun@briansun multithreading]$ ./example7 
s理想值应为2000000
1120933
[briansun@briansun multithreading]$ ./example7 
s理想值应为2000000
1217610
```

为什么？

s++看上去是一行语句，实际上对应了3条指令：

```
ILOAD
IADD
ISTORE
```



原因在于：

```ascii
┌───────┐    ┌───────┐
│Thread1│    │Thread2│
└───┬───┘    └───┬───┘
    │            │
    │ILOAD (1)   │
    │            │ILOAD (1)
    │            │IADD
    │            │ISTORE (2)
    │IADD        │
    │ISTORE (2)  │
    ▼            ▼
```

如果线程1在执行`ILOAD`后被操作系统中断，此刻如果线程2被调度执行，它执行`ILOAD`后获取的值仍然是`1`，最终结果被两个线程的`ISTORE`写入后变成了`2`，而不是期待的`3`。

这说明多线程模型下，要保证逻辑正确，对共享变量进行读写时，必须保证一组指令以原子方式执行：即某一个线程执行时，其他线程必须等待：

```ascii
┌───────┐     ┌───────┐
│Thread1│     │Thread2│
└───┬───┘     └───┬───┘
    │             │
    │-- lock --   │
    │ILOAD (1)    │
    │IADD         │
    │ISTORE (2)   │
    │-- unlock -- │
    │             │-- lock --
    │             │ILOAD (2)
    │             │IADD
    │             │ISTORE (3)
    │             │-- unlock --
    ▼             ▼
```

通过加锁和解锁的操作，就能保证3条指令总是在一个线程执行期间，不会有其他线程会进入此指令区间。即使在执行期线程被操作系统中断执行，其他线程也会因为无法获得锁导致无法进入此指令区间。只有执行线程将锁释放后，其他线程才有机会获得锁并执行。**这种加锁和解锁之间的代码块我们称之为临界区（Critical Section），任何时候临界区最多只有一个线程能执行。**



以上的这种情况就是race condition

**What is race condition?**

Briefly Def: 

* a timing dependent error involving shared state 

  ​	It depends on how threads are scheduled.

(Hard to detect)



**Defined by Wikipedia**

A race condition arises in software when a computer program, to operate properly, depends on the sequence or timing of the program's [processes](https://en.wikipedia.org/wiki/Process_(computing)) or [threads](https://en.wikipedia.org/wiki/Thread_(computing)). Critical(临界的) race conditions cause invalid execution and [software bugs](https://en.wikipedia.org/wiki/Software_bug). Critical race conditions often happen **when the processes or threads depend on some shared state.** Operations upon shared states are done(解决) in [critical sections](https://en.wikipedia.org/wiki/Critical_section)(临界区段*) that must be [mutually exclusive](https://en.wikipedia.org/wiki/Mutual_exclusion).（加锁） Failure to obey this rule can corrupt(破坏) the shared state.

*临界区段：在并行计算中，同时接入共享的资源会导致不可预估或出现错误的行为，因此，程序中访问共享资源的部分需要以避免并发访问的方式加以保护。（加锁）



## example8.c 解决race condition -- 代码段加锁

锁：用来锁一段代码，当某段代码被加锁后，多条线程使用这段代码时只允许一条线程使用。

当一条线程抢占到这部分代码的锁时，对这段代码拥有执行权，在代码执行到解锁前，其他线程访问到加锁代码前想要加锁时，必须得等待占用锁的线程执行解锁操作

[man](https://linux.die.net/man/3/pthread_mutex_init)

初始化一个锁

`pthread_mutex_t`

`int pthread_mutex_init(pthread_mutex_t *restrict mutex,
const pthread_mutexattr_t *restrict attr);`

代码段加锁

`int pthread_mutex_lock(pthread_mutex_t *mutex);`

代码段解锁

`int pthread_mutex_unlock(pthread_mutex_t *mutex);`

销毁一个锁

`int pthread_mutex_destroy(pthread_mutex_t  *mutex);`



```c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t lock;

int s = 0;

void* myfunc(void* args){
	int i = 0;
	for(i = 0; i < 1000000; i++){
        pthread_mutex_lock(&lock);
		s++;
        pthread_mutex_unlock(&lock);
	}
}

int main()
{
	pthread_t th1;
	pthread_t th2;

	pthread_create(&th1, NULL, myfunc, NULL);
	pthread_create(&th2, NULL, myfunc, NULL);

    pthread_mutex_init(&lock, NULL);
    
	pthread_join(th1, NULL);
	pthread_join(th2, NULL);

	printf("s理想值应为2000000\n");
	printf("实际值：%d\n", s);
}
```

相当于：

[![rBpHXR.png](https://s3.ax1x.com/2020/12/21/rBpHXR.png)](https://imgchr.com/i/rBpHXR)



[![rB9Qun.png](https://s3.ax1x.com/2020/12/21/rB9Qun.png)](https://imgchr.com/i/rB9Qun)

#### 问题所在：很慢

```c
$ time ./example8
s理想值应为2000000
实际值：2000000

real    0m0.131s
user    0m0.178s
sys     0m0.076s
```

由于每次s++都会加锁解锁，加锁总共加了2000000次，也是需要时间的



#### 更好的解决方案

* 如果把加锁解锁放在for循环外面呢？

```c
s理想值应为2000000
实际值：2000000

real    0m0.016s
user    0m0.016s
sys     0m0.000s
```

是快了很多，但是有必要吗？为什么不直接让一条线程执行两次myfunc，还省去四次加锁解锁的时间

* **解决方案**：

  1. 让两条线程把需要加和的数据存储在两个变量里，最终再对这两个变量进行最终加和，即`example5.c`

  2. 加锁的两种人为实现都太complicated了，有没有更好的解决方案？

     **Semaphores -- 信号量**

     [![rBPSOA.png](https://s3.ax1x.com/2020/12/21/rBPSOA.png)](https://imgchr.com/i/rBPSOA)

     这些TestAndASet、Swap、Semaphors实现都是**Spinlock -- 自旋锁**，他们都需要一个busy waiting(忙时等待)

     自旋锁：

     [![rBFlzF.png](https://s3.ax1x.com/2020/12/21/rBFlzF.png)](https://imgchr.com/i/rBFlzF)

     解释一下为什么cpu会被wait浪费，当一个线程进入了临界区内，另一条线程如果尝试进入临界区会先执行wait，由于不符合出wait条件会使用cpu不断地在wait的while循环里待着，这也是消耗cpu的行为，不如先让他阻塞掉，当在临界区执行完毕的线程执行结束后会发signal唤醒刚刚等待的线程(这个存在senaphore结构体的list成员中)

     下图的伪代码`add this process to..`和`remove a process P ...`个人认为应是thread而不是process

[![rBFIyQ.png](https://s3.ax1x.com/2020/12/21/rBFIyQ.png)](https://imgchr.com/i/rBFIyQ)

## 单条线程的私有数据 -- TSD (Thread-specific Data)

在某些情况下，需要设计**进程自己的全局变量**，这种特殊变量仅在某个线程内部有效。

采用**一键多值**技术 -- 一个变量名，在不同线程下访问对应不同值

这个TSD键类型：`pthread_key_t`，在不同线程下再具体设置数据

`pthread_key_create`

`pthread_setspecific`

`pthread_getspecific`

`pthread_key_delete`



