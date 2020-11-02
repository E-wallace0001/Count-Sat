#ifndef CHAIN_BLOCK_H
#define CHAIN_BLOCK_H

typedef struct chain_block{
	int chain_number;
	int block;
	struct chain_block* previous;
} chain_block;



static inline chain_block* create_block(){
	chain_block* new_block = (chain_block*)malloc(sizeof(chain_block));

	return(new_block);
	
}

chain_block* add_block(chain_block* chain, int block){
	chain_block* new_block=create_block();
	new_block->previous=chain;
	new_block->block=block;
	return(new_block);

}

chain_block* remove_block(chain_block* chain){
	chain_block* previous_block=chain->previous;
	free(chain);
	return(previous_block);
}
#endif
