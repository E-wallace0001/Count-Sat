
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#include <string.h>

#include "var_pos.h"

#include "stest.h"
#include "clause_func.h"
#include "stest.h"
#include <gmp.h>
#include "llist.h"
#include "bfs_s.h"
#include "infini_tree.h"

#include "index.h"



// find all the set variables
//	ones[];
/*
if(ones[0]==0) //printf(" there are no set variables\n"); exit(0);
*/

int cnt=0;
int t_count=0;
variable_pos* set;
variable_pos* tmp;
	variable_pos* set_copy=NULL;
bool clause_visited[9000]={0};
//bool inline_visited[8000]={0};
int  new_old_clause[8000]={0};
bool counted_set[8000]={0};

bool CountVariable[9000]={0};

bool KnownSet[9000]={0};
bool IsVariableSet[9000]={0};
bool VariableSet[9000]={0};

variable_pos* variable_list=NULL;

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


void raw(){

	for(int clause=1;clause<=f_clause_count;clause++){

	set=copy_clause(clause, set);
	}
//Export(set);

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


void deduce(link_node* list, variable_pos** set){
mpz_t saved;
mpz_init(saved);
variable_pos* var_list;
variable_pos* tmp;
var_list=variable_list;

printf("variable_count %i \n", variable_count);

SetIndex_LH(list,set);
//Export(*set);

(*set)=(*set)->first;
printf("variable_count %i \n", variable_count);

if(variable_list==NULL){printf("null variable_list \n");exit(0);}
	(*set)=(*set)->first;
	null_add(set);

printf(" this %i %i \n", ones[0],clause_count);
//debug_list(list);exit(0);}
	solve();
printf(" ones %i \n", ones[0]);
//if(clause_count==40)exit(0);
	gmp_printf(" %Zd 1 end \n", pnt->data);
	mpz_set(saved, pnt->data);
	null_remove(set);
	//dispose(pnt->next_layer);
	dispose(clause_node);
halt();

//exit(0);


var_list=var_list->first->end;
	while(var_list!=NULL){
	//

		if(IsVariableSet[var_list->clause]==0 ){
			create_new_clause();
			AddToClause(var_list->clause,*set);

			null_add(set);

			solve();
printf(" ones %i \n", ones[0]);
			RemoveFromClause(clause_count, &(*set));
			null_remove(set);
				if(mpz_cmp(pnt->data,saved)==0){
					VariableSet[var_list->clause]=1;
IsVariableSet[var_list->clause]=1;
					Assert_Variable(var_list->clause);
					CountVariable[var_list->clause]=0;
					IsVariableSet[var_list->clause]=1;

					cnt++;
				}
				if( mpz_cmp_ui(pnt->data,0)==0){
VariableSet[var_list->clause]=1;
					IsVariableSet[var_list->clause]=1;
					VariableSet[var_list->clause]=0;
					Assert_Variable(-(var_list->clause));

					CountVariable[var_list->clause]=0;
					cnt++;
				}
			//	gmp_printf(" %Zd 2 end \n", pnt->data);
			//halt();
dispose(clause_node);
		//dispose(pnt->next_layer);
			
		}
		
		var_list=var_list->previous;
//		list->next=NULL;
//		free(list->next);
	}



while((*set)!=NULL){
//tmp=(*set)->previous;
RemoveFromClause(clause_count, set);
//(*set)=(*set)->previous;
}


mpz_clear(saved);
}

void init_graph(int ones[]){

	link_node* list=NULL;

	variable_pos* temp_clause;	
	int count=0;

	layer* layer=set_layer(-1,2);


		int k=0;
//variable set is state:
//0 =-1
//1 =1

for(int i=1; i<=ones[0];i++){
	KnownSet[abs(f_variable_connections[ones[i]][1])]=1;
//CountVariable[abs(f_variable_connections[ones[i]][1])]=1;
	IsVariableSet[abs(f_variable_connections[ones[i]][1])]=1;
	if(f_variable_connections[ones[i]][1]>0){
		VariableSet[abs(f_variable_connections[ones[i]][1])]=1;
	}else{
		VariableSet[abs(f_variable_connections[ones[i]][1])]=0;
	}
}
printf("%i \n",ones[0]);
	for(int i=1;i<=(ones[0]);i++){

//init_pos(variable_position);

		//clause_count++;

		//copy the original
//		set=copy_clause(ones[i], set);
		list=link_append(ones[i], list);
		clause_visited[ones[i]]=1;

//		printf(" i %i\n", i);

		for(int variable=f_clause_size[ones[i]];variable!=0;variable--){

			if(CountVariable[abs(f_variable_connections[ones[i]][variable])]==0){
				variable_list=create_clause(abs(f_variable_connections[ones[i]][variable]),variable_list);

		CountVariable[abs(f_variable_connections[ones[i]][variable])]=1;

			}else{
				continue;
			}

			printf(" f_Var %i \n", f_variable_connections[ones[i]][variable]);
			// collect all clauses connected to this variable
			temp_clause=f_variable_position[abs(f_variable_connections[ones[i]][variable])];
//			printf("tmp cl %i clause_count %i \n",temp_clause->clause,clause_count);

			//coppy all the clauses

			while(temp_clause!=NULL){

				//set it to vistied
				//clause_visited[temp_clause->clause]=1;
				if(clause_visited[temp_clause->clause]==0 ){

					printf(" clause %i %i \n", i , temp_clause->clause);
//					set=copy_clause(temp_clause->clause, set);
					list=link_append(temp_clause->clause, list);

					clause_visited[temp_clause->clause]=1;
				}
		
				temp_clause=temp_clause->next;

			}

		}

//exit(0);

		bfs_graph(layer,list);
list=list->first->end;

		//printf("this set %i \n", set->clause);

		while(1){
					//	printf("this is it \n");
			if(clause_visited[list->data]==0){
				printf(" unset vist %i \n",list->data);
				exit(0);
			}
			//clause_visited[list->data]=0;
			//copy_removed(set->clause,&set);
			pop_link(&list);
			if(list==NULL)break;
		}



	

//printf(" variable_list->clause %i \n", variable_list->clause);

while(1){

CountVariable[abs(variable_list->clause)]=0;

if(variable_list->previous!=NULL){
variable_list=variable_list->previous;


}else{
break;
}
}
	}
printf("end count %i %i\n", cnt,ones[0]);
FILE* fp2;
fp2=fopen("out3.cnf","w");

for(int i=1;i<=f_clause_count;i++)
{
	for(int k=f_clause_size[i];k!=0;k--){
		fprintf(fp2,"%i ", f_variable_connections[i][k]);
	}

	fprintf(fp2,"0\n");
}
fclose (fp2);
exit(0);
}
void bfs_graph( layer* layer,link_node* list){
	if(list==NULL){printf("nullset\n");exit(0);}
	link_node* first=list->first;
	int end_clause=list->first->end->data;

	int end_var=variable_list->first->end->clause;


	variable_pos* iter;
	variable_pos* temp_var;
	//variable_pos* set_saved=(*set);
	variable_pos* temp_clause;
	variable_pos* var_clause;
	int count=0;
	bool skipped=0;

	while(first->data!=end_clause){
		for(int variable=f_clause_size[first->data];variable!=0;variable--)
		{

			if(CountVariable[abs(f_variable_connections[first->data][variable])]==0){

				variable_list=create_clause(abs(f_variable_connections[first->data][variable]),variable_list);
				CountVariable[abs(f_variable_connections[first->data][variable])]=1;
			}else{
				continue;
			}

			temp_clause=f_variable_position[abs(f_variable_connections[first->data][variable])];
			if(temp_clause==NULL){printf(" null temp_clause \n");exit(0);}

			while(temp_clause!=NULL){
				//printf("temp_clause->clause %i \n", temp_clause->clause);
				if(clause_visited[temp_clause->clause]==0 ){
					//(*set)=copy_clause(temp_clause->clause,(*set));
					list=link_append(temp_clause->clause,list);
					clause_visited[temp_clause->clause]=1;


					for(int variable2=f_clause_size[temp_clause->clause];variable2!=0;variable2--){

						if(CountVariable[abs(f_variable_connections[temp_clause->clause][variable2])]==0){

							variable_list=create_clause(abs(f_variable_connections[temp_clause->clause][variable2]),variable_list);
							CountVariable[abs(f_variable_connections[temp_clause->clause][variable2])]=1;
						}
					}

			
				}	
				temp_clause=temp_clause->next;
			}

			printf(" this %i %i \n", ones[0],clause_count);

			deduce(list, &set);

			printf(" this count %i  %i \n", list->data, end_clause);
			if(list==NULL){ printf("error \n"); exit(0);}

			list=list->first->end;
			while(list->data!=end_clause){
				if(clause_visited[list->data]==0){
					printf(" this unset vist %i\n",list->data);
					exit(0);
				}
				//clause_visited[list->data]=0;
				pop_link(&list);
				//(*set)=(*set)->previous;
			}
			list=list->first->end;
		}

		if(first->next==NULL){break;printf(" null next %i %i \n", first->data, end_clause);exit(0);}
		first=first->next;
	}
		variable_list=variable_list->first->end;

		
		while(abs(variable_list->clause)!= abs(end_var)){
			CountVariable[abs(variable_list->clause)]=0;
			variable_list=variable_list->previous;
			variable_list->next=NULL;
			free(variable_list->next);
		}
		variable_list=variable_list->first->end;

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


