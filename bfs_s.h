#ifndef BFS_S_H
#define BFS_S_H

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

variable_pos* bfs_graph( layer*,variable_pos* );

bool clause_visited[9000];
bool counted_set[8000];
void init_graph();
int new_old_clause[8000];

#endif
