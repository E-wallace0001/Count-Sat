
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#include <string.h>
#include "headers/map.h"

#include "headers/stest.h"
#include "headers/var_pos.h"
#include "headers/clause_func.h"
#include "headers/stest.h"
#include <gmp.h>
#include "headers/llist.h"
#include "headers/bfs_s.h"
#include "headers/infini_tree.h"

#include "headers/index.h"
#include "headers/cpu_pool.h"

#include "headers/m_map.h"
#include <unistd.h> 
#include <stdint.h>
#include "headers/cnf_read.h"

int work_count=0;
int cnt=0;
int t_count=0;
variable_pos* set;
variable_pos* tmp;
variable_pos* set_copy=NULL;
bool clause_visited[csize]={0};
int  new_old_clause[vsize]={0};
bool counted_set[csize]={0};

bool CountVariable[vsize]={0};

bool KnownSet[vsize]={0};
bool IsVariableSet[vsize]={0};
bool VariableSet[vsize]={0};

variable_pos* variable_list=NULL;

void Export(variable_pos* iter, char title[], com_line* Coms){

	if(iter==NULL){
		printf("null export \n");
		exit(0);
	}
	int * clause_size= Coms->clause_size;
	char file[]=".cnf";
	strcat(title,file);

	//printf(" %s \n", title);

	iter=iter->first;
	FILE* fp2;
	fp2=fopen(title,"w");
	fprintf(fp2, "p cnf 1 %i\n", Coms->clause_count);
	while(1){
		for(int variable=clause_size[(iter)->clause]; variable!=0;variable--){
			fprintf(fp2,"%i ", Coms->clause_connections[(iter)->clause][variable]);
		}
		

		fprintf(fp2,"0\n");
		if((iter)->next!=NULL){
			(iter)=(iter)->next;
		}
		else{break;}
	}
	fprintf(fp2,"0\n");

fclose (fp2);
}

void rawex( char title[]){


	char file[]=".cnf";
	strcat(title,file);

	FILE* fp2;
	fp2=fopen(title,"w");
	fprintf(fp2, "p cnf 1 %i\n", f_clause_count);
	for( int i=1; i<= f_clause_count; i++){
		for(int variable=1; variable<=f_clause_size[i];variable++){
			if( f_variable_connections[i][variable]>0){
			fprintf(fp2,"%i ", orvar[ abs(f_variable_connections[i][variable]) ] );
			}else{
			fprintf(fp2,"%i ", -orvar[ abs(f_variable_connections[i][variable])  ] );
			}
		}
		
		fprintf(fp2,"0\n");
	}
	//fprintf(fp2,"0\n");

fclose (fp2);
}

void select_var_print( char title[]){


	char file[]=".cnf";
	strcat(title,file);
uint8_t ans1 = 0 ;
uint8_t ans2 = 0;
uint8_t answ = 0;
	FILE* fp2;
	fp2=fopen(title,"w");
	fprintf(fp2, "p cnf 1 %i\n", f_clause_count);
	for( int i=17; i>= 10; i--){
		
		fprintf(fp2," %i: %i ",   i, VariableSet[ variable[i][0] ] );
		ans1 = ans1 + VariableSet[conns[i]] ;
		ans1 = ans1 << 1 ;	
		
	}
	printf(" \n");
	fprintf(fp2, " \n");
	for( int i=9; i>= 2; i--){
		
		fprintf(fp2," %i: %i ",   i, VariableSet[variable[i][0]] );
		ans2 = ans2 + VariableSet[variable[i][0]] ;
		ans2 = ans2 << 1 ;	
		
	}
	answ = ans1*ans2;

	printf (" %d \n %d \n %d \n", ans1, ans2, answ);
	
	//fprintf(fp2,"0\n");

fclose (fp2);
}

void raw(com_line* Coms){

	for(int clause=1;clause<=f_clause_count;clause++){

	set=copy_clause(clause,new_old_clause,Coms);
	}

}

bool CheckClauseIsNeg(int clause ){
int variable;
	for(int var=f_clause_size[clause];var!=0;var--){
		variable=f_variable_connections[clause][var];
		for(int i=1;i<=ones[0];i++){
			if( abs(variable)==abs(f_variable_connections[ones[i]][1]) && (variable)!=f_variable_connections[ones[i]][1] ){
				return(1);
			}
		}
	}
return(0);

}



void SearchAndCollect(void* arg_a){

if( arg_a ==NULL) exit(0);

	com_line* arg_p =  arg_a;
	com_line* data;
	
//	printf("ding %i \n", arg_p->limit);
	int TriedSaved=0;

	link_node* TempList	= NULL;
	link_node* Tried		= (arg_p->Tried);
	int ListEnd 			= arg_p->SetList->first->end->data;

	if((Tried)!=NULL && (Tried)->first->end!=NULL){
		TriedSaved = (Tried)->first->end->data;
	}
	
	link_node* SetList	= arg_p->SetList;
	int eval 				= 0;
	link_node* new_list	= NULL;
	
	if(arg_p->list==NULL) return ;
	link_node* LocalTried=NULL;
		
	while( arg_p->list!=NULL ){
		new_list =NULL;
		for( int clause = f_clause_size[arg_p->list->data]; clause!=0; clause--){
		
			if(	ExistInSet( abs(f_variable_connections[arg_p->list->data][clause]), (Tried)) == 1 || 	ExistInSet( abs(f_variable_connections[arg_p->list->data][clause]), (Tried))==1){
				continue;
			}
			
			//LocalTried = link_append( abs(f_variable_connections[arg_p->list->data][clause]) , LocalTried,arg_p);
			
			Tried = link_append( abs(f_variable_connections[arg_p->list->data][clause]) , (Tried),arg_p);

			// copy all of the places of variable
			SetList  = CopySet( abs(f_variable_connections[arg_p->list->data][clause]) ,f_variable_position, SetList,arg_p);
			
			new_list = CopySet( abs(f_variable_connections[arg_p->list->data][clause]) ,f_variable_position, new_list, arg_p);
	
			//SetFirst(SetList);
			//SetFirst(new_list);
			RemoveDuplicateMembers( &SetList, arg_p );
			RemoveDuplicateMembers( &new_list, arg_p );

	 		
			if( arg_p->limit == arg_p->LimitReached ){
//			 printf(" data %p \n", arg_p);
//printf("ones[0] after subsetsize %i  \n",ListSize(arg_p->SetList));
			
				//com_line* arg_t;
				//new_list = var_subset_find( new_list, arg_p);
				//SetList = var_subset_find( SetList, arg_p);
				com_line* data =  alloc_mem(arg_p->com_pool);
				//
				//arg_t							= malloc(sizeof(*arg_t));
				data->list				= copy_list(new_list,arg_p);
				data->SetList			= copy_list(SetList,arg_p);
				
				//release_mem(arg_t,arg_p->com_pool);
				data->set= NULL;
				// this is where the job for the multi threaded workload is to go
				data->ClauseCount				= arg_p->ClauseCount;
				data->VariableCount			= arg_p->VariableCount;
				data->clause_size				= arg_p->clause_size;
				data->set_var					= arg_p->set_var;
				data->link_pool				= arg_p->link_pool;
				data->node_pool				= arg_p->node_pool;
				data->com_pool					= arg_p->com_pool;
				data->job_mem					= arg_p->job_mem;
				data->set_variable			= arg_p->set_variable;
				data->variable_position		= arg_p->variable_position;
				data->var_tab					= arg_p->var_tab;
				data->clause_connections	= arg_p->clause_connections;
				data->ret_value				= 0;

				Evaluate( data);
				//ListAddWork(WorkerBees,Evaluate,arg_t);
				
		DeleteSet(&data->SetList, arg_p);
		DeleteSet(&data->list   , arg_p);
		//free(arg_t->set);
		if(data->ret_value==-1){
			arg_p->ret_value=-1;
			RemoveUntil( ListEnd, &SetList,arg_p);
			DeleteSet( &new_list, arg_p);
			release_mem(data, arg_p->com_pool);
			return;
		}
		release_mem(data, arg_p->com_pool);
		//exit(0);
			//	return;
				
			}else{
				eval = 0;
				if(new_list!=NULL  ) {
					

				//arg_p->limit++;
					//if(new_list==NULL) break;
					//printf(" lim %i \n",arg_p->limit);
					link_node* temp	= copy_list(Tried,arg_p);
					com_line* data 	= alloc_mem( arg_p->com_pool );
					data->set= NULL;
					data->list						= new_list;
					data->SetList					= SetList;;
					data->Tried						= temp;
					data->limit 					= arg_p->limit+1;
					data->LimitReached			= arg_p->LimitReached;
					data->ClauseCount				= arg_p->ClauseCount;
					data->VariableCount			= arg_p->VariableCount;
					data->clause_size				= arg_p->clause_size;
					data->set_variable			= arg_p->set_variable;
					data->set_var					= arg_p->set_var;
					data->link_pool				= arg_p->link_pool;
					data->node_pool				= arg_p->node_pool;
					data->com_pool					= arg_p->com_pool;
					data->job_mem					= arg_p->job_mem;
					data->variable_position 	= arg_p->variable_position;
					data->clause_connections	= arg_p->clause_connections;
					data->var_tab					= arg_p->var_tab;
					data->ret_value				= 0;
					//ListAddWork(WorkerBees,SeardchAndCollect,data);
	
					SearchAndCollect(data);
				
					DeleteSet(&temp, arg_p);
					
					if( data->ret_value==-1){
					arg_p->ret_value=-1;
					release_mem(data, arg_p->com_pool);
					RemoveUntil( ListEnd, &SetList,arg_p);
					DeleteSet( &new_list, arg_p);
						return;
					}
					release_mem(data, arg_p->com_pool);
					
					
					//free(arg_t->set);
					
					//release_mem(data, arg_p->com_pool);
					//arg_p->limit--;
					
				}else{
				
				com_line* data =  alloc_mem(arg_p->com_pool);
				//
				data->list				= copy_list(new_list,arg_p);
				data->SetList			= copy_list(arg_p->SetList,arg_p);
				
				// this is where the job for the multi threaded workload is to go
					data->ClauseCount				= arg_p->ClauseCount;
					data->VariableCount			= arg_p->VariableCount;
					data->clause_size				= arg_p->clause_size;
					data->set_var					= arg_p->set_var;
					data->link_pool				= arg_p->link_pool;
					data->node_pool				= arg_p->node_pool;
					data->com_pool					= arg_p->com_pool;
					data->job_mem					= arg_p->job_mem;
					data->set_variable			= arg_p->set_variable;
					data->variable_position		= arg_p->variable_position;
					data->var_tab					= arg_p->var_tab;
					data->clause_connections	= arg_p->clause_connections;
					data->ret_value				= 0;
					Evaluate( data);

				DeleteSet(&data->SetList, arg_p);
				DeleteSet(&data->list, arg_p);
				
				if(data->ret_value==-1){
					
					arg_p->ret_value=-1;
					release_mem(data, arg_p->com_pool);
					RemoveUntil( ListEnd, &SetList,arg_p);
					//DeleteSet( &SetList, arg_p);
					DeleteSet( &new_list, arg_p);
					//SetList=NULL;
					//new_list=NULL;
					
					return;
				
				}
				
				release_mem(data, arg_p->com_pool);
				
				}
			}
		
			//RemoveUntil( TriedSaved, Tried);
			
			RemoveUntil( ListEnd, &SetList,arg_p);
			
			DeleteSet(&new_list,arg_p);
			
			new_list=NULL;
			//DestroySet(LocalTried,arg_p);
			//LocalTried=NULL;
			// disjunct found, propagate back
			
		
		}
		
		if(arg_p->list->next!=NULL){
			arg_p->list=arg_p->list->next;
		}else{

			break;
		}
		
		
	}
		
	RemoveUntil( TriedSaved, &Tried, arg_p);
	
	if(arg_p->limit==2){
	
		//release_mem(arg_p, arg_p->com_pool);
	}else{
		
	}
	//DestroySet(arg_p->list, arg_p);
	//DestroySet(arg_p->Tried, arg_p);
	//DestroySet(data->SetList, arg_p);

	//DestroySet(arg_p->list, arg_p);
	//free (arg_p);
	//release_mem(arg_p, arg_p->com_pool);
	//return 1;
	return;

}

int eevaluate(int TestVariable,link_node* guessed, com_line* boss){

	link_node* SetList  = NULL;
	link_node* Tried	  = NULL;
	link_node* List	  = NULL;
	
	Assert_Variable( TestVariable);
	//	printf(" this is the stop %i  \n",ListSize ( SetList));	
	SetList = CopySet( abs(TestVariable) ,f_variable_position, SetList, boss);
	//SetList = CollectConnections(SetList,boss);
	List    = CopySet( abs(TestVariable) ,f_variable_position, List, boss);
	//printf(" this is the stop %i  \n",ListSize ( SetList));	
	int VariableCount = 0;
	
	link_node* count  = NULL;
	link_node* PartTotal  = CollectConnections(SetList,boss);
	link_node* total  	 = CollectConnections(PartTotal,boss);

	VariableCount     	 = CountUniqueVariables(total,boss);
	VariableCount   	   += CountUniqueVariables(PartTotal,boss);

	DeleteSet( &PartTotal,boss	 );
	PartTotal=NULL;
	PartTotal 				 = CollectConnections(total,boss);
	VariableCount   	   += CountUniqueVariables(PartTotal,boss);

	int ClauseCount		 = VariableCount + ListSize( SetList );
	
	DeleteSet( &total,    boss);
	DeleteSet( &PartTotal,boss);
	int eval		 		 = 0 ;
	int limit 			 = 2 ;
	int LimitReached	 = 3;
com_line* data = NULL;

	//data=  alloc_mem(boss->com_pool);
	//link_node* try_copy = copy_list(Tried);
	//data = malloc(sizeof(*data));
	data = alloc_mem( boss->com_pool );
	data->list						= copy_list(List   , boss);
	data->SetList					= copy_list(SetList, boss);
	data->Tried						= Tried;
	data->limit 					= limit;
	data->LimitReached			= LimitReached;
	data->ClauseCount				= ClauseCount;
	data->VariableCount			= VariableCount;
	data->clause_size				= boss->clause_size;
	data->variable_position 	= boss-> variable_position;
	data->link_pool				= boss->link_pool;
	data->node_pool				= boss->node_pool;
	data->com_pool					= boss->com_pool;
	data->job_mem					= boss->job_mem;
	data->set_var					= boss->set_var;
	data->set_variable			= boss->set_variable;
	data->clause_connections	= boss->clause_connections;
	data->var_tab					= boss->var_tab;
	data->ret_value				= 0;
	SearchAndCollect(data);
	///Evaluate(data);
	eval=data->ret_value;
	
	DeleteSet(&SetList, boss);
	DeleteSet(&List, boss);
	DeleteSet(&Tried, boss);
	DeleteSet(&data->SetList, boss);
	release_mem(data, boss->com_pool);
	//free(data);
	RemoveLastAssert();
	//reset(boss->link_node);
		return eval;
}	

void* Evaluate(void *arg_v){
com_line* arg_t= arg_v;

//usleep(10);
	
	//arg_t=(com_line*) arg_t;

	link_node* subset = copy_list(arg_t->SetList,arg_t);
	variable_pos* set = arg_t->set;
	 
	mpz_t		saved;
	mpz_init(saved);
	int check=0;
	subset = var_subset_find( subset, arg_t);
	//arg_t->clause_connections=clause_connections;

	int savedsize				 = 0;

	AddKnownVariables(subset,arg_t);
	
	savedsize	= ListSize(subset->first);
	

	RemoveDuplicateMembers( &subset , arg_t);
	
		
	map* Location	= NULL;
	map* Map 		= NULL;

	savedsize	= ListSize(subset->first);


	

	// Sort set		
	BinSort( &subset, arg_t );
	//subset = GroupSet( subset , arg_t );
	//subset = GroupSingles(subset,arg_t);
	subset = sort_match(subset);
	
//	printf("go \n");
	//subset=subset->first;
	check=ListSize(subset->first);
		
			if(savedsize!= check) {
			printf("savesize != lsit  %i %i \n", savedsize,check);
			exit(0);
		}
		
		int* GroupDictionary = CreateArray( arg_t->ClauseCount );
	//	if(arg_t==NULL) exit(0);
		
		SetNewIndex( subset , &(arg_t->set), GroupDictionary ,Map, &Location, arg_t);
		null_add( arg_t );
		char out[20]= "tst2";
		//Export( (arg_t->set) , out,arg_t);
		//printf(" sorted \n");
		//halt();
		
		solve(arg_t);
		

		mpz_set(saved, arg_t->clause_node->data);
		
		dispose(&arg_t->clause_node,arg_t);

		mpz_clear(arg_t->clause_node->removed);
		mpz_clear(arg_t->clause_node->data);
		//free(arg_t->clause_node);
		release_mem(arg_t->clause_node,arg_t->node_pool);

		//reset set
		ResetSolve( arg_t );
		
		DestroyArray(GroupDictionary,arg_t);
		//if( mpz_cmp_ui(clause_node->data,0) == 0 ){	 //printf(" eval ==-1\n"); return -1; }
		DeleteSet(&subset,arg_t);
	
		FreeMap( Map );
		FreeMap( Location );
		
	
		if( mpz_cmp_ui(saved,0) == 0 ){
				mpz_clear( saved );		
				arg_t->ret_value=-1;
				return NULL;
		}
		


		mpz_clear( saved );

//free(arg_t->set_var);
//free(arg_t->clause_size);

//free(arg_t->SetList);
//free(arg_t);
	// return 1;
	
}

link_node* BackTrack(link_node* guessed, com_line* boss){
		int eval =0;
		link_node* tmp=guessed->first->end;
		while(1){
						printf("list %li \n", tmp->data);
			
			if( tmp->data<0) 
				break;

			if( tmp->previous == NULL){
				printf(" no solotuions \n");
				exit(0);
			}
			
			tmp=tmp->previous;
		}
		
		printf("backtrack is go %li \n", tmp->data);
			
			//		printf(" halted\n");
			//halt();
		//RemoveUntil( tmp->data, &guessed->first->end,boss);
		
		
		while( abs(f_variable_connections[f_clause_count][1]) != abs(tmp->data) ){
		 	if(abs(f_variable_connections[f_clause_count][1]) == abs(guessed->first->end->data) ){
		 		RemoveMember( &guessed->first->end, boss);
		 		guessed=guessed->first->end;
		 	}
			RemoveLastAssert();		
		}
		RemoveLastAssert();
		eval = eevaluate( abs(guessed->first->end->data) ,guessed, boss);
	
		if( eval==-1){
			
			RemoveMember( &guessed->first->end, boss);
			guessed=guessed->first->end;
			BackTrack( guessed, boss);

			return(guessed->first->end);
			//return;
			
		}
		Assert_Variable( abs(guessed->first->end->data) );
		guessed->first->end->data = abs( guessed->first->end->data); 
		return guessed->first->end;
}


int FindUnset(link_node** tested,com_line* Coms, bool test[]){
variable_pos* node;
 // for each set
	for(int k=ones[0]; k> 0; k-- ){
	
		node = f_variable_position[ abs( f_variable_connections[ones[k]][1] ) ];
		
		while( 1 ){
		
			for(int variable=f_clause_size[node->clause];variable!=0; variable--){
				//if it's not been accounted for 
				if( test != NULL){
				 	if(test[ abs(f_variable_connections[node->clause][variable]) ] == 1)
						continue;
					test[ abs(f_variable_connections[node->clause][variable]) ] = 1;
				}
				//if it's not sett
				if( IsVariableSet[ abs(f_variable_connections[node->clause][variable]) ] == 0 ) 
					return  abs(f_variable_connections[node->clause][variable]) ; 
			}
			if( node->next == NULL) 
				break;
	 		node=node->next;
	 	}
	 	
 }
return( -1);

}



void reset_visited(bool test[], link_node* sub_con){
	while(1){
		test[abs(sub_con->data)]=0;
		if(sub_con->next==NULL) 
			break;
						
		sub_con->next;
							
	}
}



void init_graph(int ones[]){

char raw1[20]="saved2";
			rawex(raw1);
//raw();

//char raw[20]="raw";
//Export(set, raw);
//exit(0);
printf ("ones %i \n" , ones[0]);
com_line* boss = memory_init();
link_node* list=NULL;

variable_pos* temp_clause;	
int count=0;

layer* layer=set_layer(-1,2);


int k=0;
		
for(int i=1; i<=ones[0];i++){

	IsVariableSet[abs(f_variable_connections[ones[i]][1])]=1;
		
	if(f_variable_connections[ones[i]][1]>0){
		VariableSet[abs(f_variable_connections[ones[i]][1])]=1;
		}else{
		VariableSet[abs(f_variable_connections[ones[i]][1])]=0;
	}

}
		
//variable set is state:
//0 =-1
//1 =1

int f   = 1;
int TestVariable = 0;
int last =0;
bool first=0;
bool second =0;
bool TryAgain=0;

link_node* tested = NULL;
link_node* guessed=NULL;
link_node* nil = NULL;
link_node* sub_con=NULL;


	link_node* SetList  = NULL;
	link_node* Tried	  = NULL;
	link_node* List	  = NULL;
	
	
	 com_line* data;
	
int VariableCount;
bool redo=NULL;
int saved_removed = 0;
bool test[vsize] ={0};
bool blank[0];
int eval =0;
while(1){

	TestVariable = FindUnset(&tested,boss, test);
		//if( TestVariable==246703) exit(0);
	if( TestVariable == -1 ){

		printf(" guessed %i \n ", ones[0]);

		TestVariable = FindUnset(&nil, boss, NULL);
		printf("\n test variable %i \n", TestVariable);
	
		if( TestVariable == -1 ) {
			printf(" neg 1\n");
			break;
		}
		eval =0;
		eval = eevaluate(-TestVariable,guessed, boss);
		printf(" guessed eval %i \n",eval );
		if( eval==-1){
		
			eval = eevaluate(TestVariable,guessed, boss);
			printf(" guessed eval %i \n",eval );
			
			if( eval==-1){
				guessed=BackTrack(guessed, boss);
				TestVariable= guessed->end->data;
				
			}else{
				Assert_Variable(abs(TestVariable));
				guessed 	= link_append( abs(TestVariable) ,guessed, boss);
			}
			printf ("1\n");
		}else{
			printf(" 2 \n");
			Assert_Variable(-abs(TestVariable));
			guessed 	= link_append( -abs(TestVariable) ,guessed, boss);
		}
			//if(TestVariable==440) 
			//exit(0);
		printf(" guessed var %li \n", guessed->first->end->data);
		// reset bool for tested
		sub_con = ConnectedVariables( abs(TestVariable), boss);
		reset_visited(test, sub_con);
		DeleteSet(&sub_con, boss);
		sub_con=NULL;
		continue;
	
	}
	
	
	eval=0;
	
	printf("  %i to test %i \n", ones[0], TestVariable);

	eval = eevaluate(-TestVariable,guessed, boss);

	
	if( eval==-1) {first=1;}
	eval=0;
	
	printf("second \n");

	eval = eevaluate(abs(TestVariable),guessed, boss);	
	
	if( eval  == -1 ) second = 1;
	
	if( first == 1 && second == 1 ) {
		guessed=BackTrack( guessed, boss);
		first=0;
		second=0;
		sub_con = ConnectedVariables( abs(guessed->first->end->data), boss);
		reset_visited(test, sub_con);
		DeleteSet(&sub_con, boss);
		sub_con=NULL;
				continue;
	}
	
	if( first == 1 ){
		
		sub_con = ConnectedVariables( abs(TestVariable), boss);
		
		reset_visited(test, sub_con);

		//RemoveAfromB(sub_con, &tested, boss);
		
		DeleteSet(&sub_con,boss);
		
		sub_con=NULL;
		
		Assert_Variable(abs(TestVariable) );
		
		printf("count %i, variable %i \n",ones[0], abs(TestVariable));
		//halt();
		
		// This is for saving progress!
		
		//	raw();

		//char raw[20]="saved";
		
		//	Export(set, raw);
		//	ResetSolve(arg_t);
		
		first = 0;
	}
	
	if(second == 1){
		
		sub_con = ConnectedVariables( abs(TestVariable), boss);

		reset_visited(test, sub_con);


		//RemoveAfromB(sub_con, &tested, boss);
		DeleteSet(&sub_con,boss);
		sub_con=NULL;
		
		Assert_Variable(-abs(TestVariable) );
		printf("count %i, variable %i \n",ones[0], -abs(TestVariable));
		
		
		//	raw();

		//	char raw[20]="saved";
		//	Export(set, raw);
		//	ResetSolve(arg_t);
				
		second =0;

	}
	
	
	//if(ones[0]==50) exit(0);
	
}

printf("fini %i %i\n",f_clause_count, ones[0]);
set=NULL;
		//raw(boss);


			char raw[20]="saved";
			select_var_print(raw);
			char raw2[20]="raw";

			rawex(raw2);
//			Export(set->first, raw,boss);
	//		ResetSolve(boss);
			
			
			

//	AbortWork(WorkerBees);
	DeleteSet( &tested, boss);

/*
	//printf("end count %i %i\n", f_variable_count,ones[0]);
	FILE* fp2;
	fp2=fopen("out3.cnf","w");
	for(int i=1;i<=f_clause_count;i++)
	{
		for(int k=f_clause_size[i];k!=0;k--){
			//fprintf(fp2,"%i ", f_variable_connections[i][k]);
		}

		fprintf(fp2,"0\n");
	}
	fclose (fp2);
*/
	
	//WaitForWorkers(WorkerBees);
	
return;
	exit(0);
}




