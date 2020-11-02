#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "headers/cnf_read.h"

#include "headers/llist.h"
#include "headers/var_pos.h"
#include "headers/bfs_s.h"
#include "headers/stest.h"
#include "headers/map.h"
#include "headers/clause_func.h"
#include "headers/index.h"

#include "headers/h_table.h"



void create_new_clause(com_line* Coms){	
	Coms->clause_count++;
	
	create_clause(Coms);

}

void AddVariableToGroup(int Variable, variable_pos* set){

	//create_new_clause()

}

void del_clause(int clause,com_line* Coms){
	//clause_count--;
//	printf("del clause %i  \n", clause);
	//removed variables from the clause
	for(int variable=Coms->clause_size[clause];variable>0;variable--){
		pop_var_clause(clause, variable, Coms);
	}
}

void pop_clause_(){
	//clause_count--;
}

variable_pos* copy_clause(int old_clause, int translation[], com_line* Coms){

	variable_pos* set						= Coms->set;
	variable_pos* variable_position = variable_position;
	
	
	Coms->clause_count++;
	
	translation[Coms->clause_count]	=	old_clause;
	set										=	create_clause(Coms);
	//int* clause_size 						=  Coms->clause_size;
	
	for(int variable =f_clause_size[old_clause]; variable!=0; variable--){
	
		Coms->clause_size[ Coms->clause_count]++;
		
		append_var_clause(set->clause,f_variable_connections[old_clause][variable],Coms);

	}
	
	return(set);
}


void null_add(com_line* Coms){

	variable_pos* set;
	
	Coms->clause_count++;
	//new_old_clause[clause_count]=old_clause;
	set=create_clause(Coms);
	(set)=set->first;

	
}
void null_remove(com_line* Coms){
	copy_removed(Coms->clause_count, Coms);

}

void copy_removed(int this_clause,com_line* Coms){

	variable_pos* set				= Coms->set;
	set= set->first->end;
	int** clause_connections = Coms->clause_connections;
	
	variable_pos* prev			= set;
	
	//(*set)=(*set)->end;

	if( set->previous != NULL ){
		prev = set->previous;
	}else{
		prev = set->first;
	}

//	printf(" copy remove this_clause %i %i \n",(*set)->clause, 0);

	for(int variable = Coms->clause_size[this_clause]; variable!=0; variable--){
		pop_var_clause(this_clause, variable,Coms);
		clause_connections[this_clause][variable]=0;	
	}
	new_old_clause[Coms->clause_count]=0;
	Coms->clause_size[this_clause]=0;
//	printf("this set %i \n", (*set)->clause);
	pop_clause(&set, Coms);

}

void RemoveFromClause(int this_clause,com_line* Coms){
	
	variable_pos* set		= Coms->set;


//	printf(" copy remove this_clause %i %i \n",(*set)->clause, 0);
	if( set ==NULL) return;
	for(int variable = Coms->clause_size[this_clause]; variable!=0; variable--){
		pop_var_clause(this_clause, variable, Coms);
		Coms->clause_connections[this_clause][variable] = 0;	
	}
	
	Coms->clause_size[this_clause]							= 0;
	pop_clause(&(Coms->set),Coms);
}



void append_var_clause(int clause, int variable, com_line* Coms){

	variable_pos** position = Coms->variable_position;
	
	//if the index clause is 0, this is a new variable to this index
	if(position[abs(variable)]->clause==0){
		Coms->variable_count++;
	}

	SetIndex(clause,variable,position);

	//store the variable at clause/variable num
	Coms->clause_connections[ clause ][ Coms->clause_size[clause] ] = variable;
}	


void pop_var_clause(int clause, int this_place, com_line* Coms){
// this only turns the placement of the clause to 0
// reorgranising the table required if you want to delete a non end;
	//search for the position of the variable in this clause
//printf("1pop variable %i %i %i \n",clause, this_place,variable_connections[2][1]);
//printf("clause %i this_place %i \n", clause, this_place);

	variable_pos**	variable_position		= Coms->variable_position;
	int** 			clause_connections	= Coms->clause_connections;


	variable_pos* this_clause	= variable_position[abs(clause_connections[clause][this_place])];
	

	variable_pos* mod;

	if(this_clause == NULL){
		printf("null var pos: pop var\n");
		exit(0);
	}

	if(this_clause->first->end != NULL){
		this_clause = this_clause->first->end;
	}


		variable_pos* tmp;
	if(this_clause->previous==NULL 
	&& this_clause->next==NULL){
	
		this_clause->clause		= 0;
		this_clause->first		= this_clause;
		this_clause->end			= this_clause;
		Coms->variable_count--;
		this_clause->next			= NULL;

		this_clause->previous	= NULL;
		return;
	}

	if(this_clause->previous != NULL 
	&& this_clause->next		 == NULL){
	
		tmp							= this_clause;
		this_clause					= this_clause->previous;
		this_clause->first->end	= this_clause;
		this_clause->next			= NULL;
		free(tmp);	
		return;
	}


	if(this_clause->next!=NULL ){
		printf(" theres a next\n");
		exit(0);
/*		if(this_clause->previous!=NULL){
			this_clause->previous->next=this_clause->next;
			this_clause->next->previous=this_clause->previous;
			free(this_clause);
		}else{
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
		*/
	}

}

void RemoveClause(int this_clause, com_line* Coms){
	variable_pos* set = Coms->set;
	variable_pos* prev=(set);

	(set)=(set)->first->end;

	if((set)->previous!=NULL){
		prev=(set)->previous;
	}else{
		prev=(set)->first;
	}

//	printf(" copy remove this_clause %i %i \n",(*set)->clause, 0);

	for(int variable = Coms->clause_size[this_clause]; variable!=0; variable--){
		fpop_clause(this_clause, variable);
		
		f_variable_connections[this_clause][variable]=0;	
	}
//	new_old_clause[clause_count]=0;
	f_clause_size[this_clause]=0;
//	printf("this set %i \n", (*set)->clause);
	pop_clause(&set,Coms);
	

}


void fpop_clause(int clause, int this_place){
// this only turns the placement of the clause to 0
// reorgranising the table required if you want to delete a non end;
	//search for the position of the variable in this clause
//printf("1pop variable %i %i %i \n",clause, this_place,variable_connections[2][1]);
//printf("clause %i this_place %i \n", clause, this_place);
	variable_pos* this_clause= f_variable_position[abs(f_variable_connections[clause][this_place])];
	

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
	f_variable_count--;
	this_clause->next=NULL;

	this_clause->previous=NULL;
return;
}

if(this_clause->previous!=NULL && this_clause->next==NULL){
		tmp = this_clause;
		this_clause=this_clause->previous;
		this_clause->first->end=this_clause;
		this_clause->next=NULL;
		free(tmp);	
		
		
	//	
return;
}


		if(this_clause->next!=NULL ){
printf(" theres a next\n");
exit(0);
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


//search for another clause that contains an exact subset of this clause


link_node* var_subset_find(link_node* clause, com_line* Coms){
	clause = clause->first;
	variable_pos* node;
	
	link_node* found = NULL;
	
	int count =0;
	hash_t* h_table ;
	while(1){
		h_table = hasht_create(257);

		for( int var = f_clause_size[clause->data] ; var!=0; var--){
			table_add( abs( f_variable_connections[ clause->data][var] ),  h_table);
		}
		
		// for the variables in the clause
		for(int k=f_clause_size[clause->data] ; k!= 0; k-- ){
			
			node = f_variable_position[ abs( f_variable_connections[ clause->data ][ k ] ) ];
				
			while( 1 ){
				
				for(int variable=f_clause_size[node->clause];variable!=0; variable--){
					if( check_table( abs(f_variable_connections[node->clause][variable]) , h_table ) ==1)
						count++;
				}
					
				if ( count == f_clause_size[clause->data] ){
					//add link
					found = link_append( node->clause, found, Coms);
				}
				count=0;
					
				if( node->next == NULL) {
					break;
				}
		 		node=node->next;
		 	}
			
		}
		hash_t_destroy( &h_table );
		h_table=NULL;
		if( clause->next == NULL)
			break;
		clause = clause->next;
	}

	clause=(AddLists( found, clause, Coms));
	return clause;
}

void RemoveLastAssert(){

	IsVariableSet[ abs(f_variable_connections[f_clause_count][1]) ] =0;

	fpop_clause(f_clause_count, 1);
		
	f_variable_connections[f_clause_count][1]=0;	
	f_clause_size[f_clause_count]=0;
	ones[abs(ones[0])]=0;
	ones[0]--;
	f_variable_connections[0][0]--;
	f_clause_count--;
}

