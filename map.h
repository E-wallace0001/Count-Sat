#ifndef MAP_H
#define MAP_H

typedef struct map{
	int key 	 ;
	int value ;
	int size ;
	struct map* next;
	} map;
	
	
	map* PutMap( int, map** );
	
	int MapGetValue(int, map* );
	
	void FreeMap(map* );
	
	int MapGetKey(int , map* );

	#endif
