/**
#include <pthread.h>

// @param 
// thread: point to thread ID
// attr: pointer to thread attributes
// start_routine：函数指针，指向线程创建后要调用的函数
// arg：要传给线程线程函数的参数

int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
					void *(*start_routine) (void *), void *arg);

Compile and link with -pthread.

*/
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

	return 0;
}
