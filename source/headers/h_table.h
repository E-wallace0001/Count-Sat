#ifndef H_TABLE_H
#define H_TABLE_H

typedef struct hasht_bucket{
	
	int clause;
	struct hasht_bucket *previous;
	struct hasht_bucket *next;
	struct hasht_bucket *end;
	struct hasht_bucket *first;

} hasht_bucket;


typedef struct hash_t{
	struct hasht_bucket** 	table;
	size_t size;

}hash_t;


hash_t* hasht_create(size_t );

void table_add( size_t , hash_t* );

bool check_table(size_t , hash_t* );

void hash_t_destroy( hash_t** );


#endif
