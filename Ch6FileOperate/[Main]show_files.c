#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>

int my_readDir(char* path){
    DIR * dir;
    struct dirent * item;

    if((dir = opendir(path)) == NULL){
        perror("opendir");
    }

    while((item = readdir(dir)) != NULL){
        printf("%s\n", item->d_name);
    }
    closedir(dir);

    return 0;
}

int main(int argc, char** argv)
{
    if(argc < 2){
        printf("Usage: %s <path name>", argv[0]);
        exit(1);
    }

    if(my_readDir(argv[1]) < 0){
        exit(1);
    }

    return 0;
}
