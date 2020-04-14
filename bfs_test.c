
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "var_pos.h"
#include "bfs_s.h"
#include "stest.h"
#include "clause_func.h"
#include "stest.h"
#include <gmp.h>

#include "infini_tree.h"
// find all the set variables
//	ones[];
/*
if(ones[0]==0) //printf(" there are no set variables\n"); exit(0);
*/


variable_pos* set;
variable_pos* tmp;
	variable_pos* set_copy=NULL;
bool clause_visited[9000]={0};
bool inline_visited[8000]={0};
int  new_old_clause[8000]={0};
bool counted_set[8000]={0};

void raw(){

	for(int clause=1;clause<=f_clause_count;clause++){

	set=copy_clause(clause, set);
	}

}
void init_graph(int ones[]){

//raw();
//solve();
//debug(clause_node);
//exit(0);
	variable_pos* temp_clause;	
	int count=0;

	layer* layer=set_layer(-1,4);
printf("ones %i \n ",ones[0]);

int k=0;

set= (variable_pos*)malloc(sizeof(variable_pos));

	for(int i=1;i<=ones[0];i++){
		layer->num++;

		printf("start %i %i %i \n", layer->num,variable_count,clause_count); 
		//clause_count++;

		//copy the original
		//set=copy_clause(ones[i], set);
		printf(" cl_Sz %i %i \n", clause_size[ones[i]],i);
		clause_visited[ones[i]]=1;
		//mark which orriginal clauses have been visited
		//inline_visited[ones[i]]=1;
//null_add(set);
		for(int variable=f_clause_size[ones[i]];variable!=0;variable--){

			// collect all clauses connected to this variable
			temp_clause=f_variable_position[abs(f_variable_connections[ones[i]][variable])];
			printf("tmp cl %i clause_count %i \n",temp_clause->clause,clause_count);

			//coppy all the clauses
			while(1){
				//if it's already been accounted for
				if(clause_visited[temp_clause->clause]==1){

					if(temp_clause->next!=NULL){
						temp_clause=temp_clause->next;
					}else{
						layer->num--;
						break;
					}
					continue;
				}
				count++;


				//set it to vistied
				//clause_visited[temp_clause->clause]=1;

				set=copy_clause(temp_clause->clause, set);

				printf(" init found && added %i %i \n",set->clause,temp_clause->clause);
printf("erro\n");
				// copy the variably data to the new clause so it can be processed
				//
				if(temp_clause->next!=NULL){
				//clause_visited[temp_clause->clause]=1;

				inline_visited[temp_clause->clause]=1;
					temp_clause=temp_clause->next;
				}else{
					break;
				}
			}

		}
printf("**** ones %i\n",ones[i]);
if(count==0){
clause_visited[ones[i]]=0;
continue;
}
		inline_visited[abs(ones[i])]=1;
		clause_visited[temp_clause->clause]=1;
		//printf(" %i \n",);

		bfs_graph(ones[i], layer,set->first,set->first);
		set=set->first;
		printf(" i %i \n", i);
		printf(" split-------------------------\n");

//copy_clause(1,set);;


//copy_clause(ones[1],set);
//variable_count++;

//create_new_clause();
//add_to_clause(-3,set);
//var_tab_add(&set->end->clause);


//create_new_clause();
//add_to_clause(-1,set);
//var_tab_add(&set->end->clause);
//variable_count--;
//create_new_clause();
//add_to_clause(2,set);
//var_tab_add(&set->end->clause);

//clause_count--;

//var_tab_add(&set->end->clause);
//copy_removed(set->end->clause,set);
//counted_for++;
//null_add(set);

//variable_count--;
//clause_count--;
//clause_count--;

//create_new_clause();
//add_to_clause(-1,set);
//var_tab_add(&set->end->clause);
//variable_count--;
//clause_count--;


//set_var[abs(k)]=1;
//set_variable[abs(k)]=k;

//copy_clause(ones[2],set);
//set_variable[3]=3;
//set_var[3]=1;
//set_variable[abs(k)]=k*-1;
//var_tab_add(&set->end->clause);

//copy_clause(ones[3],set);

/*
FILE* fp2;
fp2=fopen("new2.cnf","w");

while(set!=NULL){

for(int variable=clause_size[set->clause]; variable!=0;variable--){
fprintf(fp2,"%i ", variable_connections[set->clause][variable]);
}
fprintf(fp2,"0\n");

set=set->next;
}

fclose (fp2);
printf(" %i \n", f_clause_size[ones[1]]);
exit(0);
*/

//set=set->next->previous->end->first->end;
//clause_count++;



//copy
set=set->first;


/*
int k=0;
for(int j=1;j<=ones[0];j++){
printf("ones[j] %i\n", f_variable_connections[ones[j]][1]);
k=f_variable_connections[ones[j]][1];
set_var[abs(k)]=1;
set_variable[abs(k)]=k*-1;
counted_for++;
}


*/
//int j=9;
//k=f_variable_connections[ones[1]][1];

//set=copy_clause(ones[1],set);;

/*
k=f_variable_connections[ones[1]][1];

set_var[abs(k)]=1;
set_variable[abs(k)]=k*-1;
counted_for++;

*/
//k=f_variable_connections[ones[2]][1];

//set_var[abs(k)]=1;
//set_variable[abs(k)]=k*-1;
//counted_for++;

//k=f_variable_connections[ones[3]][1];

//set_var[abs(k)]=1;
//set_variable[abs(k)]=k*-1;
//counted_for++;


//var_tab_add(&clause_count);
//set=copy_clause(ones[1],set);

/*

k=f_variable_connections[ones[1]][1];
set_variable[abs(k)]=k;
counted_set[abs(k)]=1;
var_tab[abs(k)]++;
counted_for++;


//set=copy_clause(ones[2],set);
k=f_variable_connections[ones[2]][1];
set_variable[abs(k)]=k;
counted_set[abs(k)]=1;
var_tab[abs(k)]++;
counted_for++;

//set=copy_clause(ones[3],set);

k=f_variable_connections[ones[3]][1];
set_variable[abs(k)]=k;
counted_set[abs(k)]=1;
var_tab[abs(k)]++;
counted_for++;
*/

set=copy_clause(ones[1],set);
set=copy_clause(ones[2],set);


set=copy_clause(ones[3],set);

//create_new_clause();
//add_to_clause(-5,set);
//clause_count--;



//k=	f_variable_connections[ones[1]][1];
//create_new_clause();
//add_to_clause(k,set);


/*
k=f_variable_connections[ones[2]][1];
create_new_clause();
add_to_clause(k,set);
*/

printf(" 1 %i 2: %i 3: %i \n",f_variable_connections[ones[3]][1],
f_variable_connections[ones[2]][1],f_variable_connections[ones[1]][1]);
halt();
//exit(0);
null_add(set);

printf("%i \n", k);
	

//set_variable[abs(k)]=k;
//counted_set[abs(k)]=1;
//var_tab[abs(k)]++;
//counted_for++;
/*
k=f_variable_connections[ones[2]][1];
//create_new_clause();
//add_to_clause(k,set);
set_variable[abs(k)]=k;
counted_set[abs(k)]=1;
var_tab[abs(k)]++;
counted_for++;
*/
//variable_count--;

//set=copy_clause(ones[2],set);

//set=copy_clause(ones[3],set);
printf("clause_size %i \n", f_variable_connections[ones[1]][1]);

//var_tab_add(&set->end->clause);
//variable_count--;

set=set->first;
//clause_count--;
solve();



//clause_node->next_layer=NULL;

//free((clause_node));
printf(" this is the count %i \n", count_node(clause_node->next_layer));
//clause_node=clause_node->next_layer;
gmp_printf("clause %i Data %Zd  Removed%Zd \n",clause_node->clause,clause_node->data,clause_node->removed);

//debug_pos(set);
debug(clause_node);


//set_var[abs(f_variable_connections[ones[i]][1])]=0;
//var_tab_del(&set->first->clause);




//debug(clause_node);

//		null_remove(set->first->end);
		printf("end count %i \n", count_var_pos(set));
		halt();
	
		set=set->first;
		while(set!=NULL){
			copy_removed(set->clause,set);
			set=set->next;
		}


		//	Zero table
		for(int i =0;i!=300;i++){
			inline_visited[i]=0;
			clause_visited[i]=0;
		}



		printf("end %i %i\n", clause_count,variable_count); 
	}

}
variable_pos* bfs_graph(int old_clause, layer* layer,variable_pos* set,variable_pos* old){

	variable_pos* first=set;
	variable_pos* end=set->first->end;
	variable_pos* temp_var;
	variable_pos* set_saved=set;
	variable_pos* temp_clause;
	variable_pos* var_clause;
	int count=0;

	if(layer->num==layer->lim){
		//layer->num--;
		printf("layer lim \n");
		return(set);
	}

	printf("firstr/end %i %i old %i  \n", first->clause, end->clause,set->clause);
	while(first!=NULL){

		printf("test %i %i \n",new_old_clause[first->clause],f_clause_size[new_old_clause[first->clause]]);
		//	For each of the variables, search for their clauses
		for(int variable=f_clause_size[new_old_clause[first->clause]];variable!=0;variable--)
		{
			
			//loads all the position of *variable;
			temp_clause=f_variable_position[abs(f_variable_connections[new_old_clause[first->clause]][variable])];
		
			//debug_pos(temp_clause);
			if(!temp_clause || count_var_pos(temp_clause)<=1)	{
				printf("not_connected to anything\n");
				continue;
			}

			printf("this is the variable %i cluase: %i   \n", variable,new_old_clause[first->clause]);

			//coppy all the clauses
			while(1){
	
				//if the clause as already been visited, skip
				if(inline_visited[temp_clause->clause]==1  ){
					if(temp_clause->next!=NULL){
						temp_clause=temp_clause->next;
					}else{
						printf("skiped %i \n",temp_clause->clause);
						break;
					}
				continue;
				}
				inline_visited[temp_clause->clause]=1;
				
				printf("\n\n\n this bfs found && added %i %i \n",temp_clause->clause,clause_size[temp_clause->clause]);

				//copy the clause if it's connected
				if(clause_visited[temp_clause->clause]==0 && f_clause_size[temp_clause->clause]>1){
					set=copy_clause(temp_clause->clause,set->first->end);
					clause_visited[temp_clause->clause]=1;
				}
							
				// if the clause of size 1, and hasn't been accounted for, include this has a set variable
				if(f_clause_size[temp_clause->clause]==1 && counted_set[ abs(f_variable_connections[temp_clause->clause][1]) ]==0){
					//counted_set[ abs(f_variable_connections[temp_clause->clause][1]) ]=1;
					//int k=f_variable_connections[temp_clause->clause][1];
					//set_var[ abs(k) ]=1;
					//set_variable[abs(k)]=(k)*-1;
					//counted_for++; 
printf("ding \n"); halt();
				}
				printf(" \n\n\n\n new graph \n");
				layer->num++;
				bfs_graph(0, layer, first, NULL);

	if(layer->num==layer->lim){
		
		printf("layer lim \n");
		return NULL;
	}
layer->num--;

//				inline_visited[temp_clause->clause]=0;	
//				clause_visited[temp_clause->clause]=0;

				printf(" end %i %i %i  \n\n",set->first->end->clause,first->clause,temp_clause->clause);	
		//halt();		
				if(temp_clause->next!=NULL){
					temp_clause=temp_clause->next;
				}else{
					break;
				}
			}
		}

		//inline_visited[first->clause]=0;

		

		//if the end has been reached
		if(first->clause==end->clause){
			printf("end hast been reached \n");
			return(set);
		}
		first=first->next;
	}
//layer->num--;
}


