#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    printf("my pid is %d\n", getpid());

    fork();
    fork();
    fork();

    printf("my pid is %d\n", getpid());
}