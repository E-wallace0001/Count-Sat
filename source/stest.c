


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "headers/stest.h"
#include "headers/infini_tree.h"

#include "headers/llist.h"
#include "headers/bfs_s.h"
#include "headers/clause_func.h"
#include "headers/cnf_read.h"
#include <gmp.h>
#include "headers/var_pos.h"
#include "headers/stest.h"
#include <assert.h>
#include "headers/cpu_pool.h"
#include "headers/m_map.h"
#include "headers/group.h"
#include <unistd.h> 
#include "headers/simplify.h"



//null bignum
mpz_t nul;

// base(n) -- algo works of any base
mpz_t base;

// table of placement of variables
variable_pos* f_variable_position[csize*2];

void init_position(variable_pos* table[]){
	for(long int  i=1;i<vsize;i++){
		table[i]					= make_clause(0);
		table[i]->first		= table[i];
		table[i]->end			= table[i];
		table[i]->previous	= NULL;
		table[i]->next			= NULL;
	}
}

void destroy_position(variable_pos* table[]){
for(long int  i=1;i<vsize;i++){
		free(table[i]);
	}
}

int f_clause_size[csize*2]={0};

// pause the program by interjecting the code
void halt(){
	while(1){
		char input;
		scanf("%c", &input);
		 if( getchar() != 0 )
		 	break;
	}
}

//interject code to look at the current state of a node
void debug(node *head){

	if(head==NULL){printf(" DEBG NUL\n");return;}	
	char input;
	while (1){

		scanf("%c", &input);
		if(getchar()!=0){
		}

		switch(input){
			case 'w':
				if(head->next_layer==NULL){ 
					printf("no upper lever\n");
				}
				else{ head=head->next_layer; 
	gmp_printf("next_layer \n clause %i \n Data %Zd \n Removed %Zd \n", head->clause, head->data, head->removed);
			}
			break;	
			case 'a':
				if(head->previous==NULL){ printf("no previous\n");
				}
				else{ head=head->previous; 
	gmp_printf("previous \n clause %i \n Data %Zd \n Removed %Zd \n", head->clause, head->data, head->removed);

				}
			break;
			case 's':
				if(head->previous_layer==NULL){ 
	printf("no previous_layer\n");
				}
				else{ head=head->previous_layer; 
	gmp_printf("previous_layer \n clause %i \n Data %Zd \n Removed %Zd \n", head->clause, head->data, head->removed);

				}
			break;
			case 'd':
				if(head->next==NULL){ 
	printf("no next\n");
				}
				else{ head=head->next; 
	gmp_printf("next \n clause %i \n Data %Zd \n Removed %Zd \n", head->clause, head->data, head->removed);

				}
			break;
			case 'x':
				return;
			break;
			}
	}
printf(" output: %c \n", input);
}


//this marks the variables in a clause has checked
void var_tab_add(int *clause_a, com_line* Coms)
{
	//this is the variables already been added to the table
	int* var_tab= Coms->var_tab;
	long *variables;
	variables=Coms->set_variable;
	int *clause_var;
	int** variable_connections	= Coms->clause_connections;
	long* set_variable			= Coms->set_variable;

	clause_var						=	&variable_connections[*clause_a][0];

	//this is the variables in the clause

		for(int x=Coms->clause_size[*clause_a];x!=0;x--)
		{

			clause_var++;

			//connection_count[abs(*clause_var)]++;
			if(*(variables+abs(*clause_var))==*clause_var)
			{
				var_tab[abs(*clause_var)]++;
				
			}
			else if(var_tab[abs(*clause_var)]==0)
			{	
				set_variable[abs(*clause_var)]=(*clause_var);
				var_tab[abs(*clause_var)]++;
			}
		}
}

//this deletes the clauses of the ones been checked
inline void var_tab_del(int *clause_d, com_line* Coms)
{
	int* var_tab= Coms->var_tab;
	long *variables;
	variables=Coms->set_variable;
	int *clause_var;

	clause_var=&Coms->clause_connections[*clause_d][0];

	//this is the variables in the clause

	for(int x=Coms->clause_size[*clause_d];x!=0;x--){
		clause_var++;

if(counted_set[*(variables+abs(*clause_var))]==1){continue;}

			
		if(*(variables+abs(*clause_var))!=0)
		{
			var_tab[abs(*clause_var)]--;
		}
		if(var_tab[abs(*clause_var)]==0) 
		{
			//connection_count[abs(*clause_var)]--;
			Coms->set_variable[abs(*clause_var)]=0;	
		}
		
	}
}

struct v_return{
	int connections;
	int inc;
	int mul;
};

typedef struct v_return vReturn;

//check to see if variables in clause have been checked
vReturn var_tab_check(int *clause_d, com_line* Coms){
	int* var_tab= Coms->var_tab;
	int temp_match[100]={0};
	vReturn alpha={0,0,0};
	//this is the variables already been added to the table
	long *variables;
	variables=(Coms->set_variable);
	int *clause_var;
	bool* set_var = Coms->set_var;

	clause_var=&Coms->clause_connections[*clause_d][0];


	for(int i=Coms->clause_size[*clause_d];i!=0;i--){
		clause_var++;
		if(*(variables+abs(*clause_var))==*clause_var ){

			temp_match[0]++;
			temp_match[temp_match[0]]=abs(*clause_var);

			alpha.connections++;

			if(var_tab[abs(*clause_var)]==0 || set_var[abs(*clause_var)]==0){
				alpha.mul++;
			}
			
			if(var_tab[abs(*clause_var)]>1 || set_var[abs(*clause_var)]!=0){
				alpha.inc++;
			}

		}
		else if(*(variables+abs(*clause_var))!=0){

			alpha.connections=-1;

			return(alpha);
		}

	}
	return(alpha);
}

//check to see if variables in clause have been accounted for, whether negated our connects
vReturn comp_tab_check(int *clause_d, com_line* Coms){
	int* var_tab= Coms->var_tab;
	int temp_match[100]={0};
	vReturn alpha={0,0,0};
	//this is the variables already been added to the table
	long *variables;
	variables=&Coms->set_variable[0];
	int *clause_var;

	clause_var=&Coms->clause_connections[*clause_d][0];

	//amount of variables in the clause

	//this is the variables in the clause

	for(int i=Coms->clause_size[*clause_d];i!=0;i--){
		clause_var++;

		if(*(variables+abs(*clause_var))==*clause_var){
			temp_match[0]++;
			temp_match[temp_match[0]]=abs(*clause_var);
			alpha.connections++;

			if(var_tab[abs(*clause_var)]>1){
				alpha.inc++;
			}

		}
		else if(*(variables+abs(*clause_var))!=0){
			alpha.connections++;
		}

	}
	return(alpha);
}

//check to see if variables in clause have been accounted for, whether negated our connects
vReturn neg_tab_check(int *clause_d, com_line* Coms){

	vReturn alpha={0,0,0};
	//this is the variables already been added to the table
	long *variables;
	variables=&Coms->set_variable[0];
	int *clause_var;

	clause_var=&Coms->clause_connections[*clause_d][0];

	//amount of variables in the clause
	//this is the variables in the clause
	for(int i=Coms->clause_size[*clause_d];i!=0;i--){
		clause_var++;

		if(*(variables+abs(*clause_var))!=*clause_var){
		
			if(*(variables+abs(*clause_var))!=0){
				alpha.connections++;
			}
		}
	}
	return(alpha);
}

//check to see if variables in clause have been checked
vReturn abs_tab_check(int *clause_d, com_line* Coms){

	int* var_tab 			= Coms->var_tab;
	int temp_match[100]  = {0};
	vReturn alpha			= {0,0,0};
	//this is the variables already been added to the table
	long *variables		= &Coms->set_variable[0];
	int *clause_var		= Coms->clause_size;
	clause_var 				= &Coms->clause_connections[*clause_d][0];

	for( int i = Coms->clause_size[*clause_d]; i!=0; i-- ){
		clause_var++;

		if(*(variables+abs(*clause_var)) == *clause_var){
			temp_match[0]++;
			temp_match[temp_match[0]] = abs(*clause_var);
			alpha.connections++;
			if(var_tab[abs(*clause_var)]>1){
				alpha.inc++;
			}
		}
	}
	return(alpha);
}

//compares variables of two clauses
static inline int compare_clause(int a, int b, com_line* Coms){
	int count=0;
	int** variable_connections = Coms->clause_connections;
	int* clause_size			  = Coms->clause_size;
	bool* set_var				  = Coms->set_var;

	for(int x= clause_size[a];x!=0;x--){
		for(int y=clause_size[b];y!=0;y--){
			
			if( abs(variable_connections[a][x]) == abs(variable_connections[b][y]) && set_var[abs(variable_connections[a][x])] == 0 ){
			
				if( variable_connections[a][x] == variable_connections[b][y] ){
					count++;
				}else{
					return(-1);
				}
			} 
			
		}
	}

return(count);
}


link_node* FindFVariableInRange( int variable, link_node* Node){
	if(Node == NULL) {
		printf("null fvarng \n"); return NULL;
	}
	if( Node->next!=NULL) Node=Node->next;
	while(1){
	
		for( int var= f_clause_size[Node->data]; var !=0; var--){

			if( abs( f_variable_connections[ Node->data ][ var ] ) == abs( variable ) ){
				printf( " variable %i \n", variable);
			 return Node;
			}
			
		}	
					
		if(Node->next==NULL) break;
		Node= Node->next;
	}
	return NULL;

}



//	search for variable in a given space within head 
static inline node* clause_search( int start, node* head ){

	if(head==NULL){
		printf(" null cls srch \n");
		exit(0);
	}
	
	bool error		= 0;
	bool reverse	= 0;
	head				= head->first_clause;
	
	while( head->clause != start ){

		if( reverse == 0 ){

			if( head->next != NULL && head->next->clause <= start ){
				head = head->next;
			}else{
				break;
			}

			if( head->clause >= start ){
				break;
			}

		}
		else{
			head = head->previous;

			if( head->clause <= start ){
				break;
			}
		}
	}

	//if the clause can't be found
	if( error == 1 ){
		return(NULL);
	}else{
		return(head);
	}

}


typedef struct determiner_return{
	int start;
	int command;
	int end;
} determiner_result;


int clause_contains_set(int clause, com_line* Coms){
	int count=0;
	for(int variable=Coms->clause_size[clause];variable!=0;variable--){
		if(Coms->set_var[abs(Coms->clause_connections[clause][variable])]!=0){count++;}
	}
return(count);
}

//determines where a variable has been connected, and how to responed
determiner_result determiner_solve( node* head, int start, node* root,com_line* Coms){


	determiner_result result;
	result.command							= 0;
	result.start							= 0;
	bool negate_store						= 0;
	bool pre_non_con						= 0;
	bool pre_con_match					= 0;
	int clause_comp						= 0;
	variable_pos** variable_position	= Coms->variable_position;
	int** variable_connections			= Coms->clause_connections;
	
	vReturn 			temp_connections;
	variable_pos*	link;
	variable_pos*	stored_clause		= NULL;
	variable_pos*	saved_clause 		= NULL;

	// grab the clause
	int clause								= head->clause;

	// for each variable in clause, search for previous clauses containing this variable
	for( int q = Coms->clause_size[clause]	;q!=0;q-- ){

		//if the variable has already been accounted for, skip
		if( Coms->set_var[abs(variable_connections[clause][q])] == 1 )
			continue;

		//if the variable mateches before the start
		if( variable_position[abs(variable_connections[clause][q])]->clause < head->clause ){
			pre_con_match = 1;
		}

		//if the variable has already appeared in the chain
		if( Coms->set_variable[abs(variable_connections[clause][q])] != 0 ){
			pre_non_con = 1;
			continue;		
		}
		
		//previous clauses that contain this variable
		link = variable_position[abs(variable_connections[clause][q])];

		while(1){

			if(compare_clause(head->clause, link->clause, Coms) == -1)
				break;

			if(clause_contains_set(link->clause,Coms) != 0)
				break;

			if(var_tab_check(&link->clause, Coms).connections >= 0)
				break;

			if(link->next != NULL && link->next->clause > head->clause){
				link=link->next;
			}else{
				break;
			}		
		}
		//if this is the first time it exists	
		if(saved_clause  == NULL && link->clause != head->clause){
			saved_clause  = link;
		}
		if(saved_clause != NULL && link->clause < saved_clause->clause){
			saved_clause  = link;
		}
	}

	if( saved_clause != NULL ){
		result.start	  = saved_clause->clause;
		stored_clause	  = saved_clause;
		result.end		  = 0;
		result.command   = 1;

		//if the previous branch is connected
		if(var_tab_check(&head->clause, Coms).connections != 0){
			result.end = 1;
		}

		if(stored_clause->clause <= start){
			result.start 		= start;
			result.command		= 2; 
			int clause_comp 	= compare_clause(result.start, head->clause, Coms);
			result.end 			= 2;
			
			if(var_tab_check(&result.start, Coms).connections != 0)
				result.end = 1;
			
			if(clause_comp != 0)
				result.end = 1;
		}

		return(result);
	}else{
		result.end		=	0;
		result.command	=	0;
		return(result);
	}

	//this is the end of the search variables of the orriginal clause

	return(result);
}



void pre_branch_correct(int not_start,int init_start,int init_end, node* pre_start,node* head, node* tree,com_line* Coms){

	mpz_t* sub_total 		= Coms->sub_total;
	int* 	 clause_size	= Coms->clause_size;
	determiner_result determiner;
	node* head_temp=head;
	node* outer_loop;
	node* layer_temp;
	node* outer_layer;
	vReturn var_connections;
	vReturn primary_variable_connections;
	vReturn temp_connections;
	
	
	node* temp_end;

	//total previous bisections
	mpz_t total;
	mpz_init(total);
	
	mpz_t exponent;
	mpz_init(exponent);

	mpz_t total2;
	mpz_init(total2);

	// bisection of all previous nodes - Already accounted for sets
	mpz_t removed;
	mpz_init(removed);

	// temp result
	mpz_t temp;
	mpz_init(temp);


	if(head_temp == NULL || head == NULL){
		printf("ERROR: NULL VALUE PTR\n");
		exit(0);
	}
	
	layer_temp = clause_search(init_start, head);

	temp_end = clause_search(init_end, tree);

	if(temp_end->next_layer == NULL){
		printf(" no next_layer\n");
		exit(0);
	}
	
		temp_end = temp_end->next_layer;
		temp_end = clause_search(init_start, temp_end);
	

	if(temp_end == NULL){
		printf("ERROR101 \ncheck %i\n", tree->clause);
		exit(0);
	}


	//	keep sumpre_branch
	//total=temp_end->data-temp_end->removed;
	mpz_sub(total, temp_end->data, temp_end->removed);

	var_connections=var_tab_check(&init_start, Coms);


	// if the start and end are negated, do this
	if( var_connections.connections==-1 ){

		if( temp_end->previous!=NULL 
			&& temp_end->previous->clause>=init_start 
			&&  var_connections.connections==-1 ){
			if(compare_clause(temp_end->previous->clause, init_end, Coms)==-1){

				while(1){
				if(temp_end->previous!=NULL){
					temp_end=temp_end->previous;
				}
				else{
					break;
				}
				if(compare_clause(temp_end->clause, init_end, Coms)!=-1){break;}
				}
			}
		}
	}


	outer_layer= append_layer(temp_end->clause,head,head,nul, Coms);

	if(var_connections.connections>=0){

		switch(not_start)
		case 0:{	

			// basic pre-start

			var_tab_del(&init_end, Coms);
			var_tab_add(&init_end, Coms);
			mpz_ui_pow_ui( exponent, 2, clause_size[pre_start->clause]-var_tab_check(&pre_start->clause, Coms).inc );
			mpz_divexact(total,total,exponent);
			
			mpz_ui_pow_ui( exponent, 2, clause_size[pre_start->clause]-var_tab_check(&pre_start->clause, Coms).inc);
			mpz_divexact(removed,temp_end->removed,exponent);

			mpz_add(total,total,removed);

			break;
		case 1:
			//if a connections has already been accounted for
			mpz_sub(total, temp_end->data, temp_end->removed);


			// start to pre
			
			mpz_ui_pow_ui( exponent, 2, clause_size[pre_start->clause]-var_tab_check(&pre_start->clause, Coms).inc);
			mpz_divexact(total, total, exponent );

			//pre-process var_tab_check
			var_tab_add(&init_start, Coms);
			var_tab_del(&pre_start->clause, Coms);
			
			
			mpz_ui_pow_ui( exponent, 2, clause_size[pre_start->clause]-var_tab_check(&pre_start->clause, Coms).connections);
			mpz_divexact(removed,temp_end->removed,exponent);

			//post process
			var_tab_del(&init_start, Coms);
			var_tab_add(&pre_start->clause, Coms);

			mpz_add(total,total,removed);

			break;
		case 2:
			
			///continue from a previous variable

			mpz_sub(total, temp_end->data, temp_end->removed);
			// how many variables there are that haven't been accounted for
			//total=total/pow(2,var_connections.inc)


			var_tab_add(&init_start, Coms);
			var_tab_del(&init_end, Coms);
						temp_connections=var_tab_check(&init_end, Coms);
			var_tab_add(&init_end, Coms);

			var_tab_del(&pre_start->clause, Coms);
				var_connections=var_tab_check(&pre_start->clause, Coms);
			var_tab_add(&pre_start->clause, Coms);
			var_tab_del(&init_start, Coms);

			int res=clause_size[pre_start->clause]-var_connections.connections;

			if(res<=0){
//				printf("zero %i %i \n",clause_size[pre_start->clause],var_connections.connections);
				res=0;
			}

			mpz_ui_pow_ui( exponent, 2, clause_size[pre_start->clause]-var_connections.connections);
			mpz_divexact(temp, temp_end->removed,exponent); //(a^b)^(b^c) ¬(a^b^c)  
			
			mpz_ui_pow_ui( exponent, 2, clause_size[pre_start->clause]-temp_connections.connections);
			mpz_divexact(total,total, exponent); //(b^c)
			mpz_set(removed, temp);
			mpz_add(total,total, temp);

			break;
		}		
	}
	else{
			//If the start is negate


			//  if it's negate, find one previous with which is acceptable!
			//temp_end=temp_end->previous;

//			temp_end=tmp;

			mpz_sub(total, temp_end->data, temp_end->removed);

			var_tab_del(&pre_start->clause, Coms);
			temp_connections=var_tab_check(&init_start, Coms);
			var_tab_add(&pre_start->clause, Coms);

//printf( " start %i - end %i \n", init_start, init_end);
		if(compare_clause(init_start,init_end, Coms)!=-1 && temp_connections.connections==-1){
			mpz_set(total, temp_end->data);
		}

		var_tab_del(&pre_start->clause, Coms);
		var_connections=abs_tab_check(&pre_start->clause, Coms);
		temp_connections=abs_tab_check(&head->clause, Coms);
		var_tab_add(&pre_start->clause, Coms);


//connections and inc have been changed!

		mpz_ui_pow_ui( exponent, 2, clause_size[pre_start->clause]-var_connections.connections);
		mpz_divexact(total,total,exponent);

		mpz_ui_pow_ui( exponent, 2, clause_size[pre_start->clause]-var_connections.connections);
		mpz_divexact(removed,temp_end->removed,exponent);

		if(compare_clause(init_end,init_start, Coms)==-1 && abs_tab_check(&init_end, Coms).connections!=0 ){
			mpz_add(total, total, removed);
		}
		//gmp_printf(" total %Zd removed %Zd \n", total, temp_end->data);

			mpz_set(removed, temp_end->removed);
			

//		if( neg_tab_check(&pre_start->clause).connections==clause_size[pre_start->clause]) printf(" ding\n");
	
		// if all variables have been accoutned for whether negated or connected for
		if(  neg_tab_check(&pre_start->clause, Coms).connections == clause_size[pre_start->clause]){
			mpz_ui_pow_ui( exponent, 2, clause_size[pre_start->clause]);
			mpz_divexact(total,temp_end->data,exponent);
		}
	}

	//outer_layer->removed=removed;
	mpz_set(outer_layer->removed,removed);	
	//outer_layer->data= total;
	mpz_set(outer_layer->data,total);


	//there isn't another clause to check
	if(layer_temp->next==NULL || init_end-layer_temp->clause==1){

		mpz_clear(exponent);
		mpz_clear(total);
		mpz_clear(total2);
		mpz_clear(removed);
		mpz_clear(temp);
		return;
	}


outer_loop=layer_temp->next;


while(outer_loop->clause<init_end){

	primary_variable_connections=var_tab_check(&outer_loop->clause, Coms);
	

	if(primary_variable_connections.connections==-1 ){

		if(outer_loop->next != NULL && outer_loop->next->clause != init_end){
			
			outer_loop = outer_loop->next;
		}else{
			break;
		}
		continue;
	}

	//scale the summed
	//total2=total/pow(2,clause_size[init_end]);
	
	mpz_ui_pow_ui( exponent, 2, clause_size[init_end] );
	
	mpz_divexact(total2, total,exponent);


	outer_layer 		= append_clause(outer_loop->clause, outer_layer,outer_layer->previous_layer,0,0, Coms);
	var_connections	= var_tab_check(&outer_loop->clause, Coms);
	determiner 			= determiner_solve( outer_loop,init_start,outer_loop, Coms);

 	//printf(" det start %i out %i end %i cmmand %i \n",determiner.start,outer_loop->clause,determiner.end,determiner.command);

	switch(determiner.command)
		{
			case 0:
				mpz_ui_pow_ui( exponent, 2, clause_size[outer_loop->clause]-var_connections.connections );
				mpz_divexact(*sub_total,*sub_total,exponent);

				//use data instead of removed and take away from sub_total
				mpz_ui_pow_ui( exponent, 2, clause_size[outer_loop->clause]-var_connections.connections );
				mpz_divexact(total2,outer_layer->previous->data,exponent);
				mpz_sub(total2, *sub_total, total2);

				mpz_ui_pow_ui( exponent, 2, clause_size[outer_loop->clause]-var_connections.connections );
				mpz_mul(*sub_total,*sub_total,exponent);

			break;
			case 1:
				mpz_ui_pow_ui( exponent, 2, clause_size[outer_loop->clause]-var_connections.connections );
				mpz_divexact(*sub_total,*sub_total, exponent );

				//if there's a connection after the start, restart from here
				var_tab_add(&outer_layer->clause, Coms);

				post_branch_correct(determiner.end,determiner.start,outer_layer->clause,1,outer_layer,Coms);
				//collects data from the end of the tree
				mpz_set(total2, outer_layer->next_layer->end->data);
			
				mpz_sub(total2, *sub_total,total2);
	
				var_tab_del(&outer_layer->clause, Coms);

				mpz_ui_pow_ui( exponent, 2, clause_size[outer_loop->clause]-var_connections.connections );
				mpz_mul(*sub_total,*sub_total,exponent);


			break;
			case 2:
				//if there's a connect before the start, recall the previous, and continue from the start
				mpz_ui_pow_ui( exponent, 2, clause_size[outer_loop->clause]-var_connections.connections );
				mpz_divexact(*sub_total,*sub_total,exponent);

				var_tab_add(&outer_layer->clause, Coms);

				pre_branch_correct(determiner.end,init_start,outer_layer->clause,pre_start,outer_layer,temp_end,Coms);

			if(outer_layer->next_layer!=NULL){
				mpz_set(total2,outer_layer->next_layer->end->data);
				mpz_sub(total2,*sub_total,total2);
			}
			else
			{ 
				mpz_ui_pow_ui( exponent, 2, clause_size[outer_loop->clause]-var_connections.connections);
				mpz_divexact(total2,outer_layer->previous->data,exponent);
				mpz_sub(total2, *sub_total, total2);
			}
				
				var_tab_del(&outer_layer->clause, Coms);

				mpz_ui_pow_ui( exponent, 2, clause_size[outer_loop->clause]-var_connections.connections);
				mpz_mul(*sub_total,*sub_total,exponent);

			break;
		}
			

	
	//end of conditional pre_loop
	if(outer_loop->clause!=init_start){
		mpz_add(total,total,total2);
		mpz_set(outer_layer->data,total);
		mpz_set(outer_layer->removed,total2);
	}

	if(outer_loop->next!=NULL)
	{
		outer_loop=outer_loop->next;
	}
	else{
		break;
	}
}
mpz_clear(exponent);
mpz_clear(total);
mpz_clear(total2);
mpz_clear(removed);
mpz_clear(temp);

}

void post_branch_correct(int not_start,int init_start,int init_end, int con,node* head, com_line* Coms){
	
	mpz_t* sub_total = Coms->sub_total;
	
	determiner_result determiner;
	node* head_temp=head;
	node* outer_loop;
	node* inner_loop;
	node* layer_temp;
	node* outer_layer;
	node* temp_layer;
	vReturn var_connections;
	vReturn primary_variable_connections;

	int  clause_comp = 0;
	int* clause_size = Coms->clause_size;

	mpz_t exponent;
	mpz_init(exponent);
	//total previous bisections
	mpz_t total;
	mpz_init(total);

	mpz_t total2;
	mpz_init(total2);

	// bisection of all previous nodes - Already accounted for sets
	mpz_t removed;
	mpz_init(removed);

	//temporary midway between two equations
	mpz_t temp;
	mpz_init(temp);

	if(head_temp==NULL){
		printf("ERROR: NULL VALUE PTR\n");
		exit(0);
	}

	layer_temp=clause_search(init_start, head);
	temp_layer=clause_search(init_start, head_temp);

	//initial total
	mpz_sub(total,temp_layer->data,temp_layer->removed);
	
	if(var_tab_check(&init_start, Coms).connections>=0){	
		switch(not_start)
		case 0:{ 

			//No connections
			//Scales the last data
			clause_comp=compare_clause(init_start,init_end, Coms);
			
			mpz_ui_pow_ui( exponent, 2, clause_size[init_end]-var_tab_check(&init_end, Coms).inc );
			mpz_divexact(total,total, exponent);
			
			mpz_ui_pow_ui( exponent, 2, clause_size[init_end]-clause_comp);
			mpz_divexact(removed, temp_layer->removed,exponent);
			mpz_add(total,total,removed);

			
			break;
		case 1:

			///continue from a previous clause, that's been connected before


			// count how many variables bisect with this clause
			var_tab_del(&init_end, Coms);
			var_tab_add(&init_start, Coms);
			var_connections=var_tab_check(&init_end, Coms);
			var_tab_del(&init_start, Coms);
			var_tab_add(&init_end, Coms);

			clause_comp=compare_clause(init_start,head->clause, Coms);

			mpz_sub(total, temp_layer->data, temp_layer->removed);

			 // how many variables there are that haven't been accounted for
			clause_comp=compare_clause(init_start, init_end, Coms);
 
			// Scale the previous data to generate next layer
			mpz_ui_pow_ui( exponent, 2, clause_size[init_end]-var_tab_check(&init_end, Coms).inc );
			mpz_divexact(total,total, exponent);
			mpz_ui_pow_ui( exponent, 2, clause_size[init_end]-var_connections.connections );
			mpz_divexact(temp, temp_layer->removed, exponent);

			mpz_set(removed, temp);	
			mpz_add(total, total, removed);
			break;
		}
	}
	else{

		//If the start is negate
		if(temp_layer->previous!=NULL && layer_temp->previous!=NULL ){
			temp_layer=temp_layer->previous;
			layer_temp=layer_temp->previous;
			
		}

		var_connections=var_tab_check(&head->clause, Coms);

		var_tab_del(&init_end, Coms);
		var_connections=abs_tab_check(&init_end, Coms);
		var_tab_add(&init_end,Coms);
		mpz_sub(total, temp_layer->data, temp_layer->removed);

		var_tab_del(&init_end,Coms);
		var_tab_add(&init_end,Coms);


		
		mpz_ui_pow_ui( exponent, 2, clause_size[init_end]-var_connections.connections );
		mpz_divexact(total,total, exponent);
		
		mpz_divexact(removed,temp_layer->removed,exponent);

		if(var_tab_check(&init_start, Coms).connections!=-1 || init_start!=1){
			mpz_add(total,total, removed);
		}

		var_tab_del(&init_end, Coms);
	// if all variables have been accoutned for whether negated or connected for
		if(comp_tab_check(&init_end, Coms).connections== clause_size[init_end]){
			mpz_sub(total, temp_layer->data, temp_layer->removed);
			
			mpz_ui_pow_ui( exponent, 2, clause_size[init_start] );
			mpz_divexact(total,total, exponent  );
		}
		var_tab_add(&init_end,Coms);
	}



	outer_layer= append_layer(temp_layer->clause,head,head,nul, Coms);

	mpz_set(outer_layer->removed,removed);
	mpz_set(outer_layer->data,total);

	if(layer_temp->next!=NULL
		&&var_tab_check(&layer_temp->next->clause, Coms).connections==-1) {

		while(var_tab_check(&layer_temp->next->clause, Coms).connections==-1){
			if(layer_temp->next!=NULL){
				layer_temp=layer_temp->next;
			}
			else{
				break;
			}
		}
	}
	
	if(layer_temp->next==NULL ||init_end-layer_temp->clause==1 ){
mpz_clear(exponent);
mpz_clear(total);
mpz_clear(total2);
mpz_clear(removed);
mpz_clear(temp);
		return;
	}

outer_loop=layer_temp->next;
inner_loop=layer_temp->next;

while(outer_loop->clause<head->clause){
primary_variable_connections=var_tab_check(&outer_loop->clause, Coms);

	if(primary_variable_connections.connections==-1){
		
		if(outer_loop->next!=NULL && outer_loop->next->clause<init_end){
			outer_loop=outer_loop->next;
		}
		else{
			break;
		}
		continue;
	}
	
	inner_loop=outer_layer->first_clause;

	//if this clause is already negated
	

	outer_layer=append_clause(outer_loop->clause, outer_layer,outer_layer->previous_layer,0,0, Coms);
	clause_comp=compare_clause(outer_loop->clause,inner_loop->clause, Coms);
	var_connections=var_tab_check(&outer_loop->clause, Coms);

	if(var_connections.connections==-1){
		 printf("outer loop negate \n"); exit(0);
	}

	determiner=determiner_solve( outer_layer,init_start ,outer_loop,Coms);

	//if there's a connections between init start, and current variable, do pre_branch
	switch(determiner.command)
		{
			case 0:

				mpz_ui_pow_ui( exponent, 2, clause_size[outer_loop->clause]-var_connections.connections );
				mpz_divexact(*sub_total,*sub_total, exponent);


				//use data instead of removed and take away from sub_total
				mpz_ui_pow_ui( exponent, 2, clause_size[outer_loop->clause]-var_connections.connections );
				mpz_divexact(total2,outer_layer->previous->data,exponent);
				
				mpz_sub(total2,*sub_total,total2);

				mpz_ui_pow_ui( exponent, 2, clause_size[outer_loop->clause]-var_connections.connections );
				mpz_mul(*sub_total,*sub_total,exponent);

			break;
			case 1:
				//post start
				//if there's a connection after the start, restart from here
				mpz_ui_pow_ui( exponent, 2, clause_size[outer_loop->clause]-var_connections.connections );
				mpz_divexact(*sub_total,*sub_total,exponent);

				var_tab_add(&outer_layer->clause,Coms);

				post_branch_correct(determiner.end,determiner.start,outer_layer->clause,1,outer_layer,Coms);
				if(outer_layer->next_layer!=NULL){
					mpz_set(total2,outer_layer->next_layer->end->data);
					mpz_sub(total2,*sub_total,total2);
				}
				else{
					printf("there is no return from post_branch\n");
					exit(0);
					mpz_set(total2,outer_layer->previous->removed);
				}

				var_tab_del(&outer_layer->clause, Coms);
				mpz_ui_pow_ui( exponent, 2, clause_size[outer_loop->clause]-var_connections.connections );
				mpz_mul(*sub_total,*sub_total,exponent);

			break;
			case 2:
				//if there's a connect before the start, recall the previous, and continue from the start
				mpz_ui_pow_ui( exponent, 2, clause_size[outer_loop->clause]-var_connections.connections );
				mpz_divexact(*sub_total,*sub_total,exponent);

				var_tab_add(&outer_loop->clause,Coms);
				pre_branch_correct(determiner.end,determiner.start,outer_layer->clause,outer_layer->previous_layer,outer_layer,head, Coms);

				if(outer_layer->next_layer!=NULL){
					mpz_set(total2,outer_layer->next_layer->end->data);

					mpz_sub(total2,*sub_total,total2);
				}else{
					mpz_ui_pow_ui( exponent, 2, clause_size[outer_loop->clause]-var_connections.connections );
					mpz_divexact(total2,outer_layer->previous->data,exponent);
					mpz_sub(total2,*sub_total,total2);
				}
				var_tab_del(&outer_loop->clause, Coms);

				mpz_ui_pow_ui( exponent, 2, clause_size[outer_loop->clause]-var_connections.connections );
				mpz_mul(*sub_total,*sub_total,exponent);
			break;
			case 3:
		break;
		}
			

	
	//end of conditional pre_loop
	if(outer_loop->clause!=init_start){

		mpz_add(total,total,total2);
		mpz_set(outer_layer->data,total);
		mpz_set(outer_layer->removed,total2);

	}

	if(outer_loop->next!=NULL)
	{
		outer_loop=outer_loop->next;
	}
	else{
		break;
	}
}
mpz_clear(exponent);
mpz_clear(total);
mpz_clear(total2);
mpz_clear(removed);
mpz_clear(temp);


}

int count_negate(int clause, com_line* Coms){
	int count_neg=0;
	for(int variable = Coms->clause_size[clause]	;variable != 0;variable--){

		//	if the variable has already been accounted for, skip
		if( Coms->set_variable[abs(Coms->clause_connections[clause][variable])] == Coms->clause_connections[clause][variable]){
			count_neg++;
		}
	}

return(count_neg);
}

// for each base check the previous one
void recursive_check(int layer, int begin_layer,int j,mpz_t previous_total, node* previous_layer, com_line* Coms){

	mpz_t* sub_total = Coms->sub_total;
	
	layer++;
	vReturn var_connections;	
	node* clause_node=NULL;
	node* clause_temp =NULL;
	node* previous_result=previous_layer;

	if(!previous_layer)exit(0);
	//tracks the current sum
	mpz_t current_total;
	mpz_init(current_total);
	
	// exponent
	mpz_t exponent;
	mpz_init(exponent);

	//result of the previous result
	mpz_t result;
	mpz_init(result);

	//total sum of layer and previous layer
	mpz_t sum;
	mpz_init (sum);

	mpz_t sum2;
	mpz_init (sum2);
	
	int counted = 0;
	
	//total - previously accounted for
	mpz_t union_sub;
	mpz_init(union_sub);

	//local subset option
	mpz_t recursive_total;
	mpz_init(recursive_total);

	int clause_connections	= 0;
	int* clause_size			= Coms->clause_size;
	bool second					= 0;
	
	determiner_result determiner;
	
	// for each clause, check for a previous connection
	for (int i=1; i<=(Coms->clause_count); i++){
		// check how many connections there are that exists from this clause
		var_connections = var_tab_check(&i, Coms);


		// if negated or connections is equal to the clause size of the root
		if(var_connections.connections==-1 || var_connections.inc>=clause_size[i] )
			continue;

		mpz_set_ui(exponent,0);
		mpz_ui_pow_ui( exponent, 2, (clause_size[i]-clause_contains_set(i, Coms)) );
		
		mpz_divexact(recursive_total, previous_total, exponent );

		// if the place is past the initial clause
		if(second==1){
			clause_connections=clause_size[i]-var_connections.connections;

			//scales to include the current clause
			mpz_ui_pow_ui( exponent, 2, (clause_size[i]) );
			mpz_divexact(current_total,previous_result->removed, exponent );

			//	continue to the next layer
			if(layer==1){
				layer++;	
				
				clause_temp=append_clause(i,clause_node,clause_node->previous_layer,nul,nul, Coms);

				// Find a connection to variables in this cluase, and determine what to do next
				
				determiner=determiner_solve(clause_temp,0, NULL, Coms);
				
				counted=clause_contains_set(i, Coms);
				
				switch(determiner.command)
				{
					case 0:
						// Doesn't connect to any clause
						mpz_ui_pow_ui( exponent, 2, clause_size[i]-var_connections.inc );
						mpz_divexact(*sub_total, *sub_total,exponent);
						
						mpz_ui_pow_ui( exponent, 2, clause_size[i]-counted );
						mpz_divexact(result, clause_temp->previous->data,exponent);
						
						mpz_ui_pow_ui( exponent, 2, clause_size[i]-var_connections.inc );
						mpz_sub(union_sub,*sub_total,result);
						mpz_mul(*sub_total, *sub_total,exponent);
					break;
					case 1:
						// Connects	with a clause that exists with in the range of this layer					

						mpz_ui_pow_ui( exponent, 2, clause_size[i]-var_connections.inc );
						mpz_divexact(*sub_total, *sub_total,exponent);

						var_tab_add(&clause_temp->clause,Coms);
						post_branch_correct(determiner.end,determiner.start,clause_temp->clause,0,clause_temp, Coms);
						
						// Check return of post branch, and retrieve data from the tree
						if(clause_temp->next_layer!=NULL){
							mpz_set(result, clause_temp->next_layer->end->data);
							mpz_sub(union_sub, *sub_total,result);
							mpz_ui_pow_ui( exponent, 2, clause_size[i]-var_connections.inc );
							mpz_mul(*sub_total, *sub_total,exponent);
						}else{
							printf("Error: Rec Check Case !\n");
							exit(0);
						}
						var_tab_del(&clause_temp->clause, Coms);

					break;
					case 2:
						break;
				}

				layer--;
			}

			mpz_sub(union_sub,recursive_total,result);

			mpz_add(sum2, sum2, union_sub);	

			mpz_set(clause_temp->data,sum2);
			mpz_sub(clause_temp->removed,recursive_total,result);

			clause_node=clause_temp;
		}else{
			//if the start, set the inital points
			second=1;
			if(layer==1){
				mpz_set(sum,recursive_total);
				clause_node=append_layer(i,previous_layer,previous_layer,sum, Coms);
				mpz_set(clause_node->removed,sum);
				mpz_set(sum2,recursive_total);
			}	
		}

		if(previous_result->next!=NULL){
			previous_result=previous_result->next;
		}
	}

	layer--;
	mpz_clear(exponent);
	// Release big num numbers
	mpz_clear(current_total);
	mpz_clear(result);
	mpz_clear(sum);
	mpz_clear(sum2);
	mpz_clear(union_sub);
	mpz_clear(recursive_total);
}

void solve(com_line* Coms){

	mpz_t 	sub_total;
	mpz_init (sub_total);
	int** variable_connections = Coms->clause_connections;
	int clause_count 				= Coms->clause_count;
	node* clause_node				= (void*)Coms->clause_node;
	int variable_count			= Coms->variable_count;
	clause_node						= NULL;
	
	// all possible
	mpz_t			all_possible;
	mpz_init 	(all_possible);
	
	Coms->all_possible = &all_possible;
	
	mpz_t 		exponent;
	mpz_init		(exponent);

	mpz_pow_ui  (all_possible,base, variable_count);
	Coms->sub_total = &sub_total;
	mpz_set		(sub_total,all_possible);
	clause_node	= append_clause(1, clause_node,NULL,sub_total,nul, Coms);

	//recursion begins
	recursive_check(0,1,clause_count,sub_total,clause_node,Coms);
	
	mpz_sub(sub_total,sub_total,clause_node->next_layer->end->data);
	Coms->clause_node = clause_node;
	gmp_printf("removed %Zd   \n",clause_node->next_layer->end->data);
	gmp_printf("total %Zd  possible %Zd var count %i   \n",sub_total,all_possible,variable_count);
	
	if( mpz_cmp_ui( sub_total, 0)==-1){
		halt();
	}

	//clear up 
	mpz_set		(clause_node->data,sub_total);
	mpz_clear	(sub_total);
	mpz_clear	(exponent);
	mpz_clear	(all_possible);
}

int main(int argc, char *argv[]){


	//sets the base value of the problem
	mpz_init (base);
	mpz_set_ui(base,2);

	//mpz null ptr
	mpz_init(nul);
	mpz_set_ui(nul,0);

	//init all the postitions

	init_position(f_variable_position);

	f_variable_connections = calloc(vsize*2,sizeof(int**));
	for ( size_t i =0; i<vsize*2; i++){
		f_variable_connections[i] = calloc(90, sizeof(int**));
	}

	group_s* formula = MakeGroup(); 

	// reads DIMACS file
	read_cnf(argv[1], formula);


	//creates cached memory space
	com_line* boss = memory_init();
	
	// inserts dimacs directly in to the solving space
	raw(boss);
	solve(boss);

	printf(" complete \n ");


return(0);
}

