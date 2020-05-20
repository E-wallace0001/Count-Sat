
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#include "stest.h"
#include "llist.h"
#include "var_pos.h"
#include "clause_func.h"
#include "map.h"

#include "bfs_s.h"



// 45.344




#include "index.h"

variable_pos** CreateIndex( int size ){

	variable_pos** index;

	index=(variable_pos **)calloc( size, sizeof(variable_pos) );
	
	for(int i=1; i<=size;i++){
		index[i]				=malloc(sizeof(variable_pos));
		
		index[i]->previous=NULL;
		index[i]->next=NULL;
		index[i]->first=index[i];
		index[i]->end=index[i];
		index[i]->clause	=0;
	}

	return index;
}

void DeleteIndex(variable_pos* index[],int VariableCount){

	variable_pos* ptr;

	for (int i=1; i<VariableCount;i++){
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

variable_pos* SetIndex_LH(link_node* list,variable_pos** set,int translation[], map* Map){
	int size=75;
	variable_pos** SearchIndex=CreateIndex(size);
	variable_pos* 	iter;
	
	list=list->first;
	
	//halt();
		while(list!=NULL){
		
if(f_clause_size[list->data]>75){printf(" greater than 75 \n");exit(0);}
			SetIndex(list->data,f_clause_size[list->data],SearchIndex);

			list=list->next;
		}

		for(int i=1;i<75;i++){
			if(SearchIndex[i]->clause==0){
				continue;
			}

			iter = SearchIndex[i];
			
			while(iter!=NULL){
				(*set)	= copy_clause(iter->clause,(*set),translation);
				iter		= iter->next;
			}
		}
	FreeSearch(SearchIndex);
	return *set;
}

void unindex( variable_pos* set[]){


}

void FreeSet(variable_pos* index[]){
	variable_pos* ptr;

	ptr=  *index;
		
		while(ptr!=NULL){
			//if( ptr->previous == NULL ){ptr->clause=0;ptr->next=NULL;ptr->first=ptr;ptr->end=ptr; break;}
			ptr = ptr->previous;
			
			free(ptr->next);
			
			
		}
	
}

void FreeSearch(variable_pos* index[]){
	variable_pos* ptr;

	for(int i=1; i<50;i++){
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

link_node* FindMembers( int data, variable_pos* set[], link_node* subset,int translate[] , map* Map){

	if( set == NULL ){ printf("FindMembers- NULL Data \n"); exit(0); }
	if( set[data] == 0 ){  return NULL; }

	
	variable_pos* ptr;
	
	ptr = set[data]->first;
	
	
	//if( ptr->clause ==0){ PutMap( abs(data), &Map );}
	
	while( ptr != NULL ){
		subset = link_append(translate[ptr->clause], subset);
		ptr	 = ptr->next;
	}
	return subset;
}

int CountUniqueVariables(link_node* group){

link_node* list=NULL;
int count=0;
group=group->first;
while(group!=NULL){

	for(int variable=f_clause_size[group->data]; variable!=0;variable--){
	if( ExistInSet( abs(f_variable_connections[ group->data ] [ variable]),list ) ==1) {continue;}

	list=link_append(	abs(f_variable_connections[ group->data ] [ variable]), list);
	count++;
	
	}
group=group->next;
}

DeleteSet(&list);
return (count);
}


int ExistInSet(int data, link_node* list){
if(list==NULL) return 0;
list=list->first;
	while(list!=NULL){
		if(list->data==data) return 1;
		list=list->next;
	}
return 0;
}

link_node* RemoveDuplicateMembers( link_node** set){
	if((*set)==NULL) exit(0);
	link_node* node = (*set)->first;
	
	
	link_node* checked=NULL;
	

	
	
	while(node!=NULL){

		if(ExistInSet( abs( node->data), checked)==1){	node=DeleteNode(node); 
		if(node->next==NULL) break;
		node=node->next;
		continue;
		
		 }
		
			checked=link_append(	abs(node->data), checked);
		if(node->next==NULL)break;
		node=node->next;
	}
	DeleteSet(&checked);
	return node;
}

link_node* RemoveLastMember( link_node** set){
	(*set)=(*set)->first->end;
	RemoveMember(set);
	
	return (*set);
}

link_node* RemoveThisMember(int data, link_node** set){
	(*set)=(*set)->first;
	while( (*set) != NULL ){
		if( (*set)->data == data ){ (*set)=DeleteNode(*set); }
		if( (*set)->next ==NULL) break;
		(*set) = (*set)->next;
	}
	return (*set);

}

link_node* AddMember( int data, link_node* subset ){

	
	subset = link_append(data, subset);
	return subset;
}

link_node* CopySet(int data, variable_pos* SetA[], link_node* SetB){
variable_pos* temp = SetA[ abs(data) ];

	while(temp!=NULL){
		SetB=link_append( temp->clause, SetB);
		
		temp=temp->next;
	}
return SetB->first;;
}

void DeleteSet(link_node** set){
	(*set)=(*set)->first->end;
	while((*set)!=NULL){
		pop_link(set);
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

link_node* AddKnownVariables(link_node* set){
link_node* iter_set=set->first;
while( iter_set != NULL ){
				//CountedSet  = 0;
				//CheckCount	= 0;
				for ( int clause= f_clause_size[iter_set->data]; clause!=0; clause-- ) {
				
					if( IsVariableSet[ abs(f_variable_connections[iter_set->data ][clause]) ] ==1 ){
						
						if( CheckSet( OnesPlace[abs(f_variable_connections[iter_set->data ][clause])] , set ) ==0){
							//CountedSet++;
							set = AddMember(	OnesPlace[abs(f_variable_connections[iter_set->data ][clause])] ,  set );
						}
					}
				}

				iter_set = iter_set->next;
			}
			
}

link_node* JoinSet( link_node* SetB, link_node* SetA){

	if( SetB == NULL ){ printf(" join set: NullSet\n"); exit(0);}

	if( SetA == NULL ){ return SetB; }

	SetB=SetB->first->end;
	SetA= SetA->first;
	SetB->next=SetA;
	SetA->previous=SetB;
	
	while(1){
		SetA->first=SetB->first;
		if(SetA->next==NULL) break;
		SetA=SetA->next;	
	}
	
	SetA->first->end=SetA;
	
	return(SetA);
}

void CreateGroup(link_node* list,variable_pos** group, int translate[], map** Map){

	variable_pos* 	iter;
	
	list  = list->first;
	
	link_node* NewVar=NULL;
	
	int index = 0;
	int count = 0;
	while( list != NULL ){

		index++;
		translate[index]=list->data;
		
		for(int variable=f_clause_size[list->data];variable!=0;variable--){ 
			if( ExistInSet( abs(f_variable_connections[ list->data ] [ variable]),NewVar ) ==0){
				NewVar=link_append(	abs(f_variable_connections[ list->data ] [ variable]), NewVar);
								
				 PutMap( abs(f_variable_connections[ list->data ] [ variable]), Map );
			}
			
			SetIndex( index,  MapGetValue( abs(f_variable_connections[list->data][variable]), (*Map) ), group );
			
		}
		list = list->next;
	}
	DeleteSet(&NewVar);

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

link_node* AddConnections(link_node* list, bool Searched[]){
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
				NewList=link_append(temp->clause,NewList);
				temp=temp->next;
			}
		}
		if(list->next==NULL) break;
		
		list=list->next;
	}
	JoinSet( NewList, list);
return list;

}


link_node* CollectConnections ( link_node* list){

	link_node* VariableVisited=NULL;

	list=list->first;
	variable_pos* temp;
	link_node* NewList=NULL;
	
	while( 1 ){
	
		for( int variable=f_clause_size[list->data]; variable!=0; variable--){
		
			if( ExistInSet( abs( f_variable_connections[list->data][variable] ), VariableVisited) == 1  ) continue;
			VariableVisited=link_append(	abs(f_variable_connections[ list->data ] [ variable]), VariableVisited);

			temp = f_variable_position[ abs(f_variable_connections[list->data][variable]) ];
			
			while(temp!=NULL){
				NewList=link_append(temp->clause,NewList);
				temp=temp->next;
			}
		}
		if(list->next==NULL) break;
		
		list=list->next;
	}
	DeleteSet(&VariableVisited);
	//JoinSet( NewList, list);
return NewList;
}

int* CreateArray( int size){

	int* array=(int *)calloc( size, sizeof(int) );
	return array;
}

void DestroyArray(int array[]){
	free(array);
}

void ResetSolve(){
	(set) = (set)->first->end;
	while( (set) != NULL ){
		RemoveFromClause( clause_count, &set );
	}
}







