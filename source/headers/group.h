
#ifndef GROUP_H
#define GROUP_H

typedef struct set_s{

	struct list* list;
	struct list* first;
	struct list* end;

}set_s;


typedef struct list{
	void* data;
	struct list* next;
	struct list* previous;
} list;

typedef  set_s group_s;


set_s* MakeSet();

void ExtendSet(void*, set_s*);

int ReduceSet(list*, set_s**);

void DelSet(set_s**);

group_s* MakeGroup();

void ExtendGroup(group_s*);

void ReduceGroup(group_s**, set_s**);

void DeleteGroup(group_s**);

set_s* NextSet(group_s*);
set_s* PreviousSet(group_s*);

void CopyGroupSet(set_s*, set_s*);

int CountGroupSet(set_s*);

#endif
