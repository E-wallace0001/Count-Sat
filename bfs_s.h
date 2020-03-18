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

void bfs_graph(int, layer*, variable_pos* );

bool clause_visited[300];

void init_graph();

#endif
