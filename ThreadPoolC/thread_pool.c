#include "thread_pool.h"

void *ThreadRoutine(void *arg){
	struct timespec abstime;
	int timeout;
	printf("thread 0x%x is starting\n", (int)pthread_self());
	ThreadPool_t *pool = (ThreadPool_t *)arg;
	while(1){
		timeout = 0;
		ConditionLock(&pool->ready);
		pool->idle++;
		while(pool->first == NULL || pool->quit == 0){
			clock_gettime(CLOCK_REALTIME, &abstime);
			abstime.tv_sec += 2;
			int status = ConditionTimedWait(&pool->ready, &abstime);
			if(status == ETIMEDOUT){
				printf("thread 0x%x is waiting timed out\n", (int)pthread_self());
				timeout = 1;
				break;
			}
		}
		
		pool->idle--;
		if(pool->first != NULL){
			Task_t *t = pool->first;
			pool->first = t->next;
			ConditionUnlock(&pool->ready);
			t->Run(t->arg);
			free(t);
			ConditionLock(&pool->ready);
		}
		if(pool->quit == 1 && pool->first == NULL){
			pool->counter--;
			if(pool->counter == 0){
				ConditionSignal(&pool->ready);
			}
			ConditionUnlock(&pool->ready);
			break;
		}
		if(timeout && pool->first == NULL){	
			pool->counter--;
			ConditionUnlock(&pool->ready);
			break;
		}
		ConditionUnlock(&pool->ready);
	}

	printf("thread 0x%x is exiting\n", (int)pthread_self());
}

void ThreadPoolInit(ThreadPool_t *pool, int threads){
	ConditionInit(&pool->ready);
	pool->first = NULL;
	pool->last = NULL;
	pool->counter = 0;
	pool->idle = 0;
	pool->max_threads = threads;
	pool->quit = 0;
}

void ThreadPoolAddTask(ThreadPool_t *pool, void *(*Run)(void *arg), void *arg){
	Task_t *new_task = (Task_t *)malloc(sizeof(Task_t));
	new_task->Run = Run;
	new_task->arg = arg;
	new_task->next = NULL;
	
	ConditionLock(&pool->ready);
	if(pool->first == NULL)
		pool->first = new_task;
	else
		pool->last->next = new_task;
	pool->last = new_task;
	if(pool->idle > 0){
		ConditionSignal(&pool->ready);
	}
	else if(pool->counter < pool->max_threads){
		pthread_t tid;
		pthread_create(&tid, NULL, ThreadRoutine, pool);
		pool->counter++;
	}
	
	ConditionUnlock(&pool->ready);
}

void ThreadPoolDestroy(ThreadPool_t *pool){
	if(pool->quit){
		return;
	}
	ConditionLock(&pool->ready);
	pool->quit = 1;
	if(pool->counter > 0){
		if(pool->idle > 0){
			ConditionBroadcast(&pool->ready);
		}
		while(pool->counter > 0){
			ConditionWait(&pool->ready);
		}
	}
	ConditionUnlock(&pool->ready);
	ConditionDestroy(&pool->ready);
}
