#ifndef INFINI_TREE_H
#define INFINI_TREE_H

// node structure

typedef struct node{
	int data;
	int removed;
	int clause;
	int size;
	struct node *first_clause;
	struct node *next_layer;
	struct node *previous_layer;
	struct node *previous;
	struct node *next;
} node;

// create a new node

static inline node* create(int clause,int data,node* next_layer, node* previous_layer, node* previous, node* next,int removed){
	node* new_node = (node*)malloc(sizeof(node));
	if(new_node==NULL){
		printf("error creating a new node. \n");
		exit(0);
	}
	new_node->clause=clause;
	new_node->previous_layer=previous_layer;
	new_node->next_layer=next_layer;
	new_node->data = data;
	new_node->removed= removed;
	new_node->next = next;
	new_node->previous = previous;
	return new_node;
}

// add a new node to the list
static inline node* prepend(int clause,node* head,int data){
	node* new_node=create(clause, data,NULL,NULL, head,NULL,0);
	head = new_node;
	return head;
}

static inline node* append_layer(int clause, node* head,node* previous_layer, int data){
	node* new_node=create(clause, data, NULL, head,NULL,NULL,0);

	new_node->first_clause=new_node;

	previous_layer->next_layer= new_node;

	head = new_node;
	return head;
}

static inline node* append_clause(int clause, node* head, node* previous_layer,int data, int removed_data){

	node* new_node=create(clause, data,NULL, previous_layer,head,NULL,removed_data);
	
	if(head!=NULL){
		head->next=new_node;
		new_node->first_clause=head->first_clause;
	}
	else{
		new_node->first_clause=new_node;
	}

	head = new_node;
	return head;
}


// relase from list
static inline void pop(node* cursor){
	if(cursor->next!=NULL){
		node* next = cursor -> next;
		next -> previous= cursor -> previous;
		node* prev = cursor -> previous;
		prev -> next = cursor -> next;
	}
	else{
		cursor -> previous =NULL;
	}

	free(cursor);
}

static inline int count(node* head){
	
	int count=0;
	node *tmp = head;
	
	while(tmp!=NULL){
		count++;
		tmp= tmp->next;
	}
	return (count);
}

void dispose(node *head)
{
	node *cursor, *tmp;

	if(head != NULL){
		cursor= head->next;
		
		head->next=NULL;
		while(cursor!=NULL){
			tmp=cursor->next;
			free(cursor);
			cursor=tmp;
		}
	}
}

#endif
