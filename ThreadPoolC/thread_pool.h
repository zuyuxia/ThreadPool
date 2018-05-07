#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include "condition.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <time.h>

typedef struct Task{
	void *(*Run)(void *arg);	//任务回调函数
	void *arg;					//回调函数参数
	struct task *next;
} Task_t;

typedef struct ThreadPool{
	Condition_t ready;			//任务准备就绪或线程池销毁通知
	Task_t *first;				//任务队列头指针
	Task_t *last;				//任务队列尾指针
	int counter;				//线程池中当前线程数
	int idle;					//线程池中当前正在等待任务的线程数
	int max_threads;			//线程池中最大允许的线程数
	int quit;					//销毁线程池的时候置1
} ThreadPool_t;

void ThreadPoolInit(ThreadPool_t *pool,int threads);
void ThreadPoolAddTask(ThreadPool_t *pool,void *(*Run)(void *arg),void *arg);
void ThreadPoolDestroy(ThreadPool_t *pool);

#endif