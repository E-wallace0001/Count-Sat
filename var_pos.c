

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#include "llist.h"
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

variable_pos* make_clause(int clause){
	variable_pos* new_clause = (variable_pos*)malloc(sizeof(variable_pos));
	if(new_clause==NULL){
		printf("error creating a make clause \n");
		exit(0);
	}
		new_clause->clause=clause;
		new_clause->end=new_clause;
		new_clause->first=new_clause;
		new_clause->previous=NULL;
		new_clause->next=NULL;
	return new_clause;
}

inline variable_pos* create_clause(int clause,variable_pos* previous){
	variable_pos* new_clause=make_clause(clause);
	variable_pos* tmp;
	
	
	if(previous!=NULL ){

	new_clause->previous=previous;

	new_clause->first=previous->first;
	new_clause->next=NULL;
	previous->next=new_clause;

	new_clause->first->end=new_clause;
	
	}
	else{
		new_clause->clause=clause;
		new_clause->end=new_clause;
		new_clause->first=new_clause;
		new_clause->previous=NULL;
		new_clause->next=NULL;

	}

	return new_clause;
}

inline variable_pos* append_variable(int clause, variable_pos* head){
//	variable_pos* new_variable_pos=create_pos(clause, head);
	
	variable_pos* new_variable_pos;
	
	head=head->first->end;
	if(head->clause==0){
		head->clause=clause;
		return head;
		
	}else{

	 new_variable_pos = (variable_pos*)malloc(sizeof(variable_pos));
	if(new_variable_pos==NULL || head==NULL){
		printf("error creating a new variable_pos. \n");
		exit(0);
	}

		new_variable_pos->clause=clause;
		
		new_variable_pos->first=head->first;
		new_variable_pos->previous=head;

		new_variable_pos->next=NULL;

		head->next=new_variable_pos;
		head->first->end=new_variable_pos;
	}

	return new_variable_pos;
}



// release from list
void pop_clause(variable_pos* *cursor){
if((*cursor)==NULL){printf(" null pass ptr\n");exit(0);}
	variable_pos* tmp=(*cursor);
	variable_pos* st;

	if((*cursor)->previous!=NULL){
	//	printf("previous con %i\n",(*cursor)->clause);
		
		if((*cursor)->next==NULL){
			(*cursor)=(*cursor)->previous;

			(*cursor)->first->end=(*cursor);
		
			tmp=(*cursor)->next;
			(*cursor)->next=NULL;
			free(tmp);	
			

		}else{
			(*cursor)->previous->next=tmp->next;
			(*cursor)->next->previous=tmp->previous;
			free((*cursor));
			

		}

	}
	else{
		//if there's not a previous address
		//printf("prev\n");
		tmp=(*cursor);
		if((*cursor)->next!=NULL){
			(*cursor)=(*cursor)->next;
			(*cursor)->previous=NULL;
			free(tmp);
			
			st=(*cursor)->next;
			while(st!=NULL){
				st->first=(*cursor);
				st=st->next;
			}	
		}else{
				(*cursor)=NULL;
				free(*cursor);
		}
	//	printf("end of list \n");
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

int count_var_pos(variable_pos* head){
	variable_pos *tmp;
	int count=0;
	if(head==NULL){ printf("null head count \n");return(0);}
	if(head->first!=NULL){tmp = (head)->first;}else{tmp=head;}
	
	while(1){
		count++;
		if(tmp->next!=NULL){
				tmp= tmp->next;
		}else{
			break;
		}
		}
	return (count);
}

void Assert_Variable(int variable){

f_variable_connections[0][0]++;
f_clause_size[f_variable_connections[0][0]]=1;
	

	f_variable_connections[f_variable_connections[0][0]][1]=variable;
	ones[0]++;
	ones[ones[0]]=f_variable_connections[0][0];
	OnesPlace[abs(variable)]=f_variable_connections[0][0];
	
	f_clause_count++;
	
	if( f_variable_position[abs(variable)]->clause == 0 ){
		f_variable_position[abs(variable)]->clause		= f_variable_connections[0][0];
		f_variable_position[abs(variable)]->end			= f_variable_position[abs(variable)];
	}else{	

		append_variable(f_variable_connections[0][0],f_variable_position[abs(variable)]);

	}

}

void debug_pos(variable_pos *head){

if(!head){
printf("debug pos NULL VALUE\n");
exit(0);
}
//head=head->first;
char input;
while (1){

printf("\n clause %i, old %i size[i] %i \n ", head->clause,new_old_clause[head->clause],clause_size[head->clause]);
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
	

