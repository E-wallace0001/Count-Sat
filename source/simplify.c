#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "headers/group.h"
#include "headers/simplify.h"
#include "headers/clause_control.h"


void simplify(group_s* group){

	group_s* comp_clause;
	list* 	comp_variables;
	group->list = group->first;
	
	set_s* clause = (set_s*)group->list->data;
	
	size_t size		= 1000000;
	state** list_i	= (state**)malloc( size*sizeof(state**) );
	
	
	
	size_t indx		= 0;
	set_s* inner_set = malloc( sizeof(*inner_set));
	list* state_i;
	set_s* var_set ;
	while( 1 ){

		//skips if the size is 1 or smaller
		var_set = (set_s*) group->list->data;

		while( CountGroupSet( var_set) <=1){

			if(group->list->next ==NULL){
				var_set=NULL;
				break;
			}
			group->list=group->list->next;
			var_set = (set_s*) group->list->data;
		}
		if(var_set==NULL)
			break;
			//needs to be next
	
		indx++;
		list_i[indx] = (state*)calloc( size,sizeof(state));
		
		if( group->list->next == NULL )
			break;
			
		
		set_s* variables	= (set_s*)group->list->data;
		
		AcknowledgeClause( variables, list_i[indx] );
		
		// look at all of the following clauses
		
		if (group->list->next == NULL)
			break;
			
		inner_set->list	= group->list;
		inner_set->first	= group->first;
		inner_set->end 	= group->end;
		inner_set->list	= inner_set->list->next;
		clause = (set_s*)group->list->data;
	 
		while( 1 ){
		
		var_set = (set_s*) inner_set->list->data;
		
		while( CountGroupSet( var_set) <=1){

			if(inner_set->list->next ==NULL){
				var_set=NULL;
				break;
			}
			inner_set->list=inner_set->list->next;
			var_set = (set_s*) inner_set->list->data;
		}
			
			comp_clause = (set_s*) inner_set->list->data;
			
			if( simp_comp( comp_clause, list_i[indx]) == true ){
				printf(" found \n");

/*
				//	for printing
				list* variable = comp_clause->first;
				printf (" this clause \n");
				while( variable != NULL){
					printf (" %i ", *(int*)variable->data);
					variable = variable->next;	
				}
				printf(" \n");
				*/
				//printf(" set ocunt %i \n",CountGroupSet(group) );
				RemoveKnown( comp_clause, list_i[indx] );
				
				
				if( comp_clause->first != NULL){
					
					AcknowledgeClause( comp_clause, list_i[indx] );

					CopyGroupSet( comp_clause, clause);
					
					set_s* temp= (set_s*) group->list->data;
					printf("first set \n");
					/*
					variable = temp->list;
					
					while( variable != NULL){
						printf (" %i ", *(int*)variable->data);
						variable = variable->next;	
					}
					
					printf(" \n");
					*/
					//halt();

				}
				ReduceGroup( &inner_set, &comp_clause );
					
				printf(" \nset ocunt2 %i \n",CountGroupSet(group) );
				inner_set->list = group->list;

			}
				
			if( inner_set->list->next ==NULL)
				break;
			inner_set->list = inner_set->list->next;
		}

		printf( "clause22\n\n\n \n");

		if( group->list->next == NULL) 
			break;
		group->list = group->list->next;
	}
	//exit(0);
	//resets containment variable set
	int inx = 1;
	while(list_i[inx]!=NULL){
		free (list_i[inx]);
		inx++;
	}
	free(list_i);
	
	printf(" set ocunt %i \n",CountGroupSet(group) );
	//DeleteGroup(&group);
	printf(" set ocunt %i \n",CountGroupSet(group) );
	return ;

}
