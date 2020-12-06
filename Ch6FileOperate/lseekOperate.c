/*
创建的这个文件包含空洞
*/

#include <stdio.h>
#include <stdlib.h>	// exit() defined in stdlib.h
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

// 自定义错误处理函数
void my_error(const char* err_string, int lineNum){
	fprintf(stderr, "line:%d ",  lineNum);
	perror(err_string);	// 此函数将上一个函数发生的错误信息结合perror参数输出到stderr中

	exit(1);
}


// 自定义数据读取函数
int my_read(int fd){
	int len;
	int rtn;
	char buf[64];
	
	// 获取读取文件长度并保证文件指针指向文件开头
	if(lseek(fd, 0, SEEK_END) == -1){
		my_error("lseek", __LINE__);
	}

	if((len = lseek(fd, 0, SEEK_CUR)) == -1){
		my_error("lseek", __LINE__);
	}

	if(lseek(fd,0, SEEK_SET) == -1){
		my_error("lseek", __LINE__);
	}

	printf("len = %d\n", len);

	// 读数据
	if((rtn = read(fd, buf, len)) < 0){
		my_error("read", __LINE__);
	}

	// 打印数据
	for(int i = 0; i < len; i++){
		printf("%c", buf[i]);
	}
	printf("\n");

	return rtn;
}



int main()
{
	int fd;
	char write_buf[32] = "Hello, this is my world!";

	// 创建
	 if((fd = creat("exampleFromlseekOperate", S_IRWXU) == -1)){		// 通过这种方式创建出来的文件仅写的方式打开，此时打开没有读的权限
	// if((fd = open("exampleFromlseekOperate", O_RDWR | O_CREAT | O_TRUNC, S_IRWXU)) == -1){
		my_error("creat", __LINE__);
	}else{
		printf("Create file success\n");
	}

	// 写文件
	if(write(fd, write_buf, strlen(write_buf)) != strlen(write_buf)){
		my_error("write", __LINE__);
	}
	
	my_read(fd);


	// 制作空洞
	printf("/*************************/\n");
	if(lseek(fd, 10, SEEK_END) == -1){
		my_error("lseek", __LINE__);
	}

	if(write(fd, write_buf, strlen(write_buf)) != strlen(write_buf)){
                my_error("write", __LINE__);
        }
	my_read(fd);

	close(fd);
}
