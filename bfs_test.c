
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#include <string.h>
#include "map.h"
#include "var_pos.h"

#include "stest.h"
#include "clause_func.h"
#include "stest.h"
#include <gmp.h>
#include "llist.h"
#include "bfs_s.h"
#include "infini_tree.h"

#include "index.h"





// find all the set variables
//	ones[];
/*
if(ones[0]==0) //printf(" there are no set variables\n"); exit(0);
*/

int cnt=0;
int t_count=0;
variable_pos* set;
variable_pos* tmp;
	variable_pos* set_copy=NULL;
bool clause_visited[csize]={0};
//bool inline_visited[8000]={0};
int  new_old_clause[vsize]={0};
bool counted_set[csize]={0};

bool CountVariable[vsize]={0};

bool KnownSet[vsize]={0};
bool IsVariableSet[vsize]={0};
bool VariableSet[vsize]={0};

variable_pos* variable_list=NULL;

void Export(variable_pos* iter, char title[]){

if(iter==NULL){printf("null export \n");exit(0);}
char file[]=".cnf";
strcat(title,file);

//printf(" %s \n", title);

iter=iter->first;
FILE* fp2;
fp2=fopen(title,"w");
fprintf(fp2, "p cnf 1 %i\n", f_clause_count);
while(1){
	if( clause_size[(iter)->clause]!=0){
	for(int variable=clause_size[(iter)->clause]; variable!=0;variable--){
		fprintf(fp2,"%i ", variable_connections[(iter)->clause][variable]);
	}
	

	fprintf(fp2,"0\n");
	}
	if((iter)->next!=NULL){
		(iter)=(iter)->next;
	}
	else{break;}
}
	fprintf(fp2,"0\n");

fclose (fp2);
}


void raw(){

	for(int clause=1;clause<=f_clause_count;clause++){

	set=copy_clause(clause, set,new_old_clause);
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
	
void SetOnes(){
int k=0;
	for(int variable=1;variable<=ones[0];variable++){
		if(variable_position[abs(f_variable_connections[ones[variable]][1])]->clause==0 ){
			continue;
		}

			k=f_variable_connections[ones[variable]][1];
			set_variable[abs(k)]=k;
			counted_set[abs(k)]=1;
			set_var[abs(k)]=1;
			var_tab[abs(k)]++;
			counted_for++;
	}

}


void ReSetOnes(){
int k=0;
	for(int variable=1;variable<=ones[0];variable++){
		if(variable_position[abs(f_variable_connections[ones[variable]][1])]->clause==0 ){
			continue;
		}

			k=f_variable_connections[ones[variable]][1];
			set_variable[abs(k)]=0;
			counted_set[abs(k)]=0;
			set_var[abs(k)]=0;
			var_tab[abs(k)]--;
			counted_for--;
			
	}

}

int SearchAndCollect(link_node* list, link_node* SetList,link_node** Tried, int limit, int LimitReached,int ClauseCount, int TestVariable , bool TryAll, int VariableCount){

	int TriedSaved=0;

	link_node*	TempList = NULL;
	
	int ListEnd = SetList->first->end->data;

	if((*Tried)!=NULL && (*Tried)->first->end!=NULL){
		TriedSaved=(*Tried)->first->end->data;
	}

	int eval =0;
	link_node* new_list =NULL;
	
	if(list==NULL) return 0;

	link_node* LocalTried=NULL;
		
	while( list!=NULL ){

		for( int clause = f_clause_size[list->data]; clause!=0; clause--){
			if(	ExistInSet( abs(f_variable_connections[list->data][clause]), (*Tried)) == 1 || ExistInSet( abs(f_variable_connections[list->data][clause]), (*Tried))==1){
				continue;
			}
			
			LocalTried = link_append( abs(f_variable_connections[list->data][clause]) , LocalTried);
			
			(*Tried) = link_append( abs(f_variable_connections[list->data][clause]) , (*Tried));

			// copy all of the places of variable
			SetList  = CopySet( abs(f_variable_connections[list->data][clause]) ,f_variable_position, SetList);
			new_list = CopySet( abs(f_variable_connections[list->data][clause]) ,f_variable_position, new_list);
	
			SetFirst(SetList);
			SetFirst(new_list);
			RemoveDuplicateMembers( &SetList );
			RemoveDuplicateMembers( &new_list );
			
		
			if( limit == LimitReached ){
				
				eval = 0;
				
				eval = Evaluate( SetList , ClauseCount, VariableCount);
				//printf(" limit num %i %i \n",limit,ListSize(SetList));
				//printf(" eval %i \n", eval);
				if(TryAll==0){
				}
				
				if( eval == -1) {
					
					RemoveUntil( TriedSaved, Tried);
					RemoveUntil( ListEnd, &SetList);
					DestroySet(LocalTried);
					DestroySet(new_list);
			
					return -1;
				}
				
			}else{
				eval = 0;
				if(new_list!=NULL) {

					limit++;
					eval = SearchAndCollect(new_list, SetList, Tried, limit, LimitReached, ClauseCount, TestVariable ,TryAll, VariableCount);
					limit--;
				}
				else{
					eval = Evaluate( SetList , ClauseCount, VariableCount);
				
				}
			}

			RemoveUntil( TriedSaved, Tried);
			RemoveUntil( ListEnd, &SetList);
			
			DestroySet(new_list);
			
			new_list=NULL;
		
			// disjunct found, propagate back
			
			if( eval == -1) {
				DestroySet(LocalTried);
				DestroySet(new_list);
			
				return -1;
			}
		}
		
		if(list->next!=NULL){
			list=list->next;
		}else{
			break;
		}
		
	}
		
	DestroySet(LocalTried);
	RemoveUntil( TriedSaved, Tried);
	
	return 1;

}

int Evaluate(link_node* SetList, int ClauseCount, int VariableCount){

	mpz_t		saved;
	mpz_init(saved);
	
	link_node* subset = NULL;
		
	int savedsize=0;
	
	subset = copy_list(SetList);
	
	AddKnownVariables(subset);
	
	RemoveDuplicateMembers( &subset );
	
	map* Location	= NULL;
	map* Map 		= NULL;

		if( saved == 0 ){
			exit(0);
		//	return 0;
		}

		savedsize	= ListSize(subset->first);

		// Sort set		
		BinSort( &subset );

		subset = GroupSet( subset );

		GroupSingles(subset);
		
		CheckFirstNode(subset);

		if(savedsize!= ListSize(subset)) {
			printf("1 not the same %i %i \n", savedsize,ListSize(subset));
			exit(0);
		}
		
		int* GroupDictionary = CreateArray( ClauseCount );


		SetNewIndex( subset , &set, GroupDictionary ,Map, &Location);

		null_add( &set );

		//printf("ones[0] %i after subsetsize %i %i variable %i is it set %i \n",ones[0],count_var_pos(set),ListSize(SetList),TestVariable,IsVariableSet[abs(TestVariable)]);
		//char out[20]= "tst";
		//Export( (set) , out);

		solve();

		mpz_set(saved, clause_node->data);
		
		dispose(&clause_node);

		mpz_clear(clause_node->removed);
		mpz_clear(clause_node->data);
		free(clause_node);

		set=set->first->end;
				
		DestroyArray(GroupDictionary);
		
		//reset set
		ResetSolve(set);
	
		//if( mpz_cmp_ui(clause_node->data,0) == 0 ){	 //printf(" eval ==-1\n"); return -1; }
		DestroySet(subset);
		if( mpz_cmp_ui(saved,0) == 0 ){
					
			//DeleteList( &Tried );

//			DestroyArray( GroupDictionary );
			//DeleteIndex( Group ,sizeof(Group) );
				
			FreeMap( Map );
			//FreeMap( MapLocation );
			FreeMap( Location );
				
		//	DestroySet(subset);
			mpz_clear( saved );
			return -1;
		}
	
	FreeMap( Map );
	//FreeMap( MapLocation );
	FreeMap( Location );
		
	mpz_clear( saved );
	return 1;
}



int FindUnset(link_node** tested){
variable_pos* node;
 // for each set
	for(int k=1; k<= ones[0]; k++ ){
		node			 =		f_variable_position[ abs( f_variable_connections[ones[k]][1] ) ];
		while( 1 ){
		
			for(int variable=f_clause_size[node->clause];variable!=0; variable--){
				//if it's not been accounted for 
				if( ExistInSet( abs(f_variable_connections[node->clause][variable]), *tested)==1) continue;
					*tested 	= link_append( abs(f_variable_connections[node->clause][variable]) ,*tested);
				//if it's not sett
				if( IsVariableSet[ abs(f_variable_connections[node->clause][variable]) ] == 0 ) return  abs(f_variable_connections[node->clause][variable]) ; 
			}
			if( node->next==NULL) break;
	 		node=node->next;
	 	}
	 	
 }
return( -1);

}





void init_graph(int ones[]){

//raw();

//char raw[20]="raw";
//Export(set, raw);
//exit(0);
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

bool redo=NULL;

while(1){

//************************************************************************************
//	for(f;f<=(ones[0]);f++){
////150

		TestVariable = FindUnset(&tested);
		
	if(TestVariable==-1 ){
		//break;
		TestVariable = FindUnset(&nil);
		
		sub_con = CopySet( abs(TestVariable) ,f_variable_position, sub_con);;
		sub_con = CollectConnections(sub_con);
		sub_con = CollectConnections(sub_con);
		RemoveAfromB(sub_con, &tested);
		DestroySet(sub_con);
		sub_con=NULL;
	
		printf(" to test %i \n", TestVariable);
		
		if(TestVariable==-1) break;
		
		Assert_Variable(-abs(TestVariable));
		guessed 	= link_append( -abs(TestVariable) ,guessed );
		printf(" guessed %li ! \n",guessed->first->end->data);
		
		continue;
	}

	link_node* SetList  = NULL;
	link_node* Tried	  = NULL;
	link_node* List	  = NULL;
	
	Assert_Variable( -abs(TestVariable));
		
	SetList = CopySet( abs(TestVariable) ,f_variable_position, SetList);
	List    = CopySet( abs(TestVariable) ,f_variable_position, List);

	int VariableCount = 0;
	
	link_node* count  = NULL;
	link_node* PartTotal  = CollectConnections(SetList);
	link_node* total  	 = CollectConnections(PartTotal);

	VariableCount     	 = CountUniqueVariables(total);
	VariableCount   	   += CountUniqueVariables(PartTotal);

	DeleteSet( &PartTotal );
	PartTotal = CollectConnections(total);
	VariableCount   	   += CountUniqueVariables(PartTotal);

	int ClauseCount   = VariableCount + ListSize( SetList );
	
	DeleteSet( &total );
	DeleteSet( &PartTotal );


	int eval						= 0;
	int limit 					= 2;
	int LimitReached			= 2;
	
	eval = SearchAndCollect(List, SetList, &Tried, limit, LimitReached, ClauseCount, -abs(TestVariable), 0,VariableCount);
	
	DestroySet( List );
	DeleteSet( &SetList );
	DeleteSet( &Tried );

	RemoveLastAssert();
	
	if( eval==-1) {first=1;}
	
	 SetList  = NULL;
	 Tried	  = NULL;
	 List	  = NULL;
	
	Assert_Variable( abs(TestVariable));
		
	SetList = CopySet( abs(TestVariable) ,f_variable_position, SetList);
	List    = CopySet( abs(TestVariable) ,f_variable_position, List);

	VariableCount = 0;
	
	 count 			  = NULL;
	 PartTotal  	  = CollectConnections(SetList);
	 total			  = CollectConnections(PartTotal);

	VariableCount    = CountUniqueVariables(total);
	VariableCount   += CountUniqueVariables(PartTotal);

	DeleteSet( &PartTotal );
	
	PartTotal 		 = CollectConnections(total);
	VariableCount  += CountUniqueVariables(PartTotal);
	ClauseCount		 = VariableCount + ListSize( SetList );
	
	DeleteSet( &total );
	DeleteSet( &PartTotal );
	

	eval		 		 = 0 ;
	limit 			 = 2 ;
	LimitReached	 = 2 ;
	
	eval 				 = SearchAndCollect(List, SetList, &Tried, limit, LimitReached, ClauseCount, abs(TestVariable), 0, VariableCount );
	
	DestroySet	( List );
	DeleteSet	( &SetList );
	DeleteSet	( &Tried );
	
	
	RemoveLastAssert();
	
	if( eval  == -1 ) second=1;
	
	if( first == 1 && second ==1) {
	
		if( guessed == NULL){
			printf("invalid \n ");
			exit(0);
		}
		first  = 0;
		second = 0;
		redo   = 1;
		
		if (guessed->first->end->data<0){
		
			
			while(1){
			
				if(f_variable_connections[f_clause_count][1] == guessed->first->end->data
				&& f_clause_size[f_clause_count]==1){
				
					printf("changed guess %i !\n",abs( guessed->first->end->data));
					
					sub_con = CopySet( abs(guessed->first->end->data) ,f_variable_position, sub_con);;
					sub_con = CollectConnections(sub_con);
					sub_con = CollectConnections(sub_con);
					RemoveAfromB(sub_con, &tested);
					DestroySet(sub_con);
					sub_con=NULL;
					
					RemoveLastAssert();
					Assert_Variable( abs( guessed->first->end->data));
					guessed->first->end->data =  abs( guessed->first->end->data );
					
					break;
				}else{
					RemoveLastAssert();
				}
			}
		}else{
		//1096
			printf(" break! illogical\n");
			exit(0);
		}
		
		
	}
	
	if( first==1){
	
		sub_con = CopySet( abs(TestVariable) ,f_variable_position, sub_con);;
					sub_con = CollectConnections(sub_con);
					sub_con = CollectConnections(sub_con);
					RemoveAfromB(sub_con, &tested);
					DestroySet(sub_con);
					sub_con=NULL;
		Assert_Variable(abs(TestVariable) );
		printf("count %i, variable %i \n",ones[0], abs(TestVariable));
		raw();

		char raw[20]="saved";
		
		Export(set, raw);
		ResetSolve(set);
		first =0;
	}
	
	if(second==1){
			sub_con = CopySet( abs(TestVariable) ,f_variable_position, sub_con);;
		sub_con = CollectConnections(sub_con);
		sub_con = CollectConnections(sub_con);
		RemoveAfromB(sub_con, &tested);
		
		DestroySet(sub_con);
		sub_con=NULL;
		Assert_Variable(-abs(TestVariable) );
		printf("count %i, variable %i \n",ones[0], -abs(TestVariable));
		raw();

		char raw[20]="saved";
		Export(set, raw);
		ResetSolve(set);
				
		second =0;

	}
	
}	
	DeleteSet( &tested);


	printf("end count %i %i\n", f_variable_count,ones[0]);
	FILE* fp2;
	fp2=fopen("out3.cnf","w");

	for(int i=1;i<=f_clause_count;i++)
	{
		for(int k=f_clause_size[i];k!=0;k--){
			fprintf(fp2,"%i ", f_variable_connections[i][k]);
		}

		fprintf(fp2,"0\n");
	}
	fclose (fp2);
	
	exit(0);
}

void bfs_graph( layer* layer,link_node* list){
	if(list==NULL){printf("nullset\n");exit(0);}
	link_node* first=list->first;
	int end_clause=list->first->end->data;

	int end_var=variable_list->first->end->clause;


	variable_pos* iter;
	variable_pos* temp_var;
	//variable_pos* set_saved=(*set);
	variable_pos* temp_clause;
	variable_pos* var_clause;
	int count=0;
	bool skipped=0;

	while(first->data!=end_clause){
		for(int variable=f_clause_size[first->data];variable!=0;variable--)
		{

			if(CountVariable[abs(f_variable_connections[first->data][variable])]==0){

				variable_list=create_clause(abs(f_variable_connections[first->data][variable]),variable_list);
				CountVariable[abs(f_variable_connections[first->data][variable])]=1;
			}else{
				continue;
			}

			temp_clause=f_variable_position[abs(f_variable_connections[first->data][variable])];
			if(temp_clause==NULL){printf(" null temp_clause \n");exit(0);}

			while(temp_clause!=NULL){
				//printf("temp_clause->clause %i \n", temp_clause->clause);
				if(clause_visited[temp_clause->clause]==0 ){
				
					list 	= link_append(temp_clause->clause,list);
					
					clause_visited[temp_clause->clause] = 1;


					for(int variable2=f_clause_size[temp_clause->clause];variable2!=0;variable2--){

						if(CountVariable[abs(f_variable_connections[temp_clause->clause][variable2])] == 0){
							CountVariable[abs(f_variable_connections[temp_clause->clause][variable2])]=1;
							variable_list=create_clause(abs(f_variable_connections[temp_clause->clause][variable2]),variable_list);
						}
					}
			
				}	
				temp_clause=temp_clause->next;
			}

		//	deduce(list, &set);
			if(list==NULL){ //printf("error \n"); exit(0);
			}

			list=list->first->end;
			while(list->data!=end_clause){
				if(clause_visited[list->data]==0){
					printf(" this unset vist %li\n",list->data);
					exit(0);
				}
				clause_visited[list->data]=0;
				pop_link(&list);
				//(*set)=(*set)->previous;
			}
			list=list->first->end;
		}

		if(first->next==NULL){break;
		printf(" null next %li %i \n", first->data, end_clause);
		exit(0);
		}
		first=first->next;
	
	}

	variable_list=variable_list->first->end;

}


// code archive
//copy_clause(ones[i],set);
//printf(" ontes %i %i \n", ones[i],variable_connections[ones[i]][1]);
//exit(0);

//null_add(set);
//k=	f_variable_connections[ones[1]][1];
//create_new_clause();
//add_to_clause(k,set);

//SetOnes();

//free((clause_node));
//printf(" this is the count %i \n", count_node(clause_node->next_layer));
//clause_node=clause_node->next_layer;

//printf(" ones[i] %i \n", ones[i]);
////gmp_printf("clause %i Data %Zd  Removed%Zd \n",clause_node->clause,clause_node->data,clause_node->removed);


//debug_pos(set);
//debug(clause_node);


//set_var[abs(f_variable_connections[ones[i]][1])]=0;
//var_tab_del(&set->first->clause);

//ReSetOnes();


