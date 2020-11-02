#ifndef VAR_POS_H
#define VAR_POS_H

// variable_pos structure
#include "stest.h"


typedef struct variable_pos{
	
	int clause;
	bool visited;
	struct variable_pos *orriginal;
	struct variable_pos *previous;
	struct variable_pos *next;
	struct variable_pos *end;
	struct variable_pos *first;

}variable_pos;


// create_pos a new variable_pos

variable_pos* make_clause(int);

variable_pos* create_clause(com_line* );

variable_pos* append_variable(int , variable_pos*);


// relase from list
void pop_clause(variable_pos**, com_line* );

int count_var_pos(variable_pos*);

void debug_pos(variable_pos*, com_line*);


struct variable_pos* search_var_pos(int ,variable_pos*);

void Assert_Variable(int variable);

	

void RemoveFVariablePosition(variable_pos*,int var, com_line*);

void RemoveConnection(variable_pos* , int , variable_pos** );

#endif
