#ifndef __CPU_POOL_H__
#define __CPU_POOL_H__

#include <stdbool.h>
#include <stddef.h>

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
	size_t				WorkingCount;
	size_t				ThreadCount;
	bool					stop;
} ;
typedef struct JobBoard job_b;


//static job_t* CreateWork( thread_function , void* );


static void DestroyWork( job_t*  );

static job_t* GetWork( job_b*	);

static void* worker( void* );

job_b* CreateWorkers( size_t );

void WaitForWorkers( job_b* );

bool ListAddWork( job_b* , thread_func , void* );

#endif
