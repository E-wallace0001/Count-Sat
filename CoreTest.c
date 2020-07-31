
#define _GNU_SOURCE
#include <stdlib.h>
#include <sched.h>
#include <signal.h>

#include <string.h>
#include <sys/types.h>

#include <sys/wait.h>
#include <sys/utsname.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h> 
#include <pthread.h> 
#include "cpu_pool.h"
#include "m_map.h"
#include "coms.h"

int	 fork_t(){
return(fork());
}

typedef int(*fork_ptr)(void *arg);

struct thread{
	//fork_ptr new_thread;
//	struct thread next;
};



typedef struct loc{
	int x;
	int y;
	int l;
	int p;
	int m;
	int z;
}aloc;

int table [104]={0};

int function( void* mem){
	//com_line* data = (com_line*) mem;
	int cnt =122;
	 aloc** k[cnt];
	 //m_map* heap_space = (m_map*)mem;
	 //m_map* heap_space = init_mem( sizeof(aloc), 1040000);
	 printf("hello \n" );
	 //if(data->node_pool==NULL) exit(0);
	 for(int t =0 ; t<2000; t++){
	//printf("tid %lu \n", pthread_self() );	
		for ( int i =0; i<cnt; i++){
			//table[i];
			k[i]=(aloc**)malloc(sizeof(aloc));
			// printf("%p \n", data->node_pool->list_start);
			//k[i]=	alloc_mem( (data->node_pool) );
		}
		
		for ( int i =0; i<cnt; i++){
			//table[i];
			//table[i];
			
			free(k[i]);
			//printf(" i %i \n", i );
			//release_mem(k[i], data->node_pool);
		}
	}//
	
	return(0);
}


int main(int argc, char **argv){	
	
	//aloc* mem[1040000] ;
	
	
	 pthread_t thread[10];
	 
	size_t thread_count	= 4;
	
	//job_b* WorkerBees 				= CreateWorkers(thread_count);
//	for (int w =0; w<2;w++){
	//	com_line* data= (com_line*)malloc(sizeof(com_line));
		//data = memory_init();

		int buffer;

		 //void* Stack = malloc(stack_size);
		 //void* StackEnd = (char*)Stack + stack_size;
		 //void* ptr = StackEnd;
		 //printf(" ptr %p \n", Stack);
		//int pid = clone(function, (char*)ptr, CLONE_VM, 0);
		int pipefd[2];
		int string = 200;
		int nBytes;
		pipe(pipefd);
		pid_t pid[4];
		//thread* thrd = malloc(sizeof(*thrd));
	for( int i = 0; i<1;i++){
		pid[i]=fork();
		//pid[i]=thread->new_thread();
		printf( "pid %i \n", pid[i]);
		if(pid[0]==-1)  printf("errorr\n");
		
	}
	
	if(pid[0]==0){	
	 	close(pipefd[0]);
	 	
	   write(pipefd[1], (char*)&string, (sizeof(string)));
	   printf(" string %i \n", string);
		//function(NULL);
		exit(0);
	}else{
	close(pipefd[1]);
		nBytes=read(pipefd[0],(char*)&buffer, sizeof(buffer));
	 	printf(" recieved  %i\n", buffer);
		int returnStatus; 
		waitpid(pid[0], &returnStatus, 0);
	}
	printf(" this is a go %i\n",pid[0]);
	
	//usleep(10);
	//WaitForWorkers(WorkerBees);

	//AbortWork(WorkerBees);
/*
	int num =1024;
	for( int count =0; count<num; count++){
		mem[count]= alloc_mem( heap_space );
		mem[count]->x=0;
		mem[count]->z=0;
		mem[count]->l=0;
		mem[count]->m=0;
		
	}
	
	for(int x =0; x<num;x++){
		release_mem(mem[x], heap_space);
	}
		defrag_mem(heap_space);
		printf("%p %p\n", heap_space->list_start->address, heap_space->start);
*/
	
		//free_mem(heap_space);

	return(0);
}
