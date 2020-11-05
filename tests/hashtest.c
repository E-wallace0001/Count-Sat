#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <assert.h>
#include "../source/headers/h_table.h"



int main(){


	printf(" createing hash table \n");
	
	hash_t* h_table = hasht_create( 65537);
	
	assert( h_table !=NULL );
	
	printf(" hash table complete\n");
	
	printf(" adding to hash table\n");
	int val[10] = { 1, 5 , 8 , 9, 16 , 17, 355, 2566, 3586 , 312358564 };
	
	
	for(size_t i = 0 ; i< 10; i++){ 
		table_add( val[i], h_table);
	}
	
	printf("adding complete\n");
	
	printf(" Checking hashtable \n");
	
	for( size_t i =0; i <10; i++){
		assert( check_table( val[i], h_table) == 1 );
	}
	printf(" found all in val\n");

	printf(" checking values not in table\n");
	int val2[10] = { 2, 3, 18, 22, 366, 33567, 36655, 312358563, 268888,3123584 };
	
	for( size_t i =0; i <10; i++){
		assert( check_table( val2[i], h_table) != 1 );
	}
	printf(" values not found \n");
	
	printf(" deleteing hash table \n");
	hash_t_destroy( &h_table);
	
	printf(" Finished with no problems \n");
	
	return(0);


}
