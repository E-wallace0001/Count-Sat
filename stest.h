#ifndef STEST_C
#define STEST_C

#include "infini_tree.h"

#define vsize 900000
#define csize 900000





void init_position(struct variable_pos**);

void destroy_position(struct variable_pos**);

//struct variable_pos* variable_position[csize];
struct variable_pos* f_variable_position[csize*2];



void post_branch_correct(int,int,int,int,struct node*, com_line*);
void pre_branch_correct(int, int, int, struct node*, struct node*,struct node*,com_line*);


int ones[csize];

int OnesPlace[csize];

int counted_for;
//int clause_size[csize];
int f_clause_size[csize*2];


//long set_variable[vsize];

//void init_pos(struct variable_pos* table[]);



//int variable_connections[vsize][90];
int** f_variable_connections;

//int clause_count;
int f_clause_count;

//int variable_count;
int f_variable_count;

void halt();

unsigned int nr_variables;
unsigned int nr_clauses;


int variable[vsize][90];

//FILE fp;
int argc;

void solve();

void var_tab_add(int*, com_line*);

struct node* clause_node;

struct node* pnt;

void var_tab_del(int*, com_line*);
void debug();

int f_clause_count;
//int clause_count;

//static inline int compare_clause(int, int ) ;


struct link_node* FindFVariableInRange( int ,  struct link_node* );


#endif
