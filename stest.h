#ifndef STEST_C
#define STEST_C

//#include "infini_tree.h"

#define vsize 900000
#define csize 900000


int ones[csize];

int OnesPlace[csize];

int counted_for;
int clause_size[csize];
int f_clause_size[csize];
bool set_var[csize];

int var_tab[vsize];

long set_variable[vsize];

//void init_pos(struct variable_pos* table[]);

struct variable_pos* variable_position[csize];
struct variable_pos* f_variable_position[csize];

int variable_connections[vsize][90];
int f_variable_connections[vsize][90];

int clause_count;
int f_clause_count;

int variable_count;
int f_variable_count;

void halt();

unsigned int nr_variables;
unsigned int nr_clauses;


int variable[vsize][90];

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

//static inline int compare_clause(int, int ) ;


struct link_node* FindFVariableInRange( int ,  struct link_node* );


#endif
