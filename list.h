#ifndef LIST_H_
#define LIST_H_

// node structure

typedef struct node{
	int data;
	struct node *first;
	struct node *previous;
	struct node *next;
} node;

// create a new node

static inline node* create(int data, node* previous, node* next){
	node* new_node = (node*)malloc(sizeof(node));
	if(new_node==NULL){
		printf("error creating a new node. \n");
		exit(0);
	}
	new_node->first = previous->first;
	new_node->data = data;
	new_node->next = next;
	new_node->previous = previous;
	return new_node;
}

// add a new node to the list
static inline node* prepend(node* head,int data){
	node* new_node=create(data,head,NULL);
	head = new_node;
	return head;
}


//adding a node to the next level

static inline node* append(node* head, int data){
	node* new_node=create(data, head,NULL);
	head -> next= new_node;
	
}

//iterate through the list in the main loop

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
