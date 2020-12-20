#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
	int first;
	int last;
}MY_ARGS;

int arr[5000];
int s = 0;

void* myfunc(void* args){
	for(int i = ((MY_ARGS*)args)->first; i < ((MY_ARGS*)args)->last; i++){
		s += arr[i];
	}

	return NULL;
}


int main()
{
	for(int i = 0 ; i < 5000; i++){
		arr[i] = rand() % 50;
	}

	/*
	for(int i = 0; i < 5000; i++){
		printf("arr[%d]: %d\n",i ,arr[i]);
	}
	*/
	
	pthread_t th1;
	pthread_t th2;

	MY_ARGS my_args1 = {0, 2500};
	MY_ARGS my_args2 = {2500, 5000};

	// 第四个参数是往myfunc中传参的参数
	pthread_create(&th1, NULL, myfunc, &my_args1);	// 创建一条子线程th，执行myfunc函数
	pthread_create(&th2, NULL, myfunc, &my_args2);

	pthread_join(th1, NULL);
	pthread_join(th2, NULL);

	
	printf("s: %d\n", s);


	return 0;
	
}
