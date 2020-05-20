
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#include <string.h>

#include "var_pos.h"
#include "map.h"
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

int Evaluate(int TestVariable){

	mpz_t		saved;
	mpz_init(saved);



	bool SearchedVariable[vsize] ={0};
	
	
	// This is a set-list
	link_node* 	SetList=NULL;
	
	map* Map = NULL;

	// copy all of the places of variable
	SetList=CopySet( TestVariable ,f_variable_position, SetList);

	int VariableCount=0;
	link_node* count = NULL;
	link_node* total =CollectConnections(SetList);
	VariableCount=CountUniqueVariables(total);
	//VariableCount+=CountUniqueVariables(SetList);
	
	int ClauseCount = ListSize(total)+ListSize(SetList);
	//printf(" clause %i listcount %i \n",ClauseCount,ListSize(total));
	DeleteSet(&total);


	// intiate group
	int*				GroupDictionary	= CreateArray(ClauseCount);

//	int				GroupDictionary[vsize]	=	{ 0 };
	variable_pos**	Group							=	CreateIndex( VariableCount );


	//SetList=AddConnections(SetList, SearchedVariable);
	//RemoveDuplicateMembers( &SetList );
	// Put the set into a group
	CreateGroup( SetList, Group, GroupDictionary , &Map );
	
	
//	

	
	link_node* node=SetList->first;
	// For Each clause, bisect by variable member, once
	while(node!=NULL){
		for( int variable=f_clause_size[node->data]; variable!=0; variable--){
			//check if the variable as been searched
			if( SearchedVariable[abs(f_variable_connections[node->data][variable])] == 1  || abs(f_variable_connections[node->data][variable]) ==abs(TestVariable) ) continue;
			
			SearchedVariable[abs(f_variable_connections[node->data][variable])]=1;
			link_node* subset = NULL;
	//		printf(" check not 0 - %i \n", MapGetValue(abs(f_variable_connections[node->data][variable]), Map));
			subset = FindMembers( MapGetValue(abs(f_variable_connections[node->data][variable]), Map), Group, subset ,GroupDictionary,Map);
			if( subset == NULL ) printf(" null subbsest \n");
			
				subset=RemoveDuplicateMembers( &subset );
			if( ListSize( subset ) == 0) {continue;}
			
			
			link_node* iter_set= subset->first;

			//Add all the correlating set clauses
			int CountedSet = 0;
			int CheckCount = 0;
			
			
			AddKnownVariables ( subset );
			
			
			//debug_list(subset);
			//Organize by size in to mainset
			
			if(subset==NULL) exit(0);			
			
		
	//		printf(" check\n");
			
			SetIndex_LH( subset, &set, GroupDictionary ,Map);
			
			//Export(set, out);
			create_new_clause(set);
			AddToClause( TestVariable  , set );

			(set) = (set)->first;

			null_add( &set );
			
			
	
	//	printf(" clause count %i %i %i \n", clause_count, f_variable_count,ones[0]);
char out[20]="out";
		//if(clause_count>=422) { Export(set, out);exit(0);}

			solve();
			
			if( mpz_cmp_ui(clause_node->data,0) == 0 ){
		//		char out[20]= "out";
		//		Export(set, out);
			}
			null_remove( &set );
			RemoveFromClause( clause_count, &set );
			mpz_set(saved, clause_node->data);
	//		gmp_printf(" saved %Zd \n", pnt->data);
			
			
			dispose(&clause_node);
			//reset set
			ResetSolve();
		//	FreeSet(&set);

			// this is a failed attempt
			if( mpz_cmp_ui(saved,0) == 0 ){
			
				mpz_clear( saved );
				return -1;
			}
		
		subset=subset->first;	
		while(1){
		
			if(subset->next==NULL){ free(subset);break;}

			subset=subset->next;
			free(subset->previous);
		}
		
		}
	
	node=node->next;
	}


	DeleteIndex( Group ,VariableCount);
	DestroyArray(GroupDictionary);
	mpz_clear( saved );
	//DeleteList(&SetList);
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
				eval = Evaluate( - abs(f_variable_connections[node->clause][variable]));

				if( eval == 1 ) { 
				IsVariableSet[ abs(f_variable_connections[node->clause][variable]) ] = 1 ;
				Assert_Variable( - abs(f_variable_connections[node->clause][variable]) ) ;
				VariableSet  [abs(variable_connections[node->clause][variable])] = 0 ;
				continue;
				}
				if( eval==-1) {
				
					eval = Evaluate( abs(f_variable_connections[node->clause][variable]));
					
					if( eval ==1 ) {
						IsVariableSet[ abs(f_variable_connections[node->clause][variable]) ] = 1 ;
						VariableSet  [abs(variable_connections[node->clause][variable])] =  1;
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

void deduce( link_node* list, variable_pos** set ){

	mpz_t		saved;
	mpz_init(saved);

	char out[20] = "out";

	char title[] = "out1";

	variable_pos* var_list;
	variable_pos* tmp;

	bool exp=0;
	var_list = variable_list;
	
	bool IsChecked[vsize]={0};
	int VariableCount=0;
	link_node* count = NULL;
	//count=CopySet( TestVariable ,f_variable_position, SetList);
	//count=CollectConnections(SetList);
	link_node* total =CollectConnections(list);
	VariableCount=CountUniqueVariables(total);
	VariableCount+=CountUniqueVariables(list);
	
	int ClauseCount = ListSize(total)+ListSize(list);
	//printf(" clause %i listcount %i \n",ClauseCount,ListSize(total));
	DeleteSet(&total);
	//halt();
	
	
	//this creates a translation of position from old placement to the new placement.
	// I.E. variables stay the same but get copied in to a two group
	// Only needs to be as big as the amount of places
	//int					group_b_translate[csize] = { 0 };
	int*				group_b_translate	= CreateArray(ClauseCount);
	//2:15
	variable_pos**	group_B						 = CreateIndex( VariableCount );

	int				variable						 = 0;
	int 				CheckCount					 = 0;

	int OldToNewVar[1000]={0};
	int NewToOldVar[1000]={0};
	
	map* Map=NULL;

	CreateGroup( list, group_B, group_b_translate, &Map );
//	printf(" check not 0 - %i %i\n", MapGetKey( 3, Map), f_variable_connections[list->data][variable]);
	list=list->first;





	while(list!=NULL){
	link_node* subset=NULL;
		for(int variable=f_clause_size[list->data];variable!=0;variable--){
		
			if( CountVariable[abs(f_variable_connections[list->data][variable])] == 1 ){
				continue;
			}
			
//			printf(" 1 check not 0 - %i %i\n", MapGetValue( abs(f_variable_connections[list->data][variable]), Map), f_variable_connections[list->data][variable]);
			
			subset = FindMembers( MapGetValue( abs(f_variable_connections[list->data][variable]), Map), group_B, subset ,group_b_translate, Map);
			
		}
		
		if( ListSize( subset ) == 0 ){
			
			if(list->next == NULL){		
				break;
			}

			list = list->next;
			continue;
		}
		

		//Add all the correlating set clauses
		int CountedSet = 0;
		subset=RemoveDuplicateMembers( &subset );

		link_node* iter_set= subset->first;

		link_node* temp_list=NULL;
		while( iter_set != NULL ){
			CountedSet  = 0;
			CheckCount	= 0;
			temp_list=NULL;
			
			if(iter_set->data==0) { printf("data=0 %i \n",iter_set->data);exit(0);}
			for ( int clause= f_clause_size[iter_set->data]; clause!=0; clause-- ) {
			
				if( IsVariableSet[ abs(f_variable_connections[iter_set->data ][clause]) ] ==1 ){
					CheckCount++;
					if( CheckSet( OnesPlace[abs(f_variable_connections[iter_set->data ][clause])] , subset ) ==0){
						CountedSet++;
		//				printf("f_variable_connections[iter_set->data ][clause] %i \n", f_variable_connections[iter_set->data ][clause]);
						temp_list = AddMember(	OnesPlace[abs(f_variable_connections[iter_set->data ][clause])] ,  temp_list );
						exp	 = 1;	
					}
				}
			}
			if(counted_set!=0) JoinSet( subset,temp_list);
			if( CheckCount == f_clause_size[iter_set->data] && CheckCount!=1 ){
	//			printf("* \n");
	//			printf("* %i \n",iter_set->data);
				//debug_list(iter_set);
				//RemoveThisMember( iter_set->data, &iter_set); 
			}
			if( CountedSet == f_clause_size[iter_set->data] ){
				for ( int clause = f_clause_size[iter_set->data]; clause!=0; clause--){

					//subset = RemoveLastMember( &subset );
				}
			}

		iter_set = iter_set->next;
		}
		
		// remove shared clauses
		RemoveDuplicateMembers( &subset );
		
		//order and implement into set
		SetIndex_LH( subset, set, group_b_translate,Map);

		(*set) = (*set)->first;
		


		null_add( set );
		
		if( clause_count >66 ){
	//		printf(" halted\n");
	//		char out[20]= "out";
	//		Export( (*set) , out);
	//		halt();
		}
		
//	printf (" clause count %i \n", clause_count);
		solve();
		
		mpz_set( saved, clause_node->data );

		null_remove( set );
		dispose( &clause_node );

		variable_pos* iter=(*set)->first;
		link_node* IsChecked=NULL;
		
		while( iter != NULL ){

			for( int variable = clause_size[iter->clause]; variable!=0; variable-- ){

				if( IsVariableSet[abs(variable_connections[iter->clause][variable])] == 1 ) continue; 
				if( ExistInSet( abs(variable_connections[iter->clause][variable]), IsChecked)	 == 1 ) continue;

				IsChecked=link_append(abs(variable_connections[iter->clause][variable]), IsChecked);
				// test this variable
				create_new_clause();
				AddToClause( abs( variable_connections[iter->clause][variable] ) , *set );
				null_add( set );

				solve();
				
				null_remove( set );
				
				RemoveFromClause( clause_count, &(*set) );
		
				if( mpz_cmp(clause_node->data,saved) == 0  &&  mpz_cmp_ui(clause_node->data,0) != 0 ){

					VariableSet  [abs(variable_connections[iter->clause][variable])] = 1;
					IsVariableSet[abs(variable_connections[iter->clause][variable])] = 1;
					CountVariable[abs(variable_connections[iter->clause][variable])] = 0;

					Assert_Variable( abs(variable_connections[iter->clause][variable]) );
		
					cnt++;
				}
				
				if( mpz_cmp_ui( clause_node->data, 0 ) == 0 ){

					dispose(&clause_node);

					create_new_clause();
					
					AddToClause( -(abs(variable_connections[iter->clause][variable])), *set );

					null_add(set);

					solve();
					
					if( mpz_cmp_ui(clause_node->data,0)==0){char this[]="error";Export(*set,this);printf("non logical soloution\n"); exit(0);}
					
					null_remove(set);
					
					RemoveFromClause(clause_count, &(*set));

					IsVariableSet[abs(variable_connections[iter->clause][variable])]	= 1;
						
					VariableSet[abs(variable_connections[iter->clause][variable])]		= 0;
						
					Assert_Variable(-(abs(variable_connections[iter->clause][variable])));

					CountVariable[abs(variable_connections[iter->clause][variable])]	= 0;
						
					cnt++;
				}

				dispose(&clause_node);
			}	
			
			if( iter->next == NULL ) break;

			iter=iter->next;			
		}
		
		(*set) = (*set)->first->end;
		//reset set
		while( (*set) != NULL ){
			RemoveFromClause( clause_count, set );
		}
		
		subset=subset->first;	
		
		while(1){
			if(subset->next==NULL){ free(subset);break;}

			subset=subset->next;
			free(subset->previous);
		}
		if(IsChecked!=NULL){
			DeleteSet(&IsChecked);
		}
		list = list->next;
	}

	DestroyArray(group_b_translate);
	DeleteIndex(group_B, VariableCount);
	mpz_clear( saved );
}



void init_graph(int ones[]){

	link_node* list=NULL;

	variable_pos* temp_clause;	
	int count=0;

	layer* layer=set_layer(-1,2);


	int k=0;
		
	for(int i=1; i<=ones[0];i++){

		KnownSet[abs(f_variable_connections[ones[i]][1])]=1;
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
int var = 0;

while(1){


	for(f;f<=(ones[0]);f++){

		//copy the original
		list=link_append(ones[f], list);
		clause_visited[ones[f]]=1;

		for(int variable=f_clause_size[ones[f]];variable!=0;variable--){

			if(CountVariable[abs(f_variable_connections[ones[f]][variable])]==0){
				variable_list=create_clause(abs(f_variable_connections[ones[f]][variable]),variable_list);

				//CountVariable[abs(f_variable_connections[ones[f]][variable])]=1;

			}else{
				continue;
			}

			// collects all clauses connected to this variable
			temp_clause=f_variable_position[abs(f_variable_connections[ones[f]][variable])];

			//coppy all the clauses
			while(temp_clause!=NULL){

				//set it to vistied
				if(clause_visited[temp_clause->clause]==0 ){

					list=link_append(temp_clause->clause, list);

					clause_visited[temp_clause->clause]=1;
				}
		
				temp_clause=temp_clause->next;
			}
		}

		bfs_graph(layer,list);


		list=list->first->end;
		while(1){

			if(clause_visited[list->data]==0){
	
				printf(" unset vist %i \n",list->data);
				exit(0);
			}

			clause_visited[list->data]=0;
			//copy_removed(set->clause,&set);
			
			pop_link(&list);
			
			if(list==NULL)break;
		}

		while(1){

			CountVariable[abs(variable_list->clause)]=0;

			if(variable_list->previous!=NULL){
				variable_list=variable_list->previous;
				free(variable_list->next);
			}else{
				break;
			}
		}
	}
//	printf(" end \n");
//	exit(0);
	//this is where the guess code goes
	//halt();
	// select an unset variable
	//	printf(" i %i ones 5 %i \n",f, ones[0]);

	var = FindUnset();
	if(var==-1)break;

	int eval=0;
	eval = Evaluate( - abs(var) );

		

	//	printf(" i %i %i ones 5 %i \n",f, ones[0],var);
	if(eval==1){
		Assert_Variable( -var	 );
		VariableSet  [abs(var)] = 0;
		IsVariableSet[abs(var)] = 1;
	}
	if(eval==-1){
				
		eval = Evaluate(  abs(var) );
		if(eval==-1){ 
		 	printf("illogical \n");
		 	exit(0);
		}
		Assert_Variable( var	 );
		IsVariableSet[ abs(var) ]=1;
				
		VariableSet  [abs(var)] = 1;
		IsVariableSet[abs(var)] = 1;

	}
	//var = FindUnset();
	//	printf(" i %i %i ones 5 %i \n",f, ones[0],var);
	if(ones[0]>f_variable_count)break;
		//break;		
}	
	/*
	for (int i=1; i<=ones[0];i++){
	printf (" %i \n", f_variable_connections[ones[i]][1]);
	
	
	}
	*/
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

			deduce(list, &set);
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


