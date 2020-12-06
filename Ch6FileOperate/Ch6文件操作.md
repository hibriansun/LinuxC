* man
  * man 函数名、指令名、库函数名
  * i.e. `man 3 pthread`
  * `man mkdir`
  * `man 2 mkdir` 

* macOS系统文件组成

[![DOvdrq.md.png](https://s3.ax1x.com/2020/12/06/DOvdrq.md.png)](https://imgchr.com/i/DOvdrq)

* 如何对物理磁盘访问？-- 通过`设备驱动程序`，如何访问设备驱动程序？
  * 设备驱动接口（直接访问设备驱动程序，给系统带来不稳定）
  * Virtual File System

* `drwxr-xr-x`、`-rw-r--r--`、`brw-r-----`、`lrwxr-xr-x`

  d：目录型文件(仅内核有权限写)  -：普通文件 b：块文件 l：链接文件

* `chmod 777 test.c`

  文件权限：**r**ead、**w**rite、e**x**cute，Unix给予权值：r=4 w=2 x=1

  三位数字依次属于：所有者、所属组、其他用户

* chmod函数

  `int chmod(const char* path, mode_t mode)`

  mode为八进制表示数，意义是对所有者、所属组、其他用户修改什么样的权限



## 文件输入输出

* 文件描述符（0～∞(多数OS实现的文件描述符变化范围是无限的，仅受硬盘总量、整型字长、admin设置的软硬限制)）

  | 整数值 |                          名称                           | unistd.h符号常量 | [stdio.h]文件流 |
  | :----: | :-----------------------------------------------------: | :--------------: | :-------------: |
  |   0    |  [Standard input](https://zh.wikipedia.org/wiki/Stdin)  |   STDIN_FILENO   |      stdin      |
  |   1    | [Standard output](https://zh.wikipedia.org/wiki/Stdout) |  STDOUT_FILENO   |     stdout      |
  |   2    | [Standard error](https://zh.wikipedia.org/wiki/Stderr)  |  STDERR_FILENO   |     stderr      |

  ......

* open creat close函数

  * open(文件描述符，打开方式, [mode])
  
    open一个文件成功后会返回这个文件的文件描述符，出错返回-1
  
    仅当使用O_CREAT时才引入第三个参数描述新创建文件的rwx权限
  
  * creat
  
    creat返回只写打开的文件描述符，出错返回-1
  
    int creat(const char* path, mode_t mode) 与 open(path, O_WRONLY | O_CREAT | O_TRUNC, mode)等价  
  
    **O_WRONLY---> 只写方式打开**
  
    **O_TRUNC**：若文件存在并且以可写方式打开时，这个编制会将文件长度清零，丢失原文数据，但文件属性不变
  
    mode: 仅在第二个参数有O_CREAT时的新文件的权限
  
  * close
  
    int close(int fd)
  
    成功返回0，出错 -1
  
* lseek

  * off_t lseek(int fd, off_t offset, int whence)

  * whence：SEEK_SET（距头）

    ​				SEEK_CUR（距当前）

    ​				SEEK_END（距末尾）

  * 成功返回新文件偏移量（距头），出错返回-1

