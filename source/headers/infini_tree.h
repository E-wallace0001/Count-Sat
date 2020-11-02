#ifndef INFINI_TREE_H
#define INFINI_TREE_H


#include <gmp.h>
#include "coms.h"
// node structure


typedef struct node{
	mpz_t data;
	mpz_t removed;
	
	int clause;
	struct node *first_clause;
	
	struct node *next_layer;
	struct node *previous_layer;
	
	struct node *previous;
	struct node *next;
	
	struct node *end;
} node;



// create a new node

node* create(int ,mpz_t ,node* , node* , node* , node* ,mpz_t, com_line* );



node* append_layer(int clause, node* head,node* previous_layer, mpz_t data, com_line* );

node* append_clause(int , node* , node* ,mpz_t , mpz_t , com_line* );


static inline node* prepend_clause(int , node*);


// relase from list
static inline void pop(node*, com_line* );

static inline int count(node* );

int count_node(node* );
void dispose(node**, com_line*);


#endif
