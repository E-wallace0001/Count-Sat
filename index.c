
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

link_node* RemoveKnownVariable( link_node* set){

set=set->first;
bool remove=0;

while(set!=NULL){

	for(int variable = f_clause_size[set->data]; variable!=0; variable--){

		if( IsVariableSet[ abs( f_variable_connections[set->data][variable]) ] == 1){
			if( f_variable_connections[set->data][variable] > 0 && VariableSet[ abs( f_variable_connections[set->data][variable]) ] == 1 ||  f_variable_connections[set->data][variable] < 0 && VariableSet[ abs( f_variable_connections[set->data][variable]) ] == 0	){
				remove=1;
				break;
			}
		}

	}

	if(remove==1){
		printf("removedzn\n\n\n");
		if( RemoveFromList(&set) ==-1) {set==NULL;break;}
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

link_node* RemoveDisjoint(link_node* set, int translate[], map* MapLocation, variable_pos** group){
printf(" halt %i \n", set->first->data);
	link_node* iter_set=set->first;
	while( iter_set != NULL ){
	
		for ( int clause= f_clause_size[iter_set->data]; clause!=0; clause-- ) {
				
			if( IsVariableSet[ abs(f_variable_connections[iter_set->data ][clause]) ] ==1 ){
			
				if( f_variable_connections[iter_set->data ][clause] > 0 && VariableSet[ abs(f_variable_connections[iter_set->data ][clause]) ] ==0){
					// remove the clause
					printf(" removed from index\n");
					//RemoveClauseFromIndex( MapGetValue( iter_set->data , MapLocation),  variable_position );
					//RemoveFromClause( MapGetValue( iter_set->data , MapLocation) , group);
				}
				if( f_variable_connections[iter_set->data ][clause] > 0 && VariableSet[ abs(f_variable_connections[iter_set->data ][clause]) ] ==0	
				|| f_variable_connections[iter_set->data ][clause] < 0 && VariableSet[ abs(f_variable_connections[iter_set->data ][clause]) ] ==1){
					// remove the variable from the clause
					printf(" removed from clause %i %i %i %i\n",f_variable_connections[iter_set->data ][clause],iter_set->data, translate[1], MapGetValue( iter_set->data , MapLocation) );
					RemoveVariableFromClause( abs(f_variable_connections[ iter_set->data ][clause]), MapGetValue( iter_set->data , MapLocation), variable_position );

				}
			}

		}

		iter_set = iter_set->next;
	}
return set;
}


void RemoveClauseFromIndex(int clause, variable_pos* index[] ){


	for ( int variable=clause_size[clause]; variable!=0; variable--){
	
		variable_pos* set = NULL;
		
		set = index[ abs(variable_connections[clause][variable]) ];
		
		while(1){
			if( set->clause == clause ){
			
				if( set->next == NULL && set->previous == NULL ){
				
					set->first 	= set;
					set->end 	= set;
					set->clause	= 0;
					
					break;
				}
				
				pop_clause(&set);
				break;
			}
			
			if( set->next != NULL ){
			
				set=set->next;
				
			}else{
			
				break;
			}
		}		
		
		variable_connections[clause][variable]=0;
	}
	clause_size[clause]=0;
	
}

void reduce( int var){
return;
	variable_pos* set = f_variable_position[ abs(var) ];

	bool found = 0;
	while(set!=NULL){
			printf(" this var  %i \n",f_clause_size[ set->clause ] );
	

found =0;	
		for( int variable = 1 ; variable <= f_clause_size[ set->clause ] ; variable++){
			printf (" %i %i %i \n", f_variable_connections[set->clause][variable], IsVariableSet[ abs(f_variable_connections[set->clause][variable])],VariableSet[ abs(f_variable_connections[set->clause][variable])]  );
		
			if(found==1){
				f_variable_connections[set->clause][variable] = f_variable_connections[set->clause][variable+1] ;
			}
		
			if ( abs( f_variable_connections[set->clause][variable]  ) == abs(var)
			&& f_variable_connections[set->clause][variable] != var){
				f_variable_connections[set->clause][variable] = f_variable_connections[set->clause][variable+1] ;
				found = 1;
			}
		}
		if(found==0){
			set=set->next;
			continue;
		}
		printf(" removed from base \n");
		f_clause_size[ set->clause ]--;
		
		if( f_clause_size[set->clause]==1){
			printf (" %i %i %i \n ", f_variable_connections[set->clause][1], IsVariableSet[ abs(f_variable_connections[set->clause][1]	)], VariableSet[ abs(f_variable_connections[set->clause][1]) ]);
			if( IsVariableSet[ abs(f_variable_connections[set->clause][1])] == 0 ){
				Assert_Variable( f_variable_connections[set->clause][1] );
			}else{
			
				if( VariableSet[ abs(f_variable_connections[set->clause][1]) ] ==1 && f_variable_connections[set->clause][1] < 0
				|| VariableSet[ abs(f_variable_connections[set->clause][1]) ] ==0 && f_variable_connections[set->clause][1] >0){
				printf(" this illogial\n ");
				exit(0);
				}
			}
		}
	
		
		if(set->next!=NULL){
				
			set=set->next;
			RemoveFVariablePosition(set->previous,abs(var));
		
		}else{
			RemoveFVariablePosition(set,abs(var));
		
			break;
		}
			
		set=set->next;
	}

}

void RemoveVariableFromClause(int var, int clause, variable_pos* index[] ){

	// this removed the variable from clause positon
	bool found = 0;
	for ( int variable=0; variable!=clause_size[clause]; variable++){
		if( found == 1){
			variable_connections[clause][variable] = variable_connections[clause][variable+1];
		}
		if( abs( variable_connections[clause][variable] ) == abs( var ) && found==0){
			variable_connections[clause][variable] = variable_connections[clause][variable+1] ;
			printf(" variable %i \n", variable);
			found = 1;
			
		}

	}
	clause_size[clause]--;
		
	// remove refference	
	variable_pos* set = NULL;
	set=index[ abs(var) ];
		
	while(1){
		if(  set->clause ==  clause ){
				
			RemoveVariablePosition(set,var);
			break;
		}
			
		if(set->next!=NULL){
			
			set=set->next;
		}else{
			
			break;
		}
	}		
		
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


variable_pos* SetNewIndex(link_node* list,variable_pos** set,int translation[], map* Map, map** Location){
	
	list=list->first;
	
	//halt();
		while(list!=NULL){
		
			PutMap( list->data, Location );
			(*set)	= copy_clause(list->data,(*set),translation);
			list=list->next;
		}

		
	return *set;
}

variable_pos* SetIndex_LH(link_node* list,variable_pos** set,int translation[], map* Map, map** Location){

	int size= 75;;	
	variable_pos** SearchIndex=CreateIndex(size);
	variable_pos* 	iter;
	
	list=list->first;
	
	//halt();
		while(list!=NULL){
		
	if(f_clause_size[list->data]>size){printf(" greater than %i %i \n",f_clause_size[list->data],size );exit(0);}
			SetIndex(list->data,f_clause_size[list->data],SearchIndex);

			list=list->next;
		}

		for(int i=1;i<=size;i++){
			if(SearchIndex[i]->clause==0){
				continue;
			}

			iter = SearchIndex[i];
			
			while(iter!=NULL){
				PutMap( iter->clause, Location );
				(*set)	= copy_clause(iter->clause,(*set),translation);
				iter		= iter->next;
			}
		}
	FreeSearch(SearchIndex,size);
	return *set;
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
	if((*set)==NULL){printf(" this is an empty set \n"); return NULL;}
	link_node* node = (*set)->first;
	
	
	link_node* checked=NULL;
	

	
	
	while(node!=NULL){

		if(ExistInSet( abs( node->data), checked)==1){	
		node=	DeleteNode(node); 
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
	if( set==NULL) return;
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
	
		for ( int clause= f_clause_size[iter_set->data]; clause!=0; clause-- ) {
				
			if( IsVariableSet[ abs(f_variable_connections[iter_set->data ][clause]) ] ==1 ){
						
				if( CheckSet( OnesPlace[abs(f_variable_connections[iter_set->data ][clause])] , set ) ==0){
					set = AddMember(	OnesPlace[abs(f_variable_connections[iter_set->data ][clause])] ,  set );
				}
			}

		}

		iter_set = iter_set->next;
	}
return set;			
			
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

void CreateGroup(link_node* list,variable_pos** group, map** MapLocation, map** Map){

	variable_pos* 	iter;
	
	list  = list->first;
	
	link_node* NewVar=NULL;
	
	int index = 0;
	int count = 0;
	while( list != NULL ){

 	PutMap( list->data , MapLocation );
		
		for(int variable=f_clause_size[list->data];variable!=0;variable--){ 
			if( ExistInSet( abs(f_variable_connections[ list->data ] [ variable]),NewVar ) ==0 ){
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

	if(list==NULL) exit(0);

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

void DestroySet(link_node* subset){
if(subset==NULL) return;
subset=subset->first;
	while(1){
			
		if(subset->next==NULL){ free(subset);break;}

		subset=subset->next;
		free(subset->previous);
	}
}

void ResetSolve(){
	(set) = (set)->first->end;
	while( (set) != NULL ){
		RemoveFromClause( clause_count, &set );
	}
}


link_node* RemoveUntil(int End, link_node* list){
	
	list = list->first->end;
	while(1){

		if(list->previous==NULL){break;}
if(list->previous->data==End)break;
		list = list->previous;
		
		list->next=NULL;
		free(list->next);
	}
return list;
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

void FreeSearch(variable_pos* index[], int size){
	variable_pos* ptr;
	for(int i=1; i<=size;i++){
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


