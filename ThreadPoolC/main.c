#include "thread_pool.h"

void *MyTask(void *arg){
	printf("thread 0x%0x is working on task %d\n",(int)pthread_self(),*(int *)arg);
	sleep(1);
	free(arg);
	return NULL;
}

int main(){
	ThreadPool_t pool;
	ThreadPoolInit(&pool, 3);
	
	int i;
	for(i = 0; i < 10; ++i){
		int *p = (int *)malloc(sizeof(int));
		*p = i;
		ThreadPoolAddTask(&pool, MyTask, p);
	}
	ThreadPoolDestroy(&pool);
	return 0;
}
