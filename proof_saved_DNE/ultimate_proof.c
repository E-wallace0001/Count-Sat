 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "infini_tree_DNE.h"

//linked list variables
node* head=NULL;

long long all=0;
int layer_save[500]={0};

int b=0;

int start=0;

int dels[1000]={0};

int amount=0;
int loop_count=0;
int variable[253808][10]={0};
int variable_connections[867000][50]={0};
int con[867000]={0};
int root_con[867000]={0};
//int pos_t[10000000]={0};
double stored_v[1600]={0};
long set_variable[253808]={0};

int clause_size[867000]={0};
int con_con[1000][1000]={0};

int comp_match[1000]={0};

//needs to be the size of all the variables *2
int var_tab[160000]={0};

void debug(node *head){
char input;
while (1){

scanf("%c", &input);
if(getchar()!=0){
printf("boop");
}

	switch(input){
		case 'w':
			if(head->next_layer==NULL){ printf("no upper lever\n");}
			else{ head=head->next_layer; printf("next_layer \n clause %i \n Data %i \n Removed %i \n Size %i\n", head->clause, head->data, head->removed,head->size);}
		break;	
		case 'a':
			if(head->previous==NULL){ printf("no previous\n");}
			else{ head=head->previous; printf("next_layer \n clause %i \n Data %i \n Removed %i \n Size %i\n", head->clause, head->data, head->removed,head->size);}
		break;
		case 's':
			if(head->previous_layer==NULL){ printf("no previous_layer\n");}
			else{ head=head->previous_layer;printf("next_layer \n clause %i \n Data %i \n Removed %i \n Size %i\n", head->clause, head->data, head->removed,head->size);}
		break;
		case 'd':
			if(head->next==NULL){ printf("no next\n");}
			else{ head=head->next; printf("next_layer \n clause %i \n Data %i \n Removed %i \n Size %i\n", head->clause, head->data, head->removed,head->size);}
		break;
		}
}
printf(" output: %c \n", input);
}

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
////printf("clause d %i i %i\n",*clause_d,*clause_var);


		if(*(variables+abs(*clause_var))==*clause_var){
			temp_match[0]++;
			temp_match[temp_match[0]]=abs(*clause_var);
			alpha.connections++;
			if(var_tab[abs(*clause_var)]==1){
				alpha.inc++;
			}
	
			if(comp_match[abs(*clause_var)]>0){
				alpha.mul=comp_match[abs(*clause_var)];
			}
		}
		else if(*(variables+abs(*clause_var))!=0){
			//////printf("no go!!!!!!!!!!!!1");
			alpha.connections=-1;
			return(alpha);
			break;
		}
		////////printf("i %i  - %li count %i \n",*clause_var,*(variables+abs(*clause_var)),count);

	}
////printf("comp_match 1 %i \n",comp_match[abs(*clause_var)]);
	
	for(int i=1;i<=temp_match[0];i++){
		comp_match[temp_match[temp_match[0]]]++;
	}
////printf("comp_match 2 %i \n",comp_match[abs(*clause_var)]);
	


	return(alpha);
}

void del_match(int *clause_d){
	long *variables;
	variables=set_variable;
	int *clause_var;
	int count=0;

	clause_var=&variable_connections[*clause_d][0];

	//this is the variables in the clause

		for(int x=clause_size[*clause_d];x!=0;x--)
		{
			clause_var++;
			////printf("comp_match 1 %i \n",comp_match[abs(*clause_var)]);

			if(comp_match[abs(*clause_var)]>0)
			{
				comp_match[abs(*clause_var)]--;
			}
			
		//	//printf("comp_match  %i \n",comp_match[abs(*clause_var)]);
		
		}
}




long long prev_result[300][3000]={0};
//use a nested loop to solve this 
long int result3=0;


//sub_tab[trunk][branch][primary/secondary]
//int sub_tab[100][100][40][3]={0};

node* sub_sub[500]={0};
int loop_con[500]={0};
//int (*var_check)(int)=&var_tab_check;

int prev_m[1000]={0};
int layer=0;
int prev_r[500][500]={0};
int stat=0;
//need to be edited
int con_begin=0;

//corlayertion table[loop][modifier]
//modifier=0 start
int sub_edit[500][500]={0};
int loop2=0;
//this modifies a branch of the set
int prev_add[500][500]={0};
int prev_branch[20000][20000]={0};
int var_cons[500][500]={0};
int inst_tab[500][500]={0};

int pre_con=0;
int *root;
int *begin;
int prev_modifier[500][500]={0};



//double c_tab[300][300][100]={0};


int clause_tab[100]={0};

int init_layer=0;


// for each base check the previous one
int recursive_check( int begin_layer,int j,int previous_total, node* previous_layer){
	int sum=0;
	int total_connections=0;
	vReturn variable_connections;	

	node* clause_node=NULL;
	node* clause_temp =NULL;
	for (int i=begin_layer; i<j; i++){

		
		//printf(" I: %i\n",i);
		//check how many variables match with the table	
		variable_connections=var_tab_check(&i);

		//if there are conflicting variables, continue to the next clause
		if(variable_connections.connections==-1){
			continue;
		}
		//printf ("connections %i \n", variable_connections.connections );
		//printf ("size %i con %i \n", clause_size[i],clause_size[i]-variable_connections.connections);
		//how many exist in this clause
		int recursive_total=previous_total/pow(2,clause_size[i]-variable_connections.connections);
		
	
		if(i>1){
			//add the clause to the table	
			var_tab_add(&i);
			
			int result=recursive_check(1,i,recursive_total, clause_node);

			//add the clause to the table
			var_tab_del(&i);

			sum+=recursive_total-result;

			clause_temp=append_clause(i,clause_node,previous_layer,sum,recursive_total-result);

			clause_temp->size=variable_connections.connections;

			clause_node=clause_temp;
		}
		else{
			sum=recursive_total;
			clause_node=append_layer(i,NULL,previous_layer,sum);	
		}
	}
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

vReturn polarity;
int *one;
one=ones;
 start=variable_connections[0][0];
int variables=0;

node* clause_node=NULL;
printf("start %i\n", start);
//for the root cluase
for(int j=start;j<=start;j++)
{
	
	//printf("start %i \n",j);

	if(polarity.connections==-1){
		continue;
	}				

	
	int sub_total= all/pow(2,variable_connections[start][0]);

	clause_node=append_clause(j, clause_node,NULL,sub_total,sub_total);

	//add the variables of the clause to the table
	var_tab_add(&j);
	
	//recursion begins
	int total_remove=recursive_check(1,j,sub_total,clause_node);

	//delete the variables from the table;
	var_tab_del(&j);

	sub_total-=total_remove;

	node* result=clause_node->next_layer;
	while(result!=NULL){
	printf("%i result %i %i \n",result->clause,result->data,result->removed);

	result=result->next;
	}
	printf("sub_total %i \n", sub_total);
	
printf("variable_connections[0][0] %i\n",variable[0][0]);
}

debug(clause_node->next_layer);	



}


