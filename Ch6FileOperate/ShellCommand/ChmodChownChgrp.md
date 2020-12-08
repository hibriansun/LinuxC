### chmod <全能>


### chmod
#### 1. x = 1 w = 2 r = 4
#### 2. [a/u/g/o][+/-/=][r/w/x]
a:all
u:user
g:group
o:other
=:设置为，包含了+、-
e.g
`sudo chmod a=w 1.c` 给所有人了写的权力，无论之前有没有

**支持递归修改目录下文件 -R**


### chown <更改所有者兼具文件所属的组(其他/本组)>
语法：
`chown [-R] 想要所属用户 文件名`
`chown [-R] 想要所属用户:用户组 文件名`（这个用户组不一定是前面用户名所属用户组，自定义，这里的分号相当于个和的概念[已验证√]）

[-R]可选，适用于目录文件，表示将目录下所有文件全部更改所有者（直到递归开始到递归最尾）

```
briansun@briansun:~/tmp$ ls -al
total 16
drwxrwxr-x  2 briansun briansun 4096 Dec  8 18:01 .
drwxr-xr-x 40 briansun briansun 4096 Dec  8 17:42 ..
-rw-rw-r--  1 briansun briansun   15 Dec  8 18:01 1.c
-rw-rw-r--  1 briansun briansun   26 Dec  8 18:00 2.c
lrwxrwxrwx  1 briansun briansun    3 Dec  8 18:00 3.c -> 1.c
briansun@briansun:~/tmp$ sudo chown root:root 1.c
briansun@briansun:~/tmp$ ls -al
total 16
drwxrwxr-x  2 briansun briansun 4096 Dec  8 18:01 .
drwxr-xr-x 40 briansun briansun 4096 Dec  8 17:42 ..
-rw-rw-r--  1 root     root       15 Dec  8 18:01 1.c
-rw-rw-r--  1 briansun briansun   26 Dec  8 18:00 2.c
lrwxrwxrwx  1 briansun briansun    3 Dec  8 18:00 3.c -> 1.c
```


### chgrp
chgrp [-R] 目标用户组 文件名