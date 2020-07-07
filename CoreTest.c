
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h> 
#include <pthread.h> 
#include "cpu_pool.h"


void function( ){

//printf("tid %lu \n", pthread_self() );
for ( int i =0; i<10000; i++){
int  b =1 +2;
}
//usleep(1000000);

}


int main(int argc, char **argv){
	
	job_b*	JobList;
	int*		num;
	size_t	i;
	size_t	cmpcnt = 8	; 
	
	JobList=CreateWorkers( cmpcnt );
	
	for( i=0; i<100000; i++){
	
		ListAddWork( JobList, function, NULL);
	}

	WaitForWorkers(JobList);

	return 0;
}
