#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "llist.h"
#include "stest.h"


link_node* link_create(int data, link_node* previous, link_node* next){
	link_node* new_node = (link_node*)malloc(sizeof(link_node));
	if(new_node==NULL){
		printf("error creating a new node. \n");
		
	}

	new_node->first=new_node;
	new_node->end=new_node;
	new_node->data = data;
	new_node->next = NULL;
	new_node->previous = NULL;
	return new_node;
}

link_node* link_append(int data,link_node* head){
	link_node* new_node=link_create(data, head,NULL);

	if(head!=NULL ){
	
		head=head->first->end;
		
		new_node->first=head->first;
		new_node->first->end=new_node;
		new_node->next=NULL;
		new_node->previous=head;
		head->next=new_node;
	}
	else{
		new_node->next=NULL;
		new_node->previous=NULL;
		new_node->first=new_node;
		new_node->data=data;
		new_node->end=new_node;
	}

return(new_node) ;
}

// add a new node to the list
static inline link_node* link_prepend(link_node* head,int data){
	
	link_node* new_node=link_create(data,head,NULL);
	//if(new_node->first==NULL){new_node->first = new_node;}else{new_node->first=head;}

	return new_node;
}

void pop_link(link_node** cursor){

if((*cursor)==NULL){printf(" null ptr- link_pop\n"); exit(0);}

(*cursor)=(*cursor)->first->end;
link_node* temp= (*cursor);
if((*cursor)->previous!=NULL){
	(*cursor)=(*cursor)->previous;
	(*cursor)->first->end=(*cursor);
	(*cursor)->next=NULL;
	free(temp);
}else{
	if((*cursor)->next!=NULL){
		printf("not end\n");
		exit(0);
		temp=(*cursor)->next;
		temp->previous=NULL;
	}else{
		free(*cursor);
		(*cursor)=NULL;
	}
	
}

}



int RemoveMember(link_node** cursor){
	int ret=0;
if((*cursor)==NULL){printf(" null ptr- link_pop\n"); exit(0);}

link_node* temp;
link_node* next;
link_node* previous;
		
if((*cursor)->previous!=NULL){
	
	if((*cursor)->next!=NULL){
		next=(*cursor)->next;
		previous=(*cursor)->previous;
		temp=(*cursor);
		(*cursor)=next;
		next->previous=previous;
		previous->next=next;
		
		//free(temp);
	}else{
		temp=(*cursor);
		(*cursor)=(*cursor)->previous;
		
		(*cursor)->first->end=(*cursor);
		(*cursor)->next=NULL;
		
		//free(temp);
		
		return (-1);
	}
	
	
}else{
	if((*cursor)->next!=NULL){
		temp=(*cursor);
		(*cursor)=(*cursor)->next;
		
		(*cursor)->first=(*cursor);
		(*cursor)=(*cursor)->first;
		(*cursor)->previous=NULL;
		
		while(1){
			(*cursor)->first=temp->next;
			if((*cursor)->next==NULL)break;
			(*cursor)=(*cursor)->next;
		}
		temp->next->end=(*cursor);
		free(temp);

	}else{
		(*cursor)->data=0;
		(*cursor)->next=NULL;
		(*cursor)->previous=NULL;
		//(*cursor)=NULL;
		//free(*cursor);
		return (-1);
	}

	
}
return(0);
}

int ListSize(link_node* cursor){

	int count=0;

	if(cursor==NULL) return 0;


	cursor=cursor->first;

	while(1){

		count++;
		if(cursor->next==NULL){ break; }
		//printf(" 	count %i \n", cursor->data);
		
		cursor=cursor->next;
	}
	return count;
	

}

void DeleteList(link_node** cursor){
	(*cursor)=(*cursor)->first->end;
	while((*cursor)!=NULL){
		if((*cursor)->previous!=NULL){ break;}
		(*cursor)=(*cursor)->previous;
		free((*cursor)->next);
	
	}
	free((*cursor));

}

link_node* DeleteNode(link_node* cursor){
	int ret=0;
if((cursor)==NULL){printf(" null ptr- link_pop\n"); exit(0);}

link_node* temp;
link_node* next;
link_node* previous;
		
if((cursor)->previous!=NULL){
	
	if((cursor)->next!=NULL){
		next=(cursor)->next;
		previous=(cursor)->previous;
		next->previous=previous;
		previous->next=next;
		
		free(cursor);
		return(previous);
	}else{
		temp=(cursor);
		(cursor)=(cursor)->previous;
		
		(cursor)->first->end=(cursor);
		(cursor)->next=NULL;
		
		free(temp);
		
		return (cursor);
	}
	
	
}else{
	if((cursor)->next!=NULL){
		temp=(cursor);
		(cursor)=(cursor)->next;
		
		(cursor)->first=(cursor);
		(cursor)=(cursor)->first;
		(cursor)->previous=NULL;
		
		while(1){
			(cursor)->first=temp->next;
			if((cursor)->next==NULL)break;
			(cursor)=(cursor)->next;
		}
		cursor->first->end=(cursor);
		free(temp);
		return (cursor->first);

	}else{
		(cursor)->data=0;
		(cursor)->next=NULL;
		(cursor)->previous=NULL;
		//(*cursor)=NULL;
		free(cursor);
		return NULL;
	}
	
	}
}

link_node* AddLists(link_node* OpA, link_node* OpB){

	if( OpA==NULL || OpB==NULL ){ printf(" no valid table\n"); exit(0); }

	link_node* list;

	OpA = OpA->first;
	OpB = OpB->first;

	while(OpA!=NULL){
		list=link_append(OpA->data,list);
		OpA=OpA->next;
	}
	while(OpB!=NULL){
		list=link_append(OpB->data,list);
		OpB=OpB->next;
	}
	
	return list;

}


void link_dispose(link_node *head)
{
	link_node *cursor, *tmp;

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


void debug_list(link_node *head){

if(head==NULL){
printf("debug pos NULL VALUE\n");
exit(0);
}
//head=head->first;
char input;
while (1){

printf("\n data %i \n ", head->data);
scanf("%c", &input);
if(getchar()!=0){
//printf("boop");
}

	switch(input){
		
		case 'a':
			if(head->previous==NULL){ printf("no previous\n");
			}
			else{ head=head->previous; 
printf("previous \n data %i  \n ", head->data);

			}
		break;
		case 'd':
			if(head->next==NULL){ 
printf("no next\n");
			}
			else{ head=head->next; 
printf("next \n data %i  \n", head->data);

			}
		break;
		case 'x':
			return;
		break;
		}
}
printf(" output: %c \n", input);
}

