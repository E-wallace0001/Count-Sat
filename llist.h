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

static inline link_node* link_create(int data, link_node* previous, link_node* next){
	link_node* new_node = (link_node*)malloc(sizeof(link_node));
	if(new_node==NULL){
		printf("error creating a new node. \n");
		
	}

	//new_node->first=new_node;
	new_node->data = data;
	new_node->next = next;
	new_node->previous = previous;
	return new_node;
}

// add a new node to the list
static inline link_node* link_prepend(link_node* head,int data){
	
	link_node* new_node=link_create(data,head,NULL);
	//if(new_node->first==NULL){new_node->first = new_node;}else{new_node->first=head;}
	head = new_node;

	return new_node;
}


//adding a node to the next level

static inline link_node* link_append(link_node* head, int data){
	link_node* new_node=link_create(data, head,NULL);

	if(head!=NULL){
		head->first->end=new_node;
		head->next=new_node;
		new_node->first=head->first;
	}
	else{
		new_node->first=new_node;
	}

return(new_node) ;
}

//iterate through the list in the main loop

// relase from list
static inline link_node* link_pop(link_node* cursor){

link_node* temp;
		
if(cursor->previous!=NULL){
	if(cursor->next!=NULL){
		printf("%i\n",cursor->next->data);
		cursor->next->previous=cursor->previous;
		cursor->previous=cursor->next;
		temp=cursor->previous;
	}else{
		temp=cursor->previous;
		temp->next=NULL;
	}
}else{
	if(cursor->next!=NULL){
		temp=cursor->next;
		temp->previous=NULL;
	}

}

free(cursor);
return(NULL);
}

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

void link_dispose(link_node *head)
{
	link_node *cursor, *tmp;

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
