


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "stest.h"
#include "infini_tree.h"
#include "var_pos.h"
#include "chain_block.h"
#include "llist.h"
#include "bfs_s.h"
#include "clause_func.h"
#include "cnf_read.h"
#include <gmp.h>

#include <assert.h>

int v_set=1;
int counted_for=0;

//all the single variable clauses
int ones[4000]={0};
bool set_var[40000]={0};

//null bignum
mpz_t nul;
//node* pnt=NULL;
//mpz_init(nul);

// base(n) -- algo works of any base
mpz_t base;
mpz_t sub_total;


node* clause_node=NULL;
	

//linked list variables
node* head=NULL;
node* root;

//chain_block init
chain_block* chain;

int clause_count=0;
int f_clause_count=0;
int variable_count=0;

// table of placement of variables
variable_pos* variable_position[4001];
variable_pos* f_variable_position[4001];

variable_pos* tree_position[4001];

void init_pos(variable_pos* table[]){
	for(int i=1;i<=4000;i++){
		table[i]=create_clause(0,table[i]);
		//variable_position[i]
	}
}


long long all=0;

int start=0;

int variable[127000][30]={0};

// variable _connections [clause][place]=variable
int variable_connections[127000][20]={0};
int f_variable_connections[127000][20]={0};

int connection_count[4000]={0};

long set_variable[127000]={0};

int clause_size[127000]={0};


int f_clause_size[127000]={0};



//needs to be the size of all the variables *2
int var_tab[1000]={0};

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
	int count=0;

	clause_var=&variable_connections[*clause_a][0];

	//this is the variables in the clause

		for(int x=clause_size[*clause_a];x!=0;x--)
		{

			clause_var++;

			connection_count[abs(*clause_var)]++;
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
	int count=0;

	clause_var=&variable_connections[*clause_d][0];

	//this is the variables in the clause

	for(int x=clause_size[*clause_d];x!=0;x--){
		clause_var++;


			
		if(*(variables+abs(*clause_var))!=0)
		{
			var_tab[abs(*clause_var)]--;
		}
		if(var_tab[abs(*clause_var)]==0) 
		{
			connection_count[abs(*clause_var)]--;
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
			alpha.connections=-1;
			alpha.mul=-1;
			return(alpha);
		}

	}
	return(alpha);
}

//check to see if variables in clause have been accounted for, whether negated our connects
vReturn comp_tab_check(int *clause_d){

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

	for(int i=clause_size[*clause_d];i!=0;i--){
		clause_var++;

		if(*(variables+abs(*clause_var))==*clause_var){
//printf("%i  set var %i \n",*clause_d, *clause_var);
			temp_match[0]++;
			temp_match[temp_match[0]]=abs(*clause_var);
			//alpha.connections++;

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
//check to see if variables in clause have been checked
vReturn abs_tab_check(int *clause_d){


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
	//if(head->clause<start){return(head);}
	bool reverse=0;
	head=head->first_clause;
	vReturn temp_connections;
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


bool clause_contains_set(int clause){
	for(int variable=clause_size[clause];variable!=0;variable--){
	//	if(set_var[abs(variable_connections[clause][variable])]==1) return(1);
	}
return(0);
}

//determines where a variable has been connected, and how to responed
determiner_result determiner_solve(chain_block* chain, node* head, int start, node* root){
	determiner_result result;
	result.command=0;
	result.start=0;
	bool negate=0;
	bool negate_store=0;
	bool null_check=0;
	bool equal_variable=0;
	bool saved_equal=0;
	bool disjoint=0;
	bool end_check=0;
	bool start_clause=1;
	bool pre_start=0;
	bool not_start=0;
	bool pre_non_con=0;
	bool pre_con_match=0;
	bool not_first=0;
	node* pre_process=head;
int clause_comp=0;
	//multiple same variable
	bool multiple_variable=0;
	//multiple_variable save
	bool multiple_variable_saved=0;
vReturn temp_connections;
vReturn var_connections;
	//search link
	variable_pos* link;
	//ordered list
	variable_pos* list;
	variable_pos* tist;
	//stored list
	variable_pos* stored_clause=NULL;
	//saved clause
	variable_pos* saved_clause=NULL;
	//temp list
	variable_pos* temporary_clause=NULL;

	// grab the clause
	int clause= head->clause;



	//if the root is disjoint from the start
	if(var_tab_check(&head->clause).connections==-1){
		//result.command=2;
		//exit(0);
		//temp_connections=var_tab_check(&head->clause);
		//if(temp_connections.connections>0){ result.command=2; }

	//	return(result);
	}

int p=0;
	// for each variable in clause, search for previous clauses containing this variable
	for(int q=clause_size[clause]	;q!=0;q--){

		//	if the variable has already been accounted for, skip
		if(set_variable[abs(variable_connections[clause][q])]!=0){
			continue;		
		}
		

		if(set_var[abs(variable_connections[clause][q])]==1){ break;}
		
			
		//if the variable mateches before the start
		if(variable_position[abs(variable_connections[clause][q])]->clause < head->clause){
		pre_con_match=1;
		}
		//if the variable has already appeared in the chain

		
		//previous clauses that contain this variable
		link=variable_position[abs(variable_connections[clause][q])];

//		if(count_var_pos(link)==1)continue;
//if(var_tab_check(&link->clause).connections==-1){continue;;}

		//link=link->first;
		while(link!=NULL){

			printf("link->clause %i %i %i \n", link->clause,clause_contains_set(link->clause),clause);

if(compare_clause(head->clause,link->clause)==-1){
link=link->next;
continue;
}

			if(clause_contains_set(abs(link->clause))==0){
				break;
			}

			if(link->next && link->next->clause<clause){
				link=link->next;
			}
			else{
			if(clause_contains_set(abs(link->clause))==1){
				link=NULL;
			}
			break;
			}
		}


		if(link==NULL){
			continue;
		}
		printf(" this one %i \n", link->clause);



		temp_connections=var_tab_check(&link->clause);

		//if this is the first time it exists	
		if(saved_clause==NULL && link->clause!=head->clause &&clause_contains_set(abs(link->clause))!=1){
			saved_clause=link;
		}
		if(saved_clause!=NULL && link->clause<saved_clause->clause){
			saved_clause=link;
			not_first=1;
		}

	}


//if(saved_clause!=NULL){printf("\nsaved %i \n", saved_clause->clause);}

	if(saved_clause!=NULL){
		result.start=saved_clause->clause;
		stored_clause=saved_clause;
		result.end=0;
				
		result.command=1;

		var_connections=var_tab_check(&head->clause);

		//if the previous branch is connected
		if(var_connections.connections!=0){
			result.end=1;
		}
		temp_connections=var_tab_check(&saved_clause->clause);
		int clause_comp=compare_clause(result.start, head->clause);


		if(stored_clause->clause<=start){
			result.start=start;result.command=2; 
		int clause_comp=compare_clause(result.start, head->clause);
result.end=2;
			//result.end=0;
	var_connections=var_tab_check(&result.start);
			if(var_connections.connections!=0){
				result.end=1;
//
			}
			 if(clause_comp!=0){
				result.end=1;

			}			
		}


		if(stored_clause->clause==start	){
			
			//result.command=1;
			if(compare_clause(start, head->clause)!=0 || var_connections.connections!=0){
			//dresult.end=2;
			}
			
		}
		return(result);
	}
	else{
			//result.start=head->clause;
		result.end=0;
		result.command=0;
		return(result);
	}


	//this is the end of the search variables of the orriginal clause

	//if there is no connection
	if(stored_clause==NULL){
		result.command=0;return(result);}
	else{
		printf("stored_clause->clause %i ",stored_clause->clause);
	}
	
	//uncomment if starting from the last variable
	//if(multiple_variable_saved==1){result.end=1;}


	//if the connection appears after the start, continue from this point

temp_connections=var_tab_check(&head->clause);
	if(stored_clause->clause > start){

		//store the clause
		result.start=stored_clause->clause;
//printf("head %i %i \n", stored_clause->clause,result.start);
//exit(0);
		//printf("result.start %i\n", result.start);

		//if it's a negate
		if( negate_store==1 ){  
			result.command=3;
			return(result);
		}

		if(saved_clause!=NULL){

			result.command=1;
		}	
		
		
		if(stored_clause->clause == start){result.command=2;result.end=3;return(result);}
		if(pre_non_con==1){
			//result.end=3;
			//if(var_tab_check(&head->clause).connections>0){ result.command=2; result.end=6;}
		}

	}


	if(stored_clause->clause <=start){

		result.command=2;


		//if the variable has appeared before
		if(pre_non_con==1){
			//result.end=5;
		}

		//if there's a connection with the head, don't do 
		int clause_comp=compare_clause(start, head->clause);

		if(clause_comp!=0){
			//result.end=3;
			//if(temp_connections.connections!=0){result.end=6;}

		}


if(clause_comp==-1){result.command=2;}


		if(pre_con_match==1){
			
		//result.end=4;
		}

		//printf("%i\n", result.end);
		result.start=start;

	}


//if the saved clause is equal to the start
	if(stored_clause->clause==start){
		//see if the variable is connected to the root
				
		//printf(" %i %i %i \n", head->previous_layer->clause,head->clause, saved_clause->clause);
		temp_connections=var_tab_check(&head->clause);
		clause_comp=compare_clause(stored_clause->clause,head->previous_layer->clause);

		if(temp_connections.connections>0 && temp_connections.connections-clause_comp==0){return(result);}

	}
	return(result);
}

void post_branch_correct(int,int,int,int, node*);

void pre_branch_correct(int, int, int, node*, node*,node*);

void pre_branch_correct(int not_start,int init_start,int init_end, node* pre_start,node* head, node* tree){
	determiner_result determiner;
	node* head_temp=head;
	node* outer_loop;
	node* inner_loop;
	node* layer_temp;
	node* outer_end;
	node* outer_layer;
	node* temp_layer;
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
node* start_data;
	
	int clause_comp=0;
	bool init_seed=0;
	node* temp_end;
	int t_comp =0;
	int count=0;
	int count1=0;
	int non_un=0;

	//total previous bisections
	mpz_t total;
	mpz_init(total);

	mpz_t total2;
	mpz_init(total2);

	// bisection of all previous nodes - Already accounted for sets
	mpz_t removed;
	mpz_init(removed);

	// temp result
	mpz_t temp;
	mpz_init(temp);


	if(head_temp==NULL){
		printf("ERROR: NULL VALUE PTR\n");
		exit(0);
	}

	layer_temp=clause_search(init_start, head);
	temp_layer=clause_search(init_start, head_temp);

	temp_end=clause_search(init_end, tree);

	if(temp_end->next_layer==NULL){
		printf(" no new layer\n");
		//return;
		exit(0);
	}
	temp_end=temp_end->next_layer;
	temp_end=clause_search(init_start, temp_end);

	if(temp_end==NULL){
		printf("ERROR101 \ncheck %i\n", check_var->clause);
		exit(0);
	}

	//	keep sumpre_branch
	//total=temp_end->data-temp_end->removed;
	mpz_sub(total, temp_end->data, temp_end->removed);

	temp_connections=var_tab_check(&layer_temp->clause);
	var_connections=var_tab_check(&init_start);

// To be experimented with later
/*
	if(var_connections.connections>=clause_size[init_start]){
		//printf(" end here\n");
		//return;
	}
*/

//if the start and end are negated, do this
clause_comp=compare_clause(init_start, init_end);
if(var_tab_check(&init_start).connections==-1){

	if(temp_end->previous!=NULL && temp_end->previous->clause>=init_start &&  var_connections.connections==-1){
		//temp_connections=var_tab_check(&temp_end->clause);
		clause_comp=compare_clause(temp_end->previous->clause, init_end);
		if(clause_comp==-1){

			while(1){
			if(temp_end->previous!=NULL){
				temp_end=temp_end->previous;
			}
			else{
				break;
			}
			clause_comp=compare_clause(temp_end->clause, init_end);
			if(clause_comp!=-1){break;}
			}
		}
	}
}


	outer_layer= append_layer(temp_end->clause,head,head,nul,0 );

	if(var_connections.connections>=0){

		switch(not_start)
		case 0:{	

			// basic pre-start

			clause_comp=compare_clause(init_start,pre_start->clause);
			var_tab_del(&init_end);
			var_connections=var_tab_check(&pre_start->clause);
			var_tab_add(&init_end);
			//total=total/pow(2,clause_size[con]-clause_comp2);

			mpz_divexact_ui(total,total,pow(2,clause_size[pre_start->clause]-var_connections.inc));
			mpz_divexact_ui(removed,temp_end->removed,pow(2,clause_size[pre_start->clause]-clause_comp));
			mpz_add(total,total,removed);

			break;
		case 1:
			//if a connections has already been accounted for
			mpz_sub(total, temp_end->data, temp_end->removed);
			//var_connections=var_tab_check(&init_start);

			var_tab_add(&init_start);
			var_connections=var_tab_check(&pre_start->clause);
			var_tab_del(&init_start);

			var_connections=var_tab_check(&pre_start->clause);


			// start to pre
			int tot=clause_size[pre_start->clause]-var_connections.inc;
			mpz_divexact_ui(total, total,pow(2,tot));


			var_tab_add(&init_start);
			var_tab_del(&pre_start->clause);
			temp_connections=var_tab_check(&pre_start->clause);
			var_tab_del(&init_start);
			var_tab_add(&pre_start->clause);

			//needs to be temp
			mpz_divexact_ui(removed,temp_end->removed,pow(2,clause_size[pre_start->clause]-temp_connections.connections));
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
			var_tab_del(&init_start);

			var_tab_add(&init_start);
			var_tab_del(&pre_start->clause);
				var_connections=var_tab_check(&pre_start->clause);
			var_tab_add(&pre_start->clause);
			var_tab_del(&init_start);

			int res=clause_size[pre_start->clause]-var_connections.connections;

			if(res<=0){
				printf("zero %i %i \n",clause_size[pre_start->clause],var_connections.connections);
				res=0;
			}


			mpz_divexact_ui(temp, temp_end->removed,pow(2,res)); //(a^b)^(b^c) ¬(a^b^c)  
			mpz_divexact_ui(total,total,pow(2,clause_size[pre_start->clause]-temp_connections.connections)); //(b^c)
			mpz_set(removed, temp);
			mpz_add(total,total, temp);

			break;
		}		
	}
	else{
			//If the start is negate

			clause_comp=compare_clause(init_end,init_start);

			//  if it's negate, find one previous with which is acceptable!
			//temp_end=temp_end->previous;

			mpz_sub(total, temp_end->data, temp_end->removed);

			var_tab_del(&pre_start->clause);
			temp_connections=var_tab_check(&init_start);
			var_tab_add(&pre_start->clause);


		if(compare_clause(init_start,init_end)!=-1 && temp_connections.connections==-1){
			mpz_set(total, temp_end->data);
		}

		var_tab_del(&pre_start->clause);
		var_connections=abs_tab_check(&pre_start->clause);
		temp_connections=var_tab_check(&temp_end->clause);
		var_tab_add(&pre_start->clause);
		int res=clause_size[pre_start->clause]-var_connections.connections;

		mpz_divexact_ui(total,total,pow(2,res));

		mpz_divexact_ui(removed,temp_end->removed,pow(2,clause_size[pre_start->clause]-var_connections.connections));


		if(compare_clause(init_end,init_start)==-1 && abs_tab_check(&init_end).connections!=0){
			mpz_add(total, total, removed);
		}


		// if all variables have been accoutned for whether negated or connected for
		if(neg_tab_check(&pre_start->clause).connections== clause_size[pre_start->clause]){
			mpz_divexact_ui(total,temp_end->data,pow(2,clause_size[pre_start->clause]));
		}
	}

	//outer_layer->removed=removed;
	mpz_set(outer_layer->removed,removed);	
	//outer_layer->data= total;
	mpz_set(outer_layer->data,total);



	//there isn't another clause to check
	if(layer_temp->next==NULL || init_end-layer_temp->clause==1){

		return;
	}


outer_loop=layer_temp->next;
inner_loop=layer_temp->next;

clause_comp=compare_clause(layer_temp->clause,init_start);

while(outer_loop->clause<init_end){

	primary_variable_connections=var_tab_check(&outer_loop->clause);
	

	if(primary_variable_connections.connections==-1 ){

		if(outer_loop->next!=NULL&& outer_loop->next->clause!=init_end){
			
			outer_loop=outer_loop->next;
		}
		else{
			break;
		}
		continue;
	}

	//scale the summed
	//total2=total/pow(2,clause_size[init_end]);
	mpz_divexact_ui(total2, total,pow(2,clause_size[init_end]));

	inner_loop=outer_layer->first_clause;

	outer_layer=append_clause(outer_loop->clause, outer_layer,outer_layer->previous_layer,0,0,0);

	clause_comp=compare_clause(outer_loop->clause,inner_loop->clause);
	var_connections=var_tab_check(&outer_loop->clause);

	determiner=determiner_solve(chain, outer_loop,init_start,outer_loop);

printf(" det start %i out %i end %i cmmand %i \n",determiner.start,outer_loop->clause,determiner.end,determiner.command);

	switch(determiner.command)
		{
			case 0:
				//sub_total=sub_total/pow(2,clause_size[outer_loop->clause]-var_connections.connections);	
				mpz_divexact_ui(sub_total,sub_total,pow(2,clause_size[outer_loop->clause]-var_connections.connections));

				//use data instead of removed and take away from sub_total
				//total2=(sub_total)-outer_layer->previous->data/pow(2,clause_size[outer_loop->clause]-var_connections.connections);
				mpz_divexact_ui(total2,outer_layer->previous->data,pow(2,clause_size[outer_loop->clause]-var_connections.connections));
				mpz_sub(total2, sub_total, total2);

				//sub_total=sub_total*pow(2,clause_size[outer_loop->clause]-var_connections.connections);	
				mpz_mul_ui(sub_total,sub_total,pow(2,clause_size[outer_loop->clause]-var_connections.connections));
			break;
			case 1:
				//sub_total=sub_total/pow(2,clause_size[outer_loop->clause]-var_connections.connections);	
				mpz_divexact_ui(sub_total,sub_total,pow(2,clause_size[outer_loop->clause]-var_connections.connections));

				//if there's a connection after the start, restart from here
				var_tab_add(&outer_layer->clause);

				post_branch_correct(determiner.end,determiner.start,outer_layer->clause,1,outer_layer);
				//collects data from the end of the tree
				mpz_set(total2, outer_layer->next_layer->end->data);
			
				//total2=sub_total-total2;
				mpz_sub(total2, sub_total,total2);

				var_tab_del(&outer_layer->clause);

				//sub_total=sub_total*pow(2,clause_size[outer_loop->clause]-var_connections.connections);	
				mpz_mul_ui(sub_total,sub_total,pow(2,clause_size[outer_loop->clause]-var_connections.connections));


			break;
			case 2:
				//if there's a connect before the start, recall the previous, and continue from the start

				//sub_total=sub_total/pow(2,clause_size[outer_loop->clause]-var_connections.connections);	
				mpz_divexact_ui(sub_total,sub_total,pow(2,clause_size[outer_loop->clause]-var_connections.connections));
				var_tab_add(&outer_layer->clause);

				pre_branch_correct(determiner.end,init_start,outer_layer->clause,pre_start,outer_layer,temp_end);

			if(outer_layer->next_layer!=NULL){
				mpz_set(total2,outer_layer->next_layer->end->data);
				mpz_sub(total2,sub_total,total2);
			}
			else
			{ 
				mpz_set(total2,sub_total);;
			}
				//total2=sub_total-total2;
				
				var_tab_del(&outer_layer->clause);

				//sub_total=sub_total*pow(2,clause_size[outer_loop->clause]-var_connections.connections);	
				mpz_mul_ui(sub_total,sub_total,pow(2,clause_size[outer_loop->clause]-var_connections.connections));

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
	node* outer_end;
	node* outer_layer;
	node* temp_layer;
	vReturn var_connections;
	vReturn temp_connections;
	vReturn primary_variable_connections;


	int clause_comp=0;

	int k=0;
	int clause_diff=0;
	int t_comp=0;
	int m1=0;

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
	
	temp_connections=var_tab_check(&temp_layer->clause);
	var_connections=var_tab_check(&init_start);

	if(var_connections.connections>=0){

		switch(not_start)
		case 0:{ 

			//No connections
			//Scales the last data
			clause_comp=compare_clause(init_start,init_end);

			//total=total/pow(2,clause_size[init_end]);
			mpz_divexact_ui(total,total,pow(2,clause_size[init_end]));

			//removed=temp_layer->removed/pow(2,clause_size[init_end]-clause_comp);
			mpz_divexact_ui(removed, temp_layer->removed,pow(2,clause_size[init_end]-(clause_comp)));

			//total+=removed;
			mpz_add(total,total,removed);
			
			break;
		case 1:

			///continue from a previous clause, that's been connected before


			// count how many variables bisect with this clause
			temp_connections=var_tab_check(&init_end);
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
			mpz_divexact_ui(total,total,pow(2,clause_size[init_end]-temp_connections.inc));
			mpz_divexact_ui(temp, temp_layer->removed,pow(2,clause_size[init_end]-var_connections.connections));

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
		temp_connections=abs_tab_check(&init_start);
		var_tab_add(&init_end);

		mpz_divexact_ui(total,total,pow(2,clause_size[init_end]-var_connections.connections));
		mpz_divexact_ui(removed,temp_layer->removed,pow(2,clause_size[init_end]-var_connections.connections));

				
		if(var_tab_check(&init_start).connections!=-1 || init_start!=1){
			mpz_add(total,total, removed);
		}

		var_tab_del(&init_end);
	// if all variables have been accoutned for whether negated or connected for
		if(comp_tab_check(&init_end).connections== clause_size[init_end]){
			mpz_sub(total, temp_layer->data, temp_layer->removed);
			mpz_divexact_ui(total,total,pow(2,clause_size[init_start]));
		}
		var_tab_add(&init_end);
	}

	outer_layer= append_layer(temp_layer->clause,head,head,nul,0 );

	//outer_layer->removed=removed;
	mpz_set(outer_layer->removed,removed);
	//outer_layer->data= total;
	mpz_set(outer_layer->data,total);

	if(layer_temp->next!=NULL&&var_tab_check(&layer_temp->next->clause).connections==-1) {

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
	primary_variable_connections=var_tab_check(&init_start);



//if this clause is already negated
if(var_tab_check(&outer_loop->clause).connections==-1){ printf("outer loop negate \n"); exit(0);break;}


	outer_layer=append_clause(outer_loop->clause, outer_layer,outer_layer->previous_layer,0,0,0);
	//outer_layer->previous_layer=outer_loop->previous_layer	;
	clause_comp=compare_clause(outer_loop->clause,inner_loop->clause);
	var_connections=var_tab_check(&outer_loop->clause);

	if(var_connections.connections==-1)exit(0);

	determiner=determiner_solve(chain, outer_layer,init_start ,outer_loop);

	//if there's a connections between init start, and current variable, do pre_branch
	int comp =compare_clause(outer_layer->clause,determiner.start);

	switch(determiner.command)
		{
			case 0:

				//sub_total=sub_total/pow(2,clause_size[outer_loop->clause]-var_connections.connections);
				mpz_divexact_ui(sub_total,sub_total,pow(2,clause_size[outer_loop->clause]-var_connections.connections));


				//use data instead of removed and take away from sub_total
				//total2=(sub_total)-outer_layer->previous->data/pow(2,clause_size[outer_loop->clause]-var_connections.connections);

				mpz_divexact_ui(total2,outer_layer->previous->data,pow(2,clause_size[outer_loop->clause]-var_connections.connections));
				mpz_sub(total2,sub_total,total2);

				//sub_total=sub_total*pow(2,clause_size[outer_loop->clause]-var_connections.connections);	
				mpz_mul_ui(sub_total,sub_total,pow(2,clause_size[outer_loop->clause]-var_connections.connections));

			break;
			case 1:

				//post start
				//if there's a connection after the start, restart from here
				//sub_total=sub_total/pow(2,clause_size[outer_loop->clause]-var_connections.connections);	
				mpz_divexact_ui(sub_total,sub_total,pow(2,clause_size[outer_loop->clause]-var_connections.connections));

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
				mpz_mul_ui(sub_total,sub_total,pow(2,clause_size[outer_loop->clause]-var_connections.connections));
			break;
			case 2:
				//if there's a connect before the start, recall the previous, and continue from the start
				var_connections=var_tab_check(&outer_loop->clause);

				//sub_total=sub_total/pow(2,clause_size[outer_loop->clause]-var_connections.connections);
				mpz_divexact_ui(sub_total,sub_total,pow(2,clause_size[outer_loop->clause]-var_connections.connections));

				var_tab_add(&outer_loop->clause);

				pre_branch_correct(determiner.end,determiner.start,outer_layer->clause,outer_layer->previous_layer,outer_layer,head);


				if(outer_layer->next_layer!=NULL){
					mpz_set(total2,outer_layer->next_layer->end->data);
					mpz_sub(total2,sub_total,total2);
				}else{
					printf("error stop \n");
					exit(0);		
					mpz_set(total2, sub_total);
				}

				var_tab_del(&outer_loop->clause);

				//sub_total=sub_total*pow(2,clause_size[outer_loop->clause]-var_connections.connections);	
				mpz_mul_ui(sub_total,sub_total,pow(2,clause_size[outer_loop->clause]-var_connections.connections));
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
bool break_loop=0;
	layer++;
	vReturn var_connections;	
	int match=0;
	node* clause_node=NULL;
	node* clause_temp =NULL;
	node* base=NULL;
	node* previous_result=previous_layer->first_clause;

	//tracks the current sum
	mpz_t current_total;
	mpz_init(current_total);

	//result of the previous result
	mpz_t result;
	mpz_init(result);

mpz_t op3;
mpz_init(op3);


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
	for (int i=begin_layer; i<(clause_count); i++){


		// check how many connections there are that exists from this clause
		var_connections=var_tab_check(&i);
		if(var_connections.connections==-1){continue;}

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

		// how many in total exist as a subset
		// recursive_total=previous_total/pow(2,clause_size[i]-var_connections.connections);
//find and count negates

		mpz_divexact_ui(recursive_total, previous_total, pow(2,clause_size[i]-var_tab_check(&i).connections));

		//if the place is past the initial clause
		if(second==1){
			clause_connections=clause_size[i]-var_connections.connections;

			//scales to include the current clause
			mpz_divexact_ui(current_total,previous_result->removed, pow(2,clause_size[i]));

			//	continue to the next layer
			if(layer==1){
				layer++;	
				
				clause_temp=append_clause(i,clause_node,clause_node->previous_layer,nul,nul,clause_connections);
				//scales sub_total to include the branch
				mpz_divexact_ui(sub_total, sub_total,pow(2,clause_size[i]));

				// Find a connection to variables in this cluase, and determine what to do next
				determiner=determiner_solve(chain,clause_temp,0, NULL);
printf("%i determiner.command %i %i \n",clause_temp->clause, determiner.command,determiner.start);
//if(clause_temp->clause==3){exit(0);halt();determiner.command=0;determiner.end=0;determiner.start=1;}
				switch(determiner.command)
				{
					case 0:
						// Doesn't connect to any clause

						mpz_divexact_ui(result, clause_temp->previous->data,pow(2,clause_size[i]));

					break;
					case 1:
						// Connects	with a clause that exists with in the range of this layer					
halt();
						var_tab_add(&clause_temp->clause);
						post_branch_correct(determiner.end,determiner.start,clause_temp->clause,0,clause_temp);
						// Check return of post branch, and retrieve data from the tree
						if(clause_temp->next_layer!=NULL){
							mpz_set(result, clause_temp->next_layer->end->data);
						}else{
							printf("Error: Rec Check Case !\n");
						}
						var_tab_del(&clause_temp->clause);

					break;
					case 2:
						break;
				}

				// catch if the value is greater than allowed
				if(mpz_cmp(result,current_total)==-1){printf("Error: Rech Check invalid result\n");exit(0);}

				mpz_mul_ui(sub_total, sub_total,pow(2,clause_size[i]));
				layer--;
			}

			//add all the possible, subtract by subset possiblies already taken
			//sum+=current_total-result;

			mpz_sub(current_total,current_total,result);			
			mpz_add(sum,sum,current_total);
			
			//sum2+=recursive_total-result;
			mpz_sub(union_sub,recursive_total,result);
			mpz_add(sum2, sum2, union_sub);	
gmp_printf(" %Zd %Zd %Zd \n", union_sub,sum2,result);
			int k=0;
			
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
			mpz_set(sum,recursive_total);

			if(layer==1){

				clause_node=append_layer(i,previous_layer,previous_layer,sum,clause_connections);
				mpz_set(clause_node->removed,sum);
				mpz_set(sum2,recursive_total);
			}	
			second=1;
					
		}
	
		if(previous_result->next!=NULL){
			previous_result=previous_result->next;
		}
	}

layer--;
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
	all = pow(2,variable_count);
clause_node=NULL;
	// all possible
	mpz_t all_possible;
	mpz_init (all_possible);

	mpz_pow_ui (all_possible,base, variable_count);

	//	Sub_total

	//last of the variables
	//start of the linked list

	mpz_set(sub_total,all_possible);
	mpz_divexact_ui(sub_total,sub_total,pow(2,counted_for));

	clause_node=append_clause(start, clause_node,NULL,sub_total,nul,variable_connections[clause_count][0]);
	pnt=clause_node;
	//recursion begins
	recursive_check(0,1,clause_count,sub_total,clause_node);
		
	if(!clause_node->next_layer){
		mpz_set(pnt->data,sub_total);
//		gmp_printf("total %Zd  possible %Zd var count %i   \n",sub_total,all_possible,variable_count);

//		printf("there'se no clauses\n");
		return;
		
	}
	mpz_mul_ui(sub_total,sub_total,pow(2,variable_connections[start][0]));
	//
	mpz_sub(sub_total,sub_total,clause_node->next_layer->end->data);
mpz_set(pnt->data,sub_total);

//	pnt= (void*)sub_total;
//mpz_set(pnt->data, sub_total);
//debug(pnt);
	gmp_printf("total %Zd  possible %Zd var count %i   \n",sub_total,all_possible,variable_count);

}

int main(int argc, char *argv[]){


//sets the base value of the problem
mpz_init (base);
mpz_set_ui(base,2);

//mpz null ptr
mpz_init(nul);
mpz_set_ui(nul,0);


unsigned int nr_variables;
unsigned int nr_clauses;

// reads the amount of variables and clauses
	//stores temporary literals
int tmp[300]={0};

//init all the postitions
init_pos(variable_position);

init_pos(f_variable_position);
init_pos(tree_position);


//stores all the ones
//stores variable number in clause
int b=0;
int cl=1;
//scans file
int literal;

// imports cnf into local memory
read_cnf(argv[1]);


//**********************
//start of the algorithm
//**********************

	printf("%i %i\n", clause_count,variable_count); 


init_graph(ones);


printf(" complete \n ");
//	halt();
//exit(0);

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

