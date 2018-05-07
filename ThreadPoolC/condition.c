#include "condition.h"

int ConditionInit(Condition_t *cond){
	int status = pthread_mutex_init(&cond->pmutex,NULL);
	if(status!=0)
		return status;
	status = pthread_cond_init(&cond->pcond,NULL);
	if(status!=0)
		return status;
	return 0;
}

int ConditionLock(Condition_t *cond){
	return pthread_mutex_lock(&cond->pmutex);
}

int ConditionUnlock(Condition_t *cond){
	return pthread_mutex_unlock(&cond->pmutex);
}

int ConditionWait(Condition_t *cond){
	return pthread_cond_wait(&cond->pcond,&cond->pmutex);
}

int ConditionTimedWait(Condition_t *cond,const struct timespec *abstime){
	return pthread_cond_timedwait(&cond->pcond,&cond->pmutex,abstime);
}

int ConditionSignal(Condition_t *cond){
	return pthread_cond_signal(&cond->pcond);
}

int ConditionBroadcast(Condition_t *cond){
	return pthread_cond_broadcast(&cond->pcond);
}

int ConditionDestroy(Condition_t *cond){
	int status = pthread_mutex_destroy(&cond->pmutex);
	if(status!=0)
		return status;
	status = pthread_cond_destroy(&cond->pcond);
	if(status!=0)
		return status;
	return 0;
}
