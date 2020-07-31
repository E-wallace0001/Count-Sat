#ifndef llink_H_
#define llink_H_

#include "stest.h"

// node structure

typedef struct link_node{
	long int data;
	struct link_node *first;
	struct link_node *previous;
	struct link_node *next;
	struct link_node *end;
} link_node;

// create a new node

link_node* link_create(int , link_node* , link_node* ,com_line*);

//adding a node to the next level

link_node* link_append(int ,link_node*,com_line*);

//iterate through the list in the main loop

// relase from list
void pop_link(link_node**, com_line* );

static inline int link_count(link_node* head){

	if(head==NULL){printf("null head link count\n");return(0);}
	int count=0;

	link_node *tmp = head->first;
	
	while(tmp!=NULL){
		count++;
		tmp=tmp->next;
	}
	return (count);
}

link_node* AddLists(link_node*, link_node*,com_line*);

void link_dispose(link_node*,com_line*);

void debug_list(link_node*);

void DeleteList(link_node**,com_line*);

int ListSize(link_node*);

int RemoveMember(link_node**, com_line*);

link_node* DeleteNode(link_node*, com_line*);

int RemoveFromList(link_node** , com_line*);

link_node* copy_list(link_node*, com_line*);

link_node* GroupTogether( link_node* ,com_line*);

int FindLargestClause( link_node* );

link_node* BinSort ( link_node**,  com_line*);

link_node* GroupSingles( link_node* , com_line*);

link_node* GroupSet( link_node* ,com_line*);

link_node** CreateSet(int );

link_node* CollectVariables( link_node* , link_node** , link_node**,com_line*);


link_node* RemoveSubset(link_node**  , link_node* , link_node** , int,com_line*);
link_node* RemoveAfromB(link_node* , link_node**,com_line* );

link_node* NearestConnecttion( link_node* );
link_node* MoveToPre( link_node* , link_node* );

void CheckFirstNode( link_node*);
void SetFirst(link_node*);

link_node* FindNode(int, link_node** );

#endif
