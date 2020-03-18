

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "var_pos.h"

void dispose_var_pos(variable_pos *head)
{
	variable_pos *cursor, *tmp;

	if(head != NULL){
		cursor= head->next;
		
		head->next=NULL;
		while(cursor!=NULL){
			tmp=cursor->next;
			free(cursor);
			cursor=tmp;
		}
	}
}


inline variable_pos* create_pos(int clause,variable_pos* previous){
	variable_pos* new_variable_pos = (variable_pos*)malloc(sizeof(variable_pos));
	if(new_variable_pos==NULL){
		printf("error creating a new variable_pos. \n");
		exit(0);
	}
	
	new_variable_pos->clause=clause;
	if(previous!=NULL){
		new_variable_pos->previous = previous->end;
		previous->next=new_variable_pos;
		new_variable_pos->first=previous->first;
		previous->end=new_variable_pos;
	}
	else{
		new_variable_pos->first=new_variable_pos;
		new_variable_pos->end=new_variable_pos;
	}

	return new_variable_pos;
}

inline variable_pos* append_variable(int clause, variable_pos* head){

	variable_pos* new_variable_pos=create_pos(clause, head);
	
	if(head!=NULL){
		new_variable_pos->previous=head;
		//
		new_variable_pos->previous=head->end;
		head->end->next=new_variable_pos;
		head->end=new_variable_pos;
		new_variable_pos->first=head->first;
	}
	else{			
		new_variable_pos->end=new_variable_pos;
	}

	return new_variable_pos;
}



// relase from list
inline variable_pos* pop_pos(variable_pos* cursor){
	variable_pos* return_pos;
	if(cursor->previous) return_pos=cursor->previous;
	if(cursor->next!=NULL){
		variable_pos* next = cursor -> next;
		next -> previous= cursor -> previous;
		variable_pos* prev = cursor -> previous;
		prev -> next = cursor -> next;
	}
	else{
		cursor -> previous =NULL;
	}

	free(cursor);
	return(return_pos);
}

inline int count_var_pos(variable_pos* head){
	
	int count=0;
	variable_pos *tmp = head;
	
	while(tmp!=NULL){
		count++;
		tmp= tmp->next;
	}
	return (count);
}


