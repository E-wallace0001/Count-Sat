/******
Todo:

finish sub clause matches

sub_tab can lose the root as this won't change

make sure it's capable for other clause sizes other than 4


todo sec_sub can be memory optimized


*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "list.h"

int start=0;

int dels[100]={0};

int amount=0;
int loop_count=0;
int variable[253808][10]={0};
int clause[867000][50]={0};
int con[867000]={0};
int root_con[867000]={0};
//int pos_t[10000000]={0};
float stored_v[1600]={0};
long set_variable[253808]={0};

int clause_size[867000]={0};
int con_con[1000][1000]={0};

int comp_match[1000]={0};

//needs to be the size of all the variables *2
int var_tab[160000]={0};


//this adds a clause to the ones being checked
static inline int var_tab_add(int *clause_a)
{
	//this is the variables already been added to the table
	long *variables;
	variables=set_variable;
	int *clause_var;
	int count=0;

	clause_var=&clause[*clause_a][0];

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
			//	////printf("fail\n");
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

	clause_var=&clause[*clause_d][0];

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
				//////printf("removed %i \n",var_tab[abs(*clause_var)]);
				set_variable[abs(*clause_var)]=0;
				
			}
	
		
		}
	
}

struct v_return{
int com;
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

	clause_var=&clause[*clause_d][0];

	//amount of variables in the clause

	//this is the variables in the clause

	for(int i=clause_size[*clause_d];i!=0;i--)
	{
		clause_var++;
//printf("clause d %i i %i\n",*clause_d,*clause_var);


		if(*(variables+abs(*clause_var))==*clause_var){
			temp_match[0]++;
			temp_match[temp_match[0]]=abs(*clause_var);
			alpha.com++;
			if(var_tab[abs(*clause_var)]==1){
				alpha.inc++;
			}
	
			if(comp_match[abs(*clause_var)]>0){
				alpha.mul=comp_match[abs(*clause_var)];
			}
		}
		else if(*(variables+abs(*clause_var))!=0){
			////printf("no go!!!!!!!!!!!!1");
			alpha.com=-1;
			return(alpha);
			break;
		}
		//////printf("i %i  - %li count %i \n",*clause_var,*(variables+abs(*clause_var)),count);

	}
//printf("comp_match 1 %i \n",comp_match[abs(*clause_var)]);
	
	for(int i=1;i<=temp_match[0];i++){
		comp_match[temp_match[temp_match[0]]]++;
	}
//printf("comp_match 2 %i \n",comp_match[abs(*clause_var)]);
	


	return(alpha);
}

void del_match(int *clause_d){
	long *variables;
	variables=set_variable;
	int *clause_var;
	int count=0;

	clause_var=&clause[*clause_d][0];

	//this is the variables in the clause

		for(int x=clause_size[*clause_d];x!=0;x--)
		{
			clause_var++;
			//printf("comp_match 1 %i \n",comp_match[abs(*clause_var)]);

			if(comp_match[abs(*clause_var)]>0)
			{
				comp_match[abs(*clause_var)]--;
			}
			
		//	printf("comp_match  %i \n",comp_match[abs(*clause_var)]);
		
		}
}


long long prev_result[100][300000]={0};
//use a nested loop to solve this 
long int result3=0;


//sub_tab[trunk][branch][primary/secondary]
//int sub_tab[100][100][40][3]={0};

int sub_sub[100]={0};
int loop_con[100]={0};
//int (*var_check)(int)=&var_tab_check;

int prev_m[1000]={0};
int rec=0;
int prev_r[100][100]={0};
int stat=0;
//need to be edited
int con_begin=0;

//correction table[loop][modifier]
//modifier=0 start
int sub_edit[100][100]={0};
int loop2=0;
//this modifies a branch of the set
int prev_add[100][100]={0};
int prev_branch[20000][20000]={0};
int var_cons[100][100]={0};
int con_edit[100][100][100]={0};
int inst_tab[100][100]={0};

int pre_con=0;

int var_correct(int *begin,int *root,int *prev_con){
	int *in_con;

	in_con=&con_con[*root][0];

	float test;
	////printf("we're go %i\n",*end);
	int *pre_result;
	pre_result=&sub_edit[loop2][0];
	
	int modifier=0;	
	int *prev_b;
	prev_b=&prev_branch[*root][1];
	int *clauses;
	clauses=&con[*begin-1];
	int temp=0;
	vReturn rem;
	int n;
	int accumulator=0;
	int size_diff;
	int *s;
	int inner_beg=0;
	int ding=1;
	vReturn beg_var={0};
	vReturn t={0};
	int subtrack[100]={0};
	int saved_clause;

	for(int sub_mod=*begin	;sub_mod<*root;sub_mod++){
	clauses++;	
	printf("\n*	root %i submod %i- %i  prev: %i\n",*root,*clauses,sub_mod,var_cons[*root][*clauses]);
	//int c=2;
	rem=var_tab_check(clauses);
	printf("		rem %i %i %i\n", rem.com, rem.inc,rem.mul);

	
	beg_var=var_tab_check(begin);


	printf("	prev_R: %lli begvar com: %i inc: %i mul: %i \n",prev_result[0][*root],beg_var.com,beg_var.inc,beg_var.mul);

	n=prev_branch[*root][*clauses];

	del_match(clauses);
	del_match(begin);

if(*prev_con!=0){
if(beg_var.mul==var_cons[*root][*clauses])
{

		printf("******		clause_size %i prev_b %i\n",clause_size[*clauses],var_cons[*root][*clauses]);

		printf("con %i \n",pre_con);
		//return(0);
}}

	//


	switch(inst_tab[rec-1][*clauses]){
		case 0: //general scale

n=sub_edit[rec][*clauses];
if(*root==5){
//n=accumulator<<4;
}

	if(*root==5){
if(*clauses>3)
{
	int ke=3;
	rec++;
	//n=sub_edit[rec][*clauses];
		
	//inst_tab[1][3]=0;
	n=var_correct(&ke,clauses,&clause_size[*root]);
	//modifier+=n;
printf("*//*rec: %i this is go: %i mod : %i root %i, n: %i \n\nprev: %i \n",rec,ke,modifier,*root,n,sub_edit[2][5]);
	rec--;
}
	}

if(inner_beg==1){
	rec++;
	n=var_correct(&saved_clause,clauses,&clause_size[*clauses]);
	printf("*//*this is go: %i mod : %i root %i, n: %i \n",saved_clause,modifier,*root,n);
	rec--;
	}
	printf("	rec: %i n: %i\n",rec,n);

			modifier=n>>(clause_size[*root]-var_cons[*root][*clauses]);					

			
			printf("		gen scale rem %i prev_r %i-%i \n", clause_size[*root],n,subtrack[*clauses]);
			
			break;
		case 1:	//scale begin
			n=prev_branch[*root][*clauses];	
			accumulator=(n<<(loop_con[rec]))-n;

if(*root==5){
if(*clauses==3){
loop_con[rec+1]=loop_con[rec]-var_cons[*root][*clauses];
sub_edit[rec+1][*clauses]=-accumulator<<clause_size[*root];
inst_tab[rec+1][*clauses]=0;
}
}
			break;
		case 2: //deletion
			if(var_cons[*root][*clauses]!=-1){
			n=prev_branch[*root][sub_mod];
			modifier=n;
			printf("		del %i\n", modifier);
			}
			break;
		case 3:
			n=sub_edit[rec][*clauses];
			modifier=n>>(clause_size[*root]);
			break;
			
		}



	//adding it all to the table		
	//con_edit[rec][*root][sub_mod]=modifier;

	if(loop_con[rec]<=(var_cons[*root][*clauses]) ){
		//secondary to begin next round
		printf("****			 ina-ding %i \n", modifier);
		inner_beg=1;
		loop_con[rec+1]=var_cons[*root][*clauses];
		sub_edit[rec+1][*clauses]=-modifier<<clause_size[*root];
		saved_clause=*clauses;
		inst_tab[rec][*clauses]=3;
		
		}

	con_edit[rec][*root][sub_mod]=modifier;
	accumulator-=modifier;
subtrack[*clauses]=accumulator;


printf("		accumulator %i \n",subtrack[*clauses]);

		//if(*clauses==pre_con){continue;}
	}

printf("accum %i \n", pre_con);
		if(pre_con!=0){
			if(*root>=pre_con)
			{
			printf("**	sub_edit[rec][*root] %i\n", subtrack[3]);
			
			accumulator+=subtrack[*begin+1];
			sub_edit[rec][*root]=accumulator;
			}
		}

		

sub_edit[rec][*root]=accumulator;
inst_tab[rec-1][*root]=0;

return(accumulator);	
}

/* dump
//if a connection was made: go to recursion
	if(con_begin!=0){
		rec++;
		int kre=sub_con_fix(&con_begin,&sub_mod,&var_cons[*root][sub_mod]);	
		modifier=kre;
		//printf("inner change is go\n ");
		rec--;
	}

	//if a connection is made
	if(var_cons[*root][sub_mod]>*prev_con ){
		loop_con[rec]=var_cons[*root][sub_mod];
		con_begin=sub_mod;
	}
*/


float  in_solve(int *root,int *begin)
{

//////////printf("n= %li\n",n);
int result=0;
long long add=0;
long long d=0;
float result4=0;
float add2=0;
float result5=0;


//instruction table

int *in_clause;
in_clause= &con[(*begin)-1];
long long *prev_k;
prev_k=&prev_result[0][*begin];
int *clause_s;
clause_s=&clause_size[10];
int *rc;
rc=root_con;
int *clause_v;
clause_v=&clause[0][0];
int c1=0;
int pk=0;
vReturn sec_match={0,0};
int inner_check=0;
int add_t;
int pk2;
int first=0;

vReturn check;

	for (int i=*begin;i<*root;i++){
	//printf("i %i \n",i);

		pk=prev_result[rec][i];
		in_clause++;

		sec_match=var_tab_check(in_clause);
		del_match(in_clause);

		printf("incheck %i\n", sec_match.com);
		var_tab_add(&start);

		var_cons[*root][*in_clause]=sec_match.com;
		add_t=sec_match.com;
printf("%i %i-%i sec_match com: %i inc %i \n",*root,i,*in_clause, sec_match.com, sec_match.inc);

		//deletion later
		if(sec_match.com==-1){
		if(inner_check==1){
			inst_tab[rec][i]=2;
			printf("ic1	we have a no go! \n");
			continue;
		}
		inst_tab[rec][i]=2;
		inner_check=1;
		dels[*in_clause]=1;
		dels[0]=1;
		sub_sub[rec+1]=i;
		//loop_con[rec]=-1;
		
		printf("**	we have a no go! \n");
		continue;
		}

		//continue if allowed
		if (add_t!=-1){
			
			d=prev_m[*root]>>(clause_size[i]-sec_match.inc);
			add+=d;
			prev_add[*root][*in_clause]=add;
			//begin recursion
			if(inner_check==1){
				if(rec==0)
				{
				//increment recursion level
				rec++;
				//recusrive function
				//sub_sub[1]=3;
				printf("sub sub %i \n", sub_sub[1]);
				pk2=var_correct(&sub_sub[1],&i,&sec_match.com);
				//the corrected previous answer
				pk+=pk2;
				//decrement recursion level
				rec--;	
				//printf("pk: %i pk2: %i\n",pk, pk2);
				}
			}

	del_match(in_clause);
		
		result5=(pk)/pow(2,(clause_size[*root]-(root_con[*root]+sec_match.com)));


		printf("result5 %f\n", result5);
			result4+=result5;

			//if there are connections
			if(root_con[*in_clause]<(sec_match.com)){
	
				printf("ding\n");
				//if recursion hasn't begun
					loop_con[1]=sec_match.com;
					printf("loop_con %i\n",loop_con[1]);
					//this points to the start point
					if(sub_sub[1]==0)
					{
					sub_sub[rec+1]=*in_clause;
					}
					prev_branch[0][1]=d-result5;
				
				//tell the recursive function what to do with this clause
				inst_tab[rec][i]=1;
				//engage recursion in the next loop
				inner_check=1;
			}

			//save the state for memoization
			prev_branch[*root][*in_clause]=d-result5;
			prev_r[*root][*in_clause]=add-result4;
			//printf("in root: %i i: %i pk:%f\n",*root,i, d-result5);	
		}
	//	printf("root: %i i: %i pk:%f\n",*root,i, add-result4);
	}
sub_sub[1]=0;

prev_result[rec][*root]=add-result4;

return prev_result[rec][*root];
}

int main(int argc, char *argv[]){

// variable for current clause 
int current_clause=1;

//opens file for reading
FILE *fp;
if (argc > 1) {
	fp = fopen(argv[1], "r");
} else {
	////////printf("fail \n");
}

unsigned int nr_variables=1;
unsigned int nr_clauses=1;

// reads the amount of variables and clauses
fscanf(fp, "p cnf %i	 %u\n", &nr_variables, &nr_clauses);
////////printf("number of variables %u \n Number of clauses %u \n", nr_variables, nr_clauses);
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
		//fprintf(stderr, "error: expected literal\n");
		exit(EXIT_FAILURE);
	}
	if (literal!=0){
		//stores which literal
		tmp[b]= literal;
		b++;			
	}

	//at the end of each line
	if (literal == 0){
		clause[0][0]++;
		for (int unload=0;unload<b;unload++){

			//stores where each variable is store			
			clause[clause[0][0]][0]=b;

			//convert variable to progessive number
			if(variable[abs(tmp[unload])][1]==0){
				//////////printf("variable status %i %i \n",abs(tmp[unload]), variable[abs(tmp[unload])][0]);
				variable[0][0]++;
				variable[abs(tmp[unload])][0]=variable[0][0];
				variable[abs(tmp[unload])][1]=1;
			}


			if(tmp[unload]>0){
				clause[clause[0][0]][unload+1]=variable[abs(tmp[unload])][0];
			}

			if(tmp[unload]<0){
				clause[clause[0][0]][unload+1]=-variable[abs(tmp[unload])][0];
			}

		}
		if(b==1){
			ones[0]++;
			ones[ones[0]]=clause[0][0];
		}
		clause_size[clause[0][0]]=b;
		//resets clause size
		b=0;
		//increments clause count
		cl++;
	}


	//if read all clauses
	if(cl==nr_clauses+1){
		////////printf("variables %i\n",variable[0][0]);
		////////printf(" this is the amount of clauses %d \n ", cl-1);
		break;
	}
}

//**********************
//start of the algorithm
//**********************

//the total number of all possibilities
long long all = pow(2,variable[0][0]);

long long add=0;
long long add2=0;
long int exist=0;

long long remain=0;
long int left=0;

vReturn polarity;
int *one;
one=ones;
 start=clause[0][0];

node* head=NULL;
node* list=NULL;
node* temp = NULL;

//for the root cluase
for(int j=start;j<=start;j++)
{
	long long result=0;;
	long int loop2=0;
	long int test=0;
	int sec_clause=0;
	int *root_shift;
	root_shift=&root_con[0];
	//sec_clause=con;
	float loop=0;
	int first=1;
	int m;
	con[0]=0;
	
	exist=all>>clause[start][0];
	//printf("all possible, start: %i exist %li \n", start, exist);
	var_tab_add(&j);
	
	//trunk clause check
	for(int l=1;l<j;l++){
		polarity=var_tab_check(&l);
		del_match(&l);
		
		if(polarity.com==-1){
			continue;
		}				
		con[0]++;
		con[con[0]]=l;
		root_con[l]=polarity.com;

		if(con[0]==1){
			list = prepend(list,l);
			head=list;
			continue;	
		}
		temp=append(list,l);		
		list=temp->next;
	}

	//needs to check if it is doable across the board
list=head;
	//for the size of connected
	for(int k =1;k<=(con[0]);k++){
		printf("list data %i\n",list->data);
		sec_clause=list->data;
		
		m=(clause_size[sec_clause]-root_con[sec_clause]);
		printf("			 %i root con %i \n",sec_clause,root_con[sec_clause]);
		var_tab_add(&sec_clause);
		result=exist>>(m);
		prev_m[k]=result;
		test+=result;

		//continues only if the first one is done
		if(k-1!=0){
			loop=in_solve(&sec_clause,&first);
			printf("loop %f\n",loop);
			loop2=loop2+loop;
			stored_v[sec_clause]=result-loop;

		}
		rec=0;	
			del_match(&sec_clause);
		
		
		var_tab_del(&sec_clause);
		list=list->next;		
	}
	loop=0;
	add=test-loop2;
	
	var_tab_del(&j);	
	left=exist-add;

	printf("%i possibilities %li\n",j,left);
	//pos_t[0]++;
	//pos_t[j]=left;
	remain=remain+left;


}

printf("head %i\n", count(head));

dispose(head);



for(int t=1;t<=9;t++)
{
//////printf("n_var %i = %i\n",t,prev_add[5][4][t]);
}

for(int t=1;t<=12	;t++)
{
printf("    stored_v[%i] = %f \n",t,stored_v[t]);
}

for(int t=1;t<=6	;t++)
{
////printf("    prev %i= %lli \n",t,prev_result[t]);
}

////////printf("variables %i\n", variable[0][0]);
all=all-remain;
//printf("possibilities %lli remain %lli loop_count %i \n",all,remain,loop_count); 

return(0);
}


