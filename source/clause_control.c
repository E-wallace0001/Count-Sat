#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "headers/group.h"
#include "headers/simplify.h"

bool simp_comp ( group_s* var_state, state* list_i ){
	bool match=0;
	bool not_match =0;
	list *comp_variables = var_state->list;
	while(comp_variables != NULL){
	//printf("  %i \n", *(int*) comp_variables->data );
		if( list_i[ abs(*(int*) comp_variables->data) ].asserted == 1){
			if( list_i[ abs(*(int*) comp_variables->data) ].assigned == 1 
					&& *(int*) comp_variables->data > 0){
					match = 1;
				//	printf("correct \n");
			}
			if( list_i[ abs(*(int*) comp_variables->data) ].assigned == 0 
					&& *(int*) comp_variables->data < 0){
					match = 1;
				//	printf("correct \n");
			}
			if( list_i[ abs(*(int*) comp_variables->data) ].assigned == 0 
					&& *(int*) comp_variables->data > 0){
				//	printf(" skipped \n\n");
				return(0);
			}
			if( list_i[ abs(*(int*) comp_variables->data) ].assigned == 1
					&& *(int*) comp_variables->data <0 ){
			//		printf(" skipped \n\n");
				return(0);
			}
		}
		if(comp_variables->next == NULL) 
			break;
			
		comp_variables = comp_variables->next;
	}
	
	if(match == 1){
		return 1;
	}else{
		return 0;
	}
}

void AcknowledgeClause( set_s* clause_set, state* list_i){
	list* clause = clause_set->first;
	while(1){
	
		list_i[ abs(*(int*) clause->data) ].asserted = 1;
		if ( *(int*) clause->data > 0 ){
			list_i[ abs(*(int*) clause->data) ].assigned = 1; 
		}else{
		
			list_i[ abs(*(int*) clause->data) ].assigned = 0;
		}
		if(clause->next==NULL) break;
		clause=clause->next;
	}
}	

void RemoveKnown( set_s* var_state, state* list_i ){
	bool  match=0;
	bool  not_match =0;
	list* comp_variables = var_state->list;
	while(1){
		if( list_i[ abs(*(int*) comp_variables->data) ].asserted == 1){
		printf(" removed known  %i \n", *(int*) comp_variables->data);
			if( list_i[ abs(*(int*) comp_variables->data) ].assigned == 1 
					&& *(int*) comp_variables->data>0){
					
				if( ReduceSet(var_state->list, &var_state) == 1){
					comp_variables= var_state->list;
					if(comp_variables==NULL)
						break;
					continue;
				}

				comp_variables= var_state->list;

				if ( comp_variables==NULL) 
					break;
			}
			
			if( list_i[ abs(*(int*) comp_variables->data) ].assigned == 0 
					&& *(int*) comp_variables->data<0){
					
				if( ReduceSet(var_state->list, &var_state) == 1){
					comp_variables= var_state->list;
					if(comp_variables==NULL)
						break;
					continue;
				}

				comp_variables= var_state->list;

				if ( comp_variables==NULL) 
					break;
			}
		}
		if (comp_variables->next==NULL ) 
			break;
		comp_variables = comp_variables->next;
		var_state->list = comp_variables;
		
	}
}



