#include <stdio.h>
#include <stdlib.h>
#include "group.h"


int main(){

printf("hello world !\n");


group_s* gr1 = MakeSet();
set_s* s2 = MakeSet();

ExtendSet(s2, gr1);
//group_s* group;
//group = MakeGroup();


int num[] ={56,27 ,28,32,33};

//set_s* s1;
//list* dat;

//set_s* s2 = (set_s*)group->list->data;
//set_s* s2 = (set_s*)s3 ;
//list* t1 = (void*)&group->list;	
//printf(" %p %p \n ", s1, group->list->data);

//set = dat->list;
//printf(" len %lu\n ", sizeof(num)/sizeof(int) );


size_t len = sizeof(num)/sizeof(int);

for (size_t i=0; i<len;i++){
	ExtendSet( &num[i] , s2);
}

//

ExtendGroup(gr1);
set_s* s1 = NextSet(gr1);
len = sizeof(num)/sizeof(int);
for (size_t i=0; i<len;i++){
	ExtendSet( &num[i] , s1);
}

ReduceSet( s1->first->next->next->next->next->next, &s1);
if (s1!=NULL){
	while( s1->list !=NULL ){
		printf(" set %i \n",*(int*)s1->list->data ); 
		s1->list=s1->list->next;
	}
}

ReduceGroup( &gr1, &s1 );
DeleteGroup(&gr1);
//DeleteSet(&s2);
//DeleteSet(&gr1);
//free(gr1);
//free(s2);
//free(group);
return(0);

}
