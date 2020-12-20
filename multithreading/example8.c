#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t lock;

int s = 0;

void* myfunc(void* args){
	int i = 0;
	for(i = 0; i < 1000000; i++){
        pthread_mutex_lock(&lock);
		s++;
        pthread_mutex_unlock(&lock);
	}
}

int main()
{
	pthread_t th1;
	pthread_t th2;

	pthread_create(&th1, NULL, myfunc, NULL);
	pthread_create(&th2, NULL, myfunc, NULL);

    pthread_mutex_init(&lock, NULL);
    
	pthread_join(th1, NULL);
	pthread_join(th2, NULL);

	printf("s理想值应为2000000\n");
	printf("实际值：%d\n", s);
}
