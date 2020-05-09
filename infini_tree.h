#ifndef INFINI_TREE_H
#define INFINI_TREE_H


#include <gmp.h>
// node structure


typedef struct node{
	mpz_t data;
	mpz_t removed;
	int clause;
	int size;
	int data2;
	struct node *negate;
	struct node *first_change;
	struct node *first_clause;
	struct node *next_layer;
	struct node *previous_layer;
	struct node *previous;
	struct node *next;
	struct node *end;
	bool mod;
} node;



// create a new node

static inline node* create(int clause,mpz_t data,node* next_layer, node* previous_layer, node* previous, node* next,mpz_t removed,int size){
	node* new_node = (node*)malloc(sizeof(node));
	if(new_node==NULL){
		printf("error creating a new node in create. \n");
		exit(0);
	}
	mpz_init(new_node->data);
	mpz_init(new_node->removed);
	new_node->clause=clause;
	new_node->previous=previous;

	return new_node;
}

// add a new node to the list
static inline node* prepend(int clause,node* head,mpz_t data,int size){
	node* new_node=create(clause, data,NULL,NULL, head,NULL,0,size);
	head = new_node;
	return head;
}

static inline node* append_layer(int clause, node* head,node* previous_layer, mpz_t data,int size){
	node* new_node=create(clause, data, NULL, head,NULL,NULL,data,size);

	new_node->first_clause=new_node;

	new_node->end=new_node;

	new_node->previous_layer=previous_layer;

	previous_layer->next_layer= new_node;
	
	new_node->clause=clause;
	
	mpz_set(new_node->data,data);


	head = new_node;
	return head;
}

node* append_clause(int , node* , node* ,mpz_t , mpz_t ,int );


static inline node* prepend_clause(int , node*);


// relase from list
static inline void pop(node*);

static inline int count(node* );

int count_node(node* );
void dispose(node*);


#endif
