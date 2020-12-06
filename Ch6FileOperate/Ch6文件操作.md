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

