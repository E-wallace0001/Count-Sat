#ifndef COMS_H
#define COMS_H


typedef struct com_line{

	struct variable_pos**	variable_position;
	struct variable_pos**	var_pos;
	int**							clause_connections;
	 int** 	f_variable_connections;
	struct variable_pos*	set;

	struct link_node* SetList;
	struct link_node* list;
	struct link_node* Tried;
	mpz_t* all_possible;
	
	mpz_t* sub_total;
	int* var_tab;
	
	int * f_clause_size;
	int limit;
	int LimitReached;
	
	long* set_variable;
	int* clause_size;
	struct node* clause_node;
	int ClauseArraySize;
	int clause_count;
	bool* set_var;
	int ClauseCount;
	int VariableCount;
	bool* IsVariableSet;
	struct m_map* job_mem;
	struct m_map* node_pool;
	struct m_map* link_pool;
	struct m_map* com_pool;
	//bool set_var[csize]={0};
	int ret_value;
	int variable_count;
}com_line;

#endif
