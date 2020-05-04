
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#include <string.h>

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

int t_count=0;
variable_pos* set;
variable_pos* tmp;
	variable_pos* set_copy=NULL;
bool clause_visited[9000]={0};
//bool inline_visited[8000]={0};
int  new_old_clause[8000]={0};
bool counted_set[8000]={0};

void raw(){

	for(int clause=1;clause<=f_clause_count;clause++){

	set=copy_clause(clause, set);
	}

}

bool CheckClauseIsNeg(int clause ){
int variable;
	for(int var=f_clause_size[clause];var!=0;var--){
		variable=f_variable_connections[clause][var];
		for(int i=1;i<=ones[0];i++){
			if( abs(variable)==abs(f_variable_connections[ones[i]][1]) && (variable)!=f_variable_connections[ones[i]][1] ){
				return(1);
			}
		}
	}
return(0);

}
	

void Export(variable_pos* iter){

if(iter==NULL){printf("null export \n");exit(0);}
iter=iter->first;
FILE* fp2;
fp2=fopen("out2.cnf","w");

while(1){

	for(int variable=clause_size[(iter)->clause]; variable!=0;variable--){
		fprintf(fp2,"%i ", variable_connections[(iter)->clause][variable]);
	}

	fprintf(fp2,"0\n");

	if((iter)->next!=NULL){
		(iter)=(iter)->next;
	}
	else{break;}
}
fclose (fp2);
}


void SetOnes(){
int k=0;
	for(int variable=1;variable<=ones[0];variable++){
		if(variable_position[abs(f_variable_connections[ones[variable]][1])]->clause==0 ){
			continue;
		}

			k=f_variable_connections[ones[variable]][1];
			set_variable[abs(k)]=k;
			counted_set[abs(k)]=1;
			set_var[abs(k)]=1;
			var_tab[abs(k)]++;
			counted_for++;
			
		

	}

}


void ReSetOnes(){
int k=0;
	for(int variable=1;variable<=ones[0];variable++){
		if(variable_position[abs(f_variable_connections[ones[variable]][1])]->clause==0 ){
			continue;
		}

			k=f_variable_connections[ones[variable]][1];
			set_variable[abs(k)]=0;
			counted_set[abs(k)]=0;
			set_var[abs(k)]=0;
			var_tab[abs(k)]--;
			counted_for--;
			
		
		

	}

}

void init_graph(int ones[]){


	variable_pos* temp_clause;	
	int count=0;

	layer* layer=set_layer(-1,2);
printf("ones %i \n ",ones[0]);

		int k=0;

//	set=make_clause(0);

	for(int i=1;i<=(ones[0]);i++){
//init_pos(variable_position);

		printf("start %i %i %i \n", layer->num,variable_count,clause_count); 
		//clause_count++;

		//copy the original
		set=copy_clause(ones[i], set);

//		printf(" cl_Sz %i %i %i\n", ones[i],ones[0],i);
		clause_visited[ones[i]]=1;
		//mark which orriginal clauses have been visited
//		inline_visited[ones[i]]=1;
//null_add(set);



//		printf(" i %i\n", i);

		for(int variable=f_clause_size[ones[i]];variable!=0;variable--){

			// collect all clauses connected to this variable
			temp_clause=f_variable_position[abs(f_variable_connections[ones[i]][variable])];
//			printf("tmp cl %i clause_count %i \n",temp_clause->clause,clause_count);

			//coppy all the clauses

			while(temp_clause!=NULL){

				//set it to vistied
				//clause_visited[temp_clause->clause]=1;
				if(clause_visited[temp_clause->clause]==0 ){
					set=copy_clause(temp_clause->clause, set);
					clause_visited[temp_clause->clause]=1;
				}
		
				temp_clause=temp_clause->next;

			}

		}

//printf("**** ones %i\n",ones[i]);
//Export(NULL);

//		inline_visited[abs(ones[i])]=1;
//		clause_visited[temp_clause->clause]=1;
		//printf(" %i \n",);

		bfs_graph(layer,&set);

//		printf(" i %i \n", i);
//		printf(" split-------------------------\n");
//printf(" count %i \n",count_var_pos(set->first));



set=set->first->end;
		//printf("this set %i \n", set->clause);
		while(set!=NULL){
		//	printf("this is it \n");
if(clause_visited[new_old_clause[(set)->clause]]==0){
	printf(" unset vist\n");
	exit(0);
}
			//clause_visited[new_old_clause[(set)->clause]]=0;
			copy_removed(set->clause,&set);

			//(set)=(set)->previous;

		}
for(int i=1;i<9000;i++){
clause_visited[i]=0;
}

	//	printf(" clause_count %i %i \n", clause_count,variable_count);
	}
printf(" t_count %i \n", t_count);
}
void bfs_graph( layer* layer,variable_pos** set){
if((*set)==NULL){printf("nullset\n");exit(0);}
	variable_pos* first=(*set)->first;
int end_clause=first->end->clause;

	variable_pos* iter;
	variable_pos* temp_var;
	variable_pos* set_saved=(*set);
	variable_pos* temp_clause;
	variable_pos* var_clause;
	int count=0;
	bool skipped=0;

	while(first->clause!=end_clause){
		for(int variable=clause_size[first->clause];variable!=0;variable--)
		{
			temp_clause=f_variable_position[abs(f_variable_connections[new_old_clause[first->clause]][variable])];
			if(temp_clause==NULL){Export(*set);printf(" null temp_clause \n");exit(0);}

			while(temp_clause!=NULL){
				//printf("temp_clause->clause %i \n", temp_clause->clause);
				if(clause_visited[temp_clause->clause]==0 ){
					(*set)=copy_clause(temp_clause->clause,(*set));
					clause_visited[temp_clause->clause]=1;
				}
				
			temp_clause=temp_clause->next;
			}

//dispose(clause_node);
//null_add(set);

//solve();
//dispose(clause_node);
//printf("solved %i\n", clause_count);
if(clause_count==374)t_count++;
//Export((*set)->first);

//null_remove(set);
(*set)=(*set)->first;
null_add(set);
solve();

dispose(clause_node);

null_remove(set);

(*set)=(*set)->first->end;
//printf(" set %i \n", end_clause);
while((*set)->clause>end_clause){
if(clause_visited[new_old_clause[(*set)->clause]]==0){
	printf(" unset vist\n");
	exit(0);
}
//	clause_visited[new_old_clause[(*set)->clause]]=0;
	copy_removed((*set)->clause,set);
//	(*set)=(*set)->previous;
}

		}


		first=first->next;
	}


}


// code archive
//copy_clause(ones[i],set);
//printf(" ontes %i %i \n", ones[i],variable_connections[ones[i]][1]);
//exit(0);

//null_add(set);
//k=	f_variable_connections[ones[1]][1];
//create_new_clause();
//add_to_clause(k,set);

//SetOnes();

//free((clause_node));
//printf(" this is the count %i \n", count_node(clause_node->next_layer));
//clause_node=clause_node->next_layer;

//printf(" ones[i] %i \n", ones[i]);
//gmp_printf("clause %i Data %Zd  Removed%Zd \n",clause_node->clause,clause_node->data,clause_node->removed);


//debug_pos(set);
//debug(clause_node);


//set_var[abs(f_variable_connections[ones[i]][1])]=0;
//var_tab_del(&set->first->clause);

//ReSetOnes();


