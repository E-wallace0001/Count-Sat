#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "var_pos.h"
#include "bfs_s.h"
#include "stest.h"
#include "clause_func.h"
#include "infini_tree.h"
#include <gmp.h>
// node structure

/*
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
		printf("error creating a new node. \n");
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

static inline node* append_clause(int clause, node* head, node* previous_layer,mpz_t data, mpz_t removed_data,int size){

	node* new_node=create(clause, data,NULL, previous_layer,head,NULL,removed_data,size);
	
	if(head!=NULL){
		head->first_clause->end=new_node;
		head->next=new_node;
		new_node->first_clause=head->first_clause;
		new_node->previous_layer=previous_layer;
		
	}
	else{
		new_node->first_clause=new_node;
	}

	head = new_node;
	return head;

}

static inline node* prepend_clause(int clause, node* head){

	node* new_node=create(clause, 0,NULL, NULL,NULL,head,0,0);
	
	if(head!=NULL){
		head->first_clause->end=NULL;
		head->previous=new_node;
		
	}
	else{
		new_node->first_clause=new_node;
	}

	head = new_node;
	return head;
}


*/
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
int count_node(node* head){
	
	int count=0;
	node *tmp = head;
	
	while(tmp!=NULL){
		count++;
		tmp= tmp->next;
	}
	return (count);
}

void dispose(node* head){
	node *cursor, *tmp;
while(1){
	
	if(head->next!=NULL){
		cursor=head;
		head=head->next;
		free(cursor);
			if(head->next_layer!=NULL){head=head->next_layer;}
		continue;
	}
	if(head->previous_layer!=NULL){
		cursor=head->next;
		head=head->previous_layer;
		free(cursor);
		continue;
	}
break;

}



}

