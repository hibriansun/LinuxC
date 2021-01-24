/*
>> 初始化一个锁：
`pthread_mutex_t`

`int pthread_mutex_init(pthread_mutex_t *restrict mutex, const pthread_mutexattr_t *restrict attr);`
第二个参数一般为NULL

>> 代码段加锁

`int pthread_mutex_lock(pthread_mutex_t *mutex);`

>> 代码段解锁

`int pthread_mutex_unlock(pthread_mutex_t *mutex);`

>> 销毁一个锁

`int pthread_mutex_destroy(pthread_mutex_t  *mutex);`

*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>