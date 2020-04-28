#ifndef STEST_C
#define STEST_C

//#include "infini_tree.h"

int ones[4000];

int counted_for;
int clause_size[127000];
int f_clause_size[127000];
bool set_var[40000];

int var_tab[10000];

long set_variable[127000];

struct variable_pos* variable_position[6001];
struct variable_pos* f_variable_position[6001];

int variable_connections[127000][40];
int f_variable_connections[127000][40];

int clause_count;
int f_clause_count;

int variable_count;
int f_variable_count;

void halt();

unsigned int nr_variables;
unsigned int nr_clauses;
int variable[127000][40];

//FILE fp;
int argc;

void solve();

void var_tab_add(int*);

struct node* clause_node;

struct node* pnt;

void var_tab_del(int*);
void debug();

int f_clause_count;
int clause_count;


#endif
