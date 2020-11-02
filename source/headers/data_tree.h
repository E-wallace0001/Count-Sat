#ifndef DATA_TREE_H
#define DATA_TREE_H



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




#endif
