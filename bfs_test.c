
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
while(1){

	for(int variable=clause_size[(iter)->clause]; variable!=0;variable--){
		fprintf(fp2,"%i ", variable_connections[(iter)->clause][variable]);
	}

	fprintf(fp2,"0\n");

	if((iter)->next!=NULL){
		(iter)=(iter)->next;
	}
	else{break;}
}
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

int SearchAndCollect(link_node* list, link_node* SetList,link_node* Tried, int limit, int LimitReached,int GroupDictionary[], int TestVariable, map* Map, variable_pos** Group , bool TryAll){

printf(" 		cluase %i \n", TestVariable);
limit++;

int TriedSaved=0;
	link_node*	TempList = NULL;
	int ListEnd = SetList->first->end->data;

	if(Tried!=NULL && Tried->first->end!=NULL){
		TriedSaved=Tried->first->end->data;
	}

	int eval =0;

	list=list->first;
	while( list!=NULL ){
	link_node* 	NewList  = NULL;
	printf(" list->data %i\n", list->data);

		for( int clause = f_clause_size[list->data]; clause!=0; clause--){
			if(	ExistInSet( abs(f_variable_connections[list->data][clause]), Tried) == 1 ) continue;
			
			Tried = link_append( abs(f_variable_connections[list->data][clause]) , Tried);

			// copy all of the places of variable
			NewList = CopySet( abs(f_variable_connections[list->data][clause]) ,f_variable_position, NewList);
			SetList = CopySet( abs(f_variable_connections[list->data][clause]) ,f_variable_position, SetList);
			
		}
				NewList = RemoveDuplicateMembers( &NewList );
		
		if( NewList==NULL ) { 
			printf(" null \n");
			if(list->next!=NULL){ 
				list=list->next;
				continue;
			}
			
			break;
		}

		SetList = RemoveDuplicateMembers( &SetList );
		
		
		if( limit == LimitReached ){
			
			printf(" limit reached %i \n",limit);
			
			eval = 0;
			eval = Evaluate( SetList,  Tried, TestVariable, Map, Group , GroupDictionary, TryAll);
			printf(" limit num %i %i \n",limit,ListSize(SetList));
			printf(" eval %i \n", eval);
			if(TryAll==0){
//halt();
			}
		}else{
		eval = 0;
			
			printf("limit %i \n", limit);
			
			eval = SearchAndCollect(list, SetList, Tried, limit, LimitReached, GroupDictionary, TestVariable, Map, Group ,TryAll);
	limit--;
		}
		
		
		DestroySet( NewList);

		if(SetList==NULL) return 1;

		SetList=RemoveUntil( ListEnd, SetList);
		
		//debug_list(SetList);
		//Tried=RemoveUntil( TriedSaved, Tried);
		
		if( eval == -1) {
			printf("halt eval %i \n", eval);
			//exit(0);
			return -1;
		}


		if(list->next!=NULL){

			list=list->next;
		}else{
		printf(" ding \n");
			break;
		}

		/*
		DeleteIndex( Group ,VariableCount);
		FreeMap(Map);
		Bisector=Bisector->next;
		*/
		

	}
	printf("returned \n");
	return 1;

}

int Evaluate(link_node* SetList, link_node* Tried, int TestVariable, map* Map, variable_pos** Group , int GroupDictionary[], bool TryAll){

	map* MapLocation = NULL;

	mpz_t		saved;
	mpz_init(saved);
	
	
	//CreateGroup( SetList, Group, &MapLocation , &Map );

	
	link_node* FoundSet = NULL;
	
	link_node* subset = NULL;

	// For Each clause, bisect by variable member, once
	//while(node!=NULL){
		link_node* ThisSet  = NULL;
		
/*
		for( int variable=f_clause_size[node->data]; variable!=0; variable--){
			//	if( ExistInSet( abs(f_variable_connections[node->data][variable]),Tried) == 1 ) continue;

			subset = FindMembers( MapGetValue(abs(f_variable_connections[node->data][variable]), Map), Group, subset ,GroupDictionary,Map);

		}
*/

	subset=copy_list(SetList);
	AddKnownVariables(subset);
	subset = RemoveDuplicateMembers( &subset );
	map* Location=NULL;
			
	if(TryAll==0){
		
		//subset=RemoveKnownVariable(subset);

		if( ListSize(subset)==0 ){
			return 0;
		}
		
		printf("subsetsize %i %i \n",ListSize(subset),ListSize(SetList));
		
		subset = GroupTogether( subset );
		subset = GroupSingles( subset );
		SetNewIndex( subset , &set, GroupDictionary ,Map, &Location);
		//	SetIndex_LH( subset , &set, GroupDictionary ,Map, &Location);
		//RemoveDisjoint(subset,GroupDictionary, Location, &set);

		create_new_clause(set);

		AddToClause( TestVariable  , set );

		null_add( &set );
		if(ListSize(subset)>100){
		
					}
		printf("subsetsize %i %i \n",count_var_pos(set),ListSize(SetList));

		solve();
		mpz_set(saved, clause_node->data);
		dispose(&clause_node);
				if( mpz_cmp_ui(saved,0) == 0 ){
					char out[20]= "tst";
					Export( (set) , out);
					}
		gmp_printf(" clause_node->data %Zd\n ", clause_node->data);
				
		null_remove( &set );

		RemoveFromClause( clause_count, &set );
				
		//reset set
		ResetSolve();

		//if( mpz_cmp_ui(clause_node->data,0) == 0 ){	 printf(" eval ==-1\n"); return -1; }
		
		if( mpz_cmp_ui(saved,0) == 0 ){
					
			//DeleteList( &Tried );

			//DestroyArray( GroupDictionary );
			//DeleteIndex( Group ,sizeof(Group) );
			//FreeMap( Map );
			//FreeMap( MapLocation );
				
			FreeMap( Map );
			FreeMap( MapLocation );
			FreeMap( Location );
			
			DestroySet(subset);
			mpz_clear( saved );
			return -1;
		}
	}else{
	

		link_node* IsChecked=NULL;
		link_node* iter = subset->first;
		while(iter!=NULL){
		
		
		printf("subsetsize %i %i \n",ListSize(subset),ListSize(SetList));
	
		//AddKnownVariables(subset);
		// sort algos		
		///subset = BinSort( subset );
	//	subset = RemoveDuplicateMembers( &subset );
	//	subset = GroupTogether( subset );
	//	subset = GroupSingles( subset );
		SetNewIndex( subset , &set, GroupDictionary ,Map, &Location);
		//	SetIndex_LH( subset , &set, GroupDictionary ,Map, &Location);
		//RemoveDisjoint(subset,GroupDictionary, Location, &set);
		
		null_add( &set );

		if( ListSize(subset) > 100){

		}

		solve();
		
		if( mpz_cmp_ui(clause_node->data,0) ==0){
				printf(" this halt \n");
		char out[20]= "tst";
		Export( (set) , out);
			printf ("backtrack error\n"); 
			exit(0);
			return -1;
			exit(0);
		}
		mpz_set(saved, clause_node->data);
		dispose(&clause_node);
				
		gmp_printf(" 	start clause_node->data %Zd\n ", clause_node->data);
						
		null_remove( &set );

		//reset set
		ResetSolve();

		
		
			for( int variable = f_clause_size[iter->data]; variable!=0 ; variable--){
			
				printf( "var %i  \n",f_variable_connections[iter->data][variable]);
				
				if( IsVariableSet[ abs(f_variable_connections[iter->data][variable ]) ] ==1)			  continue;
				if( ExistInSet( abs(f_variable_connections[iter->data][variable]), IsChecked)	 == 1 ) continue;
				
				IsChecked=link_append(abs(f_variable_connections[iter->data][variable]), IsChecked);
				
				
				AddKnownVariables(subset);
				subset = RemoveDuplicateMembers( &subset );
				subset = GroupTogether( subset );
				subset = GroupSingles(subset);
				//variable_node* this = CollectConnections(subset);
				SetNewIndex( subset , &set, GroupDictionary ,Map, &Location);
				create_new_clause(set);
				
				printf("subsetsize %i %i \n",count_var_pos(set),ListSize(SetList));
				
				
				AddToClause( -abs(f_variable_connections[iter->data][variable])  , set );
		
				null_add( &set );


				solve();
					printf(" this halt \n");
		char out[20]= "tst";
		Export( (set) , out);
		null_add( &set );

				if( mpz_cmp(saved,clause_node->data) ==0){

					Assert_Variable( -abs(f_variable_connections[iter->data][variable]));
									if( abs(f_variable_connections[iter->data][variable]) ==165 ) exit(0);
						printf (" asserted %i \n", abs(f_variable_connections[iter->data][variable]));
					printf( "set! %i \n", iter->data);
					
					//exit(0);
				}
				if( mpz_cmp_ui(clause_node->data,0) == 0){
					printf("not_set\n");
								if( abs(f_variable_connections[iter->data][variable]) ==165 ) exit(0);
						printf (" asserted %i \n", abs(f_variable_connections[iter->data][variable]));
					Assert_Variable( abs(f_variable_connections[iter->data][variable]));
				}

				
				dispose(&clause_node);
				
				gmp_printf(" clause_node->data %Zd\n ", clause_node->data);
						
				null_remove( &set );

				RemoveFromClause( clause_count, &set );
						
				//reset set
				ResetSolve();
			

			//if( mpz_cmp_ui(clause_node->data,0) == 0 ){	 printf(" eval ==-1\n"); return -1; }
				if( mpz_cmp_ui(saved,0) == 0 ){
							
					//DeleteList( &Tried );

					//DestroyArray( GroupDictionary );
					//DeleteIndex( Group ,sizeof(Group) );
					//FreeMap( Map );
					//FreeMap( MapLocation );
						
					FreeMap( Map );
					FreeMap( MapLocation );
					FreeMap( Location );
					
					DestroySet(subset);
					mpz_clear( saved );
					return -1;
				}
				
			}
			if(iter->next==NULL) break;
			iter=iter->next;
		}
		
		DestroySet(IsChecked);
	
	
	}
		
	FreeMap( Map );
	FreeMap( MapLocation );
	FreeMap( Location );
		
	DestroySet(subset);
	mpz_clear( saved );
	return 1;
}

int Prove(){	

	link_node* 	Set=NULL;
	variable_pos*	node;
	int eval=0;

	for(int i=1; i<ones[0];i++){
			
		node=f_variable_position[ abs( f_variable_connections[ones[i]][1] ) ];
	//	printf("\nnode dat %i %i %i \n", ones[i], node->clause, f_variable_connections[ones[i]][1]);


		while(node!=NULL){

			
			//this finds the unset variables linked to set variables
			for(int variable=f_clause_size[node->clause];variable!=0; variable--){
				if( IsVariableSet[ abs(f_variable_connections[node->clause][variable]) ]==1)	continue;
				eval=0;
//				printf(" all node dat %i %i %i \n", node->clause, f_variable_connections[node->clause][variable], - abs(f_variable_connections[node->clause][variable]));
				// unset-> variable f_variable_clause[node->clause][variable]
//				eval = Evaluate( - abs(f_variable_connections[node->clause][variable]));
//printf(" asserted %i \n", abs(f_variable_connections[node->clause][variable]));
				if( eval == 1 ) { 
					IsVariableSet[ abs(f_variable_connections[node->clause][variable]) ] = 1 ;
					
						printf (" asserted %i \n", abs(f_variable_connections[node->clause][variable]));
					Assert_Variable( - abs(f_variable_connections[node->clause][variable]) ) ;
					VariableSet  [abs(variable_connections[node->clause][variable])] = 0 ;
					continue;
				}
				if( eval==-1) {
				
		//			eval = Evaluate( abs(f_variable_connections[node->clause][variable]));
					
					if( eval ==1 ) {
						IsVariableSet[ abs(f_variable_connections[node->clause][variable]) ] = 1 ;
						VariableSet  [abs(variable_connections[node->clause][variable])] =  1;
						printf (" asserted %i \n", abs(f_variable_connections[node->clause][variable]));
						Assert_Variable( abs(f_variable_connections[node->clause][variable]));
					}
					if( eval == -1 ) {
						char out[]= "out";
						
						printf(" illogical assignment\n");
						exit(0);
					}
				}
				
			}
			node=node->next;
			
		}
	}
	printf(" exited\n");
exit(0);
}

int FindUnset(){
variable_pos* node;
 // for each set
	for(int k=1; k<= ones[0]; k++ ){
		node=f_variable_position[ abs( f_variable_connections[ones[k]][1] ) ];
		while(node!=NULL){
			for(int variable=f_clause_size[node->clause];variable!=0; variable--){
				if( IsVariableSet[ abs(f_variable_connections[node->clause][variable]) ] == 0 ) { return  abs(f_variable_connections[node->clause][variable]) ; }
			}
	 		node=node->next;
	 	}
	 	
 }
printf("the end \n");
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

//Prove();

int f   = 1;
int TestVariable = 0;
int last =0;

while(1){

//************************************************************************************
	for(f;f<=(ones[0]);f++){
printf("*************** ones %i \n",f_variable_connections[ones[f]][1]);
				TestVariable = f_variable_connections[ones[f]][1];
		

		link_node* SetList  = NULL;
		link_node* Tried	  = NULL;
		link_node* List	  = NULL;
		
		SetList = CopySet( abs(TestVariable) ,f_variable_position, SetList);
		List    = CopySet( abs(TestVariable) ,f_variable_position, List);

		int VariableCount = 0;
		
		link_node* count  = NULL;
		link_node* PartTotal  = CollectConnections(SetList);
		link_node* total  	 = CollectConnections(PartTotal);
		VariableCount     	 = CountUniqueVariables(total);
		VariableCount   	   += CountUniqueVariables(SetList);

		int ClauseCount   = ListSize( total ) + ListSize( SetList );
		
		DeleteSet( &total );
		DeleteSet( &PartTotal );
		map* Map = NULL;
		

		variable_pos**	Group 			 =	CreateIndex( VariableCount );
		int* 				GroupDictionary = CreateArray( ClauseCount );

		int eval=0;
		int limit =2 ;
		int LimitReached=3;
		
		printf(" start size %i\n ", ListSize(List));
		
		eval = SearchAndCollect(List, SetList, Tried, limit, LimitReached, GroupDictionary, TestVariable, Map, Group, 1 );
		if (eval==-1){
			printf(" back track  %i last variable assigned: %i\n",TestVariable,last );
			exit(0);
			}	
	
		DestroySet( List );
		DestroySet( SetList );
		DestroySet( Tried );
		DeleteIndex( Group ,VariableCount);

	}
	
	//this is where the guess code goes
	//halt();
	// select an unset variable
	//	
//****************************************************************************

	TestVariable = FindUnset();
	last=TestVariable;
	if(TestVariable==-1)break;

if( abs(TestVariable)==165) exit(0);

	link_node* SetList  = NULL;
	link_node* Tried	  = NULL;
	link_node* List	  = NULL;
	
	SetList = CopySet( abs(TestVariable) ,f_variable_position, SetList);
	List    = CopySet( abs(TestVariable) ,f_variable_position, List);

	int VariableCount = 0;
	
	link_node* count  = NULL;
	link_node* PartTotal  = CollectConnections(SetList);
	link_node* total  	 = CollectConnections(PartTotal);
	VariableCount     	 = CountUniqueVariables(total);
	VariableCount   	   += CountUniqueVariables(SetList);

	int ClauseCount   = ListSize( total ) + ListSize( SetList );
	
	DeleteSet( &total );
	DeleteSet( &PartTotal );
	map* Map = NULL;
	

	variable_pos**	Group 			 =	CreateIndex( VariableCount );
	int* 				GroupDictionary = CreateArray( ClauseCount );

	int eval=0;
	int limit =2 ;
int LimitReached=4;
	
	printf(" start size %i\n ", ListSize(List));
	
	eval = SearchAndCollect(List, SetList, Tried, limit, LimitReached, GroupDictionary, -abs(TestVariable), Map, Group, 0 );
	DestroySet( List );
	DestroySet( SetList );
	DestroySet( Tried );
	DeleteIndex( Group ,VariableCount);

	//FreeMap(Map);
	


	printf(" i %i %i eval %i\n",IsVariableSet[abs(TestVariable)], TestVariable,eval);
	//if(abs(TestVariable)==500)exit(0);
	if(eval==1){
		printf(" this is the var set %i \n", -abs(TestVariable));
		Assert_Variable( -abs(TestVariable)	 );
		
	}
	
	if(eval==-1){
	//if(abs(TestVariable)==5)exit(0);
eval=0;	link_node* SetList  = NULL;
	link_node* Tried	  = NULL;
	link_node* List	  = NULL;
	
	SetList = CopySet( abs(TestVariable) ,f_variable_position, SetList);
	List    = CopySet( abs(TestVariable) ,f_variable_position, List);

	int VariableCount = 0;
	
	link_node* count  = NULL;
	link_node* PartTotal  = CollectConnections(SetList);
	link_node* total  	 = CollectConnections(PartTotal);
	VariableCount     	 = CountUniqueVariables(total);
	VariableCount   	   += CountUniqueVariables(SetList);

	int ClauseCount   = ListSize( total ) + ListSize( SetList );
	
	DeleteSet( &total );
	DeleteSet( &PartTotal );
	map* Map = NULL;
	

	variable_pos**	Group 			 =	CreateIndex( VariableCount );
	int* 				GroupDictionary = CreateArray( ClauseCount );

	int eval=0;
	int limit =2 ;
int LimitReached=4;
	printf(" start size %i\n ", ListSize(List));
	
	eval = SearchAndCollect(List, SetList, Tried, limit, LimitReached, GroupDictionary, abs(TestVariable), Map, Group, 0 );
	DestroySet( List );
	DestroySet( SetList );
	DestroySet( Tried );
	DeleteIndex( Group ,VariableCount);
		if(eval==-1){ 
		 	printf("	end illogical %i %i \n",ones[0], (TestVariable));
		 	exit(0);;
		}
		
		Assert_Variable( abs(TestVariable)	 );


	}
	
	printf(" end of eval %i \n", ones[0]);
}	
	
	for (int i=1; i<=ones[0];i++){
//	printf (" %i \n", f_variable_connections[ones[i]][1]);
	
	
	}
	
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
			if(list==NULL){ printf("error \n"); exit(0);}

			list=list->first->end;
			while(list->data!=end_clause){
				if(clause_visited[list->data]==0){
					printf(" this unset vist %i\n",list->data);
					exit(0);
				}
				clause_visited[list->data]=0;
				pop_link(&list);
				//(*set)=(*set)->previous;
			}
			list=list->first->end;
		}

		if(first->next==NULL){break;printf(" null next %i %i \n", first->data, end_clause);exit(0);}
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
//gmp_printf("clause %i Data %Zd  Removed%Zd \n",clause_node->clause,clause_node->data,clause_node->removed);


//debug_pos(set);
//debug(clause_node);


//set_var[abs(f_variable_connections[ones[i]][1])]=0;
//var_tab_del(&set->first->clause);

//ReSetOnes();


