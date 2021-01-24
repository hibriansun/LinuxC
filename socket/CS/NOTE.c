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

*/





/*
================== BIND SOCKET WITH PORT =======================

#include <sys/types.h>          // portable application including 
#include <sys/socket.h>

int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);


*/




/*
================= ACCEPT CONNECTION FROM CLIENT SOCKET ===================
#include <sys/types.h>          // portable application including 

int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

socketfd: listening socket(server)

addr: client socket address, 传进去空值， accept函数负责填充客户协议##地址##到addr

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

*/




/*
================ SEND MSG TO ANOTHER SOCKET ==================
#include <sys/types.h>
#include <sys/socket.h>

ssize_t send(int sockfd, const void *buf, size_t len, int flags);

socketfd: 主动发送数据的socket
*/




/*
=============== RECV DATA FROM ANOTHER SOCKET ===================
#include <sys/types.h>
#include <sys/socket.h>

ssize_t recv(int sockfd, void *buf, size_t len, int flags);

sockfd: 从这个socket上收取消息
*/