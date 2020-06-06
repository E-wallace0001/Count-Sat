#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#include "llist.h"
#include "var_pos.h"
#include "bfs_s.h"
#include "stest.h"
#include "map.h"
#include "clause_func.h"
#include "index.h"




void create_new_clause(){
	clause_count++;
	
	create_clause(clause_count,(set)->first->end);

}

void AddVariableToGroup(int Variable, variable_pos* set){

	//create_new_clause()

}

void del_clause(int clause){
	//clause_count--;
//	printf("del clause %i  \n", clause);
	//removed variables from the clause
	for(int variable=clause_size[clause];variable>0;variable--){
		pop_var_clause(clause, variable);
	}
}

void pop_clause_(){
	//clause_count--;
}

variable_pos* copy_clause(int old_clause, variable_pos* set, int translation[]){

//if(clause_count!=0 ){set=set->first->end;}

	clause_count++;
	
	translation[clause_count]=old_clause;
	set=create_clause(clause_count,(set));

	for(int variable = f_clause_size[old_clause]; variable!=0; variable--){
		clause_size[clause_count]++;
		append_var_clause(set->clause,f_variable_connections[old_clause][variable],variable_position);
		
	}
//		printf("++++++++++++++++++++++++++clause_size %i \n", clause_count);
	return(set);
}


variable_pos* AddToClause(int new_variable, variable_pos* set){
		set=set->first->end;
	clause_size[set->clause]++;
	append_var_clause(set->clause,new_variable,variable_position);
		
	return(set);
}

void null_add(variable_pos**set){
	clause_count++;
	//new_old_clause[clause_count]=old_clause;
	(*set)=create_clause(clause_count,(*set)->first->end);
	(*set)=(*set)->first;

	
}
void null_remove(variable_pos** set){
	copy_removed(clause_count,&((*set)->first->end));

}

void copy_removed(int this_clause,variable_pos** set){
	variable_pos* prev=(*set);

	//(*set)=(*set)->end;

	if((*set)->previous!=NULL){
		prev=(*set)->previous;
	}else{
		prev=(*set)->first;
	}

//	printf(" copy remove this_clause %i %i \n",(*set)->clause, 0);

	for(int variable = clause_size[this_clause]; variable!=0; variable--){
		pop_var_clause(this_clause, variable);
		variable_connections[this_clause][variable]=0;	
	}
	new_old_clause[clause_count]=0;
	clause_size[this_clause]=0;
//	printf("this set %i \n", (*set)->clause);
	pop_clause(set);

}

void RemoveFromClause(int this_clause,variable_pos** set){
	variable_pos* prev=(*set);

	(*set)=(*set)->first->end;

	if((*set)->previous!=NULL){
		prev=(*set)->previous;
	}else{
		prev=(*set)->first;
	}

//	printf(" copy remove this_clause %i %i \n",(*set)->clause, 0);

	for(int variable = clause_size[this_clause]; variable!=0; variable--){
		pop_var_clause(this_clause, variable);
		
		variable_connections[this_clause][variable]=0;	
	}
//	new_old_clause[clause_count]=0;
	clause_size[this_clause]=0;
//	printf("this set %i \n", (*set)->clause);
	pop_clause(set);
	

}



void append_var_clause(int clause, int variable, variable_pos* position[]){

	//if the index clause is 0, this is a new variable to this index
	if(position[abs(variable)]->clause==0){variable_count++;}

	SetIndex(clause,variable,position);

	//store the variable at clause/variable num
	variable_connections[clause][clause_size[clause]]=variable;
}


void pop_var_clause(int clause, int this_place){
// this only turns the placement of the clause to 0
// reorgranising the table required if you want to delete a non end;
	//search for the position of the variable in this clause
//printf("1pop variable %i %i %i \n",clause, this_place,variable_connections[2][1]);
//printf("clause %i this_place %i \n", clause, this_place);
	variable_pos* this_clause= variable_position[abs(variable_connections[clause][this_place])];

	variable_pos* mod;

if(this_clause==NULL){printf("null var pos: pop var\n");exit(0);}

if(this_clause->first->end!=NULL){
	this_clause=this_clause->first->end;
}


	variable_pos* tmp;
if(this_clause->previous==NULL && this_clause->next==NULL){
	this_clause->clause=0;
	this_clause->first=this_clause;
	this_clause->end=this_clause;
	variable_count--;
	this_clause->next=NULL;

	this_clause->previous=NULL;
return;
}

if(this_clause->previous!=NULL && this_clause->next==NULL){
		this_clause=this_clause->previous;
		this_clause->first->end=this_clause;
		this_clause->next=NULL;
		free(this_clause->next);	
		
		
	//	
return;
}


		if(this_clause->next!=NULL ){
//free(this_clause->next);	
printf(" theres a next\n");
exit(0);
//halt();
			if(this_clause->previous!=NULL){
				this_clause->previous->next=this_clause->next;
				this_clause->next->previous=this_clause->previous;
				free(this_clause);
			}
			else{
				this_clause->next->previous=NULL;
				variable_pos* tmp=this_clause->next;
				while(1){
					tmp->first=this_clause->next;
					tmp=tmp->next;
				}
				free(this_clause);
			}
		this_clause=this_clause->previous;
		this_clause->first->end=this_clause;
this_clause->next=NULL;
		free(this_clause->next);	
		
return;
}
//	pop_pos(this_clause);
//
	//variable_connections[this_clause->clause][this_place]=0;	
//printf("pop\n");
//debug_pos(this_clause);
	// decrement the size of the clause
	
}



