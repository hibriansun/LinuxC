/**
 * 
 * https://www.cnblogs.com/Anker/p/3271773.html
 * 
 * 
 * 子进程先于父进程结束，如果父进程没有调用wait/waitpid函数子进程就会进入僵死状态
 * 
 * 僵尸进程对系统有害吗？
不会。由于僵尸进程并不做任何事情， 不会使用任何资源也不会影响其它进程， 因此存在僵尸进程也没什么坏处。 不过由于进程表中的退出状态以及其它一些进程信息也是存储在内存中的，因此存在太多僵尸进程有时也会是一些问题。

你可以想象成这样：

“你是一家建筑公司的老板。你每天根据工人们的工nega作量来支付工资。 有一个工人每天来到施工现场，就坐在那里， 你不用付钱， 他也不做任何工作。 他只是每天都来然后呆坐在那，仅此而已！”

这个工人就是僵尸进程的一个活生生的例子。但是， 如果你有很多僵尸工人， 你的建设工地就会很拥堵从而让那些正常的工人难以工作。
*/

/**
 * 使用pu aux可以看到
 * briansun    8733  0.0  0.0   2364   588 pts/1    S+   15:38   0:00 ./wait
 * briansun    8734  0.0  0.0   2496    80 pts/1    S+   15:38   0:00 ./wait
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    pid_t pid;
    pid = fork();
    int exit_code;
    char* msg;

    switch (pid)
    {
        case 0:
            msg = "Child Process is running.";
            exit_code = 30;
            break;
        
        case -1:
            perror("Process creation failed.\n");
            break;
            
        default:
            exit_code = 0;
            break;
    }

    
    if(pid != 0){
        // 只允许父进程进来
        sleep(10);      // 等待子进程结束
        
        // 解决子进程为僵尸进程的方案：去除下列注释，将父进程wait子进程结束，读取子进程结束信息子进程彻底消失
        // int status;         // for wait function, used to record exitcode from child process

        // // 暂停父进程，开始等待子进程结束
        // // pid_t childpid = wait(&status);
        // pid_t childpid = waitpid(-1, &status, 0);        // 等价于wait(&status) --> man wait

        // // 此时子进程已经结束，父进程得以恢复
        // printf("Child process has exited, child pid = %u\n", childpid);

        // if(WIFEXITED(status)){
        //     printf("Child exited with exit_code:%u\n", WEXITSTATUS(status));    // WEXITSTATUS取子进程exit/_exit参数的低八位
        // }else{
        //     printf("Child process exit abnormally.\n");
        // }

        
    }else{
        // 子进程进入，此时可查看父进程状态
        for(int i = 0; i < 5; i++){
            puts(msg);
            sleep(1);
        }
    }

    system("ps -o pid,ppid,state,tty,command");
    /**
     * 当子进程执行完毕变成僵尸进程时，此时父进程sleep完执行到这里时，ps aux看到子进程是一个僵尸进程
     *   10394    9450 S      pts/3    ./wait
     *   10395   10394 Z(僵尸) pts/3    [wait] <defunct>
     * 
    */

    exit(exit_code);
}
