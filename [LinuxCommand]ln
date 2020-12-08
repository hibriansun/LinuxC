# 硬链接、软链接（符号连接）
## ln命令
* 创建硬链接：ln source target
* 创建软件链接：ln -s source target

## ls-i选项
在Linux的文件系统中，保存在磁盘分区中的文件不管是什么类型都给它分配一个编号，称为索引节点号(Inode Index)，你可以简单把它想成 C 语言中的指针，它指向了物理硬盘的一个区块，事实上文件系统会维护一个引用计数，只要有文件指向这个区块，它就不会从硬盘上消失。
使用`ls -i, --inode`，列出文件的inode index
(`print the index number of each file`)

## 硬链接、软链接

### 硬链接：
实验：
[![rpnUiV.md.png](https://s3.ax1x.com/2020/12/08/rpnUiV.md.png)](https://imgchr.com/i/rpnUiV)
这里创建了个硬链接2.c到1.c上，看到两个文件的**inode index一样**，内容一样，指向同一区块

当我们通过硬链接2.c写入内容，在查看1.c 2.c发现内容一致
[![rpKwDJ.png](https://s3.ax1x.com/2020/12/08/rpKwDJ.png)](https://imgchr.com/i/rpKwDJ)

当我们删除1.c试试
[![rpn2i6.md.png](https://s3.ax1x.com/2020/12/08/rpn2i6.md.png)](https://imgchr.com/i/rpn2i6)
发现2.c这个硬链接还在，并且内容与1.c一致

==>
总结：
普通文件就是拥有一个inode指向磁盘中实际文件，硬链接其实和普通文件一样，有inode指向磁盘中的文件，不过Linux允许同一磁盘上的内容有多个目录路径指向，硬连接的作用是允许一个文件拥有多个有效路径名，
即使一个指向路径被删也有其他指向路径指向磁盘中同一块内容，除非磁盘中同一块内容没有文件inode指向，此时这块内容将不能通过目录被访问

### 软链接：
实验：
[![rpMDsg.md.png](https://s3.ax1x.com/2020/12/08/rpMDsg.md.png)](https://imgchr.com/i/rpMDsg)
发现软链接特别像Windows下的快捷方式
自己是磁盘上单另的一个文件，有自己的inode，这个文件指向另一个**文件目录路径**，另一个文件一旦消失，则无法通过这个软链接访问


==>
总结：保存了其代表的文件的绝对路径，是另外一种文件，在硬盘上有独立的区块，访问时替换自身路径。


link:https://www.jianshu.com/p/dde6a01c4094