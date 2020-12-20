#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void* myfunc(void* args){
	int i;
	for(i = 1; i <= 50; i++){
		printf("%s: %d\n", (char*)args, i);
	}
	return NULL;
}


int main()
{
	pthread_t th1;
	pthread_t th2;

	// 第四个参数是往myfunc中传参的参数
	pthread_create(&th1, NULL, myfunc, " th1");	// 创建一条子线程th，执行myfunc函数
	pthread_create(&th2, NULL, myfunc, " th2");

	pthread_join(th1, NULL);
	pthread_join(th2, NULL);

	return 0;
}
