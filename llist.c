#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "llist.h"
#include "stest.h"
#include "index.h"
#include "var_pos.h"
#include "map.h"
#include "m_map.h"
#include "coms.h"

link_node* link_create(int data, link_node* previous, link_node* next,com_line* Coms){
	//link_node* new_node = malloc(sizeof(*new_node));
	link_node* new_node	= alloc_mem(Coms->link_pool);
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

link_node* link_append(int data,link_node* head,com_line* Coms){
	link_node* new_node=link_create(data, head,NULL, Coms);
	if(head!=NULL ){
	
		head=head->first->end;
		
		new_node->first=head->first;
		new_node->first->end=new_node;
		new_node->previous=head;
		head->next=new_node;
	}else{
		new_node->first=new_node;
		new_node->end=new_node;
	}

return(new_node) ;
}


link_node* copy_list(link_node* list,com_line* Coms){

	link_node* new_list=NULL;

//	if(list==NULL){ printf("null list copy \n"); exit(0);}


	list=list->first;
	while(list!=NULL){
		if(list->data!=0){
			new_list=link_append(list->data, new_list,Coms);
		}
		if(list->next==NULL) break;
		list=list->next;
	}
	return new_list;

}

// add a new node to the list
static inline link_node* link_prepend(link_node* head,int data,com_line* Coms){
	
	link_node* new_node=link_create(data,head,NULL, Coms);
	//if(new_node->first==NULL){new_node->first = new_node;}else{new_node->first=head;}

	return new_node;
}

void pop_link(link_node** cursor,com_line* Coms){

if((*cursor)==NULL){printf(" null ptr- link_pop\n"); exit(0);}

(*cursor)=(*cursor)->first->end;
link_node* temp= (*cursor);
if((*cursor)->previous!=NULL){
	(*cursor)=(*cursor)->previous;
	(*cursor)->first->end=(*cursor);
	(*cursor)->next=NULL;
	release_mem((*cursor), Coms->link_pool);
	*cursor=NULL;
}else{
	if((*cursor)->next!=NULL){
		printf("not end\n");
		exit(0);
		temp=(*cursor)->next;
		temp->previous=NULL;
	}else{
		release_mem((*cursor), Coms->link_pool);
		(*cursor)=NULL;
	}
	
}

}



int RemoveMember(link_node** cursor, com_line* Coms){
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
		
	}else{
		temp=(*cursor);
		(*cursor)=(*cursor)->previous;
		
		(*cursor)->first->end=(*cursor);
		(*cursor)->next=NULL;
		
		
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
		//free(temp);
		release_mem(temp, Coms->link_pool);
		temp=NULL;
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
int TestRemoveMember(link_node** cursor, com_line* Coms){
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
		release_mem(temp, Coms->link_pool);
		temp=NULL;
	}else{
		temp=(*cursor);
		(*cursor)=(*cursor)->previous;
		
		(*cursor)->first->end=(*cursor);
		(*cursor)->next=NULL;
		
		//free(temp);
		release_mem(temp, Coms->link_pool);
		temp=NULL;
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
		//free(temp);
		release_mem(temp, Coms->link_pool);
		temp=NULL;

	}else{
		//(*cursor)->data=0;
		(*cursor)->next=NULL;
		(*cursor)->previous=NULL;
		//(*cursor)=NULL;
		//free(*cursor);
		release_mem(*cursor, Coms->link_pool);
		*cursor=NULL;
		return (-1);
	}

	
}
return(0);
}

int RemoveFromList(link_node** cursor, com_line* Coms){
	int ret=0;
	if((*cursor)==NULL){return -1;}

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
			
		}else{
			temp=(*cursor);
			(*cursor)=(*cursor)->previous;
			
			(*cursor)->first->end=(*cursor);
			(*cursor)->next=NULL;
			
			
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
			//free(temp);
			release_mem(temp, Coms->link_pool);
			temp=NULL;
		}else{
			(*cursor)->next=NULL;
			(*cursor)->previous=NULL;
			//(*cursor)=NULL;
			//free(*cursor);
			release_mem(*cursor, Coms->link_pool);
			*cursor=NULL;
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

link_node* BinSort(link_node** Node,com_line* Coms){

	if( (*Node) == NULL) exit(0);
	(*Node)=(*Node)->first;
	int size= FindLargestClause(*Node);
	
	variable_pos** SearchIndex=CreateIndex(size);
	
	
	//halt();
	link_node* tmp=NULL;
	
	while(1){
		//	printf("%li  size %i adr %p \n",(*Node)->data, Coms->f_clause_size[(*Node)->data], &(Coms->f_clause_size[(*Node)->data]) );
			SetIndex((*Node)->data,f_clause_size[(*Node)->data],SearchIndex);
		
		if((*Node)->next==NULL) {break;}
		
		tmp=(*Node);
		(*Node)=(*Node)->next;
		//free(tmp);
		release_mem(tmp, Coms->link_pool);
		tmp=NULL;
	}
	release_mem((*Node), Coms->link_pool);
	//free(*Node);
	*Node = NULL;
	
	for(int i=1;i<=size;i++){
		if(SearchIndex[i]->clause==0){
			continue;
		}

		variable_pos* iter = SearchIndex[i];
			
		while(1){
			if(iter->clause!=0){
				(*Node) = link_append( iter->clause, 	(*Node) , Coms);
			}
			if(iter->next==NULL) break;
			iter	 = iter->next;
			
		}
		
	}
	
	FreeSearch(SearchIndex,size);
	return (*Node);
}

link_node* GroupSingles( link_node* set ,com_line* Coms){

	link_node* tried = NULL; 

	 set=set->first;
	link_node* iter = NULL;
	link_node* temp = NULL;
	bool found = 0;
	link_node* this_temp = NULL;
	link_node* saved= NULL;
	iter= set->first;
	while(1){
	
		if( f_clause_size[ iter->data ] == 1 ){
			
			if( ExistInSet( abs(f_variable_connections[ iter->data ][ 1 ]) , tried) == 1){
				if( iter->next == NULL) break;
				iter=iter->next;
				continue;
			}
			
			//printf(" this single %i \n", f_variable_connections[ iter->data ][ 1 ]);
			
			tried = link_append( abs(f_variable_connections[ iter->data ][ 1 ]), tried,Coms);
			
			temp = iter->first;
			
			while(temp!=NULL){
			
				for( int variable = f_clause_size[ temp->data ]; variable != 0; variable--){
					if( temp==iter ){ 
						if(temp->next==NULL) break;
						temp=temp->next;
						continue;
					}
					
					if( abs( f_variable_connections[temp->data][variable] ) == abs(f_variable_connections[ iter->data ][ 1 ])  ){
						//printf( "found %i \n",f_variable_connections[temp->data][variable]);
						
						found=1;
						break;
					}
				}
				
				if( found==1){
					// Swap places
						saved=iter->next;
						MoveToPre(iter, temp);
						//SetFirst(temp);
						//printf(" single \n");
					
					break;
				}
				if(temp->next==NULL) break;
				temp=temp->next;
			}	
			
		}
	
		if (found ==1){
		found=0;
		if(saved==NULL) break;
		iter=saved;
		saved=NULL;
		continue;
		
		}
		
		if(iter->next==NULL) break;
		iter=iter->next;
	}

DeleteSet( &tried ,Coms);
	return set;
}

link_node* MoveToPre( link_node* source, link_node* drain){
	link_node* saved= source->next;
	link_node* temp = NULL;
	
	//remove
			
	int savedsize= ListSize(drain->first);
	//printf(" check the same %li %li ; %i %i \n", source->data,drain->data, savedsize,ListSize(drain->first));

	//if( drain->previous==source) return drain;
	//remove
	
	if( source->previous!=NULL){
	
		if(source->next!=NULL){
			source->previous->next=source->next;
			source->next->previous=source->previous;
		}else{
			source->previous->next=NULL;
			source->first->end=source;
		}
	}else{
		if (source->next!=NULL){
			source->next->previous=NULL;
			temp=source->next;
			temp->first=temp;
			//source=source->next;
			//SetFirst(source->next);
		}
	}
	source->next=NULL;
	source->previous=NULL;
	//move
	
	if( drain->previous!=NULL){
		//printf("w/prev %li \n",drain->data);
		drain->previous->next= source;
		source->previous=drain->previous;
		source->next=drain;
		drain->previous=source;
		
	}else{
	
		//printf(" no/prev\n");
		drain->previous=source;
		source->next=drain;
		source->first=source;
		
	}
	SetFirst(drain);
	if(savedsize!= ListSize(drain->first) ) {
		printf(" not the same %li %li ; %i %i \n", source->data,drain->data, savedsize,ListSize(drain->first));

		debug_list(source->first);
		exit(0);
	}
	return saved;
}


link_node* NearestConnecttion( link_node* set ){
if(set==NULL) return set;
	link_node* tried = NULL; 

	 set=set->first->end;
	 
	link_node* iter = NULL;
	link_node* temp = NULL;
	link_node* saved=NULL;
	
	link_node* TempFound=NULL;
	link_node* SavedFound=NULL;
	
	bool found = 0;
	saved=set;
	
	while(1){
	
			
		
		for( int variable = f_clause_size[ set->data ]; variable != 0; variable--){
		
			TempFound=NULL;
			TempFound=FindFVariableInRange( abs(f_variable_connections[set->data][variable]),  set->next);
			
			if( TempFound!=NULL && (SavedFound==NULL||SavedFound->data>TempFound->data) ){
				SavedFound=TempFound;
				found=1;
				break;
			}
	
		}
			
		if( found==1 ){
	
				saved=set->next;
				set=MoveToPre( set, SavedFound);
				//SetFirst(set);
			
			found=0;
		}
	
	
	if(set->previous==NULL) break;
	set=set->previous;
	
	}
	return set->first;
}




link_node* GroupSet( link_node* Node, com_line* Coms ){

	Node							= Node->first;
	link_node* temp			= NULL;
	link_node* Mod				= NULL;
	link_node* Compare		= NULL;
	link_node* This			= NULL;
	link_node* list			= NULL;
	link_node* tmp				= NULL;
	link_node* found			= NULL;
	link_node* appended		= NULL;
	link_node* OrderedList	= NULL;
	link_node* copy			= NULL;
	link_node* Group 			= Node;
	Group							= Group->first->end;
	link_node* saved			= Group;
	
	int limit					= 0;
	bool brk						= 0;
	int count 					= 0;
	// Start from the end of the group, and work backwards
	while(1){
	
		if( Group->previous==NULL){
			break;
		}
		
		limit = Group->previous->data;

		int Variable = 1;
		
		link_node** GroupSet	= CreateSet( f_clause_size[Group->data], Coms );
		

		//ThisFunction searches all nodes before and collects those clauses
		CollectVariables(Group, GroupSet, &list,Coms);
			
			int count=0;
			int var =0 ;
			
			// select the variable with the most connections in total
			while(1){

				for( int v = f_clause_size[Group->data]; v!=0; v--){
					if( ExistInSet( v, appended ) == 1 ) continue;
					
					if( ListSize(GroupSet[v]) <= count && count!=0 || count ==0){
						count = ListSize(GroupSet[v]);
						var=v;
					}
				}
				//this is the escape of the search
				if(count==0){
					break;
				}
				
				appended = link_append( var, appended,Coms);
				
				count=0;

				//append the variables here
				if(var==0){
					break;
				}
				
				//RemoveSubset(&GroupSet[var], OrderedList, GroupSet, var);
				
				//printf(" %i :", var);
				
				copy=copy_list(GroupSet[var],Coms);
				
				if(copy==NULL)continue;
				
				if( copy->data==0){
					DeleteSet(&copy,Coms);
					copy=NULL;
					continue;
				}
				
				OrderedList=JoinSet( copy, OrderedList );
				RemoveDuplicateMembers( &OrderedList,Coms);
				copy=NULL;
				//printf("1 list size %i %i \n", ListSize(OrderedList),ListSize(Group));
				var=0;
			}
			
			FreeSet(GroupSet,f_clause_size[Group->data],Coms);
			//DeleteSet(GroupSet, Coms);
			GroupSet=NULL;
			
			//SetFirst(OrderedList);
			

			//if empty list, continue through the groups
			if(OrderedList==NULL){
				if( Group->previous==NULL)break;
				Group=Group->previous;
				continue;
			}
			
			
			;
			BinSort( &OrderedList, Coms);
			//SetFirst(OrderedList);
			
			RemoveAfromB(OrderedList, &Group, Coms);
			if(saved->first->previous==NULL){
			
				saved->first->previous=OrderedList->first->end;
				OrderedList->first->end->next=saved->first;
				//saved->first->previous->next=saved;
				saved->first=OrderedList->first;
				OrderedList->first->previous= NULL;
			//
			}else{
			
				saved->previous->next=OrderedList->first;
				OrderedList->first->previous=saved->previous;
				saved->previous=OrderedList->first->end;
				OrderedList->first->end->next=saved;
				saved->first= OrderedList->first;
			}
			SetFirst(saved);
			OrderedList=NULL;
			
			//halt();
			//
			//inserts selected group in to the end of the group
			
			
			
		if( saved->previous==NULL) break;
	
		saved=saved->previous;
		Group=saved;
	}
	
	DeleteSet(&appended,Coms);
	//appended=NULL;
	DeleteSet(&list,Coms);
//	list=NULL;
//	CheckFirstNode(OrderedList);
	return saved;
}

link_node* FindNode(int data, link_node** Node ){

	(*Node)=(*Node)->first;
	while(1){
		if((*Node)->data==data) return *Node;
		if((*Node)->next==NULL) break;
		(*Node)=(*Node)->next;
	}
	
	
	return NULL;

}

//For this node, search all nodes before for those that contain this variable
link_node* CollectVariables( link_node* Group, link_node* GroupSet[], link_node** list, com_line* Coms){
	link_node* Compare= NULL;

	for( int Variable=f_clause_size[Group->data]; Variable!=0; Variable-- ){

		if( ExistInSet( abs(f_variable_connections[ Group->data ] [ Variable]),(*list) ) ==1) {
			
			continue;
		}
			
		*list = link_append(abs(f_variable_connections[ Group->data ] [ Variable]),(*list),Coms);

		// for all previous, connect to the this point
		Compare=Group->previous;
				
		while(1){
				
			for( int Variable2=f_clause_size[Compare->data]; Variable2 !=0 ; Variable2--){
					
				if( abs(f_variable_connections[ Compare->data ][ Variable2 ] ) == abs( f_variable_connections[ Group->data ][ Variable ] )){
							
					//if it's the start
					if( GroupSet[Variable]->data == 0){
						GroupSet[Variable]->data = Compare->data;
					}else{
						link_append( Compare->data, GroupSet[Variable], Coms);
					}
							
				}
						
			}
					
			if( Compare->previous == NULL) break;
			
			Compare=Compare->previous;
		}

	}
}

link_node* RemoveAfromB(link_node* SetA, link_node** SetB,com_line* Coms){

	if(SetA==NULL){
		if(*SetB==NULL){
			printf(" rem a-b\n");
			exit(0);
		}
		return *SetB;
	}
	if( *SetB ==NULL) {
		printf(" rem from ab null b\n");
		return *SetB;
	}
	SetA=SetA->first;
	while(1){

		if(ExistInSet( SetA->data, *SetB)==1){
			
			 FindNode(SetA->data, SetB);
			 
			 TestRemoveMember(SetB, Coms);
		}

		if(SetA->next==NULL) break;

		SetA=SetA->next;


	}
return *SetB;
}

// If this exists in that, remove this.
link_node* RemoveSubset(link_node**  Remove, link_node* set, link_node** table, int var,com_line* Coms){
if(set==NULL) return NULL;;
set=set->first;
	while(1){

		if(ExistInSet( set->data, *Remove)==1){
			
			RemoveInSet( set->data, *Remove,table,var,Coms);
		}

		if(set->next==NULL) break;

		set=set->next;


	}

}

link_node* GroupTogether( link_node* Node,com_line* Coms){
	Node=Node->first;

	link_node* temp		= NULL;
	link_node* Mod			= NULL;
	link_node* Compare	= NULL;
	link_node* This=NULL;
	link_node* list=NULL;
	link_node* tmp = NULL;
	link_node* found = NULL;
	
	link_node* Group=Node;
	
	Group=Group->first->end;
	
	int limit=0;
	
	bool brk=0;
	

	while(1){
	
		if( Group->previous==NULL){
			break;
		}
		
		limit=Group->previous->data;

		int Variable=1;
		
		for( int Variable=1; Variable<=f_clause_size[Group->data]; Variable++){

			found = NULL;

			if( ExistInSet( abs(f_variable_connections[ Group->data ] [ Variable]),list ) ==1) {continue;}
			list = link_append(abs(f_variable_connections[ Group->data ] [ Variable]),list, Coms);

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
	DeleteSet(&list,Coms);
	return Group;
}



void DeleteList(link_node** cursor,com_line* Coms){
	if(cursor==NULL) return;
	(*cursor)=(*cursor)->first->end;
	while((*cursor)!=NULL){
		if((*cursor)->previous!=NULL){ break;}
		(*cursor)=(*cursor)->previous;
		//free((*cursor)->next);
		release_mem((*cursor)->next, Coms->link_pool);
		(*cursor)->next=NULL;
	}
	//free((*cursor));
	release_mem(*cursor, Coms->link_pool);
	*cursor=NULL;
}

link_node* DeleteNode(link_node* cursor, com_line* Coms){
	int ret=0;
if((cursor)==NULL){printf(" null ptr- link_pop\n"); exit(0);}

link_node* temp;
link_node* next;
link_node* previous;
		
if((cursor)->previous!=NULL){
	
	if((cursor)->next!=NULL){
		temp=cursor->previous;
		cursor->next->previous = (cursor)->previous;
		cursor->previous->next = (cursor)->next;
		
		release_mem(cursor, Coms->link_pool);
		cursor=NULL;
		return(temp);
	}else{
		temp=(cursor)->previous;
		(cursor)->previous->next=NULL;
		
		(cursor)->first->end=temp;
		
		release_mem(cursor, Coms->link_pool);
		cursor=NULL;
		return (temp);
	}
	
}else{

	if((cursor)->next!=NULL){
		temp=(cursor)->next;
		(cursor)->next->previous=NULL;
		
		temp->first=temp;
		temp->previous=NULL;
		
		while(1){
			(temp)->first=cursor->next;
			if((temp)->next==NULL)break;
			(temp)=(temp)->next;
		}
		temp->first->end=(temp);
		release_mem(cursor, Coms->link_pool);
		cursor=NULL;
		return (temp);

	}else{
		(cursor)->data=0;
		(cursor)->next=NULL;
		(cursor)->previous=NULL;
		(cursor)=NULL;
		//free(cursor);
		return NULL;
	}
	
	}
}

link_node* AddLists(link_node* OpA, link_node* OpB,com_line* Coms){

	if( OpA==NULL || OpB==NULL ){ printf(" no valid table\n"); exit(0); }

	link_node* list;

	OpA = OpA->first;
	OpB = OpB->first;

	while(OpA!=NULL){
		list=link_append(OpA->data,list, Coms);
		OpA=OpA->next;
	}
	while(OpB!=NULL){
		list=link_append(OpB->data,list, Coms);
		OpB=OpB->next;
	}
	
	return list;

}


void link_dispose(link_node *head, com_line* Coms)
{
	link_node *cursor, *tmp;

	if(head == NULL) return;
		cursor= head->first;
		
		while(cursor!=NULL){
			tmp=cursor->next;
			release_mem(cursor, Coms->link_pool);
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

printf("\n data %li \n ", head->data);
scanf("%c", &input);
if(getchar()!=0){
//printf("boop");
}

	switch(input){
		
		case 'a':
			if(head->previous==NULL){ printf("no previous\n");
			}
			else{ head=head->previous; 
printf("previous \n data %li  \n ", head->data);

			}
		break;
		case 'd':
			if(head->next==NULL){ 
printf("no next\n");
			}
			else{ head=head->next; 
printf("next \n data %li  \n", head->data);

			}
		break;
		case 'x':
			return;
		break;
		}
}
printf(" output: %c \n", input);
}

