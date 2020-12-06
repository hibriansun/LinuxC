/*
创建一个名为create_file.txt文件
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main()
{
	// 文件描述符
	int fd;

	// 使用open函数来创建文件
	if( (fd = open("create_file.txt", O_CREAT | O_EXCL, S_IRUSR | S_IWUSR)) == -1){
		// perror("open"); // or
		printf("open:%s	with errno:%d\n", strerror(errno), errno);
	}else{
		printf("Create successfully\n");
	}
}
