#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERV_PORT   12000
#define LISTENQ     50      // 连接请求最大请求


// recv data type
#define LOGIN 0


struct login{
    char name[20];
    char passwd[20];
    char correct;
};

typedef struct {
    int type;
    void* data;
}DATA;


int main()
{
    int server_socket;
    struct sockaddr_in server_addr;

    

    // create socket for server
    if((server_socket = socket(AF_INET, SOCK_STREAM, 0) ) < 0){
        perror("Fail to create socket");
    }

    // ensure being able to bind
    int optval = 1;
    if(setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, (void*)&optval, sizeof(int)) < 0){
        perror("setsocketopt error");
    }

    // Init server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERV_PORT);      // host to network short(16)
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);    // host to network long(32)

    // bind socket on port
    if(bind(server_socket, (struct sockaddr_in*)&server_addr, sizeof(struct sockaddr_in)) < 0){
        perror("Bind error");
    }

    // change status into listening
    if(listen(server_socket, LISTENQ) < 0){
        perror("Listen error");
    }



    while(1){
        // 阻塞等待有client连接
        struct sockaddr_in client_addr;
        int sizeofCliaddr = sizeof(struct sockaddr_in);
        int connected_fd = accept(server_socket, (struct sockaddr*)&client_addr, &sizeofCliaddr);

        if(connected_fd < 0){
            perror("accept failure");
        }

        // accept successful msg
        printf("Accept client successfully! Client IP: %s\n", inet_ntoa(client_addr.sin_addr));
    
        pid_t pid = fork();
        if(pid = 0){         // 子进程执行此段
            // 从accept后新分配的socket中读取数据
            


        }else{               // 父进程执行此段
            wait(NULL);
            close(connected_fd);
        }
    }
}