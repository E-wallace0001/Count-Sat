#ifndef VAR_POS_H
#define VAR_POS_H

// variable_pos structure

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

variable_pos* create_clause(int ,variable_pos* );

variable_pos* append_variable(int , variable_pos*);


// relase from list
void pop_clause(variable_pos* * );

int count_var_pos(variable_pos*);

void debug_pos(variable_pos*);


struct variable_pos* search_var_pos(int ,variable_pos*);


#endif
