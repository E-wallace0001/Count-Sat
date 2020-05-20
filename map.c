
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#include "map.h"


map* PutMap( int key, map** PreviousMap){
	map* NewMap = (map*)malloc(sizeof(map));
	if (NewMap == NULL){
		printf("Error Creating map \n");
	}
	if((*PreviousMap) == NULL){
	 NewMap->size=1;
	}else{
	 NewMap->size = (*PreviousMap)->size+1;
	}
	NewMap->key=key;
	NewMap->value=NewMap->size;
	NewMap->next=(*PreviousMap);
	(*PreviousMap) = NewMap;
	
	return NewMap;
}


int MapGetValue(int key, map* ThisMap){

	while(ThisMap!=NULL){
		if(ThisMap->key==key){
			return(ThisMap->value);
		}
		ThisMap=ThisMap->next;
	}
	
	return(-1);

}

int MapGetKey(int value, map* ThisMap){

	while(ThisMap!=NULL){
		if(ThisMap->value==value){
			return(ThisMap->key);
		}
		ThisMap=ThisMap->next;
	}
	
	return(0);
}

void FreeMap(map* ThisMap){
map* tmp =ThisMap;
	while(tmp->next!=NULL){
	tmp=tmp->next;
	free(ThisMap);
	ThisMap=tmp;
	}
free(tmp);
}
