/**
 * vfork:
 * 1. fork和vfork都是调用一次返回两次
 * 2. vfork创建的子进程不会复制父进程的完整一份资源，vfork创建的子进程共享父进程的地址空间，也就是或vfork创建的子进程
 * 完全运行在父进程的地址空间上，该子进程对地址空间中数据修改为父进程可见
 * 3. vfork保证子进程先运行，直到调用exec或exit后父进程可以被调度运行，如果在调用exec或exit前子进程依赖于父进程的某个行为，
 * 则会导致死锁
 * 
 * Tip 最好不要允许vfork出的子进程修改与父进程共享的全局变量和局部变量
*/

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int glb_var = 10;

int main()
{
    int inner_var = 10;
    pid_t pid;
    // pid = fork();
    pid = vfork();

    int i;
    switch (pid) {
    case 0:
        i = 5;
        while(i){
            --i;
            glb_var++;
            inner_var++;
            printf("Child Process is running\n");
            sleep(1);
        }
        printf("Child's glb_var = %d, Child inner_var = %d\n", glb_var, inner_var);
        exit(0);    // 调用父进程前先使用exit()
    
    case -1:
        perror("Process creation failed\n");
        break;

    default:
        i = 10;
        while(i){
            glb_var++;
            inner_var++;
            --i;
            printf("Parent Process is running\n");
            sleep(1);
        }
        printf("Parent's glb_var = %d, Parent inner_var = %d\n", glb_var, inner_var);
        exit(0);
    }
}