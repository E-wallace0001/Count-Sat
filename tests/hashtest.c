#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "h_table.h"



int main(){


hash_t* h_table = hasht_create(32);


table_add( 67,  h_table);
printf(" check : %u \n", check_table( 131, h_table));

hash_t_destroy( &h_table );

	
	return(0);


}
