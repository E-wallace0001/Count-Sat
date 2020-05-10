#ifndef llink_H_
#define llink_H_

// node structure

typedef struct link_node{
	int data;
	struct link_node *first;
	struct link_node *previous;
	struct link_node *next;
	struct link_node *end;
} link_node;

// create a new node

link_node* link_create(int , link_node* , link_node* );

//adding a node to the next level

link_node* link_append(int ,link_node* );

//iterate through the list in the main loop

// relase from list
void pop_link(link_node** );

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

void link_dispose(link_node*);

void debug_list(link_node*);

#endif
