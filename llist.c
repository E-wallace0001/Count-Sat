#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "llist.h"
#include "stest.h"
#include "var_pos.h"
#include "map.h"
#include "index.h"

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


link_node* copy_list(link_node* list){
link_node* new_list=NULL;
if(list==NULL){ printf("null list copy \n"); exit(0);}

list=list->first;

while(1){
	new_list=link_append(list->data, new_list);
	if(list->next!=NULL){
		list=list->next;
	}else{
		break;
	}
}

return new_list;

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

int RemoveFromList(link_node** cursor){
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
		(*cursor)->next=NULL;
		(*cursor)->previous=NULL;
		//(*cursor)=NULL;
		free(*cursor);
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


int FindLargestClause( link_node* Node){

	if( Node == NULL ) return 0;
	Node=Node->first;	
	int largest=0;
	while(Node!=NULL){

		if( f_clause_size[Node->data] >largest){
			largest=f_clause_size[Node->data];
		}
		
		
		Node=Node->next;
	}
	
	return largest;

}

link_node* BinSort(link_node* Node){

	if( Node == NULL) exit(0);
	Node=Node->first;
	int size= FindLargestClause(Node);
	
	
	variable_pos** SearchIndex=CreateIndex(size);
	
	link_node* Sorted=NULL;
	
	Node=Node->first;
	
	//halt();
	while(Node!=NULL){
		
		SetIndex(Node->data,f_clause_size[Node->data],SearchIndex);
if(Node->next==NULL) break;
		Node=Node->next;
	}

	for(int i=1;i<=size;i++){
		if(SearchIndex[i]->clause==0){
			continue;
		}

		variable_pos* iter = SearchIndex[i];
			
		while(iter!=NULL){
			Sorted = link_append( iter->clause, Sorted);
			if(iter->next==NULL) break;
			iter	 = iter->next;
		}
		
	}
	
	FreeSearch(SearchIndex,size);
	return Sorted->first;
}

link_node* GroupSingles( link_node* set ){

	link_node* tried = NULL; 

	 set=set->first;
	link_node* iter = NULL;
	link_node* temp = NULL;
	bool found = 0;
	
	while(1){
	
		if( f_clause_size[ set->data ] == 1 ){
			
			if( ExistInSet( abs(f_variable_connections[ set->data ][ 1 ]) , tried) == 1){
				if( set->next == NULL) break;
				set=set->next;
				continue;
			}
			printf(" this single %i \n", f_variable_connections[ set->data ][ 1 ]);
			tried = link_append( abs(f_variable_connections[ set->data ][ 1 ]), tried);
			
			if( set->next == NULL ) break;
			
			temp = set->next;
			
			while(temp!=NULL){
				for( int variable = f_clause_size[ temp->data ]; variable != 0; variable--){
				
					if( abs( f_variable_connections[temp->data][variable] ) == abs(f_variable_connections[ set->data ][ 1 ]) ){
						printf( "found %i \n",f_variable_connections[temp->data][variable]);
						
						found=1;
						break;
					}
				}
				
				if( found==1){
					// Swap places
					if( set->previous != NULL){
						set->previous->next = set->next;
						set->next->previous = set->previous;

					}else{
					
						set->next->previous=NULL;
						set->first->end= set->next;
						iter=set->next;
						while( iter!=NULL){
						//iter->first = set->next;
						if( iter->next==NULL) break;
						iter=iter->next;
						}
					}
						set->previous = temp->previous;
						if(temp->previous!=NULL){
						temp->previous->next= set;
						}
						
					set->next = temp;
					temp->previous=set;
					found=0;
				}
				if(temp->next==NULL) break;
				temp=temp->next;
			}	
		}
	
		if(set->next==NULL) break;
		set=set->next;
	}

DestroySet( tried );
	return set->first;
}

link_node* GroupTogether( link_node* Node ){
	Node=Node->first;
link_node* Group=Node;
//	link_node* Group = BinSort( Node );
	//link_node* Group = Node ;
	link_node* temp		= NULL;
	link_node* Mod			= NULL;
	link_node* Compare	= NULL;
	link_node* This=NULL;
	

	Group=Group->first->end;
	int limit=0;
	bool brk=0;
	link_node* list=NULL;
	link_node* tmp = NULL;
	 link_node* found = NULL;
	while(1){
		if( Group->previous==NULL){
			break;
		}
		limit=Group->previous->data;
		//printf(" group %i \n", Group->data);
		int Variable=1;
		
		for( int Variable=1; Variable<=f_clause_size[Group->data]; Variable++){
			 found = NULL;
		if( ExistInSet( abs(f_variable_connections[ Group->data ] [ Variable]),list ) ==1) {continue;}
		list = link_append(abs(f_variable_connections[ Group->data ] [ Variable]),list);
		//	
		
			// for all previous, connect to the this point
				Compare=Group->previous;
				
				while(1){
				
					
					for( int Variable2=1; Variable2<=f_clause_size[Compare->data]; Variable2++){
					
						if( abs(f_variable_connections[ Compare->data ][ Variable2 ] ) == abs( f_variable_connections[ Group->data ][ Variable ] )){
							brk=1;

							break;
						}
					}
					
					
					if( Compare->previous==NULL)break;
					
					Compare=Compare->previous;
					
					if(brk==1){
						This=Compare->next;
						if( This->previous!=NULL){
							This->previous->next=This->next;
							This->next->previous = This->previous;
						}else{
							tmp = This->next;
								
							if(tmp->data == Group->data)  break;
								
							This->next->previous = NULL;
								
								
							while(tmp!=NULL){
								tmp->first= This->next;
								if(tmp->next==NULL) break;
								tmp=tmp->next;
							}
							This->end = tmp;
							This->next=NULL;
								
						}
							
						if(found==NULL) {
							found = This;
							found->first=found;
							found->end=found;
						}else{
							found->end->next 	= This;
							This->previous= found->end;
							found->end 			= This;
						}
						brk=0;
					}

				}
				
				if(found!=NULL){
					found=found->first;
					Group->previous->next= found->first;
					found->first->previous=Group->previous;
					Group->previous=found->end;

					found->end->next=Group;
					
					while(1){
						found->first= Group->first;
						if( found->data==Group->data) break;
						found=found->next;
					}
				}
				
			}
		if(Group->previous==NULL)break;
	
		Group=Group->previous;
	}
	//DeleteSet(&list);
	return Group;
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

	if(head == NULL) return;
		cursor= head->first;
		
		while(cursor!=NULL){
			tmp=cursor->next;
			free(cursor);
			cursor=tmp;
		
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

