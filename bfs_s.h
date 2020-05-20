#ifndef BFS_S_H
#define BFS_S_H

#include "stest.h"

typedef struct layer{
	int num, lim;
} layer;

static inline layer* set_layer(int num, int lim){
	layer* new_layer = (layer*)malloc(sizeof(layer));
	new_layer->num=num;
	new_layer->lim=lim;
return new_layer;
}

void raw();

variable_pos* set;

void bfs_graph( layer*,link_node* );

bool clause_visited[csize];
bool counted_set[vsize];
void init_graph();
int new_old_clause[vsize];

bool IsVariableSet[vsize];

#endif
