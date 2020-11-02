#ifndef __CPU_POOL_H__
#define __CPU_POOL_H__

#include <stdbool.h>
#include <stddef.h>
#include "stest.h"
typedef void (*thread_func)(void *arg);

struct job{
	thread_func		func;
	void				*arg;
	struct job 	*next;	

};
typedef struct job job_t;

struct JobBoard{

	job_t 				*first;
	job_t 				*last;
	pthread_mutex_t 	JobMutex;
	pthread_cond_t 	WorkCondition;
	pthread_cond_t 	WorkingCondition;
	size_t				BufferCount;
	size_t				WorkingCount;
	size_t				ThreadCount;
	bool					stop;
	com_line*			data;
	bool					complete;
} ;
typedef struct JobBoard job_b;

com_line* memory_init();
//static job_t* CreateWork( thread_function , void* );

job_b* WorkerBees;

static void DestroyWork( job_t*  );

static job_t* GetWork( job_b*	);

static void* worker( void* );

job_b* CreateWorkers( size_t );

void WaitForWorkers( job_b* );

bool ListAddWork( job_b* , thread_func , void* );

com_line* memory_init();
void AbortWork(job_b*);
void ListComplete(job_b*);
#endif
