
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "var_pos.h"
#include "bfs_s.h"
#include "stest.h"

// find all the set variables
//	ones[];
/*
if(ones[0]==0) printf(" there are no set variables\n"); exit(0);
*/


bool clause_visited[300]={0};

void init_graph(int ones[]){
	variable_pos* set=NULL;

	layer* layer=set_layer(0,2);

	for(int i=1;i<=ones[0];i++){
		set=create_pos(ones[i],set);
		printf(" found %i \n",set->clause);

		bfs_graph(ones[i], layer, set);

	}
}

void bfs_graph(int clause, layer* layer, variable_pos* set){
	variable_pos* temp_var;
	variable_pos* set_saved=set;
	variable_pos* temp_clause;

	layer->num++;

	if(layer->num==layer->lim){
		//solve
		printf(" this is the end\n");
		layer->num--; 
		return;
	}

printf("not the end \n");

/*
	//for each variable in clause, add each clause that also connects with this variable
	for(int i=clause_size[clause];i<0;i--){
		// collect all clauses connected to this variable
		temp_clause=variable_position[abs(variable_connections[clause][i])];

		//coppy all the clauses
		while(1){
			//if it's already been accounted for
			if(clause_visited[temp_clause->clause]==1){
				if(temp_clause->next){
					temp_clause=temp_clause->next;
				}else{
					break;
				}
			continue;
			}
			//create the link
			set=create_pos(temp_clause->clause, set);
			//set it to visited
			clause_visited[temp_clause->clause]=1;
			if(temp_clause->next){
				temp_clause=temp_clause->next;
			}else{
				break;
			}
		}
	}

	// for each added clauses, 
	temp_var=temp_clause->first;
	while(1){
		bfs_graph(temp_var->clause, layer, temp_var);

		if(temp_var->next){
			temp_var=temp_var->next;
		}else{
			break;
		}
	}

	//delete all the clauses
		while(1){
			//remove visited
			clause_visited[temp_var->clause]=0;			
			//create the link
			temp_var=pop_pos(set);

			if(!temp_var || temp_var->clause==set_saved->clause){
				break;
			}
		}
*/	
}
