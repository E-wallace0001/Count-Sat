#include <stdbool.h>
#include	<stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <pthread.h>
#include "cpu_pool.h"

typedef struct JobBoard job_b;

static job_t* CreateWork( thread_func function, void* arg){

	job_t* work;
	
	if(function==NULL) return NULL;
		
	work 			= malloc(sizeof(*work));
	work->func 	= function;
	work->arg	= arg;
	work->next	= NULL;
	
	return work;

}

static void DestroyWork( job_t* work ){
	if ( work ==NULL) return;
	
	free(work);

}

static job_t* GetWork( job_b*	list){

	job_t* work;
	
	if( list == NULL ) return NULL;
	
	work = list->first;
	
	if( work == NULL ) return NULL;
	
	if( work->next==NULL) {
		list->first = NULL;
		list->last  = NULL;
	}else{
		list->first=work->next;
	}
	
	return work;
}

static void* worker( void* arg){

	job_b* 	list = arg;
	job_t* 	work;
	
	while( 1 ){
		//mutes thread to allow a single thread to pull a job off the board
		pthread_mutex_lock(&(list->JobMutex));
		
		while( list->first == NULL && list->stop!=true){
			pthread_cond_wait(&(list->WorkCondition), &(list->JobMutex));
		}
		
		if( list->stop != false ) break;
		
		work = GetWork( list );
		list->WorkingCount++;
		
		pthread_mutex_unlock(&(list->JobMutex));
		
		if( work !=NULL){
			work->func ( work->arg );
			DestroyWork( work );
		}
		
		pthread_mutex_lock(&(list->JobMutex));
		list->WorkingCount--;
		
		if( list->stop==0 && list->WorkingCount == 0 && list->first==NULL){
			pthread_cond_signal(&(list->WorkingCondition));
		}
		
		pthread_mutex_unlock(&(list->JobMutex));
	}
	
	list->ThreadCount--;
	pthread_cond_signal(&(list->WorkingCondition));
	pthread_mutex_unlock(&(list->JobMutex));
	return NULL;

}

job_b* CreateWorkers( size_t num ){

	job_b*		list;
	pthread_t 	thread;
	size_t		i;
	
	if( num ==0 ) num = 2;
	
	list					= calloc( 1, sizeof(*list));
	list->ThreadCount = num;
	
	pthread_mutex_init(&(list->JobMutex), NULL);
	pthread_cond_init(&(list->WorkCondition), NULL);
	pthread_cond_init(&( list->WorkingCondition), NULL);
	
	list->first= NULL;
	list->last = NULL;
	
	for( i=0; i< num; i++){
		pthread_create(&thread, NULL, worker, list);
		if(pthread_detach(thread)!=0){ printf("thread error \n"); exit(0);}
	}
	
	return list;
}

bool ListAddWork( job_b* list, thread_func function, void *arg){


	job_t* work;
	
	if( list == NULL) return NULL;
	
	work = CreateWork( function, arg);
	
	if( work == NULL ) return false;
	
	pthread_mutex_lock(&( list->JobMutex));
	
	if( list->first == NULL ) {
		list->first	= work;
		list->last 	= work;
	}else{
		list->last->next = work;
		list->last= work;
	}

	pthread_cond_broadcast(&(list->WorkCondition));
	pthread_mutex_unlock(&(list->JobMutex));
	
	return true;

}



void WaitForWorkers( job_b* list){

	if (list == NULL) return;
	
	pthread_mutex_lock(&(list->JobMutex));
	while(1){
		if(( list->stop!=1 && list->WorkingCount!=0) || (list->stop &&  list->ThreadCount!=0)){
			 pthread_cond_wait(&(list->WorkingCondition), &(list->JobMutex));
		}else{
			break;
		}
	}
	pthread_mutex_unlock(&(list->JobMutex))	;
}


void AbortWork(job_b* list){

	job_t* job;
	job_t* job2;

	if( list==NULL ) return;
	
	pthread_mutex_lock(&(list->JobMutex));
	
	job = list->first;
	
	while( job != NULL){
		job2 = job->next;
		DestroyWork(job);
		job = job2;
	}
	list->stop=true;
	pthread_cond_broadcast(&(list->WorkCondition));
	pthread_mutex_unlock(&(list->JobMutex));
	WaitForWorkers(list);
	
	/*
	pthread_mutex_destroy(&(list->JobMutex));
	pthread_cond_destroy(&(list->WorkCondition));
	pthread_cond_destroy(&(list->WorkingCondition));
	
	free(list);
	*/
}


































