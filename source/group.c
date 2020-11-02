#include <stdlib.h>
#include <stdio.h>
#include "headers/group.h"



set_s* MakeSet(){
	set_s* NewSet		= malloc(sizeof(*NewSet));
	NewSet->first	= NULL;
	NewSet->end		= NULL;
	NewSet->list	= NULL;
	return NewSet;
}


void ExtendSet(void* data, set_s* set){
	if(set==NULL){
		printf(" ExtendSet empty \n");
		exit(0);
	}
	list* NewMember			= malloc(sizeof(*NewMember));
	NewMember->data			= data;
	NewMember->next			= NULL;
	NewMember->previous 		= NULL;

	if( set->list == NULL){
		set->list				= NewMember;
		set->first				= NewMember;
		set->end					= NewMember;
	}else{
		set->end->next			= NewMember;
		NewMember->previous	= set->end;
		set->end					= NewMember;
	}
	
}

int ReduceSet(list* list_t, set_s** set){
	
	if( list_t==NULL) return -1;

	if(list_t->previous!=NULL){
		if(list_t->next!=NULL){
			list_t->previous->next	= list_t->next;
			list_t->next->previous	= list_t->previous;
			(*set)->list= list_t->previous;
			free(list_t);
		}else{
			list_t->previous->next	= NULL;
			(*set)->end					= (*set)->end->previous;
			(*set)->list= list_t->previous;
			free(list_t);
			
		}
	}else{
		if(list_t->next!=NULL){
			list_t->next->previous	= NULL;
			(*set)->first				= list_t->next;
			(*set)->list 				=(*set)->first;
			(*set)->list= list_t->next;
			free(list_t);
			return 1;
		}else{
			(*set)->first				= NULL;
			(*set)->end					= NULL;
			(*set)->list= NULL;
			
			free(list_t);
			return 2;
		}
		
	}
	//return 1;
}

void DelSet(set_s** set){

	(*set)->list=(*set)->first;

	while((*set)->list!=NULL){

		ReduceSet( (*set)->list , (set) );

		(*set)->list = (*set)->first;
		
		if( (*set)->list == NULL)
			break;
		
	}
	free( *set );
}


// group y
// set 	x
group_s* MakeGroup(){

	set_s*	group 	= MakeSet();
	set_s*	set		= MakeSet();
	ExtendSet(set, group);
	//group->list->data = set;
	return group;
}

void ExtendGroup(group_s* group){

	set_s* NewSet = MakeSet();
	ExtendSet(NewSet, group);
}

set_s* NextSet(group_s* group ){

	list* set = group->list;
	
	if( set->next == NULL){
		printf(" no next \n");
		
		return NULL;
	}

	group->list = group->list->next;
	
	return group->list->data;


}

set_s* PreviousSet(group_s* group){
	
	list* set = group->list;

	if( set->previous ==NULL ){	
		printf(" no next pre-set \n");
		exit(0);
		return NULL;
	}
	group->list = group->list->previous;
	
	return group->list->data;
}


// shrinks group by getting rid of selected set
void ReduceGroup(group_s** group, set_s** set){

	list* temp;
	if( (*group)->list->previous!=NULL ){
		temp = (*group)->list->previous;
	}else if( (*group)->list->next!=NULL ){
		temp = (*group)->list->next;
	}else{
		temp = NULL;
	}
		
//	DelSet( (set_s**)&((*group)->list->data) );
	
	ReduceSet( (*group)->list, group);
	
	(*group)->list = temp;
	
	if( (*group) ->list!=NULL){
		if ( set != NULL)
			//(*set)= NULL;
		return;
	}
	
	if( set!=NULL ){
		(*set) = (set_s*)(*group) ->list->data;
	}
	
}

void DeleteGroup(group_s** group){
(*group)->list = (*group)->first;
	while((*group)->list!=NULL){
	
		ReduceGroup( (group), NULL );
		(*group)->list = (*group) ->first;
	}
	free(*group);
}

void CopyGroupSet(set_s* s1, set_s* s2){
	list* l1 = s1->first;
	while(l1!=NULL){
		ExtendSet( l1->data, s2);
		l1 = l1->next;
	}
}

int CountGroupSet(set_s* set){
	list* l1 = set->first;
	int count=0;
	while(l1!=NULL){
		count++;
		l1=l1->next;
	}
	return count;
}



