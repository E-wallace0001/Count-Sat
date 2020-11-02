


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "headers/infini_tree.h"
#include "headers/var_pos.h"
#include "headers/chain_block.h"
#include "headers/llist.h"

#include <gmp.h>
#include <assert.h>

//null bignum
mpz_t nul;
//mpz_init(nul);

// base(n) -- algo works of any base
mpz_t base;
mpz_t sub_total;


//linked list variables
node* head=NULL;
node* root;

//chain_block init
chain_block* chain;

// table of placement of variables
variable_pos* variable_position[1001];

void init_pos(){
	for(int i=1;i<=1000;i++){
		variable_position[i]=create_pos(0,variable_position[i]);
		//variable_position[i]
	}
}


long long all=0;

int start=0;

int amount=0;
int loop_count=0;
int variable[258][20]={0};
int variable_connections[127000][20]={0};
int direct_connections[253]={0};
int connection_count[200]={0};

int first_connect[1000]={0};

long set_variable[2538]={0};

int clause_size[200]={0};


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
static inline void var_tab_add(int *clause_a)
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
static inline void var_tab_del(int *clause_d)
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

// counts new variable connections in the pre branch
static inline int count_tree(int branch_start, node* head){
link_node* root=NULL;
int count=0;
//printf(" count tree %i %i\n",branch_start, head->clause);
for(int x=clause_size[branch_start];x!=0;x--){

	if(set_variable[abs(variable_connections[branch_start][x])]==variable_connections[branch_start][x]){
		//link_pop(root);
		printf("%i N\n",branch_start);
		//count++;
		if(x<0){
		//continue;
		}
	}


	root=link_append(root,variable_connections[branch_start][x]);

}


while(1){

	//compare the node of the tree to the pre branch
	for(int x=clause_size[head->clause];x!=0;x--){

			//printf("error %i %i \n",head->clause,x);

		//if there are no variables left, all variables of the node have been accounted for
		if(root->first==NULL){

			//All variables taken
			return(clause_size[branch_start]);
			break;
		}

		//start from the begining of the root variables
		root=root->first;
		while(1){

			
			//if there's a match between the two nodes, remove them
			if(abs(root->data)==abs(variable_connections[head->clause][x])){

				link_pop(root);
				count++;
				break;
			}



			if(root->next!=NULL){
				root=root->next;
			}
			else{
				break;
			}

		}

	}	

	if(head->previous_layer->clause>=branch_start){
	break;
	}
	else{

		head=head->previous_layer;
	
	}

}
return(count);
}

//check to see if there are matches between two clauses that are independant of the ones checked
static inline int subset_check(int a, int b){
   link_node* variable_saved=NULL;
	int clause_a;
	int clause_b;
	int count=0;
	clause_a=variable_connections[a][0];
	clause_b=variable_connections[b][0];

	for(int x= clause_size[a];x!=0;x--){
		for(int y=clause_size[b];y!=0;y--){
			
			if(abs(variable_connections[a][x])==abs(variable_connections[b][y])){
				if(variable_connections[a][x]==variable_connections[b][y]){

					variable_saved=link_append(variable_saved,variable_connections[a][x]);
					//printf("found\n");

				}else{
					variable_saved=link_append(variable_saved,variable_connections[a][x]);
				
				}
			}
		}
	}
if(variable_saved==NULL){return(0);}
	//printf("subset_check variable_saved %i\n", variable_saved->first->data);

	variable_saved=variable_saved->first;


	while(1){	
		
		if(set_variable[abs(variable_saved->data)]==0){

			count++;
		}

		if(variable_saved->next!=NULL){
			variable_saved=variable_saved->next;
			//printf("l %i \n", variable_saved->data);
		}else{
			break;
		}

		
	}
//printf(" count %i \n", count);

link_dispose(variable_saved);

return(count);
}

static inline int sub_check(int a){
	int clause_a;
	int count=0;

	for(int x= clause_size[a];x!=0;x--){

		if(set_variable[abs(variable_connections[a][x])]!=0){
			count++;
		}
	}

return(count);
}

static inline int compare(int a, int b){
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
					//return(-1);
				}
			}
		}
	}

return(count);
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

//compares variables of two clauses- discounts those already accounted
static inline int nabc(int a, int b){
	int clause_a;
	int clause_b;
	int count=0;
//	clause_a=variable_connections[a][0];
//	clause_b=variable_connections[b][0];

var_tab_del(&b);
	for(int y=clause_size[b];y!=0;y--){
			for(int x= clause_size[a];x!=0;x--){
					
				if(variable_connections[a][x]==variable_connections[b][y]){
					if(set_variable[abs(variable_connections[a][x])]!=0){
						count++;
						//continue;
					}
					
				//	break;
	count++;
				}
	//count++;

			}
		}

var_tab_add(&b);

return(count);
}




//compares variables of two clauses- discounts those already accounted
static inline int abc(int a, int b,int c){
	int clause_a;
	int clause_b;
	int count=0;
	int count1;

	bool list[300]={0};
	bool stop;

	for(int x= clause_size[a];x!=0;x--){
		for(int y=clause_size[c];y!=0;y--){
			if(variable_connections[a][x]==variable_connections[c][y]){	
				count1=0;
				for(int z= clause_size[b];z!=0;z--){
					if(variable_connections[b][z]==variable_connections[c][y]){	
					count1++;
					}
				}
				if(count1!=0){continue;}
				count++;

			}
		}
	}

	for(int x= clause_size[b];x!=0;x--){
		for(int y=clause_size[c];y!=0;y--){
			if(variable_connections[b][x]==variable_connections[c][y]){	
				count1=0;
				for(int z= clause_size[a];z!=0;z--){
					if(variable_connections[a][z]==variable_connections[b][x]){	
					count1++;
					}
				}
				if(count1!=0){continue;}
				count++;
			}
		}
	}

return(count);
}

//compares variables of two clauses- discounts those already accounted
static inline int abcc(int a, int b){
	int clause_a;
	int clause_b;
	int count=0;
	int count1;

	bool list[300]={0};
	bool stop;

	for(int x= clause_size[a];x!=0;x--){
		for(int y=clause_size[b];y!=0;y--){
			if(variable_connections[a][x]==variable_connections[b][y]){	
					if(set_variable[abs(variable_connections[a][x])]!=0 ||set_variable[abs(variable_connections[b][y])]!=0){	
			//	continue;
					}
				count++;

			}
		}
	}

	

return(count);
}

//compares clauses a-b with root
static inline int abr(int a, int b,int c){
	int clause_a;
	int clause_b;
	int count=0;
	int count1;
//	clause_a=variable_connections[a][0];
//	clause_b=variable_connections[b][0];

bool list[300]={0};

bool stop;

	for(int x= clause_size[a];x!=0;x--){
			if(set_variable[abs(variable_connections[a][x])]!=0){	
printf( " A set %li \n",set_variable[abs(variable_connections[a][x])]);
							count++;
				list[abs(variable_connections[a][x])]=1;
				continue;
			}
	}
	for(int y=clause_size[b];y!=0;y--){

		if(set_variable[abs(variable_connections[b][y])]!=0 && list[abs(variable_connections[b][y])]==0){	
printf( " B set %li \n",set_variable[abs(variable_connections[b][y])]);
			//count++;
			//list[abs(variable_connections[b][y])]=1;
		}
	}

return(count);
}


//compares variables of two clauses- discounts those already accounted
static inline int abnc(int a, int b){
	int clause_a;
	int clause_b;
	int count=0;
//	clause_a=variable_connections[a][0];
//	clause_b=variable_connections[b][0];

	for(int x= clause_size[a];x!=0;x--){
		for(int y=clause_size[b];y!=0;y--){
			
			if(abs(variable_connections[a][x])==abs(variable_connections[b][y])){
		printf("a:%i x:%i - %i || b%i y %i - %i\n", a, x,variable_connections[a][x],b,y,variable_connections[b][y]);
					var_tab_del(&b);
					if(set_variable[abs(variable_connections[b][y])]==variable_connections[b][y]){

						count++;						
					}
					var_tab_add(&b);

			}
		}
	}

return(count);
}

//compares variables of two clauses- discounts those already accounted
static inline int aabc(int a, int b){
	int clause_a;
	int clause_b;
	int count=0;


	for(int x= clause_size[a];x!=0;x--){
		for(int y=clause_size[b];y!=0;y--){
			
			if(abs(variable_connections[a][x])==abs(variable_connections[b][y])){
		printf("a:%i x:%i - %i || b%i y %i - %i\n", a, x,variable_connections[a][x],b,y,variable_connections[b][y]);
					count++;
					continue;
			}
			if(set_variable[abs(variable_connections[b][y])]==variable_connections[b][y]){
				count++;	
				continue;
			}
		}
	}

return(count);
}



//compares variables of two clauses- discounts those already accounted
static inline int indie_comp(int a, int b){
	int clause_a;
	int clause_b;
	int count=0;
	clause_a=variable_connections[a][0];
	clause_b=variable_connections[b][0];

	for(int x= clause_size[a];x!=0;x--){
		for(int y=clause_size[b];y!=0;y--){
			
			if(abs(variable_connections[a][x])==abs(variable_connections[b][y])){
				count++;
			}
		}
	}

return(count);
}



static inline int t_clause_compare(int a, int b, int c){
   link_node* variable_saved=NULL;
	int clause_a;
	int clause_b;
	int count=0;
	clause_a=variable_connections[a][0];
	clause_b=variable_connections[b][0];

	for(int x= clause_size[a];x!=0;x--){
		for(int y=clause_size[b];y!=0;y--){
			
			if(abs(variable_connections[a][x])==abs(variable_connections[b][y])){
				if(variable_connections[a][x]==variable_connections[b][y]){
					//count++;

					variable_saved=link_append(variable_saved,variable_connections[a][x]);
					//printf("found\n");
				}else{
	variable_saved=link_append(variable_saved,variable_connections[a][x]);
				
					//return(-1);
				}
			}
		}
	}
if(variable_saved==NULL){return(0);}
//printf("t_clause_comp variable_saved %i\n", variable_saved->first->data);
	for(int x= clause_size[c];x!=0;x--){

		variable_saved=variable_saved->first;

		while(1){	
			//printf("%i %i \n", variable_saved->data,variable_connections[c][x]);
			if(variable_saved->data==variable_connections[c][x]){
				count++;

				break;
			}
			if(variable_saved->next!=NULL){
				variable_saved=variable_saved->next;
				//printf("l %i \n", variable_saved->data);
			}else{

				break;
			}

		}
	}
//printf(" count %i \n", count);

link_dispose(variable_saved);

return(count);
}

//counts how many connections between clauses a and b that have been counted for before
static inline int non_unique(int a, int b){
   link_node* variable_saved=NULL;
	link_node* variable_saved2=NULL;
	int clause_a;
	int clause_b;
	int count=0;
	clause_a=variable_connections[a][0];
	clause_b=variable_connections[b][0];


	for(int y=clause_size[a];y!=0;y--){
			
		if(set_variable[abs(variable_connections[a][y])]==variable_connections[a][y]){
				variable_saved=link_append(variable_saved,variable_connections[a][y]);
				count++;
		}
	}

	for(int y=clause_size[b];y!=0;y--){
	
		if(set_variable[abs(variable_connections[b][y])]==variable_connections[b][y]){
				variable_saved=link_append(variable_saved,variable_connections[b][y]);	
				count++;
		}
	}


	for(int x= clause_size[a];x!=0;x--){

		for(int y=clause_size[b];y!=0;y--){
			


			if(abs(variable_connections[a][x])==abs(variable_connections[b][y])){
				variable_saved=variable_saved->first;
while(1){

					if(variable_saved->data==variable_connections[b][y]){
						
						count--;
						break;
					}

					if(variable_saved->next!=NULL){
						variable_saved=variable_saved->next;
					}
					else{
						break;
					}

				}				

			}


		}
	}



//printf("count5 %i\n", count5);
//count=0;
link_dispose(variable_saved);

return(count);	



}

// searches for unique variable matchs between three clauses
static inline int unique_test(int a, int b){
   link_node* variable_saved=NULL;
	link_node* variable_saved2=NULL;
	int clause_a;
	int clause_b;
	int count=0;
	clause_a=variable_connections[a][0];
	clause_b=variable_connections[b][0];


	for(int y=clause_size[a];y!=0;y--){
			printf("%i \n", variable_connections[a][y]);
		if(set_variable[abs(variable_connections[a][y])]!=0){
				count++;
				variable_saved=link_append(variable_saved,variable_connections[a][y]);
				
		}
	}

	for(int y=clause_size[b];y!=0;y--){
	
		if(set_variable[abs(variable_connections[b][y])]!=0){
				//count++;

		//		variable_saved=link_append(variable_saved,variable_connections[b][y]);	
		}
	}
//printf(" a %i b %i \n", a,count);exit(0);

	for(int x= clause_size[a];x!=0;x--){

		for(int y=clause_size[b];y!=0;y--){
			
			if((abs(variable_connections[a][x]))==abs(variable_connections[b][y])){
				
				//if(variable_saved==NULL){return(0);}
				//count--;
				variable_saved=variable_saved->first;
				while(1){

					if(abs(variable_saved->data)==abs(variable_connections[b][y])){
						
						//count--;
						//break;
					}

					if(variable_saved->next!=NULL){
						variable_saved=variable_saved->next;
					}
					else{
						break;
					}
				}
			
			}
		}
	}


	for(int x= clause_size[a];x!=0;x--){

		for(int y=clause_size[b];y!=0;y--){
			
			if(((variable_connections[a][x]))==(variable_connections[b][y]) ){
				
			//	count++;
			printf("count %i \n", count);
			}
		}
	}
exit(0);
printf("count5 %i\n", count);
//count=0;
link_dispose(variable_saved);

return(count);	



}

// searches for unique variable matchs between three clauses
static inline int unique_var(int a, int b, int c){
   link_node* variable_saved=NULL;
	link_node* variable_saved2=NULL;
	int clause_a;
	int clause_b;
	int count=0;
	clause_a=variable_connections[a][0];
	clause_b=variable_connections[c][0];

	for(int x= clause_size[a];x!=0;x--){
		for(int y=clause_size[c];y!=0;y--){
			
			if(abs(variable_connections[a][x])==abs(variable_connections[c][y])){
					count++;
					variable_saved=link_append(variable_saved,variable_connections[a][x]);
					
			}
		}
	}
//printf("count1 %i\n", count);
//count=0;

int count5=0;

	clause_a=variable_connections[b][0];
	clause_b=variable_connections[c][0];

	for(int x= clause_size[b];x!=0;x--){
		for(int y=clause_size[c];y!=0;y--){
			//printf(" %i %i \n",variable_connections[b][x],variable_connections[c][y]);
			if(abs(variable_connections[b][x])==abs(variable_connections[c][y])){
				count++;
				if(variable_saved==NULL){break;}
				variable_saved=variable_saved->first;
				while(1){
					if(abs(variable_saved->data)==abs(variable_connections[b][x])){
						//c;
						//
						count5++;


						if(variable_saved->next==NULL){break;}else{variable_saved=variable_saved->next;}
					}else{
					
						if(variable_saved->next==NULL){break;}else{variable_saved=variable_saved->next;}
						variable_saved2=link_append(variable_saved2,variable_connections[b][y]);
					}
				}


					
			}
		}
	}
//printf("count5 %i\n", count5);
//count=0;

return(count-count5);	

bool counted =0;

if(variable_saved==NULL){return(count);}
if(variable_saved2==NULL){return(count);}


// Find a subset between  A -> B && A->C
//count=0;
variable_saved=variable_saved->first;
int count2=0;
int count3=0;
while(1){
	variable_saved2=variable_saved2->first;

//printf(" %i %i \n ", abs(variable_saved->data),(abs(variable_saved2->data)));
	while(1){
//printf(" %i %i \n ", abs(variable_saved->data),(abs(variable_saved2->data)));
//printf(" count %i \n",count);
		if(abs(variable_saved->data)==abs(variable_saved2->data)){ 
//	count++;
		counted=1;
		//
		break;
		}

		if(variable_saved2->next==NULL){
count2++;
//printf("added \n");

			break;
		}
		else{
		variable_saved2=variable_saved2->next;
		}

	}

if (counted==1){ 
count3++;
counted=0;
};
	if(variable_saved->next==NULL){
		
		break;
	}
	else{

		variable_saved=variable_saved->next;
	}

}

//printf(" count %i %i %i \n", count,count2,count3);

link_dispose(variable_saved);

return(6);
}
//check to see if there are matches between two clauses that are independant of the ones checked
static inline int subset_check2(int a, int b){
   link_node* variable_saved=NULL;
	link_node* test=NULL;
	int clause_a;
	int clause_b;
	int count=0;
	clause_a=variable_connections[a][0];
	clause_b=variable_connections[b][0];

	//make sure start doesn't interfere
	var_tab_del(&b);


	for(int x= clause_size[a];x!=0;x--){
		if(set_variable[abs(variable_connections[a][x])]==variable_connections[a][x]){
			variable_saved=link_append(variable_saved,variable_connections[a][x]);
			count++;
			continue;
		}
		for(int y= clause_size[b];y!=0;y--){
			if(variable_connections[a][x]==variable_connections[b][y]){
				count++;
			}

		}
		
	}

	for(int x= clause_size[b];x!=0;x--){
		if(set_variable[abs(variable_connections[b][x])]==variable_connections[b][x]){
			if(variable_saved==NULL){ count++; }
			else{
				test=variable_saved->first;
				while(1){
					if(test->data==variable_connections[b][x]){
						break;
					}
					if(test->next!=NULL){
						test=test->next;
					}else{
						count++;
						break;
					}
				}
			}
		}
		
	}

	var_tab_add(&b);

	printf(" count %i \n", count);
	if(variable_saved==NULL){return(count);}
	variable_saved=variable_saved->first;







link_dispose(variable_saved);

return(count);
}

// searches for unique variable matchs between three clauses
static inline int unique_check(int a, int b){
   link_node* variable_saved=NULL;
	link_node* variable_saved2=NULL;
	int clause_a;
	int clause_b;
	int count=0;
	clause_a=variable_connections[a][0];
	clause_b=variable_connections[b][0];

	for(int x= clause_size[a];x!=0;x--){
		if(set_variable[abs(variable_connections[a][x])]==variable_connections[a][x]){
//printf(" set_variable[abs(variable_connections[%i][%i] %li \n",a,x,set_variable[abs(variable_connections[a][x])]);

			count++;
			//continue;
		}
	}

	for(int x= clause_size[b];x!=0;x--){
		if(set_variable[abs(variable_connections[b][x])]==variable_connections[b][x]){
//printf(" set_variable[abs(variable_connections[%i][%i] %li \n",b,x,set_variable[abs(variable_connections[b][x])]);
			count++;

			for(int y= clause_size[a];y!=0;y--){
				
				if(variable_connections[a][y]==variable_connections[a][y]){
						count--;
						break;
				}
			}	



		}
	}


//link_dispose(variable_saved);

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


int pre_connected(int head){

	int connected=0;

	for(int q=clause_size[head]	;q!=0;q--){
		//printf("nclause %i x %i %i \n",clause, q,variable_connections[clause][q]);

		//if the variable has already been accounted for, skip
			
		//if the first show of this variables appears before the clause
		if(variable_position[abs(variable_connections[head][q])]->clause < head){
			if(set_variable[abs(variable_connections[head][q])]==0){
			connected++;
}
			continue;
		}
	}

	return connected;

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
	bool halt=0;
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
	if(compare_clause(start,head->clause)==-1){
		//result.command=2;
		//exit(0);
		//temp_connections=var_tab_check(&head->clause);
		//if(temp_connections.connections>0){ result.command=2; }

		//return(result);
	}

int p=0;
	// for each variable in clause, search for previous clauses containing this variable
	for(int q=clause_size[clause]	;q!=0;q--){

		//printf("nclause %i x %i %i \n",clause, q,variable_connections[clause][q]);

		//if the variable has already been accounted for, skip
			
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

			//printf("--test test %i %i %i  \n", variable_connections[clause][q],clause, q);

		while(1){
			temp_connections=var_tab_check(&link->clause);
//printf("  link %i \n", link->clause);
//if(link->clause==head->clause){break;}

			if(temp_connections.connections>=0){break;}

			if(link->next!=NULL && link->next->clause!=head->clause ){

				link=link->next;

			}
			else{

				break;
			}		
			
		}

		temp_connections=var_tab_check(&link->clause);

		//if this is the first time it exists	
		if(saved_clause==NULL && link->clause!=head->clause){
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
				printf("exp\n");		
				result.end=1;
//

			}
			 if(clause_comp!=0){
				printf("exp %i \n",stored_clause->clause);	
				result.end=1;

			}			
		}


printf("start %i \n",var_connections.connections);
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

		//result.start=start;
		//result.command=1;
		//result.end=2;

//exit(0);
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

	//head_temp=layer_temp;

	if(temp_end->next_layer==NULL){
		printf(" no new layer\n");
	return;
	//	exit(0);
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

	if(var_connections.connections>=clause_size[init_start]){
printf(" end here\n");
		//return;
	}


//if the start and end are negated, do this
clause_comp=compare_clause(init_start, init_end);
if(var_tab_check(&init_start).connections==-1){

	if(temp_end->previous!=NULL && temp_end->previous->clause>=init_start &&  var_connections.connections==-1){
printf(" pre change \n");
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
		printf(" temp end pre %i , head %i \n", temp_end->clause, head->clause);
		//	temp_connections=var_tab_check(&temp_end->clause);
		printf("%i %i \n",	temp_end->clause,clause_comp);
	}
}

if(temp_layer->clause<=pre_start->next_layer->clause){

//layer_temp=layer_temp->next;

//temp_layer=temp_layer->next;
printf("really not the right one temp %i ini %i \n",temp_end->clause ,pre_start->next_layer->clause);

}


printf("error\n");
	outer_layer= append_layer(temp_end->clause,head,head,nul,0 );

//printf("\n\ninit %i connectiosn %i\n", init_start,var_connections.connections);
	if(var_connections.connections>=0){

	int clause_diff=0;

	printf("\nhead %i init_start %i  init_end %i \n\n\n", head->clause, init_start, init_end);


		switch(not_start)
		case 0:{	

			// basic pre-start
			int clause_comp2=compare_clause(init_start, init_end);


			clause_comp=compare_clause(init_start,pre_start->clause);
var_tab_del(&init_end);
var_connections=var_tab_check(&pre_start->clause);
var_tab_add(&init_end);
			//total=total/pow(2,clause_size[con]-clause_comp2);
//var_connections.connections=0;

printf("pre start %i init start %i init end %i head->clause %i head->previous_layer->clause %i \n",pre_start->clause,init_start, init_end, head->clause, head->previous_layer->clause);
clause_comp2=abnc(init_start, init_end);
printf("%iabc %i \n", clause_comp,clause_comp2);

printf("var_ con %i %i \n",var_connections.connections, var_connections.inc);
			mpz_divexact_ui(total,total,pow(2,clause_size[pre_start->clause]-var_connections.inc));
	//gmp_printf("removed %Zd data %Zd == removed %Z dtotal %Zd \n", temp_layer->removed,temp_layer->data,removed, total);

			//removed=temp_end->removed/pow(2,clause_size[con]-1);
			mpz_divexact_ui(removed,temp_end->removed,pow(2,clause_size[pre_start->clause]-clause_comp));
		mpz_add(total,total,removed);

			break;
		case 1:
printf("pre branch case 1 \n\n");
			//if a connections has already been accounted for
			mpz_sub(total, temp_end->data, temp_end->removed);
			//var_connections=var_tab_check(&init_start);

			clause_comp=compare_clause(init_end,pre_start->clause);

			printf(" pre 1 \n\n\n");

			//clause_comp=compare_clause(init_start,init_end);



//var_tab_del(&init_end);
var_tab_add(&init_start);
var_connections=var_tab_check(&pre_start->clause);
//var_connections=var_tab_check(&init_end);
//var_tab_add(&init_end);


var_tab_del(&init_start);
//temp_connections=var_tab_check(&init_end);
//var_tab_add(&init_end);

var_connections=var_tab_check(&pre_start->clause);
int cc= compare_clause(pre_start->clause, init_end);

clause_comp2=abcc(init_start, init_end);
//clause_comp2=compare_clause(pre_start->clause, init_start);
//clause_comp=1;


//if(init_end==4){clause_comp=1;}

// start to pre
int tot=clause_size[pre_start->clause]-var_connections.inc;

				mpz_divexact_ui(total, total,pow(2,tot));
//debug(temp_end);
//if(head->clause==5){clause_comp-=1;}


clause_comp=pre_connected(pre_start->clause);
var_tab_add(&init_start);
var_tab_del(&pre_start->clause);
temp_connections=var_tab_check(&pre_start->clause);
var_tab_del(&init_start);
var_tab_add(&pre_start->clause);

//needs to be temp
			mpz_divexact_ui(removed,temp_end->removed,pow(2,clause_size[pre_start->clause]-temp_connections.connections));
				//if all is taken care of
gmp_printf("data %Zd removed %Zd  %Zd  %Zd \n",temp_end->data,temp_end->removed,removed, total);
printf("pre_start %i init start %i %i %i \n ",pre_start->clause, init_start, init_end,head->previous_layer->clause);



printf(" binnnng \n\n\n\nvar_con %i %i %i \n",var_connections.connections,var_connections.inc,cc);
printf(" var_con %i %i %i \n",temp_connections.connections,temp_connections.inc,clause_comp);

gmp_printf("cc-%i clause_comp %i temp_comp %i pre_con %i \n",clause_comp,clause_comp2,pre_connected(pre_start->clause),pre_connected(init_end));

gmp_printf("data %Zd removed %Zd  %Zd \n",temp_end->data,removed, total);
mpz_add(total,total,removed);
//exit(0);

			break;
		case 2:
			
printf("\n\n\n\n\n\n\n\n\n\n\npre_Branch 2\n\n\n");

			///continue from a previous variable
		//	var_tab_del(&init_end);
//		var_tab_del(&pre_start->clause);
printf(" init_start %i init_end %i pre_start %i \n", init_start, head->previous_layer->clause, pre_start->clause);

	
//			var_tab_add(&pre_start->clause);

			//var_tab_add(&init_end);

			t_comp=t_clause_compare(head->clause,init_start, init_end);

			//m1=pow(2,clause_comp-t_comp);

			//total= temp_layer->data-temp_layer->removed*m1;
			mpz_sub(total, temp_end->data, temp_end->removed);
			 // how many variables there are that haven't been accounted for
			//total=total/pow(2,var_connections.inc)

//gmp_printf("tempdata %Zd removed %Zd  %Zd \n",temp_end->data,temp_end->removed, total);
////gmp_printf("tempdata %Zd removed %Zd  %Zd \n",temp_layer->data,temp_layer->removed, total);

			//total+=temp_layer->removed/pow(2,clause_size[head->clause]-clause_comp);
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

			clause_comp=compare_clause(init_start, head->previous_layer->clause);
//printf("zero %i %i \n",clause_size[pre_start->clause],var_connections.connections);
int res=clause_size[pre_start->clause]-var_connections.connections;
if(res<=0){
printf("zero %i %i \n",clause_size[pre_start->clause],var_connections.connections);
res=0;
}
clause_comp2=abnc(init_start, head->clause);

printf("abc %i %i %i  \n", res,clause_size[pre_start->clause], head->previous_layer->clause);

			mpz_divexact_ui(temp, temp_end->removed,pow(2,res)); //(a^b)^(b^c) ¬(a^b^c)  
//modified
			mpz_divexact_ui(total,total,pow(2,clause_size[pre_start->clause]-temp_connections.connections)); //(b^c)
			mpz_set(removed, temp);
			mpz_add(total,total, temp);

			printf("init start %i %i %i\n ", init_start, init_end,pre_start->clause);

			//gmp_printf("data %Zd removed %Zd  %Zd var_Con %i %i %i\n",total,temp, total,var_connections.inc,var_connections.connections,temp_connections.connections);

			break;
		}		
	}
	else{
			//If the start is negate
			printf("\n\n pre_branch Negate\n\n");

			printf("nonun %i %i \n", init_start, init_end);

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

		printf(" compare_clause(init_end,init_start) %i %i\n",compare_clause(init_start,init_end),var_connections.connections);
//if(temp_layer->clause<=temp_layer->previous_layer->first_clause->clause) temp_layer=temp_layer->next;printf("this is set before the stat \n");halt();

		var_tab_del(&pre_start->clause);
		var_connections=abs_tab_check(&pre_start->clause);
		temp_connections=var_tab_check(&temp_end->clause);
		var_tab_add(&pre_start->clause);
		int res=clause_size[pre_start->clause]-var_connections.connections;


		printf(" compare_clause(init_start %i,init_end %i) %i %i\n",temp_end->clause,init_end,compare_clause(init_start,init_end),abs_tab_check(&temp_end->clause).connections);
		if(clause_size[init_end]<=temp_connections.connections){
			printf("zero\n");
			//res=0;
		}
		gmp_printf("-1 dat..:%i %i %Zd %Zd  %Zd \n",res,temp_connections.connections, temp_end->removed, temp_end->data,total);

		mpz_divexact_ui(total,total,pow(2,res));

		mpz_divexact_ui(removed,temp_end->removed,pow(2,clause_size[pre_start->clause]-var_connections.connections));

		printf(" init start %i init end %i %i\n", init_start,init_end, temp_end->clause);
		gmp_printf("removed %Zd %Zd \n", removed, total);
		if(var_tab_check(&temp_end->clause).connections==-1) {


		}

		if(compare_clause(init_end,init_start)==-1 && abs_tab_check(&init_end).connections!=0){
			printf(" added %i \n", head->previous_layer->first_clause->clause);
			mpz_add(total, total, removed);
		}


		// if all variables have been accoutned for whether negated or connected for
		if(neg_tab_check(&pre_start->clause).connections== clause_size[pre_start->clause]){

		mpz_divexact_ui(total,temp_end->data,pow(2,clause_size[pre_start->clause]));
		//	mpz_divexact_ui(removed,temp_end->removed,pow(2,clause_size[pre_start->clause]-1));
//gmp_printf("removed %Zd %Zd \n", removed, temp_end->removed);

		}





	printf("this is what i want %i %i\n",pre_start->clause,head->previous_layer->clause);
	}



	//outer_layer->removed=removed;
	mpz_set(outer_layer->removed,removed);	
	//outer_layer->data= total;
	mpz_set(outer_layer->data,total);




	if(layer_temp->next==NULL || init_end-layer_temp->clause==1){
//gmp_printf("removed %Zd %Zd \n", removed, total);
printf(" no next \n"); 

		return;
	}

printf("go go \n");

//this is the return
//return;

outer_loop=layer_temp->next;
inner_loop=layer_temp->next;

//	clause_comp=compare_clause(outer_loop->clause,outer_layer->first_clause->clause);
clause_comp=compare_clause(layer_temp->clause,init_start);


while(outer_loop->clause<init_end){
printf(" %i outer loop %i \n",init_start, outer_loop->clause);

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

	int comp =compare_clause(outer_layer->clause,init_start);
	//if there's a connection between the start and the current variable, pre_correct
	if(determiner.command==2){
		//determiner.command=1;determiner.end=2;
	}

//if (determiner.start==0 && determiner.command!=0){printf("no start\n");determiner.start=3;}
printf("\n\npre branch start: %i end:%i cmd:%i  outer:%i\n",determiner.start,determiner.end, determiner.command,outer_layer->clause);

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
printf("this one to thceck \n");
			break;
			case 1:
printf("case 1 \n");
//exit(0);
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

//exit(0);

			break;
			case 2:
				//if there's a connect before the start, recall the previous, and continue from the start
printf("pre pre\n");

//exit(0);
//
//debug(temp_end);
//determiner.end=1;
//				var_connections=var_tab_check(&outer_loop->clause);

				//sub_total=sub_total/pow(2,clause_size[outer_loop->clause]-var_connections.connections);	
				mpz_divexact_ui(sub_total,sub_total,pow(2,clause_size[outer_loop->clause]-var_connections.connections));
				var_tab_add(&outer_layer->clause);

//int k2=4;
//var_connections= var_tab_check(&k2);
printf(" %i\n", var_connections.connections);

				pre_branch_correct(determiner.end,init_start,outer_layer->clause,pre_start,outer_layer,temp_end);

			if(outer_layer->next_layer!=NULL){
				mpz_set(total2,outer_layer->next_layer->end->data);
mpz_sub(total2,sub_total,total2);
			}
			else
			{ 
//mpz_set_ui(total2,0);
mpz_set(total2,sub_total);;
			}
				//total2=sub_total-total2;
				


//gmp_printf("attempt %Zd \n", total2);

				var_tab_del(&outer_layer->clause);

				//sub_total=sub_total*pow(2,clause_size[outer_loop->clause]-var_connections.connections);	
				mpz_mul_ui(sub_total,sub_total,pow(2,clause_size[outer_loop->clause]-var_connections.connections));

			break;
			case 3: 

exit(0);
				//sub_total=sub_total/pow(2,clause_size[outer_loop->clause]);	
				mpz_divexact_ui(sub_total,sub_total,pow(2,clause_size[outer_loop->clause]));
				var_tab_add(&outer_layer->clause);

				post_branch_correct(determiner.end,determiner.start,outer_layer->clause,1,outer_layer);
				mpz_set(total2, outer_layer->next_layer->end->data);

				//total2=sub_total-total2;
				mpz_sub(total2,sub_total,total2);
				var_tab_del(&outer_layer->clause);
				//sub_total=sub_total*pow(2,clause_size[outer_loop->clause]);
				mpz_mul_ui(sub_total,sub_total,pow(2,clause_size[outer_loop->clause]));
			break;
			case 4:
			break;
		}
			

	
	//end of conditional pre_loop
	if(outer_loop->clause!=init_start){
	
		//total+=total2;
		mpz_add(total,total,total2);
//gmp_printf("attempt %Zd \n", total2);

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
	

	//printf("total %i \n", total);
	temp_connections=var_tab_check(&temp_layer->clause);
	var_connections=var_tab_check(&init_start);


//if(var_connections.connections>=clause_size[init_end]) return;





printf(" head %i \n", head->clause);
	if(var_connections.connections>=0){

		switch(not_start)
		case 0:{ 
//printf("had %i \n", head->clause);
			//No connections
			//Scales the last data
			clause_comp=compare_clause(init_start,init_end);
			//total=total/pow(2,clause_size[init_end]);
			mpz_divexact_ui(total,total,pow(2,clause_size[init_end]));
			//removed=temp_layer->removed/pow(2,clause_size[init_end]-clause_comp);
			mpz_divexact_ui(removed, temp_layer->removed,pow(2,clause_size[init_end]-(clause_comp)));

			//total+=removed;
			mpz_add(total,total,removed);
			
		
			gmp_printf("removed %Zd %Zd \n", removed, total);

//
			break;
		case 1:
printf("post_Branch 1\n\n\n");

//halt();
			///continue from a previous variable

temp_connections=var_tab_check(&init_end);
var_tab_del(&init_end);
var_tab_add(&init_start);
			var_connections=var_tab_check(&init_end);
var_tab_del(&init_start);
var_tab_add(&init_end);

			clause_comp=compare_clause(init_start,head->clause);

			t_comp=t_clause_compare(head->clause,init_start, init_end);

			m1=pow(2,clause_comp-t_comp);

			//total= temp_layer->data-temp_layer->removed*m1;
			//mpz_mul_ui(total, temp_layer->removed,m1);
			mpz_sub(total, temp_layer->data, temp_layer->removed);
			//gmp_printf("data %Zd \n", total);
//
			 // how many variables there are that haven't been accounted for
			//total=total/pow(2,var_connections.inc);
			clause_comp=compare_clause(init_start, init_end);
 
printf("init start %i %i \n ", init_start, init_end);
//			clause_comp=subset_check2(init_start,init_end);
m1=clause_size[init_end]-var_connections.connections;
if(m1<=0){m1=clause_size[init_end];}
printf("error\n");
int t= clause_size[init_end]-var_connections.connections;
			mpz_divexact_ui(total,total,pow(2,clause_size[init_end]-temp_connections.inc));
			mpz_divexact_ui(temp, temp_layer->removed,pow(2,clause_size[init_end]-var_connections.connections));

printf(" %i %i \n", var_connections.connections, clause_comp);
printf("var_con %i temp_con %i \n ", var_connections.connections, temp_connections.connections);

printf("var_con %i temp_con %i \n ", var_connections.inc, temp_connections.inc);
			mpz_set(removed, temp);	
			mpz_add(total, total, removed);
	gmp_printf("removed %Zd data %Zd removed %Zd %Zd \n", temp_layer->removed,temp_layer->data,removed, total);

//if(mpz_cmp_ui(removed,0)==0){return;}

			//mpz_set_ui(removed,1);
			break;
		case 2:
		//there's a connection and a preconnection
printf("post_Branch 2\n\n\n");

			///continue from a previous variable
var_tab_del(&head->clause);
			var_connections=var_tab_check(&head->clause);
var_tab_add(&head->clause);
			clause_comp=compare_clause(init_start,head->clause);

			t_comp=t_clause_compare(head->clause,init_start, init_end);

			m1=pow(2,clause_comp-t_comp);

			//total= temp_layer->data-temp_layer->removed*m1;
			//mpz_mul_ui(total, temp_layer->removed,m1);
			mpz_sub(total, temp_layer->data, temp_layer->removed);
		//	mpz_set(total, temp_layer->data);
			 // how many variables there are that haven't been accounted for
			//total=total/pow(2,var_connections.inc)
			clause_comp=unique_var(head->previous_layer->clause,init_start, init_end);

//sprintf("lclause comp %i %i %i \n\n",head->clause, clause_comp2, 0);
//exit(0);

//			clause_comp=subset_check2(init_start,init_end);

//gmp_printf("data %Zd removed %Zd  %Zd \n",temp_layer->data,temp_layer->removed, total);

//if(head->previous_layer->clause==4){clause_comp=3;}

			//total+=temp_layer->removed/pow(2,clause_size[head->clause]-clause_comp);
			int a=1;
//sum of prev
			mpz_divexact_ui(temp, total,pow(2,2)); //(a^b)^(b^c) ¬(a^b^c)  
			mpz_divexact_ui(total,total,pow(2,clause_size[4]-5)); //(b^c)
			
//mpz_divexact_ui(temp, temp_layer->removed,pow(2,clause_size[4]-4)); //(a^b)^(b^c) ¬(a^b^c)  
		//	mpz_add(total,total, temp);
printf("init start %i %i \n ", init_start, init_end);
//gmp_printf("data %Zd removed %Zd  %Zd clause_comp %i var_Con %i \n",total,temp, total,clause_comp2,var_connections.connections);
debug(head);
exit(0);
	
//mpz_set(total, temp_layer->data);
			mpz_set(removed, temp);	
			//gmp_printf("removed %Zd %Zd \n", temp, total);
///exit(0);
			break;
			case 3:
			var_connections=var_tab_check(&head->clause);
			clause_comp=compare_clause(init_start,head->clause);
			t_comp=t_clause_compare(head->clause,init_start, init_end);

			m1=pow(2,clause_comp-t_comp);
			// how many variables there are that haven't been accounted for
			//total= temp_layer->data+temp_layer->removed*m1;
			mpz_mul_ui(total, temp_layer->removed,m1);
			mpz_add(total, temp_layer->data, total);

			//total=total/pow(2,var_connections.inc); 
			mpz_divexact_ui(total,total,pow(2,var_connections.inc));
			

		break;
		case 4:
	printf("case4\n");
//exit(0);
			//	total= temp_layer->data+temp_layer->removed;
	//		mpz_add(total, temp_layer->data, temp_layer->removed);

mpz_set_ui(total,32);
debug(temp_layer);
			clause_comp=compare_clause(init_start,init_end);
			//total=total/pow(2,clause_size[init_end]);
//debug(temp_layer);
//gmp_printf("data %Zd removed %Zd  %Zd \n",temp_layer->data,temp_layer->removed, total);
////gmp_printf("dat..:%i %i %i  %Zd \n",init_start, init_end, head->clause,total);
//exit(0);
			mpz_divexact_ui(total,total,pow(2,clause_size[init_end]));

//gmp_printf("dat..:%i %i %i  %Zd \n",init_start, init_end, head->clause,total);

//mpz_set_ui(total,5);

			//removed=temp_layer->removed/pow(2,clause_size[init_end]-clause_comp);
			//mpz_divexact_ui(removed, temp_layer->removed,pow(2,clause_size[init_end]-(2)));
mpz_set_ui(removed,0);
printf("Exit: Undefined variable\n");
			//exit(0);
		break;
		case 5:
printf("case4\n");
	exit(0);

			//printf("head %i start %i end %i \n",head->clause, init_start, init_end); 
			//total= temp_layer->data-temp_layer->removed;
			mpz_sub(total, temp_layer->data, temp_layer->removed);

			clause_comp=compare_clause(head->clause,init_start);

			//total=total/pow(2,clause_size[head->clause]-1);
			mpz_divexact_ui(total,total,pow(2,clause_size[head->clause]-1));

			//removed=temp_layer->removed/pow(2,clause_size[head->clause]-clause_comp);
			mpz_divexact_ui(total,temp_layer->removed,pow(2,clause_size[head->clause]-1));

			printf("Exit: Undefined variable\n");
			exit(0);
		break;
		case 8:
printf("case4\n");
exit(0);
			//total= temp_layer->data-temp_layer->removed;
			mpz_sub(total, temp_layer->data, temp_layer->removed);

			clause_comp=compare_clause(head->clause,head->previous_layer->clause);
			//total=total/pow(2,clause_size[head->previous_layer->clause]-clause_comp); 
			mpz_divexact_ui(total,total,pow(2,clause_size[head->previous_layer->clause]-clause_comp));

		break;
		}


	}
	else{

			//If the start is negate
			printf("\n\nstart Negate \n\n");
			//	total=temp_layer->data-temp_layer->removed;
		//	



gmp_printf("dat1..:%i %i %Zd %Zd  %Zd %Zd\n",clause_comp,var_connections.connections, layer_temp->removed, temp_layer->data,total,removed);
if(temp_layer->previous!=NULL && layer_temp->previous!=NULL ){
temp_layer=temp_layer->previous;
layer_temp=layer_temp->previous;

}



			var_connections=var_tab_check(&head->clause);
		//	clause_comp=compare_clause( head->clause,head->previous_layer->clause);
			clause_comp=indie_comp(temp_layer->previous_layer->clause,head->clause);

printf( "temp %i init_Start %i %i \n", temp_layer->clause,init_start,init_end);
		//	var_tab_add(&init_start);
			var_tab_del(&init_end);
			var_connections=abs_tab_check(&init_end);
			var_tab_add(&init_end);
		//	var_tab_del(&init_start);
			//total=total/pow(2,clause_size[head->clause]-clause_comp);
				mpz_sub(total, temp_layer->data, temp_layer->removed);

			var_tab_del(&init_end);
			temp_connections=abs_tab_check(&init_start);
			var_tab_add(&init_end);



	printf(" compare_clause(init_end,init_start) %i %i\n",compare_clause(init_start,init_end),var_tab_check(&temp_layer->clause).connections);

				mpz_divexact_ui(total,total,pow(2,clause_size[init_end]-var_connections.connections));
				mpz_divexact_ui(removed,temp_layer->removed,pow(2,clause_size[init_end]-var_connections.connections));

if(mpz_cmp_ui(removed,0)==0){

printf("removed =0\n");	
}

if( var_tab_check(&temp_layer->clause).connections==-1){
//	mpz_divexact_ui(total,temp_layer->data,pow(2,clause_size[init_end]));
//	mpz_divexact_ui(removed,temp_layer->removed,pow(2,clause_size[init_end]-					abs_tab_check(&init_start).connections));

printf("init_end-init start=1 \ninit_end %i init_start %i \n", init_end, init_start);

}
else{
//return;
}
if(var_tab_check(&init_start).connections!=-1 || init_start!=1){
mpz_add(total,total, removed);
printf(" \n\n added \n\n ");	
}


gmp_printf("dat..:%i %i %Zd %Zd  %Zd %Zd\n",clause_comp,var_connections.connections, layer_temp->removed, temp_layer->data,total,removed);
//exit(0);


var_tab_del(&init_end);
		// if all variables have been accoutned for whether negated or connected for
		if(comp_tab_check(&init_end).connections== clause_size[init_end]){
mpz_sub(total, temp_layer->data, temp_layer->removed);
			mpz_divexact_ui(total,total,pow(2,clause_size[init_start]));
//			mpz_set_ui(removed,0);
//gmp_printf("removed %Zd %Zd \n", removed, temp_end->removed);

		}
var_tab_add(&init_end);
	}



	outer_layer= append_layer(temp_layer->clause,head,head,nul,0 );
	printf(" outer->layer %i %i\n", layer_temp->next->clause,head->clause);

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
printf(" return no dif \n");
		return;
	}


outer_loop=layer_temp->next;
inner_loop=layer_temp->next;

while(outer_loop->clause<head->clause){
primary_variable_connections=var_tab_check(&outer_loop->clause);
//clause_comp=compare_clause(outer_loop->clause,outer_layer->first_clause->clause);

	if(primary_variable_connections.connections==-1){
		
		if(outer_loop->next!=NULL && outer_loop->next->clause<init_end){

			//outer_layer=append_clause(outer_loop->clause, outer_layer,outer_layer->previous_layer,0,0,0);
			outer_loop=outer_loop->next;
		}
		else{

			break;
		}

	continue;
	}
	
	inner_loop=outer_layer->first_clause;
primary_variable_connections=var_tab_check(&init_start);


if(primary_variable_connections.connections==clause_size[inner_loop->clause]){
printf("%i %i %i  \n",primary_variable_connections.connections,inner_loop->clause, init_start);
}


//if this clause is already negated
if(var_tab_check(&outer_loop->clause).connections==-1){ printf("outer loop negate \n"); exit(0);break;}

//if(head->clause==8 && outer_loop->clause==7 ){continue;}
printf("-- outer:%i head: %i \n",outer_loop->clause,head->clause);

	outer_layer=append_clause(outer_loop->clause, outer_layer,outer_layer->previous_layer,0,0,0);
	//outer_layer->previous_layer=outer_loop->previous_layer	;
	clause_comp=compare_clause(outer_loop->clause,inner_loop->clause);
	var_connections=var_tab_check(&outer_loop->clause);

if(var_connections.connections==-1)exit(0);

	determiner=determiner_solve(chain, outer_layer,init_start ,outer_loop);


	//if there's a connections between init start, and current variable, do pre_branch
	int comp =compare_clause(outer_layer->clause,determiner.start);


	printf("ding start: %i end:%i cmd:%i  outer:%i head: %i \n",determiner.start,determiner.end, 			determiner.command,outer_loop->clause,head->clause);


	switch(determiner.command)
		{
			case 0:

				//sub_total=sub_total/pow(2,clause_size[outer_loop->clause]-var_connections.connections);
				mpz_divexact_ui(sub_total,sub_total,pow(2,clause_size[outer_loop->clause]-var_connections.connections));
				clause_comp=subset_check(head->clause,init_start);


				//use data instead of removed and take away from sub_total
				//total2=(sub_total)-outer_layer->previous->data/pow(2,clause_size[outer_loop->clause]-var_connections.connections);

				mpz_divexact_ui(total2,outer_layer->previous->data,pow(2,clause_size[outer_loop->clause]-var_connections.connections));
				mpz_sub(total2,sub_total,total2);

gmp_printf("attempt 1: %i %Zd \n",var_connections.connections, total2);
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

gmp_printf("this %Zd %Zd \n",outer_layer->next_layer->end->data,outer_layer->next_layer->end->removed);

gmp_printf("this %Zd %Zd \n",outer_layer->previous->data,outer_layer->previous->removed);
//					total2=sub_total-total2;
					mpz_sub(total2,sub_total,total2);

//mpz_set(total2,outer_layer->previous->removed);
				}
				else{
//
printf("there is no return from post_branch\n");
//gmp_printf("this %Zd %Zd \n",outer_layer->previous->data,outer_layer->previous->removed);

mpz_set(total2,outer_layer->previous->removed);
				}
			

				var_tab_del(&outer_layer->clause);

				//sub_total=sub_total*pow(2,clause_size[outer_loop->clause]-var_connections.connections);	
				mpz_mul_ui(sub_total,sub_total,pow(2,clause_size[outer_loop->clause]-var_connections.connections));
			break;
			case 2:
printf("pre_branch error check %i%i\n",determiner.start,determiner.end);
				//if there's a connect before the start, recall the previous, and continue from the start
				var_connections=var_tab_check(&outer_loop->clause);

				//sub_total=sub_total/pow(2,clause_size[outer_loop->clause]-var_connections.connections);
				m1=clause_size[outer_loop->clause]-var_connections.connections;
				if(m1<=0){m1=clause_size[init_end];}

				mpz_divexact_ui(sub_total,sub_total,pow(2,clause_size[outer_loop->clause]-var_connections.connections));

				var_tab_add(&outer_loop->clause);

				pre_branch_correct(determiner.end,determiner.start,outer_layer->clause,outer_layer->previous_layer,outer_layer,head);


				if(outer_layer->next_layer!=NULL){
				mpz_set(total2,outer_layer->next_layer->end->data);
mpz_sub(total2,sub_total,total2);
				}else{
		printf("error stop \n");		
mpz_set(total2, sub_total);
				}

////gmp_printf("this %Zd \n",total2);
//				total2=sub_total-total2;
				


				var_tab_del(&outer_loop->clause);

				//sub_total=sub_total*pow(2,clause_size[outer_loop->clause]-var_connections.connections);	
				mpz_mul_ui(sub_total,sub_total,pow(2,clause_size[outer_loop->clause]-var_connections.connections));
			break;
			case 3:
printf("pre_branch error check %i%i\n",determiner.start,determiner.end);
				//if there's a connect before the start, recall the previous, and continue from the start
				var_connections=var_tab_check(&outer_loop->clause);

				mpz_divexact_ui(sub_total,sub_total,pow(2,clause_size[outer_loop->clause]));


		printf("error stop \n");	
				mpz_set(total2, sub_total);
			//	mpz_sub(total2,sub_total,total2);


			//	var_tab_del(&outer_loop->clause);

				//sub_total=sub_total*pow(2,clause_size[outer_loop->clause]-var_connections.connections);	
				mpz_mul_ui(sub_total,sub_total,pow(2,clause_size[outer_loop->clause]));
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

// for each base check the previous one
void recursive_check(int layer, int begin_layer,int j,mpz_t previous_total, node* previous_layer){
bool break_loop=0;
	layer++;
	vReturn var_connections;	
	int match=0;
	node* clause_node=NULL;
	node* clause_temp =NULL;
	node* previous_result=previous_layer->first_clause;

	//tracks the current sum
	mpz_t current_total;
	mpz_init(current_total);

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

	// for each clause, check for a previous connection
	for (int i=begin_layer; i<j; i++){
printf("\n\n\n\n rec_check i %i\n", i);
		//check how many connections there are that exists from this clause
		var_connections=var_tab_check(&i);

		//if in the second layer, there is a connection, fix it
		if(var_connections.connections!=0 && layer>1){

			// create the linked list object
			clause_temp=append_layer(i,previous_layer,previous_layer,nul,0);



			//set the variables up to be continues
			mpz_set(sum, previous_layer->next_layer->end->data);
			mpz_set(sum2, previous_layer->next_layer->end->data);
		
			break;					
		}

		//how many in total exist as a subset
		//recursive_total=previous_total/pow(2,clause_size[i]-var_connections.connections);
		mpz_divexact_ui(recursive_total, previous_total, pow(2,clause_size[i]-var_connections.connections));


		//if the place is past the initial statepost_branch
		if(i>1){

			clause_connections=clause_size[j]-var_connections.connections;
if(var_connections.connections>clause_size[j]){
printf(" %i %i \n", var_connections.connections,clause_size[j]);;
}

			//scales to include the current clause
			mpz_divexact_ui(current_total,previous_result->removed, pow(2,clause_connections));

			//	continue to the next layer
			if(layer==1){
				layer++;	
				
				clause_temp=append_clause(i,clause_node,clause_node->previous_layer,nul,nul,clause_connections);
				//scales sub_total to include the branch
				mpz_divexact_ui(sub_total, sub_total,pow(2,clause_size[i]));

				//decides where the variables match
				determiner=determiner_solve(chain,clause_temp,0, NULL);
				//determiner.command=0;
				printf("\n\nstart: %i end:%i cmd:%i  outer:%i\n",determiner.start,determiner.end, determiner.command,clause_temp->clause);
				//var_tab_add(&i);
				//if(i==5){determiner.start=2;determiner.end=0;}
				//printf(" %i start %i command %i end %i \n",clause_temp->clause, determiner.start, determiner.command,determiner.end);

				//printf("i %i :det.command %i \n", i,determiner.command);
				switch(determiner.command)
				{
					case 0:

						//mpz_divexact_ui(result, clause_temp->previous->data,pow(2,clause_size[i]));

					break;
					case 1:
//printf("end %i start %i clause %i\n", determiner.end,determiner.start, clause_temp->clause);

					//	mpz_divexact_ui(sub_total,sub_total,pow(2,clause_size[clause_temp->clause]-2));
						
						var_tab_add(&clause_temp->clause);
						post_branch_correct(determiner.end,determiner.start,clause_temp->clause,0,clause_temp);
if(clause_temp->next_layer!=NULL){
						mpz_set(result, clause_temp->next_layer->end->data);
}else{
halt();
}
						var_tab_del(&clause_temp->clause);
						break_loop=1;
					//	mpz_mul_ui(sub_total,sub_total,pow(2,clause_size[clause_temp->clause]-2));
////gmp_printf("this %Zd \n",clause_temp->next_layer->end->data);

//debug(clause_temp);
					break;
				
					case 2:
exit(0);
					break;
				}
				if(break_loop==0){

					mpz_divexact_ui(result, clause_temp->previous->data,pow(2,clause_size[i]));


				}
				else{
					break_loop=0;
				}

				mpz_mul_ui(sub_total, sub_total,pow(2,clause_size[i]));
				//var_tab_del(&i);
				layer--;
			}
//var_tab_del(&j);
			//add all the possible, subtract by subset possiblies already taken
			//sum+=current_total-result;
			mpz_sub(current_total,current_total,result);
			mpz_add(sum,sum,current_total);
			

			//sum2+=recursive_total-result;
			mpz_sub(union_sub,recursive_total,result);
			mpz_add(sum2, sum2, union_sub);	
			int k=0;
			
			//update the clause
			if(layer==1){
				//clause_temp->data=sum2;
				mpz_set(clause_temp->data,sum2);
				//clause_temp->removed=recursive_total-result;
				mpz_sub(clause_temp->removed,recursive_total,result);

			}

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
				
//				printf("error_search %i \n",clause_node->clause);
//				//gmp_printf("this %Zd \n",clause_node->data);

			}			
		}
	
		if(previous_result->next!=NULL){
			previous_result=previous_result->next;
		}
	}

layer--;

mpz_clear(current_total);
mpz_clear(result);
mpz_clear(sum);
mpz_clear(sum2);
mpz_clear(union_sub);
mpz_clear(recursive_total);
}



int main(int argc, char *argv[]){


//sets the base value of the problem
mpz_init (base);
mpz_set_ui(base,2);

//mpz null ptr
mpz_init(nul);
mpz_set_ui(nul,0);

//opens file for reading
FILE *fp;
if (argc > 1) {
	fp = fopen(argv[1], "r");
} else {
	printf(" no file\n");
}

if(!fp){
printf("Cannot find %s \n", argv[1]);
exit(0);
}
unsigned int nr_variables;
unsigned int nr_clauses;

// reads the amount of variables and clauses
fscanf(fp, "p cnf %i	 %u\n", &nr_variables, &nr_clauses);
	//stores temporary literals
int tmp[300]={0};

init_pos();

//stores all the ones
//stores variable number in clause
int b=0;
int cl=1;
//scans file
int literal;
int ones[4000]={0};
while (1){
	if (fscanf(fp, "%d", &literal) != 1){
		printf("error: expected literal\n");
		exit(EXIT_FAILURE);
	}
	if (literal!=0){
		//stores which literal
		tmp[b]= literal;
		b++;			
continue;
	}

	//at the end of each line
	if (literal == 0){
//printf("cl %i \n",cl);
		variable_connections[0][0]++;
		for (int unload=0;unload<b;unload++){

			//stores where each variable is store			
			variable_connections[variable_connections[0][0]][0]=b;

			//convert variable to progessive number
			if(variable[abs(tmp[unload])][1]==0){
				//////////////printf("variable status %i %i \n",abs(tmp[unload]), variable[abs(tmp[unload])][0]);
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
			
			if(variable_position[variable[abs(tmp[unload])][0]]->clause==0){
			variable_position[variable[abs(tmp[unload])][0]]->clause=variable_connections[0][0];
			variable_position[variable[abs(tmp[unload])][0]]->end=variable_position[variable[abs(tmp[unload])][0]];
//printf("variable_position[%i][0]] %i \n",variable[abs(tmp[unload])][0],variable_connections[0][0] );

			}
			else{	
				append_variable(variable_connections[0][0],variable_position[variable[abs(tmp[unload])][0]]);
				//printf("%i \n",variable_position[variable[abs(tmp[unload])][0]]->clause);
//exit(0);

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
		
	}

cl++;
//if(cl==3){exit(0);}

	if(cl>nr_clauses){
		////////////printf("variables %i\n",variable[0][0]);
		////////////printf(" this is the amount of clauses %d \n ", cl-1);
//cl++;
		break;
	}
}

//**********************
//start of the algorithm
//**********************

//the total number of all possibilities
	all = pow(2,variable[0][0]);


	// all possible
	mpz_t all_possible;
	mpz_init (all_possible);
	mpz_pow_ui (all_possible,base, variable[0][0]);

	//	Sub_total
	mpz_init(sub_total);
	

	//last of the variables
	start=variable_connections[0][0];
	//start of the linked list
	node* clause_node=NULL;
	
	mpz_set(sub_total,all_possible);


	mpz_divexact_ui(sub_total,sub_total,pow(2,variable_connections[start][0]));

	clause_node=append_clause(start, clause_node,NULL,sub_total,nul,variable_connections[start][0]);
		
	//recursion begins
	recursive_check(0,1,start,sub_total,clause_node);

	if(!clause_node->next_layer){
printf("there'se no clauses\n");
exit(0);
}	
	//mpz_mul_ui(sub_total,sub_total,pow(2,variable_connections[start][0]));

	mpz_sub(sub_total,sub_total,clause_node->next_layer->end->data);

	
printf("clause_node->clause %i \n", clause_node->next_layer->end->clause);

	
node* result=clause_node->next_layer;
////gmp_printf("%i clause node  %Zd  %Zd \n", result->clause,result->data,result->removed);
	while(result!=NULL){
		//printf("%i clause node %i + %i \n", result->clause,result->data,result->removed);
		gmp_printf("%i clause node  %Zd  %Zd \n", result->clause,result->data,result->removed);
	
		result=result->next;
	}

	//TEST PARAMETERS
//	printf("sub_total %lli %lli \n",all-clause_node->next_layer->end->data,all);

gmp_printf("total %Zd  possible %Zd \n",sub_total,clause_node->next_layer->end->data);
//debug(clause_node);
dispose(clause_node->next_layer);

return(0);
}


