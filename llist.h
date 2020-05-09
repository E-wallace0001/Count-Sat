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

static inline link_node* link_append(int data,link_node* head){
	link_node* new_node=link_create(data, head,NULL);

	if(head!=NULL){
		new_node->first=head->first;;
		new_node->first->end=new_node;
		head->next=new_node;
		new_node->previous=head;
	}
	else{
		new_node->next=NULL;
		new_node->previous=NULL;
		new_node->first=new_node;
		new_node->end=new_node;
	}

return(new_node) ;
}

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
