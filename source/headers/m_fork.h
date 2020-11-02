#ifndef M_FORK_H
#define M_FORK_H

typedef struct thread{

	fork_ptr new_thread;
	struct thread next;

}thread;

int(*fork_ptr)();


#endif
