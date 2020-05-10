#ifndef INFINI_TREE_H
#define INFINI_TREE_H


#include <gmp.h>
// node structure


typedef struct node{
	mpz_t data;
	mpz_t removed;
	int clause;
	int size;
	struct node *first_clause;
	struct node *next_layer;
	struct node *previous_layer;
	struct node *previous;
	struct node *next;
	struct node *end;
} node;



// create a new node

node* create(int ,mpz_t ,node* , node* , node* , node* ,mpz_t ,int );

// add a new node to the list
static inline node* prepend(int clause,node* head,mpz_t data,int size){
	node* new_node=create(clause, data,NULL,NULL, head,NULL,0,size);
	head = new_node;
	return head;
}

node* append_layer(int clause, node* head,node* previous_layer, mpz_t data,int size);

node* append_clause(int , node* , node* ,mpz_t , mpz_t ,int );


static inline node* prepend_clause(int , node*);


// relase from list
static inline void pop(node*);

static inline int count(node* );

int count_node(node* );
void dispose(node**);


#endif
