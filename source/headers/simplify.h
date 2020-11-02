
#ifndef SIMPLIFY_H
#define SIMPLIFY_H

typedef struct state{
	bool asserted;
	bool assigned;
} state;

void simplify( group_s* );

#endif

