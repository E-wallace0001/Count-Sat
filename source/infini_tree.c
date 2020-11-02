#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "headers/stest.h"
#include "headers/var_pos.h"
#include "headers/infini_tree.h"

#include "headers/llist.h"
#include "headers/bfs_s.h"
#include "headers/clause_func.h"
#include "headers/cnf_read.h"
#include <gmp.h>
#include <assert.h>
#include "headers/cnf_read.h"

#include "headers/m_map.h"


node* create(int clause,mpz_t data,node* next_layer, node* previous_layer, node* previous, node* next,mpz_t removed,com_line* Coms){
 //node* new_node = (node*)malloc(sizeof(node));
	node* new_node = alloc_mem(Coms->node_pool);
	if(new_node==NULL){
		printf("error creating a new node in create. \n");
		exit(0);
	}
	
	
	new_node->first_clause=new_node;
	new_node->end=new_node;
	new_node->next_layer=NULL;
	new_node->previous_layer=NULL;
	new_node->next=NULL;
	new_node->clause=clause;
	new_node->previous=previous;
	mpz_init(new_node->data);
	mpz_init(new_node->removed);
	return new_node;
}

// relase from list
static inline void pop(node* cursor,com_line* Coms){
	if(cursor->next!=NULL){
		node* next = cursor -> next;
		next -> previous= cursor -> previous;
		node* prev = cursor -> previous;
		prev -> next = cursor -> next;
	}
	else{
		cursor -> previous =NULL;
	}
	//free(cursor);
	release_mem(cursor, Coms->node_pool);
	
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

node* append_clause(int clause, node* head, node* previous_layer,mpz_t data, mpz_t removed_data, com_line* Coms){

	node* new_node=create(clause, data,NULL, previous_layer,head,NULL,removed_data,Coms);
	
	if(head!=NULL){

		new_node->first_clause=head->first_clause;
		new_node->previous_layer=previous_layer;
		head->first_clause->end=new_node;
		head->next=new_node;
	}
	else{
		new_node->first_clause=new_node;
	}

	return new_node;

}

node* append_layer(int clause, node* head,node* previous_layer, mpz_t data,com_line* Coms){

	node* new_node=create(clause, data, NULL, head,NULL,NULL,data,Coms);

	new_node->clause=clause;
	
	new_node->previous_layer=previous_layer;

	previous_layer->next_layer= new_node;
	

	mpz_set(new_node->data,data);


	return new_node;
}

void dispose(node** head,com_line* Coms){
if((*head)==NULL){printf("null head \n");exit(0);}
	while(1){
		if( (*head)->next_layer!=NULL){
			dispose(&((*head)->next_layer),Coms);
			mpz_clear((*head)->next_layer->removed);
			mpz_clear((*head)->next_layer->data);
			release_mem((*head)->next_layer, Coms->node_pool);
		}


		if((*head)->next!=NULL){
			mpz_clear((*head)->removed);
			mpz_clear((*head)->data);
			(*head)=(*head)->next;
			release_mem((*head)->previous, Coms->node_pool);
		}else{
			break;
		}
	}
//(*head)=(*head)->first;


}

