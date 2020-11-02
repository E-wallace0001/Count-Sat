#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#include "headers/group.h"
	//stores temporary literals
	
	int f_variable_count = 0;
	int nr_variables, nr_clauses;
	
void read_cnf_list(char* argv, group_s* formula){

	set_s* s1;

	FILE *fp;
		fp = fopen(argv, "r");

	if(!fp){printf("Cannot find %s \n", argv);exit(0);}
	
	fscanf(fp, "p cnf %i	 %u\n", &nr_variables, &nr_clauses);
	
	int** variable;
	int f_clause_count =0;
	
	variable = calloc(nr_clauses, sizeof(int));
	
	for( int i =0; i < nr_variables; i++){
		variable[i] = calloc(2,sizeof(int));
	}
	
	int tmp[400]={0};
	int b=0;
	int cl=1;
	//scans file
	int literal;
	int clause;
	int* var_this;
	while (1){
		if (fscanf(fp, "%d", &literal) != 1){
			printf("error: expected literal\n");
			exit(EXIT_FAILURE);
		}
		if (literal!=0){
			//stores which literal
			tmp[b]= literal;
			b++;			
			continue;
		}
		if(literal == 'c') return;

		//at the end of each line
		if (literal == 0){
			
			s1 = (set_s*)formula->end->data;
			
			//this is where the number of clauseses is stored
			for (int unload=0;unload<b;unload++){
				//convert variable to progessive number
				if(variable[abs(tmp[unload])][1]==0){
					f_variable_count++;
					variable[0][0]++;
					variable[abs(tmp[unload])][0]=variable[0][0];
					variable[abs(tmp[unload])][1]=1;
				}

				if(tmp[unload]>0){
					var_this =  malloc( sizeof(int)) ;
					*var_this = variable[abs(tmp[unload])][0] ;
					ExtendSet( var_this , s1);
				}

				if(tmp[unload]<0){
					var_this =  malloc( sizeof(int)) ;
					*var_this = -variable[abs(tmp[unload])][0] ;
					ExtendSet( var_this , s1);
				}
			}
			b=0;
		}
		ExtendGroup( formula );
		cl++;
		f_clause_count++;
		//end of the documents
		if(cl>nr_clauses){
			break;
		}

	}

}

void export_cnf( char* file, set_s* formula){

	set_s* clause = (set_s*)formula->list->data;
	FILE* fp2;
	fp2=fopen(file,"w");
	while( 1 ){
		
	
		list* variable = clause->list;
		
		while( variable != NULL){
			if( *(int*)variable->data > 0){
				fprintf (fp2,"%i ", ( *(int*)variable->data) );
			}else{
				fprintf (fp2,"%i ", ( *(int*)variable->data) );
			}
				
			variable = variable->next;	
		}
		fprintf(fp2,"0\n");
		if( formula->list->next ==NULL) 
			break;
		formula->list= formula->list->next;
		clause = (set_s*)formula->list->data;
	}
	fclose(fp2);	

}


