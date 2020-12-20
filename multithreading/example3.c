#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void* myfunc(void* args){
	int i;
	for(i = 1; i <= 50; i++){
		printf("%d\n", i);
	}
	return NULL;
}


int main()
{
	pthread_t th1;
	pthread_t th2;
	
	pthread_create(&th1, NULL, myfunc, NULL);	// 创建一条子线程th，执行myfunc函数
	pthread_create(&th2, NULL, myfunc, NULL);

	pthread_join(th1, NULL);
	pthread_join(th2, NULL);

	return 0;
}
