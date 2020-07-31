#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "stest.h"
#include "var_pos.h"

	//stores temporary literals

void read_cnf(char* argv){

	


	printf(" %s\n", argv);

	FILE *fp;
		fp = fopen(argv, "r");

	if(!fp){printf("Cannot find %s \n", argv);exit(0);}

	// reads the amount of variables and clauses
	fscanf(fp, "p cnf %i	 %u\n", &nr_variables, &nr_clauses);

	int tmp[400]={0};
	int b=0;
	int cl=1;
	//scans file
	int literal;
	f_variable_connections[0][0]=0;
	int clause;

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

		//at the end of each line
		if (literal == 0){
			//this is where the number of clauseses is stored
			f_variable_connections[0][0]++;
			for (int unload=0;unload<b;unload++){

				//stores where each variable is store			
				f_variable_connections[f_variable_connections[0][0]][0]=b;

				//convert variable to progessive number
					if(variable[abs(tmp[unload])][1]==0){
						f_variable_count++;
						variable[0][0]++;
						variable[abs(tmp[unload])][0]=variable[0][0];
						variable[abs(tmp[unload])][1]=1;
					}


					if(tmp[unload]>0){
						f_variable_connections[f_variable_connections[0][0]][unload+1]=variable[abs(tmp[unload])][0];
					}

					if(tmp[unload]<0){
						f_variable_connections[f_variable_connections[0][0]][unload+1]=-variable[abs(tmp[unload])][0];
					}
				
				if(f_variable_position[variable[abs(tmp[unload])][0]]->clause==0){
				// this sets 
				f_variable_position[variable[ abs( tmp[unload] )] [0] ]->clause=f_variable_connections[0][0];
				f_variable_position[variable[ abs( tmp[unload] )] [0] ]->end=f_variable_position[variable[abs(tmp[unload])][0]];

				}
				else{	
					append_variable(f_variable_connections[0][0],f_variable_position[variable[abs(tmp[unload])][0]]);
				}

			}
			if(b==1){
				ones[0]++;
				ones[ones[0]]=f_variable_connections[0][0];
				OnesPlace[abs(f_variable_connections[f_variable_connections[0][0]][1])]=f_variable_connections[0][0];
			}
			f_clause_size[f_variable_connections[0][0]]=b;
			//resets clause size
			b=0;
			//increments clause count
			
		}
//printf(" cl %i \n", cl);
		//count clause
		cl++;
		f_clause_count++;
		//end of the documents
		if(cl>nr_clauses){
			break;
		}

	}

}
