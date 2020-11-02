#ifndef CLAUSE_FUNC_H
#define CLAUSE_FUNC_H

int add_clause_();

void del_clause_();

struct variable_pos* copy_clause( int, int [] , com_line*);

void create_new_clause();


struct variable_pos* AddToClause( int , struct variable_pos*, com_line* );

void append_var_clause( int, int,  com_line* );

void del_clause(int, com_line*);

void pop_var_clause(int, int, com_line*);
void remove_copy_clause(int);
void pop_clause_();
void copy_removed(int, com_line*);

void RemoveFromClause(int, com_line*);
void copy_pop_var_clause(int,int);
void null_add(com_line*);
void null_remove(com_line* );

void fpop_clause(int , int this_place);
void RemoveClause(int , com_line* );

void RemoveLastAssert();

struct link_node* var_subset_find(struct link_node* , com_line* );

#endif
