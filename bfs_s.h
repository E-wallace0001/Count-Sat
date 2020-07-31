#ifndef BFS_S_H
#define BFS_S_H

#include "stest.h"
#include "map.h"
typedef struct layer{
	int num, lim;
} layer;

static inline layer* set_layer(int num, int lim){
	layer* new_layer = (layer*)malloc(sizeof(layer));
	new_layer->num=num;
	new_layer->lim=lim;
return new_layer;
}

typedef struct graph_layer{

struct link_node* list;
struct link_node* SetList;
struct link_node* Tried;
int limit;
int LimitReached;
int ClauseCount;
int VariableCount;
int**							clause_connections;
struct variable_pos**	variable_position;
long* set_variable;
int* var_tab;
int* clause_size;
bool set_var;
		//argt->clause_connections	= clause_connections;
		//argt->variable_position		= variable_position;
		//argt->set_variable			= set_variable;
		//argt->var_tab					= var_tab;

} graph_l;

void raw();



void bfs_graph( layer*, struct link_node* );

bool clause_visited[csize];
bool counted_set[vsize];
void init_graph(int*);
int new_old_clause[vsize];

bool IsVariableSet[vsize];

void Evaluate( void* );

bool VariableSet[vsize];

#endif
