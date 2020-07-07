


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#include "infini_tree.h"
#include "var_pos.h"
#include "llist.h"
#include "bfs_s.h"
#include "clause_func.h"
#include "cnf_read.h"
#include <gmp.h>
#include "stest.h"
#include <assert.h>


int v_set=1;
int counted_for=0;

//all the single variable clauses
int ones[csize]={0};
int OnesPlace[csize]={0};
bool set_var[csize]={0};

//null bignum
mpz_t nul;

// base(n) -- algo works of any base
mpz_t base;
mpz_t sub_total;

node* clause_node=NULL;

//linked list variables

int clause_count=0;
int f_clause_count=0;
int variable_count=0;

// table of placement of variables
variable_pos* variable_position[vsize];
variable_pos* f_variable_position[vsize];



void init_index(variable_pos* table[]){
	for(long int  i=1;i<csize;i++){
		table[i]=create_clause(0,table[i]);
	}
}




// variable _connections [clause][place]=variable
int variable_connections[vsize][90]={0};
int f_variable_connections[vsize][90]={0};

//int connection_count[4000]={0};

long set_variable[vsize]={0};

int clause_size[csize]={0};


int f_clause_size[csize]={0};



//needs to be the size of all the variables *2
int var_tab[vsize]={0};

// pause the program by interjecting the code
void halt(){
	while(1){
	char input;
	scanf("%c", &input);
	 if(getchar()!=0){ break;}
	}
}



//interject code to look at the current state of a node
void debug(node *head){
if(head==NULL){printf(" DEBG NUL\n");return;}	
char input;
while (1){

scanf("%c", &input);
if(getchar()!=0){
//printf("boop");
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
void var_tab_add(int *clause_a)
{
	//this is the variables already been added to the table
	long *variables;
	variables=set_variable;
	int *clause_var;

	clause_var=&variable_connections[*clause_a][0];

	//this is the variables in the clause

		for(int x=clause_size[*clause_a];x!=0;x--)
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
inline void var_tab_del(int *clause_d)
{
	long *variables;
	variables=set_variable;
	int *clause_var;

	clause_var=&variable_connections[*clause_d][0];

	//this is the variables in the clause

	for(int x=clause_size[*clause_d];x!=0;x--){
		clause_var++;

if(counted_set[*(variables+abs(*clause_var))]==1){continue;}

			
		if(*(variables+abs(*clause_var))!=0)
		{
			var_tab[abs(*clause_var)]--;
		}
		if(var_tab[abs(*clause_var)]==0) 
		{
			//connection_count[abs(*clause_var)]--;
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


vReturn var_tab_count(int *clause_d){

	int temp_match[80]={0};
	vReturn alpha={0,0,0};
	//this is the variables already been added to the table
	long *variables;
	variables=&set_variable[0];
	int *clause_var;

	clause_var=&variable_connections[*clause_d][0];

	//amount of variables in the clause

	//this is the variables in the clause

	for(int i=clause_size[*clause_d];i!=0;i--){
		clause_var++;

		if(*(variables+abs(*clause_var))==*clause_var){
//printf("%i  set var %i \n",*clause_d, *clause_var);
			temp_match[0]++;
			temp_match[temp_match[0]]=abs(*clause_var);
			alpha.connections++;

			if(var_tab[abs(*clause_var)]>1){
				alpha.inc++;
			}
	

		}
		else if(*(variables+abs(*clause_var))!=0){
			alpha.connections++;
			//alpha.mul=-1;
			//return(alpha);
		}

	}
	return(alpha);
}



//check to see if variables in clause have been checked
vReturn var_tab_check(int *clause_d){

	int temp_match[80]={0};
	vReturn alpha={0,0,0};
	//this is the variables already been added to the table
	long *variables;
	variables=&set_variable[0];
	int *clause_var;

	clause_var=&variable_connections[*clause_d][0];

	//amount of variables in the clause

	//this is the variables in the clause

	for(int i=clause_size[*clause_d];i!=0;i--){
		clause_var++;
		if(*(variables+abs(*clause_var))==*clause_var ){

//printf("%i  set var %i \n",*clause_d, *clause_var);
			temp_match[0]++;
			temp_match[temp_match[0]]=abs(*clause_var);


			alpha.connections++;

if(var_tab[abs(*clause_var)]==0 || set_var[abs(*clause_var)]==0){alpha.mul++;}

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
vReturn comp_tab_check(int *clause_d){

	int temp_match[80]={0};
	vReturn alpha={0,0,0};
	//this is the variables already been added to the table
	long *variables;
	variables=&set_variable[0];
	int *clause_var;

	clause_var=&variable_connections[*clause_d][0];

	//amount of variables in the clause

	//this is the variables in the clause

	for(int i=clause_size[*clause_d];i!=0;i--){
		clause_var++;

		if(*(variables+abs(*clause_var))==*clause_var){
//printf("%i  set var %i \n",*clause_d, *clause_var);
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
vReturn neg_tab_check(int *clause_d){

	vReturn alpha={0,0,0};
	//this is the variables already been added to the table
	long *variables;
	variables=&set_variable[0];
	int *clause_var;

	clause_var=&variable_connections[*clause_d][0];

	//amount of variables in the clause

	//this is the variables in the clause

	for(int i=clause_size[*clause_d];i!=0;i--){
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
vReturn abs_tab_check(int *clause_d){


	int temp_match[80]={0};
	vReturn alpha={0,0,0};
	//this is the variables already been added to the table
	long *variables;
	variables=&set_variable[0];
	int *clause_var;

	clause_var=&variable_connections[*clause_d][0];

	//amount of variables in the clause

	//this is the variables in the clause

	for(int i=clause_size[*clause_d];i!=0;i--){
		clause_var++;

		if(*(variables+abs(*clause_var))==*clause_var){
//printf("%i  set var %i \n",*clause_d, *clause_var);
			temp_match[0]++;
			temp_match[temp_match[0]]=abs(*clause_var);
			alpha.connections++;

			if(var_tab[abs(*clause_var)]>1){
				alpha.inc++;
			}
	

		}


	}
	return(alpha);
}






//compares variables of two clauses
static inline int compare_clause(int a, int b){
	int count=0;

	for(int x= clause_size[a];x!=0;x--){
		for(int y=clause_size[b];y!=0;y--){
			
			if(abs(variable_connections[a][x])==abs(variable_connections[b][y]) &&set_var[abs(variable_connections[a][x])]==0){
				if(variable_connections[a][x]==variable_connections[b][y] ){
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
static inline node* clause_search(int start, node* head){
if(head==NULL){printf(" null cls srch \n"); exit(0);}
	bool error=0;
	//if(head->clause<start){return(head);}
	bool reverse=0;
	head=head->first_clause;
//if(head->clause>start){reverse=1;}else{reverse=0;}
	while(head->clause!=start){

		if(reverse==0){

			if(head->next!=NULL && head->next->clause<=start){
				head=head->next;
			}

			else{
				break;
			}

			if(head->clause>=start){
				break;
			}

		}
		else{

			head=head->previous;

			if(head->clause<=start){
				break;
			}
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




typedef struct determiner_return{
	int start;
	int command;
	int end;
} determiner_result;


int clause_contains_set(int clause){
int count=0;
	for(int variable=clause_size[clause];variable!=0;variable--){
		if(set_var[abs(variable_connections[clause][variable])]!=0){count++;}
	}
return(count);
}

//determines where a variable has been connected, and how to responed
determiner_result determiner_solve( node* head, int start, node* root){
	determiner_result result;
	result.command=0;
	result.start=0;
	bool negate_store=0;
	bool pre_non_con=0;
	bool pre_con_match=0;
int clause_comp=0;
	//multiple same variable
	//multiple_variable save
vReturn temp_connections;
	//search link
	variable_pos* link;
	//ordered list
	//stored list
	variable_pos* stored_clause=NULL;
	//saved clause
	variable_pos* saved_clause=NULL;

	// grab the clause
	int clause= head->clause;



	//if the root is disjoint from the start
//	if(compare_clause(start,head->clause)==-1){
		//result.command=2;
		//exit(0);
		//temp_connections=var_tab_check(&head->clause);
		//if(temp_connections.connections>0){ result.command=2; }

		//return(result);
//	}

	// for each variable in clause, search for previous clauses containing this variable
	for(int q=clause_size[clause]	;q!=0;q--){


		//if the variable has already been accounted for, skip
		if(set_var[abs(variable_connections[clause][q])]==1)continue;

		//if the variable mateches before the start
		if(variable_position[abs(variable_connections[clause][q])]->clause < head->clause){
		pre_con_match=1;
		}

		//if the variable has already appeared in the chain
		if(set_variable[abs(variable_connections[clause][q])]!=0){
			pre_non_con=1;
			continue;		
		}
		
		//previous clauses that contain this variable
		link=variable_position[abs(variable_connections[clause][q])];


		while(1){

			if(compare_clause(head->clause, link->clause) == -1){break;}

			if(clause_contains_set(link->clause) != 0){break;}

			if(var_tab_check(&link->clause).connections >= 0){break;}

			if(link->next != NULL && link->next->clause > head->clause){

				link=link->next;
			}
			else{

				break;
			}		
			
		}

		//if this is the first time it exists	
		if(saved_clause == NULL && link->clause!=head->clause){
			saved_clause = link;
		}
		if(saved_clause != NULL && link->clause<saved_clause->clause){
			saved_clause = link;
		}

	}

	if(saved_clause != NULL){
		result.start	= saved_clause->clause;
		stored_clause	= saved_clause;
		result.end		= 0;
		result.command = 1;

		//if the previous branch is connected
		if(var_tab_check(&head->clause).connections != 0){
			result.end = 1;
		}

		if(stored_clause->clause <= start){

			result.start = start;
			result.command = 2; 
			int clause_comp = compare_clause(result.start, head->clause);
			result.end = 2;
			
			if(var_tab_check(&result.start).connections != 0){
				result.end = 1;
			}
			
			if(clause_comp != 0){
				result.end = 1;
			}			
		}

		return(result);
	}else{
			//result.start=head->clause;
		result.end		=	0;
		result.command	=	0;
		return(result);
	}


	//this is the end of the search variables of the orriginal clause

	//if there is no connection
	if(stored_clause == NULL){
		result.command = 0;
		return(result);
	}else{
		printf("stored_clause->clause %i ",stored_clause->clause);
	}

	//uncomment if starting from the last variable
	//if(multiple_variable_saved==1){result.end=1;}

	//if the connection appears after the start, continue from this point
	if(stored_clause->clause > start){

		//store the clause
		result.start = stored_clause->clause;

		//if it's a negate
		if( negate_store == 1 ){  
			result.command = 3;
			return(result);
		}

		if(saved_clause != NULL){

			result.command = 1;
		}	
		
		
		if(stored_clause->clause == start){

			result.command = 2;
			result.end = 3;
			return(result);
		}

	}


	if(stored_clause->clause <= start){

		result.command=2;


		//if the variable has appeared before
		if(pre_non_con==1){
			//result.end=5;
		}

		//if there's a connection with the head, don't do 

		if(compare_clause(start, head->clause)==-1){result.command=2;}


		if(pre_con_match==1){
			
		//result.end=4;
		}

		//printf("%i\n", result.end);
		result.start=start;

	}


//if the saved clause is equal to the start
	if(stored_clause->clause==start){
		//see if the variable is connected to the root
				
		temp_connections=var_tab_check(&head->clause);
		clause_comp=compare_clause(stored_clause->clause,head->previous_layer->clause);

		if(temp_connections.connections>0 && temp_connections.connections-clause_comp==0){
			return(result);
		}

	}



//printf("***head:%i previous:%i start:%i end:%i\n\n\n\n\n\n\n\n", head->clause, head->previous_layer->clause, result.start, result.end);

//	if(saved_clause==NULL){result.end=1;}
//printf("End of determiner \n"); 
//printf("head %i %i \n", head->clause,result.start);
	return(result);
}

void post_branch_correct(int,int,int,int, node*);

void pre_branch_correct(int, int, int, node*, node*,node*);

void pre_branch_correct(int not_start,int init_start,int init_end, node* pre_start,node* head, node* tree){
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

//printf(" start %i end %i\n", init_start, init_end);

	//	keep sumpre_branch
	//total=temp_end->data-temp_end->removed;
	mpz_sub(total, temp_end->data, temp_end->removed);

	var_connections=var_tab_check(&init_start);

// To be experimented with later

//if the start and end are negated, do this
	if( var_connections.connections==-1 ){

		if( temp_end->previous!=NULL 
			&& temp_end->previous->clause>=init_start 
			&&  var_connections.connections==-1 ){
			if(compare_clause(temp_end->previous->clause, init_end)==-1){

				while(1){
				if(temp_end->previous!=NULL){
					temp_end=temp_end->previous;
				}
				else{
					break;
				}
				if(compare_clause(temp_end->clause, init_end)!=-1){break;}
				}
			}
		}
	}


	outer_layer= append_layer(temp_end->clause,head,head,nul,0 );

	if(var_connections.connections>=0){

		switch(not_start)
		case 0:{	

			// basic pre-start

			var_tab_del(&init_end);
			var_tab_add(&init_end);
			//total=total/pow(2,clause_size[con]-clause_comp2);
			mpz_ui_pow_ui( exponent, 2, clause_size[pre_start->clause]-var_tab_check(&pre_start->clause).inc );
			mpz_divexact(total,total,exponent);
			
			mpz_ui_pow_ui( exponent, 2, clause_size[pre_start->clause]-var_tab_check(&pre_start->clause).inc);
			mpz_divexact(removed,temp_end->removed,exponent);

			mpz_add(total,total,removed);

			break;
		case 1:
			//if a connections has already been accounted for
			mpz_sub(total, temp_end->data, temp_end->removed);


			// start to pre
			
			mpz_ui_pow_ui( exponent, 2, clause_size[pre_start->clause]-var_tab_check(&pre_start->clause).inc);
			mpz_divexact(total, total, exponent );

			//pre-process var_tab_check
			var_tab_add(&init_start);
			var_tab_del(&pre_start->clause);
			
			
			mpz_ui_pow_ui( exponent, 2, clause_size[pre_start->clause]-var_tab_check(&pre_start->clause).connections);
			mpz_divexact(removed,temp_end->removed,exponent);

			//post process
			var_tab_del(&init_start);
			var_tab_add(&pre_start->clause);

			mpz_add(total,total,removed);

			break;
		case 2:
			
			///continue from a previous variable

			mpz_sub(total, temp_end->data, temp_end->removed);
			// how many variables there are that haven't been accounted for
			//total=total/pow(2,var_connections.inc)


			var_tab_add(&init_start);
			var_tab_del(&init_end);
						temp_connections=var_tab_check(&init_end);
			var_tab_add(&init_end);

			var_tab_del(&pre_start->clause);
				var_connections=var_tab_check(&pre_start->clause);
			var_tab_add(&pre_start->clause);
			var_tab_del(&init_start);

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

			var_tab_del(&pre_start->clause);
			temp_connections=var_tab_check(&init_start);
			var_tab_add(&pre_start->clause);

//printf( " start %i - end %i \n", init_start, init_end);
		if(compare_clause(init_start,init_end)!=-1 && temp_connections.connections==-1){
			mpz_set(total, temp_end->data);
		}

		var_tab_del(&pre_start->clause);
		var_connections=abs_tab_check(&pre_start->clause);
		temp_connections=abs_tab_check(&head->clause);
		var_tab_add(&pre_start->clause);


//connections and inc have been changed!

		mpz_ui_pow_ui( exponent, 2, clause_size[pre_start->clause]-var_connections.connections);
		mpz_divexact(total,total,exponent);

		mpz_ui_pow_ui( exponent, 2, clause_size[pre_start->clause]-var_connections.connections);
		mpz_divexact(removed,temp_end->removed,exponent);

		if(compare_clause(init_end,init_start)==-1 && abs_tab_check(&init_end).connections!=0 ){
			mpz_add(total, total, removed);
		}
	//	printf(" var_connections.connections %i \n", neg_tab_check(&init_start).connections);
		//gmp_printf(" total %Zd removed %Zd \n", total, temp_end->data);

			mpz_set(removed, temp_end->removed);
			
	//		mpz_set( total, temp_end->data);
	//		mpz_sub( total, total, removed);

		if( neg_tab_check(&pre_start->clause).connections==clause_size[pre_start->clause]) printf(" ding\n");
		// if all variables have been accoutned for whether negated or connected for
		
		if(  neg_tab_check(&pre_start->clause).connections == clause_size[pre_start->clause]){
			mpz_ui_pow_ui( exponent, 2, clause_size[pre_start->clause]);
			mpz_divexact(total,temp_end->data,exponent);
		}
	}
	
	if(neg_tab_check(&init_end).connections>=clause_size[init_end]){
	//	 return;
	}

	//outer_layer->removed=removed;
	mpz_set(outer_layer->removed,removed);	
	//outer_layer->data= total;
	mpz_set(outer_layer->data,total);

//if(mpz_cmp(sub_total,outer_layer->data)==0){return;}

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

	primary_variable_connections=var_tab_check(&outer_loop->clause);
	

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


	outer_layer=append_clause(outer_loop->clause, outer_layer,outer_layer->previous_layer,0,0,0);

	var_connections=var_tab_check(&outer_loop->clause);

	determiner=determiner_solve( outer_loop,init_start,outer_loop);

 	//printf(" det start %i out %i end %i cmmand %i \n",determiner.start,outer_loop->clause,determiner.end,determiner.command);

	switch(determiner.command)
		{
			case 0:
				mpz_ui_pow_ui( exponent, 2, clause_size[outer_loop->clause]-var_connections.connections );
				//sub_total=sub_total/pow(2,clause_size[outer_loop->clause]-var_connections.connections);	
				mpz_divexact(sub_total,sub_total,exponent);

				//use data instead of removed and take away from sub_total
				//total2=(sub_total)-outer_layer->previous->data/pow(2,clause_size[outer_loop->clause]-var_connections.connections);
				mpz_ui_pow_ui( exponent, 2, clause_size[outer_loop->clause]-var_connections.connections );
				mpz_divexact(total2,outer_layer->previous->data,exponent);
				mpz_sub(total2, sub_total, total2);

				//sub_total=sub_total*pow(2,clause_size[outer_loop->clause]-var_connections.connections);	
				mpz_ui_pow_ui( exponent, 2, clause_size[outer_loop->clause]-var_connections.connections );
				mpz_mul(sub_total,sub_total,exponent);

			break;
			case 1:
				//sub_total=sub_total/pow(2,clause_size[outer_loop->clause]-var_connections.connections);	
				mpz_ui_pow_ui( exponent, 2, clause_size[outer_loop->clause]-var_connections.connections );
				mpz_divexact(sub_total,sub_total, exponent );

				//if there's a connection after the start, restart from here
				var_tab_add(&outer_layer->clause);

				post_branch_correct(determiner.end,determiner.start,outer_layer->clause,1,outer_layer);
				//collects data from the end of the tree
				mpz_set(total2, outer_layer->next_layer->end->data);
			
				//total2=sub_total-total2;
				mpz_sub(total2, sub_total,total2);
	
				var_tab_del(&outer_layer->clause);

				//sub_total=sub_total*pow(2,clause_size[outer_loop->clause]-var_connections.connections);	
				mpz_ui_pow_ui( exponent, 2, clause_size[outer_loop->clause]-var_connections.connections );
				mpz_mul(sub_total,sub_total,exponent);


			break;
			case 2:
				//if there's a connect before the start, recall the previous, and continue from the start
				//sub_total=sub_total/pow(2,clause_size[outer_loop->clause]-var_connections.connections);	
				mpz_ui_pow_ui( exponent, 2, clause_size[outer_loop->clause]-var_connections.connections );
				mpz_divexact(sub_total,sub_total,exponent);

				var_tab_add(&outer_layer->clause);

				pre_branch_correct(determiner.end,init_start,outer_layer->clause,pre_start,outer_layer,temp_end);

			if(outer_layer->next_layer!=NULL){
				mpz_set(total2,outer_layer->next_layer->end->data);
				mpz_sub(total2,sub_total,total2);
			}
			else
			{ 
				mpz_ui_pow_ui( exponent, 2, clause_size[outer_loop->clause]-var_connections.connections);
				mpz_divexact(total2,outer_layer->previous->data,exponent);
				mpz_sub(total2, sub_total, total2);
			}
				//total2=sub_total-total2;
				
				var_tab_del(&outer_layer->clause);

				//sub_total=sub_total*pow(2,clause_size[outer_loop->clause]-var_connections.connections);	
				mpz_ui_pow_ui( exponent, 2, clause_size[outer_loop->clause]-var_connections.connections);
				mpz_mul(sub_total,sub_total,exponent);

			break;
		}
			

	
	//end of conditional pre_loop
	if(outer_loop->clause!=init_start){
	
		//total+=total2;
		mpz_add(total,total,total2);

		//outer_layer->data=total;
		mpz_set(outer_layer->data,total);

		//outer_layer->removed=total2;
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



void post_branch_correct(int not_start,int init_start,int init_end, int con,node* head){

	determiner_result determiner;
	node* head_temp=head;
	node* outer_loop;
	node* inner_loop;
	node* layer_temp;
	node* outer_layer;
	node* temp_layer;
	vReturn var_connections;
	vReturn primary_variable_connections;

	int clause_comp=0;

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
	//total=temp_layer->data-temp_layer->removed;
	mpz_sub(total,temp_layer->data,temp_layer->removed);
	

//	printf("start:%i end:%i var con %i not %i \n",init_start,init_end, head->previous_layer->clause, not_start);

//	if(clause_contains_set(init_start)!=0){

		//return;
//	}

	if(var_tab_check(&init_start).connections>=0){	
		switch(not_start)
		case 0:{ 

			//No connections
			//Scales the last data
			clause_comp=compare_clause(init_start,init_end);
//clause_comp=clause_size[init_end]-var_connections.connections;
			//total=total/pow(2,clause_size[init_end]);
			
			mpz_ui_pow_ui( exponent, 2, clause_size[init_end]-var_tab_check(&init_end).inc );
			mpz_divexact(total,total, exponent);
			//removed=temp_layer->removed/pow(2,clause_size[init_end]-clause_comp);
			
			mpz_ui_pow_ui( exponent, 2, clause_size[init_end]-clause_comp);
			mpz_divexact(removed, temp_layer->removed,exponent);
//printf("varcon %i\n",var_tab_check(&init_end).inc);
			//total+=removed;
			mpz_add(total,total,removed);

			
			break;
		case 1:

			///continue from a previous clause, that's been connected before


			// count how many variables bisect with this clause
			var_tab_del(&init_end);
			var_tab_add(&init_start);
			var_connections=var_tab_check(&init_end);
			var_tab_del(&init_start);
			var_tab_add(&init_end);

			clause_comp=compare_clause(init_start,head->clause);

			mpz_sub(total, temp_layer->data, temp_layer->removed);

			 // how many variables there are that haven't been accounted for
			// total=total/pow(2,var_connections.inc);
			clause_comp=compare_clause(init_start, init_end);
 
			// Scale the previous data to generate next layer
			mpz_ui_pow_ui( exponent, 2, clause_size[init_end]-var_tab_check(&init_end).inc );
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
		//	total=temp_layer->data-temp_layer->removed;

		//gmp_printf("dat1..:%i %i %Zd %Zd  %Zd %Zd\n",clause_comp,var_connections.connections, layer_temp->removed, temp_layer->data,total,removed);
		if(temp_layer->previous!=NULL && layer_temp->previous!=NULL ){
			temp_layer=temp_layer->previous;
			layer_temp=layer_temp->previous;
			
		}

		var_connections=var_tab_check(&head->clause);

		var_tab_del(&init_end);
		var_connections=abs_tab_check(&init_end);
		var_tab_add(&init_end);
	// total=total/pow(2,clause_size[head->clause]-clause_comp);
		mpz_sub(total, temp_layer->data, temp_layer->removed);

		var_tab_del(&init_end);
		var_tab_add(&init_end);


		
		mpz_ui_pow_ui( exponent, 2, clause_size[init_end]-var_connections.connections );
		mpz_divexact(total,total, exponent);
		
		mpz_divexact(removed,temp_layer->removed,exponent);

// printf(" var %i \n", var_connections.connections);
		if(var_tab_check(&init_start).connections!=-1 || init_start!=1){
			mpz_add(total,total, removed);
		}

		var_tab_del(&init_end);
	// if all variables have been accoutned for whether negated or connected for
		if(comp_tab_check(&init_end).connections== clause_size[init_end]){
			mpz_sub(total, temp_layer->data, temp_layer->removed);
			
			mpz_ui_pow_ui( exponent, 2, clause_size[init_start] );
			mpz_divexact(total,total, exponent  );
		}
		var_tab_add(&init_end);
	}



	outer_layer= append_layer(temp_layer->clause,head,head,nul,0 );

	//outer_layer->removed=removed;
	mpz_set(outer_layer->removed,removed);
	//outer_layer->data= total;
	mpz_set(outer_layer->data,total);

	if(layer_temp->next!=NULL
		&&var_tab_check(&layer_temp->next->clause).connections==-1) {

		while(var_tab_check(&layer_temp->next->clause).connections==-1){
			if(layer_temp->next!=NULL){
				layer_temp=layer_temp->next;
			}
			else{
				break;
			}
		}
	}
	
	if(layer_temp->next==NULL ||init_end-layer_temp->clause==1 ){
//printf(" this is the exit(0);\n");
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
primary_variable_connections=var_tab_check(&outer_loop->clause);

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
if(primary_variable_connections.connections==-1){ printf("outer loop negate \n"); exit(0);break;}



	outer_layer=append_clause(outer_loop->clause, outer_layer,outer_layer->previous_layer,0,0,0);
	//outer_layer->previous_layer=outer_loop->previous_layer	;
	clause_comp=compare_clause(outer_loop->clause,inner_loop->clause);
	var_connections=var_tab_check(&outer_loop->clause);

	if(var_connections.connections==-1)exit(0);

	determiner=determiner_solve( outer_layer,init_start ,outer_loop);

	//if there's a connections between init start, and current variable, do pre_branch
//determiner.command=0;
//if(determiner.command==2){printf("determiner.comand2 %i %i\n", init_start,determiner.start);determiner.command=1;halt();}
//printf("determiner.comand2 %i %i %i\n", determiner.command,determiner.end,determiner.start);
	switch(determiner.command)
		{
			case 0:

				//sub_total=sub_total/pow(2,clause_size[outer_loop->clause]-var_connections.connections);

				mpz_ui_pow_ui( exponent, 2, clause_size[outer_loop->clause]-var_connections.connections );
				mpz_divexact(sub_total,sub_total, exponent);


				//use data instead of removed and take away from sub_total
				//total2=(sub_total)-outer_layer->previous->data/pow(2,clause_size[outer_loop->clause]-var_connections.connections);

				mpz_ui_pow_ui( exponent, 2, clause_size[outer_loop->clause]-var_connections.connections );
				mpz_divexact(total2,outer_layer->previous->data,exponent);
				
				mpz_sub(total2,sub_total,total2);

				//sub_total=sub_total*pow(2,clause_size[outer_loop->clause]-var_connections.connections);	
				mpz_ui_pow_ui( exponent, 2, clause_size[outer_loop->clause]-var_connections.connections );
				mpz_mul(sub_total,sub_total,exponent);

			break;
			case 1:
				//post start
				//if there's a connection after the start, restart from here
				//sub_total=sub_total/pow(2,clause_size[outer_loop->clause]-var_connections.connections);	
				mpz_ui_pow_ui( exponent, 2, clause_size[outer_loop->clause]-var_connections.connections );
				mpz_divexact(sub_total,sub_total,exponent);

				var_tab_add(&outer_layer->clause);

				post_branch_correct(determiner.end,determiner.start,outer_layer->clause,1,outer_layer);
				if(outer_layer->next_layer!=NULL){
					mpz_set(total2,outer_layer->next_layer->end->data);

				//	total2=sub_total-total2;
					mpz_sub(total2,sub_total,total2);
				}
				else{
					printf("there is no return from post_branch\n");
					exit(0);
					mpz_set(total2,outer_layer->previous->removed);
				}

				var_tab_del(&outer_layer->clause);

				//sub_total=sub_total*pow(2,clause_size[outer_loop->clause]-var_connections.connections);	
				mpz_ui_pow_ui( exponent, 2, clause_size[outer_loop->clause]-var_connections.connections );
				mpz_mul(sub_total,sub_total,exponent);

			break;
			case 2:
				//if there's a connect before the start, recall the previous, and continue from the start
//printf("pre branch ini start %i end %i \n", init_start, outer_layer->clause);

				//sub_total=sub_total/pow(2,clause_size[outer_loop->clause]-var_connections.connections);
				mpz_ui_pow_ui( exponent, 2, clause_size[outer_loop->clause]-var_connections.connections );
				mpz_divexact(sub_total,sub_total,exponent);

				var_tab_add(&outer_loop->clause);
				pre_branch_correct(determiner.end,determiner.start,outer_layer->clause,outer_layer->previous_layer,outer_layer,head);

//gmp_printf(" %Zd \n", sub_total);
				if(outer_layer->next_layer!=NULL){
					mpz_set(total2,outer_layer->next_layer->end->data);

					mpz_sub(total2,sub_total,total2);
				}else{
					mpz_ui_pow_ui( exponent, 2, clause_size[outer_loop->clause]-var_connections.connections );
					mpz_divexact(total2,outer_layer->previous->data,exponent);
					mpz_sub(total2,sub_total,total2);
				}
				var_tab_del(&outer_loop->clause);

				//sub_total=sub_total*pow(2,clause_size[outer_loop->clause]-var_connections.connections);	
				mpz_ui_pow_ui( exponent, 2, clause_size[outer_loop->clause]-var_connections.connections );
				mpz_mul(sub_total,sub_total,exponent);
			break;
			case 3:
		break;
		}
			

	
	//end of conditional pre_loop
	if(outer_loop->clause!=init_start){

		
		//total+=total2;
		mpz_add(total,total,total2);

		//outer_layer->data=total;
		mpz_set(outer_layer->data,total);
		//outer_layer->removed=total2;
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

int count_negate(int clause){
int count_neg=0;
	for(int variable=clause_size[clause]	;variable!=0;variable--){

		//	if the variable has already been accounted for, skip
		if( set_variable[abs(variable_connections[clause][variable])] == variable_connections[clause][variable]){
			count_neg++;
		}
	}

return(count_neg);
}

// for each base check the previous one
void recursive_check(int layer, int begin_layer,int j,mpz_t previous_total, node* previous_layer){




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
	
	//total - previously accounted for
	mpz_t union_sub;
	mpz_init(union_sub);

	//local subset option
	mpz_t recursive_total;
	mpz_init(recursive_total);

	int clause_connections=0;
	determiner_result determiner;
	bool second=0;
	// for each clause, check for a previous connection
	for (int i=1; i<=(clause_count); i++){
	//printf(" i %i \n", i);
//if(clause_size[i]==0)continue;
		// check how many connections there are that exists from this clause
		var_connections=var_tab_check(&i);

		if(var_connections.connections==-1){continue;}

		if(var_connections.inc>=clause_size[i]){
			continue;
		}

		// if in the second layer, there is a connection, fix it
		if(var_connections.connections!=0 && layer>1 && clause_size[i]>0){

			printf("appendeed\n");
			exit(0);
			// create the linked list layer 
			clause_temp=append_layer(i,previous_layer,previous_layer,nul,0);

			// set the variables up to be continues
			mpz_set(sum, previous_layer->next_layer->end->data);
			mpz_set(sum2, previous_layer->next_layer->end->data);
		
			break;					
		}

		mpz_ui_pow_ui( exponent, 2, (clause_size[i]-clause_contains_set(i)) );
		mpz_divexact(recursive_total, previous_total, exponent );

		//if the place is past the initial clause
		if(second==1){
			clause_connections=clause_size[i]-var_connections.connections;

			//scales to include the current clause
			mpz_ui_pow_ui( exponent, 2, (clause_size[i]) );
			mpz_divexact(current_total,previous_result->removed, exponent );

			//	continue to the next layer
			if(layer==1){
				layer++;	
				
				clause_temp=append_clause(i,clause_node,clause_node->previous_layer,nul,nul,clause_connections);
				//scales sub_total to include the branch

				//mpz_divexact_ui(sub_total, sub_total,pow(2,clause_size[i]));

				// Find a connection to variables in this cluase, and determine what to do next

				determiner=determiner_solve(clause_temp,0, NULL);
				int counted=clause_contains_set(i);
				switch(determiner.command)
				{
					case 0:
						// Doesn't connect to any clause
						mpz_ui_pow_ui( exponent, 2, clause_size[i]-var_connections.inc );
						mpz_divexact(sub_total, sub_total,exponent);
						
						mpz_ui_pow_ui( exponent, 2, clause_size[i]-counted );
						mpz_divexact(result, clause_temp->previous->data,exponent);
						
						
						mpz_ui_pow_ui( exponent, 2, clause_size[i]-var_connections.inc );
						mpz_sub(union_sub,sub_total,result);
						mpz_mul(sub_total, sub_total,exponent);
					break;
					case 1:
						// Connects	with a clause that exists with in the range of this layer					

						mpz_ui_pow_ui( exponent, 2, clause_size[i]-var_connections.inc );
						mpz_divexact(sub_total, sub_total,exponent);

						var_tab_add(&clause_temp->clause);
						post_branch_correct(determiner.end,determiner.start,clause_temp->clause,0,clause_temp);
						// Check return of post branch, and retrieve data from the tree
						if(clause_temp->next_layer!=NULL){
							mpz_set(result, clause_temp->next_layer->end->data);
							mpz_sub(union_sub,sub_total,result);
//							gmp_printf("mp %Zd\n", result);

							mpz_ui_pow_ui( exponent, 2, clause_size[i]-var_connections.inc );
							mpz_mul(sub_total, sub_total,exponent);
						}else{
							printf("Error: Rec Check Case !\n");
						}
						var_tab_del(&clause_temp->clause);

					break;
					case 2:
						break;
				}

				// catch if the value is greater than allowed
				//if(mpz_cmp(result,current_total)==-1){printf("Error: Rech Check invalid result\n");exit(0);}

//				mpz_mul_ui(sub_total, sub_total,pow(2,clause_size[i]));
				layer--;
			}
//				mpz_mul_ui(sub_total, sub_total,pow(2,clause_size[i]));

			//add all the possible, subtract by subset possiblies already taken
			//sum+=current_total-result;

		//	mpz_set(current_total,result);			
			//mpz_add(sum,sum,result);
			
			//sum2+=recursive_total-result;
			mpz_sub(union_sub,recursive_total,result);

			mpz_add(sum2, sum2, union_sub);	
			//update the clause
			if(layer==1){

			}
				//clause_temp->data=sum2;
				mpz_set(clause_temp->data,sum2);
				//clause_temp->removed=recursive_total-result;
				mpz_sub(clause_temp->removed,recursive_total,result);

//if(var_tab_check(&clause_temp->clause).connections==-1)continue;

			//	move on to the next clause
			clause_node=clause_temp;
						
		}
		else{

				
			//if the start, set the inital points
			second=1;
			if(layer==1){
			mpz_set(sum,recursive_total);

		//	gmp_printf("total %Zd  possible  var count %i   \n",sum,i);
				clause_node=append_layer(i,previous_layer,previous_layer,sum,clause_connections);
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

void solve(){
	//the total number of all possibilities
//	all = pow(2,variable_count);

	clause_node=NULL;
	// all possible
	mpz_t all_possible;
	mpz_init (all_possible);
	
	mpz_t exponent;
	mpz_init(exponent);
	
	
	mpz_pow_ui (all_possible,base, variable_count);

	//last of the variables
	//start of the linked list

	mpz_set(sub_total,all_possible);
	
	mpz_ui_pow_ui( exponent, 2, counted_for );
	mpz_divexact(sub_total,sub_total,exponent);

	clause_node=append_clause(1, clause_node,NULL,sub_total,nul,variable_connections[clause_count][0]);
	pnt=clause_node;

	//recursion begins
	recursive_check(0,1,clause_count,sub_total,clause_node);
	mpz_ui_pow_ui( exponent, 2, variable_connections[0][0] );
	mpz_mul(sub_total,sub_total,exponent);
	mpz_sub(sub_total,sub_total,clause_node->next_layer->end->data);
	mpz_set(pnt->data,sub_total);

//gmp_printf("2 total %Zd  possible %Zd var count %i   \n",sub_total,all_possible,variable_count);
	if( mpz_cmp_ui( sub_total, 0)==-1)halt();
	mpz_set(clause_node->data,sub_total);
mpz_clear(exponent);
	mpz_clear(all_possible);
}

int main(int argc, char *argv[]){


//sets the base value of the problem
mpz_init (base);
mpz_set_ui(base,2);

//mpz null ptr
mpz_init(nul);
mpz_set_ui(nul,0);



// reads the amount of variables and clauses
	//stores temporary literals

//init all the postitions
init_index(variable_position);
init_index(f_variable_position);


//scans file

// imports cnf into local memory
read_cnf(argv[1]);


//**********************
//start of the algorithmd

//**********************
	printf("%i %i\n", f_clause_count,f_variable_count); 

init_graph(ones);
//raw();
//solve();

node* result=clause_node->next_layer;
//////gmp_printf("%i clause node  %Zd  %Zd \n", result->clause,result->data,result->removed);
	while(result!=NULL){
		//printf("%i clause node %i + %i \n", result->clause,result->data,result->removed);
		gmp_printf("%i clause node  %Zd  %Zd \n", result->clause,result->data,result->removed);
	
		result=result->next;
	}

//dispose(&clause_node);
//solve();
//exit(0);
printf(" complete \n ");

debug(clause_node);
//	halt();
exit(0);

	//add an extra clause - end clause is blank
/*	int clause = add_clause_();
	append_var_clause(clause, 1);
	append_var_clause(clause, 2);
	clause = add_clause_();
	append_var_clause(clause, 3);
	append_var_clause(clause, 4);
	clause = add_clause_();
	append_var_clause(clause, 5);
	append_var_clause(clause, 6);
	clause = add_clause_();
	append_var_clause(clause, 7);
	append_var_clause(clause, 4);
	clause = add_clause_();
*/

	printf("%i %i\n", clause_count,variable_count); 


	
	
//printf("clause_node->clause %i \n", clause_node->next_layer->end->clause);
/*	
node* result=clause_node->next_layer;
//////gmp_printf("%i clause node  %Zd  %Zd \n", result->clause,result->data,result->removed);
	while(result!=NULL){
		//printf("%i clause node %i + %i \n", result->clause,result->data,result->removed);
		gmp_printf("%i clause node  %Zd  %Zd \n", result->clause,result->data,result->removed);
	
		result=result->next;
	}

	//TEST PARAMETERS
	//	printf("sub_total %lli %lli \n",all-clause_node->next_layer->end->data,all);

gmp_printf("total %Zd  possible %Zd \n",sub_total,clause_node->next_layer->end->data);
debug(clause_node);
dispose(clause_node);
*/
return(0);
}

