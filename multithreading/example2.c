#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void* myfunc(void* args){
	printf("Hello World\n");
	return NULL;
}


int main()
{
	pthread_t th;
	// 函数名也就是指针
	pthread_create(&th, NULL, myfunc, NULL);	// 创建一条子线程th，执行myfunc函数
	pthread_join(th, NULL);
	return 0;
}
