#include <stdio.h>
#include <stdlib.h>
#include "../source/headers/group.h"


int main(){


group_s* gr1 = MakeSet();
set_s* s2 = MakeSet();

ExtendSet(s2, gr1);

int num[] ={56,27 ,28,32,33};

size_t len = sizeof(num)/sizeof(int);

for (size_t i=0; i<len;i++){
	ExtendSet( &num[i] , s2);
}

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
printf(" completed \n");
ReduceGroup( &gr1, &s1 );
return(0);

}
