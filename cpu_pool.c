#include <stdbool.h>
#include	<stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <pthread.h>
#include "cpu_pool.h"
#include "stest.h"

#include "var_pos.h"
#include <unistd.h> 

#include "bfs_s.h"
#include "m_map.h"
#include "llist.h"

typedef struct JobBoard job_b;

static job_t* CreateWork( thread_func function, void* arg){

	job_t* work;
	
	if(function==NULL) return NULL;
	//com_line* mem = (com_line*)arg;
	//work = alloc_mem( mem->job_mem );	
	work 			= malloc(sizeof(*work));
	work->func 	= function;
	work->arg	= arg;
	work->next	= NULL;
	
	
	return work;

}

static void DestroyWork( job_t* work ){
	if ( work ==NULL) return;
	com_line* argt 	= work->arg;
	//release_mem(work, argt->job_mem);
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
	//int f_variable_connections[vsize][90];
	
	
	pthread_mutex_lock(&(list->JobMutex));
	
	int** clause_connections				= list->data->clause_connections;
	variable_pos** variable_position		= list->data->variable_position;
	long* 	set_variable					= list->data->set_variable;
	int* 		var_tab							= list->data->var_tab;
	int* 		clause_size						= list->data->clause_size;
	bool* 	set_var							= list->data->set_var;
	int ** 	f_var								= f_variable_connections;
	
	m_map*	node_pool						= list->data->node_pool;
	m_map*	link_pool						= list->data->link_pool;
	m_map*	com_pool							= list->data->com_pool;
	m_map*	job_mem							= list->data->job_mem;
//printf(" data ->%p\n", list->data->node_pool);
	
	
	//list->data=NULL;
	pthread_mutex_unlock(&(list->JobMutex));
	
	//variable_pos** test =NULL;
	while( 1 ){
		//mutes thread to allow a single thread to pull a job off the board
		pthread_mutex_lock(&(list->JobMutex));
		
		while( list->first == NULL && list->stop!=true){
			pthread_cond_wait(&(list->WorkCondition), &(list->JobMutex));
		}
		
		if( list->stop != false ) break;
		
		work = GetWork( list );
		list->WorkingCount++;
		com_line* argt 	= (com_line*)work->arg;
		
		pthread_mutex_unlock(&(list->JobMutex));

		

		argt->node_pool				= node_pool;
		argt->link_pool				= link_pool;
		argt->com_pool					= com_pool;
		argt->job_mem					= job_mem;
		
		if( work !=NULL){
			work->func ( work->arg );
			DestroyWork( work );
		}
		
		
		reset(node_pool);
		reset(link_pool);
		reset(com_pool);
		reset(job_mem);
		
		defrag_mem(node_pool);
		defrag_mem(link_pool);
		defrag_mem(com_pool);
		defrag_mem(job_mem);

//		dispose(&argt->clause_node);

//		mpz_clear(argt->clause_node->removed);
//		mpz_clear(argt->clause_node->data);
//		free(argt->clause_node);

		//reset set
//		ResetSolve( argt );
	
		
		pthread_mutex_lock(&(list->JobMutex));
		list->WorkingCount--;
		
		if( list->stop==0 && list->WorkingCount == 0 && list->first==NULL){
			pthread_cond_signal(&(list->WorkingCondition));
		}
		
		pthread_mutex_unlock(&(list->JobMutex));
	}
	
	
	for( size_t i =0; i<f_clause_count; i++){
	//	free(clause_connections[i]);
	}
	//free(clause_connections);
	//destroy_position(variable_position);
	
	list->ThreadCount--;
	pthread_cond_signal(&(list->WorkingCondition));
	pthread_mutex_unlock(&(list->JobMutex));
	return NULL;

}

job_b* CreateWorkers( size_t num ){

	job_b*		list;
	pthread_t 	thread;
	size_t		i;
	
	//if( num ==0 ) num = 1;
	
	list					= calloc( 1, sizeof(*list));
	if(list==NULL) {
		printf("worker create fail \n");
		exit(0);
	}
	list->ThreadCount = num;
	
	pthread_mutex_init(&(list->JobMutex), NULL);
	pthread_cond_init(&(list->WorkCondition), NULL);
	pthread_cond_init(&( list->WorkingCondition), NULL);
	
	list->first= NULL;
	list->last = NULL;
	
	for( i=0; i< num; i++){
	
		//initalize memory for each thread
		
		
		list->data=memory_init();
		printf(" init %lu \n", i);
		//create threads
		
		pthread_create(&thread, NULL, worker, list);
		
		//		while(list->data!=NULL){}
		//detach thread
		if(pthread_detach(thread)!=0){ 
			printf("thread error \n"); 
			exit(0);
		}
		//halt for memory release

	}
	return list;
}

com_line* memory_init(){

	com_line* memory_initalize;
	memory_initalize =  malloc( sizeof(com_line));
/*	
	int* clause_size= calloc(csize, sizeof(int*));
	
	//create a table for processing purposes
	int **clause_connections = calloc(f_clause_count, sizeof(int*));
	
	size_t i;
	
	for( i=0; i< f_clause_count; i++){
		clause_connections[i] = calloc(90, sizeof(int*));
	}
	
	long* set_variable					= calloc( csize, sizeof(long));

	variable_pos** variable_position = calloc(csize,sizeof(variable_pos));
	
	if (variable_position==NULL) exit(0);
	//init_position(variable_position);
*/
	bool*		set_var								= calloc(csize, sizeof(bool));
	int*		var_tab								= calloc(vsize, sizeof(int));
	m_map*	node_pool							= init_mem( sizeof(node), 1024 );
	m_map*	link_pool							= init_mem( sizeof(link_node), 1024 );
	m_map*	com_pool								= init_mem ( sizeof(com_line), 2048 );
	m_map*	job_mem								= init_mem ( sizeof(job_t), 4048);
	memory_initalize->node_pool				= node_pool;
	memory_initalize->link_pool				= link_pool;
	memory_initalize->com_pool					= com_pool;
	memory_initalize->job_mem					= job_mem;
	return memory_initalize;


}

bool ListAddWork( job_b* list, thread_func function, void* arg){

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


































