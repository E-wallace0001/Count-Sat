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

variable_pos* set;

variable_pos* bfs_graph(int, layer*,variable_pos*,variable_pos* );

bool clause_visited[9000];

void init_graph();
int new_old_clause[8000];

#endif
