#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

int main()
{
    int aPipe[2];
    char buf[BUFSIZ];

    if(pipe(aPipe) == -1){
        perror("Pipe creation failed.");
        exit(0);
    }

    // parent process read data from stdin, then write data into aPipe[1], after that, child process read data from aPipe[0] and print data on screen
    while(fgets(buf, BUFSIZ, stdin)){
        int len = strlen(buf);
        if(write(aPipe[1], buf, len) != len){
            perror("write into pipe");
            break;
        }
        printf("Write data into pipe by parent process (pid: %d) sucessfully.\n", getpid());

        pid_t pid = fork();

        switch (pid)
        {
        case 0:
            if((len = read(aPipe[0], buf, BUFSIZ)) == -1){
                perror("read from pipe");
                break;
            }

            printf("Read data from pipe by child process (pid: %d) successfully.\n", getpid());
            
            if(write(STDOUT_FILENO, buf, len) != len){
                perror("writing to stdout");
                break;
            }
            printf("Write data into stdout by child process (pid: %d) successfully.\n\n", getpid());
            
            break;
        
        case -1:
            perror("fork");
            exit(1);

        default:
            wait(NULL);
            break;
        }
    }
}