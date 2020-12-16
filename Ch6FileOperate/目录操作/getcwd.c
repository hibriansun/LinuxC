#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    // 情况1
    // char buf[5];
    // printf("%s\n", getcwd(buf, 5));

    // 情况2 -- 根据size自动malloc
    // char* buf = NULL;
    // printf("%s\n", getcwd(buf, 100000));
    // free(buf);

    // 情况3
    char* buf = NULL;
    printf("%s\n", getcwd(buf, 0));
    free(buf);

}