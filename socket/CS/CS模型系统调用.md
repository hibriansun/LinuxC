/*
=================== CREATE SOCKET =====================


#include <sys/types.h>          // portable application including 
#include <sys/socket.h>

int socket(int domain, int type, int protocol);
domain:
AF_INET -- IPv4 TCP/IP
AF_INET6 -- IPv6 TCP/IP

type:
SOCK_STREAM -- TCP流式socket
SOCK_DGRAM -- UDP流式socket
SOCK_RAW

protocol -- default 0



返回值：创建socket的文件描述符

*/





/*
================== BIND SOCKET WITH PORT =======================

#include <sys/types.h>          // portable application including 
#include <sys/socket.h>

int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

返回值：0成功，-1失败


*/




/*
================= ACCEPT CONNECTION FROM CLIENT SOCKET ===================
#include <sys/types.h>          // portable application including 

int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

socketfd: listening socket(server)

addr: client socket address, 传进去空值， accept函数负责填充客户协议**地址**到addr

return值: 为客户TCP连接分配的socket文件描述符
（可以这样理解：客户socket连接到服务器socket(sockfd)后，服务器accept分配一个新的socket让客户连到这个新socket上，再返回这个新socket的文件描述符）

*/





/*
================ CONNECT SERVER =================
#include <sys/types.h>         // portable application including 
#include <sys/socket.h>

int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

sockfd: client socket
addr: server address

返回值：0成功，-1失败

*/



/*
================ SEND MSG TO ANOTHER SOCKET ==================
#include <sys/types.h>
#include <sys/socket.h>

ssize_t send(int sockfd, const void *buf, size_t len, int flags);

socketfd: 主动发送数据的socket



返回值：发送出去的字节数，失败返回-1

*/



/*
=============== RECV DATA FROM ANOTHER SOCKET ===================
#include <sys/types.h>
#include <sys/socket.h>

ssize_t recv(int sockfd, void *buf, size_t len, int flags);

sockfd: 从这个socket上收取消息

       RETURN VALUE
           These calls return the number of bytes received, or -1 if an  error
           occurred.   In  the event of an error, errno is set to indicate the
           error.
       
       关于0：
       When a stream socket peer has performed an  orderly  shutdown,  the
       return value will be 0 (the traditional "end-of-file" return).
    
       Datagram  sockets  in  various domains (e.g., the UNIX and Internet
       domains) permit zero-length datagrams.  When such a datagram is re‐
       ceived, the return value is 0.
    
       The  value  0 may also be returned if the requested number of bytes
       to receive from a stream socket was 0.
*/



**服务器首先启动，通过调用socket()建立一个套接字，然后调用bind()将该套接字和本地网络地址联系在一起，再调用listen()使套接字做好侦听的准备，并规定它的请求队列的长度，之后就调用accept()来接收连接。客户端在建立套接字后就可调用connect()和服务器建立连接。连接一旦建立，客户机和服务器之间就可以通过调用read()和write()来发送和接收数据。最后，待数据传送结束后，双方调用close()关闭套接字。**
<img src="https://pic1.zhimg.com/80/v2-9e4d944b42d6a8bbf1d649cb627c49c4_720w.jpg" />