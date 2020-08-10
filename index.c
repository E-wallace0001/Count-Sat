
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#include "stest.h"
#include "llist.h"
#include "index.h"
#include "var_pos.h"
#include "clause_func.h"
#include "map.h"

#include "bfs_s.h"
#include "m_map.h"

#include "h_table.h"


// 45.344

variable_pos** CreateIndex( int size ){

	variable_pos** index;

	index=(variable_pos **)calloc( size, sizeof(variable_pos) );
	
	for(int i = 1; i <= size+1;i++){
	
		index[i] = malloc(sizeof(variable_pos));
		
		index[i]->previous = NULL;
		index[i]->next 	 = NULL;
		index[i]->first 	 = index[i];
		index[i]->end 		 = index[i];
		index[i]->clause	 = 0;
	}

	return index;
}

link_node* RemoveKnownVariable( link_node* set, com_line* Coms){

set=set->first;
bool remove=0;

while(set!=NULL){

	for(int variable = f_clause_size[set->data]; variable!=0; variable--){

		if( IsVariableSet[ abs( f_variable_connections[set->data][variable]) ] == 1){
			if( f_variable_connections[set->data][variable] > 0 
			&& VariableSet[ abs( f_variable_connections[set->data][variable]) ] == 1 
			||  f_variable_connections[set->data][variable] < 0 
			&& VariableSet[ abs( f_variable_connections[set->data][variable]) ] == 0	){
				remove=1;
				break;
			}
		}

	}

	if(remove == 1){
		printf("removedzn\n\n\n");
		if( RemoveFromList(&set, Coms) == -1) {
			set=NULL;
			break;
		}
		remove=0;
		continue;
	}
	if(set->next!=NULL){
		set=set->next;
	}else{
		break;
	}
}

return(set);

}



void RemoveClauseFromIndex(int clause, variable_pos* index[], com_line* Coms ){


	for ( int variable=f_clause_size[clause]; variable!=0; variable--){
		variable_pos* set = NULL;
		
		set = index[ abs(f_variable_connections[clause][variable]) ];
		
		while(1){
			if( set->clause == clause ){
			
				if( set->next == NULL && set->previous == NULL ){
				
					set->first 	= set;
					set->end 	= set;
					set->clause	= 0;
					
					break;
				}
				
				
				pop_clause(&set,Coms);
				break;
			}
			
			if( set->next != NULL ){
			
				set=set->next;
				
			}else{
			
				break;
			}
		}		
		
		f_variable_connections[clause][variable]=0;
	}
	f_clause_size[clause]=0;
	
}


void DeleteIndex(variable_pos* index[],int VariableCount){

	variable_pos* ptr;

	for (int i=1; i<=VariableCount+1;i++){
		ptr= index[i]->first->end;
		while(1){
			if(ptr->previous==NULL){
				free(ptr);
				break;
			}
		ptr=ptr->previous;
		free(ptr->next);
		}
		
	}
	free(index);

}



void SetIndex(int address, int data, variable_pos* position[]){

	if( data ==0 || address ==0) printf(" address %i data %i\n", address, data);

	int AbsData=abs(data);
	
	if(position[AbsData]->clause==0){

		//create the position of this variable if it hasn't been create
		position[AbsData]->clause		= address;
		position[AbsData]->end 			= position[AbsData];
		position[AbsData]->first 		= position[AbsData];
		position[AbsData]->first->end	= position[AbsData];
		position[AbsData]->next 		= NULL;
		position[AbsData]->previous 	= NULL;
	}else{
		// Add the position of this variable at the end of list of craeted variables
		append_variable(address,position[AbsData]);
	}
}


variable_pos* SetNewIndex(link_node* list,variable_pos** set,int translation[], map* Map, map** Location, com_line* Coms){
	
	list=list->first;
	
		while(list!=NULL){
			PutMap( list->data, Location );
			(*set)	= copy_clause(list->data,translation,Coms);
			list=list->next;
		}
		
	//return *set;
}



link_node* FindMembers( int data, variable_pos* set[], link_node* subset,int translate[] , map* Map,com_line* Coms){

	if( set == NULL ){ printf("FindMembers- NULL Data \n"); exit(0); }
	if( set[data] == 0 ){  return NULL; }

	variable_pos* ptr;
	
	ptr = set[data]->first;
	
	//if( ptr->clause ==0){ PutMap( abs(data), &Map );}
	
	while( ptr != NULL ){
		subset = link_append(translate[ptr->clause], subset,Coms);
		ptr	 = ptr->next;
	}
	return subset;
}

int CountUniqueVariables(link_node* group, com_line* Coms){

int count=0;
group=group->first;

hash_t* h_table = hasht_create(267);

while(group!=NULL){

	for(int variable=f_clause_size[group->data]; variable!=0;variable--){
	
	if( check_table( abs(f_variable_connections[ group->data ] [ variable]) , h_table) ==1) {continue;}

	table_add( abs(f_variable_connections[ group->data ] [ variable]),  h_table);
	count++;
	
	}
group=group->next;
}

hash_t_destroy( &h_table );
return (count);
}


int ExistInSet(int data, link_node* list){
if(list==NULL) return 0;
list=list->first;
	while(1){
	
		if(list->data==data) return 1;
		if(list->next==NULL) break;
		list=list->next;
	}
return 0;
}

link_node* RemoveInSet(int data, link_node* list, link_node** table, int var,com_line* Coms){
list=list->first;
link_node* temp=NULL;
	while(1){
	
		if(list->data==data){
			if(list->previous==NULL){
			
				if(list->next!=NULL){ 
					temp->next->previous=NULL;
					temp=list;
					while(1){
					
						temp->first=list->next;
						
						if(temp->next!=NULL) break;
						temp=temp->next;
					}
					temp->first->end=temp;
					
					table[var]=list->next;
					list=DeleteNode(list,Coms);
					return NULL;
				}else{
					list->data=0;
					return NULL;
				}
			}else{
			DeleteNode(list,Coms);
			return NULL;
			}
			
		}

		if( list->next == NULL) break;
		list = list->next;
	}
	
}

link_node* RemoveDuplicateMembers( link_node** set,com_line* Coms){
	if((*set)==NULL){printf(" this is an empty set \n"); return NULL;}
	link_node* node = (*set)->first;
	
	link_node* checked=NULL;
	link_node* next;
	
	hash_t* h_table = hasht_create(257);
	
	while(1){

		if( check_table( abs(node->data ),  h_table) == 1){	
		
			next = node->next;
			node = DeleteNode(node,Coms); 
			
			if(next == NULL)
				break;
			node = next;
			next = NULL;
			continue;
		}
		
		table_add( abs(node->data),  h_table);

		if(node->next==NULL)
			break;

		node=node->next;
	}
	hash_t_destroy( &h_table );
	return node;
}

link_node* RemoveLastMember( link_node** set, com_line* Coms){
	(*set)=(*set)->first->end;
	RemoveMember(set, Coms);
	
	return (*set);
}

link_node* RemoveThisMember(int data, link_node** set, com_line* Coms){
	(*set)=(*set)->first;
	while( (*set) != NULL ){
		if( (*set)->data == data ){ (*set)=DeleteNode(*set, Coms); }
		if( (*set)->next ==NULL) break;
		(*set) = (*set)->next;
	}
	return (*set);

}

link_node* AddMember( int data, link_node* subset,com_line* Coms){

	
	subset = link_append(data, subset,Coms);
	return subset;
}

link_node* CopySet(int data, variable_pos* SetA[], link_node* SetB,com_line* Coms){

	variable_pos* temp = SetA[ abs(data) ];

	while(1){
	//	printf(" coppy : data %i - temp->clause %i \n",data, temp->clause);
		SetB=link_append( temp->clause, SetB, Coms);
		if(temp->next==NULL) break;
		temp=temp->next;
	}
	
	return SetB->first;;
}

void DeleteSet(link_node** set, com_line* Coms){
	if( (*set)==NULL) return;

	(*set)=(*set)->first->end;
	link_node* tmp=NULL;
	while(1){
		if((*set)->previous==NULL){
			release_mem((*set), Coms->link_pool);
			(*set)=NULL;
			return;
		}
		tmp=(*set);
		(*set)=(*set)->previous;
		release_mem(tmp, Coms->link_pool);
		tmp=NULL;
	}

}


bool CheckSet( int data, link_node* set){
	set=set->first;
	while(set!=NULL){
		if(set->data==data){return 1;}
		
		set=set->next;
	}
	return 0;
}

link_node* AddKnownVariables(link_node* set, com_line* Coms){

	link_node* iter_set=set->first;

	while( iter_set != NULL ){
	
		for ( int clause= f_clause_size[iter_set->data]; clause!=0; clause-- ) {
				
			if( IsVariableSet[ abs(f_variable_connections[iter_set->data ][clause]) ] ==1 ){
						
				if( CheckSet( OnesPlace[abs(f_variable_connections[iter_set->data ][clause])] , set ) ==0){
					set = link_append(	OnesPlace[abs(f_variable_connections[iter_set->data ][clause])] ,  set, Coms);
				}
			}

		}

		iter_set = iter_set->next;
	}
return set;			
			
}


link_node* JoinSet( link_node* SetB, link_node* SetA){

	if( SetB == NULL ){ printf(" join set: NullSet\n"); exit(0);}
	if( SetB ->data ==0 ) return SetA;
	if( SetA == NULL  ){ return SetB; }

	SetA=SetA->first->end;
	SetB= SetB->first;
	SetA->next=SetB;
	SetB->previous=SetA;
	
	while(1){
		SetB->first=SetA->first;
		if(SetB->next==NULL) break;
		SetB=SetB->next;	
	}
	
	SetB->first->end=SetB;
	
	return(SetA->first);
}

void CreateGroup(link_node* list,variable_pos** group, map** MapLocation, map** Map,com_line* Coms){

	variable_pos* 	iter;
	
	list  = list->first;
	
	link_node* NewVar=NULL;
	
	int index = 0;
	int count = 0;
	while( list != NULL ){

 	PutMap( list->data , MapLocation );
		
		for(int variable=f_clause_size[list->data];variable!=0;variable--){ 
			if( ExistInSet( abs(f_variable_connections[ list->data ] [ variable]),NewVar ) ==0 ){
				NewVar=link_append(	abs(f_variable_connections[ list->data ] [ variable]), NewVar,Coms);
								
				 PutMap( abs(f_variable_connections[ list->data ] [ variable]), Map );
			}
			
			SetIndex( index,  MapGetValue( abs(f_variable_connections[list->data][variable]), (*Map) ), group );
			
		}
		list = list->next;
	}
	DeleteSet(&NewVar,Coms);

}

void RemoveGroup(variable_pos* group[],int size){
	variable_pos* ptr;
	
	for(int i=1; i<CountSet(group);i++){
		ptr = group[i]->first->end;
		
		while(1){
			ptr = ptr->previous;
			
			free( ptr->next );
			
			if( ptr->previous == NULL ) break;
		}
	}
	free(group);
	
}

int CountSet(variable_pos** set){
	int count=0;
	while(set!=NULL){
		count++;
		set++;
	}
	
	return count;
}

link_node* AddConnections(link_node* list, bool Searched[],com_line* Coms){
	bool VariableVisited[vsize]={0};
	list=list->first;
	variable_pos* temp;
	link_node* NewList=NULL;
	
	while( 1 ){
	
		for( int variable=f_clause_size[list->data]; variable!=0; variable--){
		
			if( VariableVisited[ abs( f_variable_connections[list->data][variable] ) ] == 1  ) continue;
			
			Searched[ abs( f_variable_connections[list->data][variable] ) ]=1;

			VariableVisited[ abs( f_variable_connections[list->data][variable] ) ] = 1;
			temp = f_variable_position[ abs(f_variable_connections[list->data][variable]) ];
			
			while(temp!=NULL){
				NewList=link_append(temp->clause,NewList,Coms);
				temp=temp->next;
			}
		}
		if(list->next==NULL) break;
		
		list=list->next;
	}
	JoinSet( NewList, list);
return list;

}


link_node* CollectConnections ( link_node* list,com_line* Coms){

	if(list==NULL) exit(0);

	list								= list->first;
	variable_pos* temp			= NULL;
	link_node* NewList			= NULL;
	
	hash_t* h_table = hasht_create(257);


	//	printf(" this is the collectcollection %i  \n",ListSize ( list));	
	while( 1 ){
	
		for( int variable = f_clause_size[list->data]; variable!=0; variable--){
		
			if( check_table( abs(f_variable_connections[list->data][variable]), h_table) == 1  )
				 continue;
			table_add( abs( f_variable_connections[list->data][variable] ),  h_table);
			//VariableVisited	= link_append(	abs(f_variable_connections[ list->data ] [ variable]), VariableVisited,Coms);
			temp 					= f_variable_position[ abs(f_variable_connections[list->data][variable]) ];
			
			while(1){
				NewList = link_append(temp->clause,NewList,Coms);
				if(temp->next == NULL) 
					break;
				temp = temp->next;
			}
		}
		if(list->next == NULL) break;
		
		list = list->next;
	}

hash_t_destroy( &h_table );

return NewList;
}

link_node* ConnectedVariables ( int variable, com_line* Coms){

	link_node* VariableVisited	= NULL;
	variable_pos* temp			= f_variable_position[ abs(variable) ];
	
	
	hash_t* h_table = hasht_create(267);



	
			
	while(1){
		for( int variable2 = f_clause_size[temp->clause]; variable2!=0; variable2--){
			if(  check_table(  abs( f_variable_connections[temp->clause][variable2] ),  h_table) == 1  )
				continue;

			table_add( abs( f_variable_connections[temp->clause][variable2] ),  h_table);
			VariableVisited = link_append( abs( f_variable_connections[temp->clause][variable2] ), VariableVisited,Coms);
		}
		if(temp->next == NULL) 
			break;
		temp = temp->next;
	}
hash_t_destroy( &h_table );
return VariableVisited;
}

link_node** CreateSet( int size, com_line* Coms){

	link_node** Set = ( link_node**)calloc(size, sizeof(link_node) );
	
	for(int i = 0; i <= size;i++){
	
		Set[i] = alloc_mem(Coms->link_pool);
		//Set[i] =(link_node*) malloc(sizeof(link_node));
		Set[i]->previous= NULL;
		Set[i]->next 	 = NULL;
		Set[i]->first 	 = Set[i];
		Set[i]->end 	 = Set[i];
		Set[i]->data	 = 0;
	}
	
	return(Set);

}

void FreeSet( link_node** set, int size,com_line* Coms){
link_node* list= NULL;
link_node* tmp=NULL;
	for(int i = 0; i <= size;i++){
		if(set[i]==NULL) continue;
		list=set[i];
		list=list->end;
		while(1){
			if( list->previous==NULL){
				release_mem(list, Coms->link_pool);
				list=NULL;
				break;
			}
			tmp=list;
			list=list->previous;	
			//free( tmp);
			release_mem(tmp, Coms->link_pool);
			tmp=NULL;
		}
		
	}

	free(set);
}	

int* CreateArray( int size){

	int* array=(int *)calloc( size, sizeof(int) );
	return array;
}

void DestroyArray(int* array,com_line* Coms){
	free(array);
}

void DestroySet(link_node* subset, com_line* Coms){
if(subset==NULL) return;

subset=subset->first->end;
	while(1){
			
		if(subset->previous==NULL){ break;}

		subset=subset->previous;
		//free(subset->next);
		release_mem(subset->next, Coms->link_pool);
		subset->next=NULL;
	}
	//free (subset);
	release_mem(subset, Coms->link_pool);
	subset=NULL;
}

void ResetSolve(com_line* Coms){
	 Coms->set = Coms->set->first->end;
	while( 1 ){

		RemoveFromClause( Coms->set->clause,Coms);
		
		if( Coms->set==NULL ) break;
		
	}
}


link_node* TestRest( link_node* list,com_line* Coms){

if(list==NULL) return NULL;
	link_node* tmp=NULL;
	list = list->first;
	
	while(1){

		if(list->previous==NULL){release_mem(list, Coms->link_pool);list=NULL;break;}
		tmp = list;
		list = list->previous;
		
		//free(tmp);
		release_mem(tmp, Coms->link_pool);
		tmp=NULL;
	}
	
	list->next=NULL;
	list->first->end= list;
	
	
return list;
}



link_node* RemoveUntil(int End, link_node** list, com_line* Coms){

	if((*list)==NULL) return NULL;
	
	link_node* this 	= NULL;
	link_node* tmp		=NULL;
	
	(*list) 				= (*list)->first->end;
	
	if((*list)->data==End) return (*list);

	while(1){

		if((*list)->previous==NULL){break;}
		if((*list)->data==End)break;
		tmp=(*list);
		(*list) = (*list)->previous;
		(*list)->next=NULL;
		//free(tmp);
		release_mem(tmp, Coms->link_pool);
		tmp=NULL;
		
	}
	(*list)->next=NULL;
	(*list)->first->end= (*list);

	

	
return (*list);
}

void SetFirst( link_node* head){
if( head==NULL) return;
link_node* first=NULL;

while(1){
	if(head->previous==NULL) break;
	
	head=head->previous;
	
}
first=head;
while(1){
head->first=first;
if(head->next==NULL) break;
head=head->next;
}
head->first->end=head;
}

void CheckFirstNode( link_node* head){
if( head==NULL) return;
link_node* first=NULL;

while(1){
	if(head->previous==NULL) break;
	
	head=head->previous;
	
}
first=head;
while(1){

if( head->first !=first) exit(0);
if(head->next==NULL) break;
head=head->next;
}
head->first->end=head;
}


void FreeSearch(variable_pos* index[], int size){
	variable_pos* ptr;
	for(int i=1; i<=size+1;i++){
		//if(index[i]->clause==0){continue;}
		ptr = index[i]->first->end;

		
		while(1){
			if( ptr->previous == NULL ){free(ptr); break;}
			ptr = ptr->previous;
			
			free(ptr->next);
			
			
		}
	}
	free(index);
}


