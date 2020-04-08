

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "var_pos.h"
#include "stest.h"
#include "bfs_s.h"

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


inline variable_pos* create_clause(int clause,variable_pos* previous){
	
	variable_pos* new_variable_pos = (variable_pos*)malloc(sizeof(variable_pos));

	if(new_variable_pos==NULL){
		printf("error creating a new variable_pos. \n");
		exit(0);
	}
	if(previous)previous=previous->end;
	new_variable_pos->clause=clause;
	if(previous!=NULL ){

		previous->next=new_variable_pos;
		new_variable_pos->first=previous->first;
		previous->next=new_variable_pos;

		new_variable_pos->previous = previous;
		previous->first->end=new_variable_pos;
		previous->end=new_variable_pos;
		new_variable_pos->end=new_variable_pos;

	}
	else{

		new_variable_pos->clause=clause;
		new_variable_pos->end=new_variable_pos;
		new_variable_pos->first=new_variable_pos;
		new_variable_pos->end=new_variable_pos;
		new_variable_pos->previous=NULL;
	}

	return new_variable_pos;
}

inline variable_pos* append_variable(int clause, variable_pos* head){

//	variable_pos* new_variable_pos=create_pos(clause, head);
	variable_pos* new_variable_pos = (variable_pos*)malloc(sizeof(variable_pos));
	if(new_variable_pos==NULL || head==NULL){
		printf("error creating a new variable_pos. \n");
		exit(0);
	}
	if(head)head=head->end;
	if(head->clause==0){
		new_variable_pos->clause=clause;

		new_variable_pos->first=head->first;
		head->first->end=new_variable_pos;
		head->next=new_variable_pos;
		new_variable_pos->previous=head;

		head->end=new_variable_pos;

	}else{
		new_variable_pos->clause=clause;

		new_variable_pos->first=head->first;
		head->first->end=new_variable_pos;
		head->next=new_variable_pos;
		new_variable_pos->previous=head;

		head->end=new_variable_pos;

	}

	return new_variable_pos;
}



// release from list
void pop_clause(variable_pos* *cursor){
if(!(*cursor)){printf(" null pass ptr\n");}
	variable_pos* tmp;
	variable_pos* st;
	if((*cursor)->previous!=NULL){
		printf("previous con %i\n",(*cursor)->clause);
		tmp=(*cursor);
		tmp=tmp->previous;
		tmp->next=NULL;
		tmp->first->end=tmp;


		//tmp->clause=5;
		//(*cursor)->next=NULL;
		(*cursor)=tmp;
		free(tmp->next);

	}
	else{
printf("prev\n");
		tmp=(*cursor);
		if((*cursor)->next!=NULL){
		(*cursor)=(*cursor)->next;
		(*cursor)->previous=NULL;
st=tmp->first;
while(st!=NULL){
	st->first=(*cursor);
if(st->next==NULL) (*cursor)->end=st;
	st=st->next;
}
		tmp=NULL;	
		free(tmp);
		
		}	
		else{
			(*cursor)=NULL;
			free(*cursor);

		}
		//(*cursor)->first->end=(*cursor);	
		printf("end of list \n");
		//halt();
	}
clause_count--;

}

variable_pos* search_var_pos(int clause,variable_pos* head){
//debug_pos(head);
	if(!head|| head->clause==0){
printf("sear_var_pos: NULL head\n");exit(0);return(0);
}

//printf("sear_var_pos:%i  %i \n",clause,head->clause);
	variable_pos* tmp = head->first;

	while(1){
		if(tmp->clause==clause){
	//		printf("found %i \n",tmp->clause);
			return(tmp);
		}
	if(tmp->next!=NULL){
	tmp=tmp->next;
	}
	else{break;}
	}
printf("nothing found %i \n",tmp->clause); exit(0);
}

inline int count_var_pos(variable_pos* head){
	
	int count=0;
	if(!head){ printf("null head count \n");return(0);}
	
	variable_pos *tmp = head->first;
	
	while(tmp!=NULL){
		count++;
		tmp= tmp->next;
	}
	return (count);
}


void debug_pos(variable_pos *head){

if(!head){
printf("debug pos NULL VALUE\n");
exit(0);
}
//head=head->first;
char input;

printf("\n clause %i, old %i  \n ", head->clause,new_old_clause[head->clause]);
while (1){

scanf("%c", &input);
if(getchar()!=0){
//printf("boop");
}

	switch(input){
		
		case 'a':
			if(head->previous==NULL){ printf("no previous\n");
			}
			else{ head=head->previous; 
printf("previous \n clause %i odl clause%i \n ", head->clause,new_old_clause[head->clause]);

			}
		break;
		case 'd':
			if(head->next==NULL){ 
printf("no next\n");
			}
			else{ head=head->next; 
printf("next \n clause %i old clause %i \n", head->clause,new_old_clause[head->clause]);

			}
		break;
		case 'x':
			return;
		break;
		}
}
printf(" output: %c \n", input);
}
	

