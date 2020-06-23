#ifndef CLAUSE_FUNC_H
#define CLAUSE_FUNC_H

int add_clause_();

void del_clause_();

variable_pos* copy_clause( int, variable_pos*, int [] );

void create_new_clause();


variable_pos* AddToClause( int , variable_pos* );

void append_var_clause( int, int, variable_pos*[] );

void del_clause(int);

void pop_var_clause(int, int);
void remove_copy_clause(int);
void pop_clause_();
void copy_removed(int, variable_pos**);

void RemoveFromClause(int, variable_pos**);
void copy_pop_var_clause(int,int);
void null_add(variable_pos** );
void null_remove(variable_pos** );

void fpop_clause(int , int this_place);
void RemoveClause(int ,variable_pos** );

void RemoveLastAssert();

#endif
