

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "infini_tree.h"
#include "var_pos.h"
#include "chain_block.h"

//linked list variables
node* head=NULL;
node* root;

//chain_block init
chain_block* chain;

// table of placement of variables
variable_pos* variable_position[1000]={NULL};
long long all=0;

int start=0;
int sub_total=0;
int amount=0;
int loop_count=0;
int variable[253808][10]={0};
int variable_connections[867000][50]={0};
int direct_connections[253808]={0};

int first_connect[10000]={0};

long set_variable[253808]={0};

int clause_size[867000]={0};

int comp_match[1000]={0};

//needs to be the size of all the variables *2
int var_tab[160000]={0};

typedef struct loop_result{
	double accumulator;
	double modifier;
} loop_result;

//this adds a clause to the ones being checked
static inline int var_tab_add(int *clause_a)
{
	//this is the variables already been added to the table
	long *variables;
	variables=set_variable;
	int *clause_var;
	int count=0;

	clause_var=&variable_connections[*clause_a][0];

	//this is the variables in the clause

		for(int x=clause_size[*clause_a];x!=0;x--)
		{
			clause_var++;

			if(*(variables+abs(*clause_var))==*clause_var)
			{
				var_tab[abs(*clause_var)]++;
			}
			else if(var_tab[abs(*clause_var)]==0)
			{
				set_variable[abs(*clause_var)]=(*clause_var);
				var_tab[abs(*clause_var)]++;
			}
			else
			{
			//	//////printf("fail\n");
				return (-1);
			}
		}
	return(0);
}

//this deletes the clauses of the ones being checked
static inline void var_tab_del(int *clause_d)
{
	long *variables;
	variables=set_variable;
	int *clause_var;
	int count=0;

	clause_var=&variable_connections[*clause_d][0];

	//this is the variables in the clause

		for(int x=clause_size[*clause_d];x!=0;x--)
		{
			clause_var++;

			if(*(variables+abs(*clause_var))!=0)
			{
				var_tab[abs(*clause_var)]--;
			}
			if(var_tab[abs(*clause_var)]==0) 
			{
				////////printf("removed %i \n",var_tab[abs(*clause_var)]);
				set_variable[abs(*clause_var)]=0;
				
			}
	
		
		}
	
}

struct v_return{
int connections;
int inc;
int mul;
};

typedef struct v_return vReturn;
//struct v_return alpha;

//look for variables already in the table
//look for variables already in the table
vReturn var_tab_check(int *clause_d){

	int temp_match[30]={0};
	vReturn alpha={0,0,0};
	//this is the variables already been added to the table
	long *variables;
	variables=&set_variable[0];
	int *clause_var;
	int count=0;

	clause_var=&variable_connections[*clause_d][0];

	//amount of variables in the clause

	//this is the variables in the clause

	for(int i=clause_size[*clause_d];i!=0;i--)
	{
		clause_var++;

		if(*(variables+abs(*clause_var))==*clause_var){
			temp_match[0]++;
			temp_match[temp_match[0]]=abs(*clause_var);
			alpha.connections++;
			if(var_tab[abs(*clause_var)]==1){
				alpha.inc++;
			}
	
			if(comp_match[abs(*clause_var)]==0){
				alpha.mul++;
			}
		}
		else if(*(variables+abs(*clause_var))!=0){
			alpha.connections=-1;
			alpha.mul=-1;
			return(alpha);
			break;
		}

	}

	for(int i=1;i<=temp_match[0];i++){
		comp_match[temp_match[temp_match[0]]]++;
	}

	return(alpha);
}

void del_match(int *clause_d){
	long *variables;
	variables=set_variable;
	int *clause_var;
	int count=0;

	clause_var=&variable_connections[*clause_d][0];

//	this is the variables in the clause
	for(int x=clause_size[*clause_d];x!=0;x--){
		clause_var++;
	//	printf("comp_match 1 %i \n",comp_match[abs(*clause_var)]);
		if(comp_match[abs(*clause_var)]>0)
		{
			comp_match[abs(*clause_var)]--;
		}
					
	}
}

static inline int compare_clause(int a, int b){
	int clause_a;
	int clause_b;
	int count=0;
	clause_a=variable_connections[a][0];
	clause_b=variable_connections[b][0];

	for(int x= clause_size[a];x!=0;x--){
		for(int y=clause_size[b];y!=0;y--){
			
			if(abs(variable_connections[a][x])==abs(variable_connections[b][y])){
				if(variable_connections[a][x]==variable_connections[b][y]){
					count++;
				}else{
					return(-1);
				}
			}
		}
	}

return(count);
}




//	search for variable in a given space within head 
static inline node* clause_search(int start, node* head){
	bool error=0;

	head=head->first_clause;
	while(head->clause!=start){

		head=head->next;

		if(head->clause>start){
			error=1;
			break;
		}

	}

	//if the clause can't be found
	if(error==1){
		return(NULL);
	}
	else
	{
		return(head);
	}

}
static inline int direct_check(int a){
	//this is the variables already been added to the table
	int *variables;
	variables=&direct_connections[0];
	int *clause_var;
	int count=0;

	clause_var=&variable_connections[a][0];

	for(int i=clause_size[a];i!=0;i--)
	{
		clause_var++;

		if(*(variables+abs(*clause_var))!=0){
			
			count++;
		}


	}

	return(count);
}

static inline int pre_check(int a, int init_start){
	//this is the variables already been added to the table
	long *variables;
	variables=&set_variable[0];
	int *clause_var;
	int count=0;
	vReturn tests;
	clause_var=&variable_connections[a][0];

tests=var_tab_check(&a);
//return(0);

	for(int i=clause_size[a];i!=0;i--)
	{
		clause_var++;
//return(0);
		//if the variable has already been accounted for in the tree
		if(*(variables+abs(*clause_var))==*clause_var){

			//if the variable has appeared before the start
			if(variable_position[*clause_var]->clause<init_start){
			tests=var_tab_check(&variable_position[*clause_var]->clause);
			if(tests.connections==-1){
			return(0);


			}

				return(1);
			
			}
			
		}


	}

	return(0);
}

static inline void direct_add(int a, int b){
	int clause_a;
	int clause_b;
	int count=0;
	clause_a=variable_connections[a][0];
	clause_b=variable_connections[b][0];

	for(int x= clause_size[a];x!=0;x--){
		for(int y=clause_size[b];y!=0;y--){
			
			if(abs(variable_connections[a][x])==abs(variable_connections[b][y])){
				if(variable_connections[a][x]==variable_connections[b][y]){
					if(direct_connections[variable_connections[a][x]]==0){
						direct_connections[variable_connections[a][x]]++;

					}
				}
			}
		}
	}
}

void direct_del(int a, int b){
	int clause_a;
	int clause_b;
	int count=0;
	clause_a=variable_connections[a][0];
	clause_b=variable_connections[b][0];

	for(int x= clause_size[a];x!=0;x--){
		for(int y=clause_size[b];y!=0;y--){
			
			if(abs(variable_connections[a][x])==abs(variable_connections[b][y])){
				if(variable_connections[a][x]==variable_connections[b][y]){
					if(direct_connections[variable_connections[a][x]]>0){
						direct_connections[variable_connections[a][x]]--;
					}
				}
			}
		}
	}
}

typedef struct determiner_return{
	int start;
	int command;
	bool end;
} determiner_result;


determiner_result determiner_solve(chain_block* chain, node* head, int start){
	determiner_result result;
	result.command=0;
	result.start=0;
	bool negate=0;
	bool negate_store=0;
	bool null_check=0;
	bool equal_variable=0;
	bool disjoint=0;
	bool halt=0;
	bool end_check=0;
	bool start_clause=1;
	//search link
	variable_pos* link;
	//ordered list
	variable_pos* list;
	//stored list
	variable_pos* stored_clause=NULL;
	//temp list
	variable_pos* temporary_clause=NULL;

	// grab the clause
	int clause= head->clause;

	// for each variable in clause, search for previous clauses containing this variable
	for(int x=clause_size[clause];x!=0;x--){

		//if the variable has already been accounted for, skip
		if(variable_connections[clause][x]!=0){ }

		//previous clauses that contain this variable
		link=variable_position[abs(variable_connections[clause][x])];

		//if this is the first time it exists	
		if(link->clause==head->clause){continue;}

		//search through all connections in the list
		while(1){

			//compare the variables of the clause for continuity(possible/negate), and check if there are conflicting variables
			for(int y= clause_size[link->clause]; y!=0;y--){
				printf("clause %i %i \n", variable_connections[clause][x],variable_connections[link->clause][y]);

				//if the variable match
				if(abs(variable_connections[clause][x])==abs(variable_connections[link->clause][y]) ){

					//if there is a direct confirmative match;
					if(variable_connections[clause][x]==variable_connections[link->clause][y] ){ equal_variable=1; break;  }
					//if there isn't a direct match
					else{

						//if a variable appears in the accumulated table
						if(set_variable[abs(variable_connections[link->clause][y])]!=0){
							//if a variable is disjoint from the table
							if(set_variable[abs(variable_connections[link->clause][y])]!=variable_connections[link->clause][y]){
								//this is a no go
								disjoint=1;

								break;
							}
						}
					}
				}
			}
			//**end of clause-variable search

			// if there is a conflicting variable already in the list
			// store the variable, and continue looking for a positive match	
			if (equal_variable==0 && negate==0){ negate=1; stored_clause=link; }
			// there is an equal variable, break and continue with algorithm, else there's a negate
			if(equal_variable==1){

				// if the connection appears before the block, don't save
				if(link->clause>chain->block){stored_clause=link;break;}else{equal_variable==0;}

			} 
			else{ negate_store=1; }

			//if there are no more previous clauses, break
			if(link->next->clause==head->clause){
				end_check=1;
				break;
			}else{
				link=link->next;

			}
		}

		if(end_check==1){
			end_check=0; 
			printf(" negate_store %i\n", negate_store);
			continue;
		}

		//**this is the end of looking for the variable connections
		
		printf("stored_clause %i\n", stored_clause->clause);

		// if there is an equal continue, continue to look through the other variables in the clause
		if(equal_variable==1){
			if(link->clause!=variable_position[abs(variable_connections[clause][x])]->clause){
				start_clause=0;
			}else{
				start_clause=1;
			}
			negate_store=0;
			negate=0;
			stored_clause=link;
			break;
		}
		// if there is not an equal
		if(equal_variable==0 && negate_store==0){
			if(link->clause!=variable_position[abs(variable_connections[clause][x])]->clause){
				start_clause=0;
			}else{
				start_clause=1;
			}
			negate_store=1; negate=0; stored_clause=link;  break;
		}
	}
	//**this is the end of the search variables of the orriginal clause


	//if there is no connection
	if(stored_clause==NULL){result.command=0;return(result);}

	//if the connection appears after the start, continue from this point
	if(stored_clause->clause > start){

		//store the clause
		result.start=stored_clause->clause;

		//if it's a negate
		if( negate_store==1 ){  result.command=3; }

		//if it's a connection;
		else{	result.command=1;	}
	}
	else{

		result.command=2;
		result.start=start;
	}

	if(start_clause==0){result.end=1;}

	return(result);
}

int post_branch_correct(int,int,int,int, node*,node*);
int pre_branch_correct(int, int, int, node*,node*);


int pre_branch_correct(int init_start,int init_end, int con,node* head, node* tree){
		determiner_result determiner;
	node* head_temp=head;
	node* outer_loop;
	node* inner_loop;
	node* layer_temp;
	node* inner_layer;
	node* outer_layer;
	node* search_layer;
	node* temp_layer;
	node* outer_end;
	node* begin;
	node* check_var;
	node* inner_temp;
	node *outer_saved;
	node* clause_temp2=head;
	vReturn var_connections;
	vReturn primary_variable_connections;
	vReturn secondary_variable_connections;
	vReturn outer_connections;
	vReturn temp_connections;
	int inner_total=0;
	float removed=0;
	int sum=0;
	int total_var_con=0;
	int total=0;
	int match=0;
	int sum2=0;
	int total2=0;
	int new_total=0;
	int clause_comp=0;
	int k=0;
	int total3=0;
	bool init_seed=0;
	int pre_loop=0;
	node* temp_end;
	if(head_temp==NULL){
		printf("ERROR: NULL VALUE PTR\n");
		return(-1);
	}
//	printf("erroor %i %i \n",init_end, head->clause);
//	head_temp=clause_search(init_end, head);
	

	layer_temp=clause_search(init_start, head);
	temp_layer=clause_search(init_start, head_temp);


	temp_end=clause_search(init_end, tree);
	temp_end=temp_end->next_layer;
	temp_end=clause_search(init_start, temp_end);
	printf("start data %i removed %i size %i \n", temp_end->data,temp_end->removed,temp_end->size);

//	keep sum
	total=temp_end->data-temp_end->removed;
printf("total %i \n", total);

	temp_connections=var_tab_check(&layer_temp->clause);
	var_connections=var_tab_check(&init_start);
// if there is a match,else it's a negate and no readdition is required

	if(con>=0){
	int clause_comp2=compare_clause(init_start, init_end);
clause_comp=compare_clause(init_start,head->previous_layer->clause);
		total=total/pow(2,clause_size[head->previous_layer->clause]-clause_comp2);
		
		removed=temp_end->removed/pow(2,clause_size[head->previous_layer->clause]-1);
		total+=removed;
		printf("temp->data: %i temp->removed %i \n",temp_end->data, temp_end->removed);
		printf("remoed: %f total %i \n",removed, temp_connections.connections);

	}
	else{
		printf("*******-del ago-*********\n");
		printf("temp->data: %i temp->removed %i \n",temp_layer->data, temp_layer->removed);
	
		total=total/pow(2,clause_size[init_end]);
		new_total=total;
	}
	printf("				sum total %i %i \n",total,head->clause	);
	
	outer_layer= append_layer(init_start,head,head,0,0 );
	outer_layer->removed=removed;
	outer_layer->data= total;

	if(init_end-layer_temp->next->clause==0){
		printf("no loop break!!!!!!!\n");
		printf("layer removed %i \n",total);

		
		return(total);
	}

printf("error\n");
outer_loop=layer_temp->next;
inner_loop=layer_temp->next;

	printf("clause out: %i clause in: %i\n",outer_loop->clause, init_end);
//	scale the total for the dependant layer
	//

int inner_sub_total=sub_total;

while(outer_loop->clause<init_end){

	printf("///sub_total %i \\\\\\ \n\n", sub_total);

	primary_variable_connections=var_tab_check(&outer_loop->clause);

	if(primary_variable_connections.connections==-1){
		printf("this is an exit(0)\n");
		//exit(0);
		if(outer_loop->next!=NULL){
				
			outer_loop=outer_loop->next;
		}
		else{
			break;
		}
		continue;
	}

	printf(" %i primary_variable_connections: %i\n",outer_loop->clause,inner_loop->clause);

	//scale the summed
	total2=total/pow(2,clause_size[init_end]);
	
	//	seed the initial starting point for inner layer
	if(init_seed==0){
		init_seed=1;
		begin=outer_loop;
		//scale the the size of the problem
		//subtract the second layer from this to get the final valuse
		
		printf(" %i primary_variable_connections: %i\n",begin->clause,primary_variable_connections.connections);

		//	the sum of the second layer: for initial seed, subtract this from sub total to get second result

		//	find the next clause it can be
		vReturn check= var_tab_check(&outer_loop->next->clause);
		check_var=outer_loop;
		if(check.connections==-1){
			
			printf("\n\n\n\n\n\n\n\n	***error***\n");
				printf("init start:%i, init_en:%i\n",init_start, init_end);
				

			while(check.connections==-1){
				check_var=check_var->next;
				check= var_tab_check(&check_var->clause);
				
				if(check_var->clause>init_end){
							
						printf("%i\n",outer_loop->clause);
				//	break;
				}
			}		

		}

		if(begin->clause-outer_loop->clause<=1){
			check= var_tab_check(&outer_loop->clause);
			clause_comp=compare_clause(outer_loop->clause,init_end);
			printf("***************************\ncheck %i clause_comp %i\n", check.connections,clause_comp);
		}else{
			clause_comp=0;
		}


		sub_total=sub_total/pow(2,clause_size[check_var->clause]);
		inner_sub_total=inner_sub_total/pow(2,clause_size[outer_loop->clause]-check.connections);

		var_connections=var_tab_check(&check_var->clause);

	//	append clause goes here
		outer_layer= append_clause(outer_loop->clause,outer_layer,outer_layer->previous_layer,0,0,0 );

		//if the first one appears before the start this needs to be modified
		//****modify this!!!!!!***
		total2=total/pow(2,clause_size[outer_loop->clause]-check.connections);

			printf("start total %i\n", total);
		//	printf("var_con %i %i %i \n", outer_loop->clause, check_var->clause, con);
			
			printf("start total2 %i\n", total2);
			
			int total3=total2/pow(2,clause_size[outer_loop->clause]);
		
			int new_result= inner_sub_total-total3;

			
		int temp_ans= inner_sub_total-total2;
		total+=inner_sub_total-total2;
		
	//	prepare layer outer/inner layer
		outer_layer->removed=temp_ans;

		outer_layer->data=total;

		printf("outer->data: %i outer_remvedd %i  new_result:%i \\\\\\ \n\n", outer_layer->data,outer_layer->removed,	inner_sub_total);

		printf("///inner_sub_total %i total:%i total2:%i new_total:%i \\\\\\ \n\n", inner_sub_total,total,total2,inner_sub_total	);
		

		sub_total=sub_total*pow(2,clause_size[outer_loop->clause]);	
		
	}
	else{

		sub_total=sub_total/pow(2,clause_size[outer_loop->clause]);	
		
		inner_loop=outer_layer->first_clause;

		outer_layer=append_clause(outer_loop->clause, outer_layer,outer_layer->previous_layer,0,0,0);

		clause_comp=compare_clause(outer_loop->clause,inner_loop->clause);
		var_connections=var_tab_check(&outer_loop->clause);
		printf("clause_comp %i \n", var_connections.connections);

		determiner=determiner_solve(chain, outer_loop,init_start );

		switch(determiner.command)
		{
			case 0:
				//use data instead of removed and take away from sub_total
				total2=(sub_total)-outer_layer->previous->data/pow(2,clause_size[outer_loop->clause]-var_connections.connections);
			break;
			case 1:
				//if there's a connection after the start, restart from here
				printf("this is post start %i %i  \n",determiner.start,outer_loop->clause);
				//if(outer_layer->clause==10){determiner.start=8;determiner.end=1;} 
				var_tab_add(&outer_layer->clause);
				total2= post_branch_correct(determiner.end,determiner.start,outer_layer->clause,1,outer_layer,outer_layer);
				total2=sub_total-total2;
				var_tab_del(&outer_layer->clause);				
			break;
			case 2:	
				//if there's a connect before the start, recall the previous, and continue from the start
				printf("pre_start-pre_start %i \n",temp_end->clause);
				//exit(0);

				var_tab_add(&outer_layer->clause);
				total2=pre_branch_correct(init_start,outer_layer->clause,1,outer_layer,temp_end);
				total2=sub_total-total2;
				printf("post start->pre_loop %i \n",total2);
				//exit(0);
				//return(7544);
				//total2=648/2;
				var_tab_del(&outer_layer->clause);
			break;
			case 3:
				var_tab_add(&outer_layer->clause);
				total2= post_branch_correct(determiner.end,determiner.start,outer_layer->clause,1,outer_layer,outer_layer);
				total2=sub_total-total2;
				var_tab_del(&outer_layer->clause);
			break;
		}

	}
	//end of conditional pre_loop
	
	if(outer_loop->clause!=begin->clause){
		sub_total=sub_total*pow(2,clause_size[outer_loop->clause]);	
		total+=total2;
		outer_layer->data=total;
		outer_layer->removed=total2;
		printf("\n%i removed:%i total:%i total2:%i \n\n", outer_layer->data,outer_layer->removed, sub_total,total	);
		outer_end=outer_layer;

	}

	if(outer_loop->next!=NULL)
	{
		outer_loop=outer_loop->next;
	}
	else{
		break;
	}
}  

printf("total: %i init outer %i %i \n",total, sub_total,total);

return (total);
}
int post_branch_correct(int not_start,int init_start,int init_end, int con,node* head, node* destination){
	determiner_result determiner;
	determiner_result search_determiner;
	node* head_temp=head;
	node* outer_loop;
	node* inner_loop;
	node* layer_temp;
	node* inner_layer;
	node* outer_layer;
	node* temp_layer;
	node* search_layer;
	node* outer_end;
	node* begin;
	node* check_var;
	node* inner_temp;
	node *outer_saved;
	node* clause_temp2=head;
	node* pre_layer;
	vReturn var_connections;
	vReturn primary_variable_connections;
	vReturn secondary_variable_connections;
	vReturn outer_connections;
	vReturn temp_connections;
	int inner_total=0;
	float removed=0;
	int sum=0;
	int total_var_con=0;
	int total=0;
	int match=0;
	int sum2=0;
	int total2=0;
	int new_total=0;
	int clause_comp=0;
	int k=0;
	int total3=0;
	bool init_seed=0;
	int pre_loop=0;
	node* temp_end;
	node* start_data;
	bool break_loop=0;
	int clause_diff=0;
int log_redux=0;
	if(head_temp==NULL){
		printf("ERROR: NULL VALUE PTR\n");
		return(-1);
	}

	layer_temp=clause_search(init_start, head);
	temp_layer=clause_search(init_start, head_temp);

	printf("start value %i %i  \n", init_start, init_end);

	//initial total
	total=temp_layer->data-temp_layer->removed;
	printf("total %i \n", total);
	temp_connections=var_tab_check(&init_end);
	var_connections=var_tab_check(&init_start);
	int fe=direct_check(init_end);

	if(var_connections.connections>=0){
		if(not_start==0){
			clause_comp=compare_clause(init_start,init_end);
			total=total/pow(2,clause_size[init_end]);
			new_total=total;
			removed=temp_layer->removed/pow(2,clause_size[init_end]-1);

		}
		else{
			start_data=temp_layer;

//			printf("start_data %i\n", start_data->next_layer->end->data);

			clause_diff= clause_size[init_end]- clause_size[init_start];

			//	if the difference between the two clauses is different, compensate from the ansers
			if(clause_diff<0){
				total=start_data->next_layer->end->data*pow(2, abs(clause_diff));
			} else if(clause_diff>0){

				total=start_data->next_layer->end->data/pow(2,clause_diff);
			}
			else{
				total=start_data->next_layer->end->data/pow(2,clause_diff);
			}
			removed=temp_layer->removed/pow(2,clause_size[init_end]-1);
			printf("total %i removed %f \n", total, removed);
			printf("not end\n");

		}

		printf("remoed: %f total %i \n",removed, total);

			total+=removed;
		
		printf("	temp->data: %i temp->removed %i \n",temp_layer->data, temp_layer->removed);
		printf("remoed: %f total %i \n",removed, total);
		printf("*/*/*/*/*/*/*/clause comp %i\n", clause_comp);
		
	}
	else{
		
		printf("*******-del ago-*********\n");
		printf("temp->data: %i temp->removed %i \n",temp_layer->data, temp_layer->removed);
	
		total=total/pow(2,clause_size[init_end]);
		new_total=total;
		printf("total %i\n", total);

	}

	outer_layer= append_layer(init_start,destination,destination,0,0 );
	outer_layer->removed=removed;
	outer_layer->data= total;

	if(init_end-layer_temp->next->clause==0 ){
		printf("no loop break!!!!!!!\n");
		printf("layer removed %i \n",total);
		//total+=removed;
		return(total);
	}


printf("error\n");
outer_loop=layer_temp->next;
inner_loop=layer_temp->next;

	printf("clause out: %i clause in: %i\n",outer_loop->clause, init_end);
//	scale the total for the dependant layer
	//

int inner_sub_total=sub_total;

while(outer_loop->clause<init_end){

	printf("///sub_total %i \\\\\\ \n\n", sub_total);

	primary_variable_connections=var_tab_check(&outer_loop->clause);

	if(primary_variable_connections.connections==-1){
		printf("this is an exit(0)\n");
		//exit(0);
		if(outer_loop->next!=NULL){
				
			outer_loop=outer_loop->next;
		}
		else{
			break;
		}
		continue;
	}

	printf(" %i primary_variable_connections: %i\n",outer_loop->clause,inner_loop->clause);

	//scale the summed
	total2=total/pow(2,clause_size[init_end]);
	
	//	seed the initial starting point for inner layer
	if(init_seed==0){
		init_seed=1;
		begin=outer_loop;
		//scale the the size of the problem
		//subtract the second layer from this to get the final valuse
		
		printf(" %i primary_variable_connections: %i\n",begin->clause,primary_variable_connections.connections);

		//	the sum of the second layer: for initial seed, subtract this from sub total to get second result

		//	find the next clause it can be
		vReturn check= var_tab_check(&outer_loop->next->clause);
		check_var=outer_loop;
		if(check.connections==-1){
			
			printf("\n\n\n\n\n\n\n\n	***error***\n");
				printf("init start:%i, init_en:%i\n",init_start, init_end);
				

			while(check.connections==-1){
				check_var=check_var->next;
				check= var_tab_check(&check_var->clause);
				
				if(check_var->clause>init_end){
							
						printf("%i\n",outer_loop->clause);
				//	break;
				}
			}		

		}

		if(begin->clause-outer_loop->clause<=1){
			check= var_tab_check(&outer_loop->clause);
			clause_comp=compare_clause(outer_loop->clause,init_end);
			printf("***************************\ncheck %i clause_comp %i\n", check.connections,clause_comp);
		}else{
			clause_comp=0;
		}


		sub_total=sub_total/pow(2,clause_size[check_var->clause]);
		inner_sub_total=inner_sub_total/pow(2,clause_size[outer_loop->clause]-check.connections);

		var_connections=var_tab_check(&check_var->clause);

	//	append clause goes here
		outer_layer= append_clause(outer_loop->clause,outer_layer,outer_layer->previous_layer,0,0,0 );

		//if the first one appears before the start this needs to be modified
		//****modify this!!!!!!***
		total2=total/pow(2,clause_size[outer_loop->clause]-check.connections);

			printf("start total %i\n", total);
		//	printf("var_con %i %i %i \n", outer_loop->clause, check_var->clause, con);
			
			printf("start total2 %i\n", total2);
			//if(total2==2592){exit(0);}
			int total3=total2/pow(2,clause_size[outer_loop->clause]);
		
			int new_result= inner_sub_total-total3;

			
		int temp_ans= inner_sub_total-total2;
		total+=inner_sub_total-total2;
		
	//	prepare layer outer/inner layer
		outer_layer->removed=temp_ans;
printf(" total %i total2 %i prev %i \n",total,new_result,outer_loop->data	);
		outer_layer->data=total;
//if(total==120704){exit(0);}
		printf("outer->data: %i outer_remvedd %i  new_result:%i \\\\\\ \n\n", outer_layer->data,outer_layer->removed,	inner_sub_total);

		printf("///inner_sub_total %i total:%i total2:%i new_total:%i \\\\\\ \n\n", inner_sub_total,total,total2,inner_sub_total	);

		sub_total=sub_total*pow(2,clause_size[outer_loop->clause]);	
		
	}
	else{

		sub_total=sub_total/pow(2,clause_size[outer_loop->clause]);	
		
		inner_loop=outer_layer->first_clause;

		outer_layer=append_clause(outer_loop->clause, outer_layer,outer_layer->previous_layer,0,0,0);

		clause_comp=compare_clause(outer_loop->clause,inner_loop->clause);
		var_connections=var_tab_check(&outer_loop->clause);
		printf("clause_comp %i \n", var_connections.connections);

		determiner=determiner_solve(chain, outer_loop,init_start );

		switch(determiner.command)
		{
			case 0:
				//use data instead of removed and take away from sub_total
				total2=(sub_total)-outer_layer->previous->data/pow(2,clause_size[outer_loop->clause]-var_connections.connections);
			break;
			case 1:
				//if there's a connection after the start, restart from here
				printf("this is post start %i %i  \n",determiner.start,outer_loop->clause);
				//if(outer_layer->clause==10){determiner.start=8;determiner.end=1;} 
				var_tab_add(&outer_layer->clause);
				total2= post_branch_correct(determiner.end,determiner.start,outer_layer->clause,1,outer_layer,outer_layer);
				total2=sub_total-total2;
				var_tab_del(&outer_layer->clause);				
			break;
			case 2:	
				//if there's a connect before the start, recall the previous, and continue from the start
				printf("***post start->pre_loop %i %i %i \n",init_start, determiner.start,outer_layer->clause);

				//if(chain->chain_number==0){ printf("no block");exit(0);}


				//if there's a previous connection, all previous connections have been accounted for and all previous needs to be ignored.
				//chain=remove_block(chain);
				//for all the previous
				search_layer=outer_loop->next_layer->end;
//if(search_layer->clause==10){	chain=add_block(chain,5);}
printf("error\n");
				while(1){

					search_determiner=determiner_solve(chain,search_layer,init_start );
					//if it appears before
					if(search_determiner.command==2){
						
						//append layer num=start; previous end
						pre_layer=append_layer(search_layer->clause,outer_layer,outer_layer,0,0);
						
						//if search==init start, createone here

						//shrink total

						//var_tab_add

						printf("init_ start %i search_layer %i\n",init_start, search_layer->clause);	
						printf("this is a connection before %i \n", search_layer->next_layer->end->previous->previous->removed);
						printf("this is a connection before %i \n", head->previous->next_layer->end->next_layer->next->next->data);
						search_layer=search_layer->next_layer->end;
						printf("clause %i removed %i\n",search_layer->previous->previous->data,search_layer->previous->previous->removed);
						printf(" pre_lay %i \n",sub_total	);
						log_redux=1;
						//exit(0);
						//this appears before	
					}

					if(search_layer->clause==init_start && log_redux==1){
						chain=add_block(chain,5);
						
						printf("\n\n\n\n\n\n\n\n\n\n\n\n///****************clause %i removed %i\n",search_layer->previous_layer->clause, search_layer->removed);
					//	exit(0);
						sub_total=sub_total/4;
					//total2= post_branch_correct(0,7,10,1,search_layer,pre_layer);
						//
						printf("end %i %i \n",search_layer->data,search_layer->removed);
						exit(0);
						//pre_layer->removed=sub_total-pre_layer->next_layer->end->data;
						//pre_layer->data=pre_layer->next_layer->end->data*8-pre_layer->removed;
						printf("pre %i %i \n",pre_layer->data, pre_layer->removed);
						sub_total=sub_total*4;
						chain=remove_block(chain);
						printf("search_layer %i %i \n", total2,pre_layer->data);
						//exit(0);//2592	
					}


					// this is the end
					if(pre_layer->previous_layer!=NULL && pre_layer->previous_layer->clause==outer_loop->clause){
						//printf("pre_lay %i \n", pre_layer->next_layer->data);
						//exit(0);
					}else{  }

					//Error: If there are no more connections 
					if(search_layer->previous==NULL){
						printf("ERROR:No previous search\n");
						exit(0);
					}

					//if the previous isn't the start, continue backwards
					if(search_layer->clause!=init_start){
						search_layer=search_layer->previous;
					}
					else{ break;}
				}

				//if(log_redux==1){
						//chain block

						//post_branch_correct

						//sub_total=sub_total*4
						//var_tab_del

				//}
				

/////////
			
				var_tab_add(&outer_layer->clause);
				total2=pre_branch_correct(init_start,outer_layer->clause,1,outer_layer,outer_layer->previous_layer);
				total2=sub_total-total2;
if(outer_loop->clause==11&& head->clause==12){
printf("total2 %i\n", total2);
 exit(0);
}
				var_tab_del(&outer_layer->clause);
			break;
			case 3:
				var_tab_add(&outer_layer->clause);
				total2= post_branch_correct(determiner.end,determiner.start,outer_layer->clause,1,outer_layer,outer_layer);
				total2=sub_total-total2;
				var_tab_del(&outer_layer->clause);
			break;
		}

	}
	//end of conditional pre_loop
	
	if(outer_loop->clause!=begin->clause){
		sub_total=sub_total*pow(2,clause_size[outer_loop->clause]);	
		total+=total2;
		outer_layer->data=total;
		outer_layer->removed=total2;
		printf("\n%i removed:%i total:%i total2:%i \n\n", outer_layer->data,outer_layer->removed, sub_total,total	);
		outer_end=outer_layer;
		
	

	}

	if(outer_loop->next!=NULL)
	{
		outer_loop=outer_loop->next;
	}
	else{
		break;
	}
}

printf("total: %i init outer %i %i \n",total, sub_total,total);

return (total);
}


// for each base check the previous one
int recursive_check(int layer, int begin_layer,int j,int previous_total, node* previous_layer){
	//printf("*********layer %i",layer);
	int sum=0;
	layer++;
	int total_connections=0;
	vReturn var_connections;	
	int match=0;
	node* clause_node=NULL;
	node* clause_temp =NULL;
	node* previous_result_next=NULL;
	node* inner_loop=NULL;
	node* previous_result=previous_layer->first_clause;
	int current_total=0;
	int sum2=0;	
	int result=0;
	int clause_connections=0;
	int recursive_total=0;
	int next_layer_start=0;
	int match_connections=0;
	int temp=0;
	int k=0;
	node* removed_head;
	determiner_result determiner;
	bool break_loop=0;
	// for each clause
	for (int i=begin_layer; i<j; i++){

		if(layer==1){
			printf("i:%i J:%i\n",i,j);
		}
		//check how many connections there are that exists from this clause
		var_connections=var_tab_check(&i);

		//if in the second layer, there is a connection, fix it
		if(var_connections.connections!=0 && layer>1){

			printf("this is the connection %i %i \n", previous_result->clause,previous_layer->clause);

			// create the linked list object
			clause_temp=append_layer(i,previous_layer,previous_layer,0,0);

			printf("sub_total %i \n ",sub_total);
				
			printf("***A Connection has been made** %i-%i\n",previous_layer->clause, previous_result->clause);

			//	if the clause is a match, add it to the checked list
			if(var_connections.connections>0 ){
				var_tab_add(&previous_layer->clause);
			}

			k = post_branch_correct(0,previous_result->clause,previous_layer->clause,0,previous_layer,previous_layer);

			//	delete the clause from the checked list
			if(var_connections.connections>0){
				var_tab_del(&previous_layer->clause);
			}

			printf("k %i \n ",k);

			sum=k;
			sum2=k;
		
			break;					
		}

		//how many in total exist as a subset
		recursive_total=previous_total/pow(2,clause_size[i]-var_connections.connections);
		

		//if the place is past the initial state
		if(i>1){

			clause_connections=clause_size[j]-var_connections.connections;
			current_total=previous_result->removed/pow(2,clause_connections);

			//	continue to the next layer
			if(layer==1){
				//	add the root clause to the table
				layer++;	
				var_tab_add(&i);
				clause_temp=append_clause(i,clause_node,previous_layer,0,0,clause_connections);
				sub_total=sub_total/pow(2,clause_size[i]);
				
				determiner=determiner_solve(chain,clause_temp,1 );
				
				switch(determiner.command)
				{
					case 0:
					break;
					case 1:

						//if(i==10){determiner.start=8;determiner.end=1;} 
						var_tab_add(&clause_temp->clause);
						result= post_branch_correct(determiner.end,determiner.start,clause_temp->clause,1,clause_temp,clause_temp);
						var_tab_del(&clause_temp->clause);
						printf("determiner start %i \n",determiner.start);

						printf("result %i\n",result);
						//if(clause_temp->clause==11){exit(0);result=251209;}
						break_loop=1;
					break;
					case 3:
						printf("this is del %i \n",determiner.start);

						printf("this is post start %i %i  \n",determiner.start,i);

						printf("post start->del start %i \n",var_connections.connections);

						var_tab_add(&clause_temp->clause);
						result= post_branch_correct(determiner.end,determiner.start,clause_temp->clause,1,clause_temp,clause_temp);
						var_tab_del(&clause_temp->clause);

						break_loop=1;
					break;
				}

				if(break_loop==0){
					result=(clause_temp->previous->data/pow(2,clause_size[i]));
				}
				else{
					break_loop=0;
				}
printf("******TEST: data %i clause_temp %i \n", clause_temp->previous->data,clause_temp->previous->removed);
				printf("TEST: result %i-%i\n",result,temp);
				sub_total=sub_total*pow(2,clause_size[i]);
				var_tab_del(&i);
				layer--;
			}

			sum+=current_total-result;
			sum2+=recursive_total-result;
					
			int k=0;
			
			//update the clause
			if(layer==1){
				clause_temp->data=sum2;
				clause_temp->removed=recursive_total-result;
				clause_temp->data2=sum;
			}

			//	move on to the next clause
			clause_node=clause_temp;
						
		}
		else{

			sum=recursive_total;

			if(layer==1){
				clause_node=append_layer(i,previous_layer,previous_layer,sum,clause_connections);
				sum2=recursive_total;
			}			
		}
	

	if(previous_result->next!=NULL){
		previous_result=previous_result->next;
	}
}

printf("end sum %i \n", sum);

layer--;
return(sum);
}

int main(int argc, char *argv[]){

// variable for current clause 
int current_clause=1;

//opens file for reading
FILE *fp;
if (argc > 1) {
	fp = fopen(argv[1], "r");
} else {
	//////////printf("fail \n");
}

unsigned int nr_variables=1;
unsigned int nr_clauses=1;

// reads the amount of variables and clauses
fscanf(fp, "p cnf %i	 %u\n", &nr_variables, &nr_clauses);
//////////printf("number of variables %u \n Number of clauses %u \n", nr_variables, nr_clauses);
int cant=0;
//stores temporary literals
int tmp[35]={0};

//stores all the ones
//int ones[10000]={0};
//stores variable number in clause
int b=0;
int end2=0;
int cl=1;
//scans file
int literal;
int unload=0;
//17.39.7
//long int num_check[10000]={0};
int ones[4000]={0};
while (1){
	if (fscanf(fp, "%d", &literal) != 1){
		//f//printf(stderr, "error: expected literal\n");
		exit(EXIT_FAILURE);
	}
	if (literal!=0){
		//stores which literal
		tmp[b]= literal;
		b++;			
	}

	//at the end of each line
	if (literal == 0){
		variable_connections[0][0]++;
		for (int unload=0;unload<b;unload++){

			//stores where each variable is store			
			variable_connections[variable_connections[0][0]][0]=b;

			//convert variable to progessive number
			if(variable[abs(tmp[unload])][1]==0){
				////////////printf("variable status %i %i \n",abs(tmp[unload]), variable[abs(tmp[unload])][0]);
				variable[0][0]++;
				variable[abs(tmp[unload])][0]=variable[0][0];
				variable[abs(tmp[unload])][1]=1;
			}


			if(tmp[unload]>0){
				variable_connections[variable_connections[0][0]][unload+1]=variable[abs(tmp[unload])][0];
			}

			if(tmp[unload]<0){
				variable_connections[variable_connections[0][0]][unload+1]=-variable[abs(tmp[unload])][0];
			}
			
			if(variable_position[variable[abs(tmp[unload])][0]]==NULL){
				variable_position[variable[abs(tmp[unload])][0]]=append_variable(cl,NULL);

		
			}
			else{	

					append_variable(cl,variable_position[variable[abs(tmp[unload])][0]]);
printf("cl %i\n", variable_position[variable[abs(tmp[unload])][0]]->end->clause);

			}



		}
		if(b==1){
			ones[0]++;
			ones[ones[0]]=variable_connections[0][0];
		}
		clause_size[variable_connections[0][0]]=b;
		//resets clause size
		b=0;
		//increments clause count
		cl++;
	}


	//if read all clauses
	if(cl==nr_clauses+1){
		//////////printf("variables %i\n",variable[0][0]);
		//////////printf(" this is the amount of clauses %d \n ", cl-1);
		break;
	}
}

//**********************
//start of the algorithm
//**********************

//the total number of all possibilities
all = pow(2,variable[0][0]);

long long add=0;
long long add2=0;
long int exist=0;

long long remain=0;
long int left=0;



// chain command init
chain=create_block();
//chain_block* chain;
//chain=create_block();


vReturn polarity;
int *one;
one=ones;
 start=variable_connections[0][0];
int variables=0;

node* clause_node=NULL;
printf("variables %i \n", variable[0][0]);
	
//for the root cluase
for(int j=start;j<=start;j++)
{
	
	//printf("start %i \n",j);

	if(polarity.connections==-1){
		continue;
	}				

	
	sub_total= all/pow(2,variable_connections[start][0]);

	clause_node=append_clause(j, clause_node,NULL,sub_total,0,variable_connections[start][0]);

	//add the variables of the clause to the table
	//var_tab_add(&j);
		
	//recursion begins
	int total_remove=recursive_check(0,1,j,sub_total,clause_node);

	//delete the variables from the table;
	//var_tab_del(&j);

	sub_total-=total_remove;

	node* result=clause_node->next_layer;
	while(result!=NULL){
		printf("%i clause node %i + %i \n", result->clause,result->data,result->removed);

		result=result->next;
	}

//TEST PARAMETERS
//printf("sub_total %lli \n",all/4-clause_node->next_layer->end->data);
	
}

printf("\ninit_test\n");  

//printf("head %i\n", count(head));

dispose(head);

return(0);
}


