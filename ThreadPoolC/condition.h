#ifndef CONDITION_H
#define CONDITION_H

#include <pthread.h>

typedef struct Condition{
	pthread_mutex_t pmutex;
	pthread_cond_t pcond;
} Condition_t;

int ConditionInit(Condition_t *cond);
int ConditionLock(Condition_t *cond);
int ConditionUnlock(Condition_t *cond);
int ConditionWait(Condition_t *cond);
int ConditionTimedWait(Condition_t *cond,const struct timespec *abstime);
int ConditionSignal(Condition_t *cond);
int ConditionBroadcast(Condition_t *cond);
int ConditionDestroy(Condition_t *cond);

#endif
