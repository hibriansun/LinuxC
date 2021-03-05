#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>


// send data type
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

int main(int argc, char **argv)
{

    if (argc != 5)
    {
        printf("Usage:[-p] [serv_port] [-a] [serv_address]\n");
        exit(1);
    }

    // 创建所连服务器socket信息结构体
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(struct sockaddr_in));
    serv_addr.sin_family = AF_INET;
    for (int i = 1; i < argc; i++)
    {
        if (strcmp("-p", argv[i]) == 0)
        {
            int serv_port = atoi(argv[i + 1]);
            if (serv_port < 0 || serv_port > 65535)
            {
                printf("Invalid serv_addr.sin_port\n");
                exit(1);
            }
            else
            {
                serv_addr.sin_port = htons(serv_port);
            }
        }
        else if (strcmp("-a", argv[i]) == 0)
        {
            if (inet_aton(argv[i + 1], &serv_addr.sin_addr) == 0)
            {
                printf("Invalid server ip address\n");
                exit(1);
            }
        }
    }

    // 创建客户端socket，连接到服务端socket
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);

    // 连接服务端
    if (connect(client_socket, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr_in)) < 0)
    {
        perror("Client connect error");
        exit(1);
    }

    // 连接成功后实现注册 or 登录
    homepage(client_socket);
    close(client_socket);
}

// 发送用户数据
void send_out(int client_socket, DATA* msg){
    if(send(client_socket, msg, sizeof(DATA), 0) < 0){
        perror("Client send data failed.");
    }
}

void log_in(int client_socket){
    system("clear");
    
    DATA* login = (DATA*)malloc(sizeof(DATA));
    login->type = LOGIN;
    login->data = (struct login*)malloc(sizeof(struct login));
    (login->data)->correct = 1;

    printf("=================================================\n");
    do{
        if((login->data)->correct == 0){
            printf("账户或密码错误，请重新输入\n");
        }

        printf("账户：");
        scanf("%s\n", (struct login*)(login->data)->name);
        printf("密码：");
        scanf("%s\n", (struct login*)(login->data)->passwd);

        send_out(client_socket, login);

        // 等待服务器验证后填充correct字段

        if(recv(client_socket, login, sizeof(DATA), 0) < 0){
            perror("Client recv failed.");
            exit(1);
        }
    }while((recv.data)->correct == 0);

    printf("登录成功！\n");
    getchar();

    free(login.data);
    free(login);
}

void sign_in(){

}

void find_account(){

}

void homepage(int client_socket)
{
    system("clear");
    printf("=================================================\n");
    printf("1. 登录\n");
    printf("2. 注册\n");
    printf("3. 找回账户\n");
    printf("4. 退出\n");
    printf("=================================================\n");
    printf("请输入对应功能序号以回车键结束输入:");

    int choice;
    while (scanf("%d", &choice))
    {
        switch (choice)
        {
        case 1:
            log_in(client_socket);
            return;

        case 2:
            sign_in(client_socket);
            return;

        case 3:
            find_account(client_socket);
            return;

        default:
            printf("输入有误，请重新输入!\n");
            char ch;
            while ((ch = getchar()) != '\n ' && ch != EOF)
                ;

            break;
        }
    }
}