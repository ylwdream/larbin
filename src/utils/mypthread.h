/*************************************************************************
    > File Name: mypthread.h
    > Author: wyl
    > Mail: 
    > Created Time: Wed 22 Oct 2014 12:57:48 AM PDT
 ************************************************************************/

#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <pthread.h>
#include "options.h"

#ifdef THREAD_OUTPUT
#define mypthread_cond_init(x, y) pthread_cond_init(x, y)
#define mypthread_cond_destory(x) pthread_cond_destory(x)
#define mypthread_cond_broadcast(x) pthread_cond_broadcast(x)
#define mypthread_cond_wait(c, x, y) while(c) {pthread_cond_wait(x, y); }

#define mypthread_mutex_init(x, y) pthread_mutex_init(x, y)
#define mypthread_mutex_desotry(x) pthread_mutex_destory(x)
#define mypthread_mutex_lock(x) pthread_mutex_lock(x)
#define mypthread_mutex_unlock(x) pthread_mutex_unlock(x)

#else

#define mypthread_cond_init(x, y) ((void) 0)
#define mypthread_cond_destory(x) ((void) 0)
#define mypthread_cond_wait(c, x, y) ((void) 0)
#define mypthread_cond_broadcast(x) ((void) 0)

#define mypthread_mutex_init(x, y) ((void) 0)
#define mypthread_mutex_destory(x) ((void) 0)
#define mypthread_mutex_lock(x) ((void) 0)
#define mypthread_mutex_unlock(x) ((void) 0)

#endif //THREAD_OUTPUT

typedef void* (*StartFun)(void *);

void startThread(StartFun run, void *arg);

#endif
