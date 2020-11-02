#ifndef READ_FILE_H
#define READ_FILE_H

#include <stdio.h>
#include <stdlib.h>
#include "llist.h"

static inline node* import_CNF(int argc, char *argv[])
{


	//opens file for reading
	FILE *fp;
	if (argc > 1) {
		fp = fopen(argv[1], "r");
	} else {
		printf(" no file\n");
	}

	if(!fp){printf("Cannot find %s \n", argv[1]);exit(0);}


	// reads the amount of variables and clauses
	fscanf(fp, "p cnf %i %u \n", &nr_variables, &nr_clauses);


	unsigned int nr_variables;
	unsigned int nr_clauses;


	//stores temporary variables
	link_node temp;
	int tmp[300]={0};

	

	//stores variable number in clause
	int b=0;
	int cl=1;

	//Stores literal 
	int literal;

	//Stores singley signed clause
	int ones[4000]={0};

	while (1){
		if (fscanf(fp, "%d", &literal) != 1){
			printf("error: expected literal\n");
			exit(EXIT_FAILURE);
		}
		if (literal!=0){
			//stores which literal
			temp=link_append(temp,literal);
			b++;			
		}

		//at the end of each line
		if (literal == 0){
			variable_connections[0][0]++;
			for (int unload=0;unload<b;unload++){

				//stores where each variable is store			
				variable_connections[variable_connections[0][0]][0]=b;

				//convert variable to progessive number
				if(variable[abs(tmp[unload])][1]==0){
					//////////////printf("variable status %i %i \n",abs(tmp[unload]), variable[abs(tmp[unload])][0]);
					variable[0][0]++;
					variable[abs(tmp[unload])][0]=variable[0][0];
					variable[abs(tmp[unload])][1]=1;
				}


				if(tmp[unload]>0){
					variable_connections[variable_connections[0][0]][unload+1]=variable[abs(tmp[unload])][0];
				}

				if(tmp[unload]<0){
					variable_connections[variable_connections[0][0]][unload+1]=-variable[abs(tmp[unload])][0];
				}
				
				if(variable_position[variable[abs(tmp[unload])][0]]==NULL){
					variable_position[variable[abs(tmp[unload])][0]]=append_variable(cl,NULL);

			
				}
				else{	

						append_variable(cl,variable_position[variable[abs(tmp[unload])][0]]);
	//printf("cl %i\n", variable_position[variable[abs(tmp[unload])][0]]->end->clause);

				}



			}
			if(b==1){
				ones[0]++;
				ones[ones[0]]=variable_connections[0][0];
			}
			clause_size[variable_connections[0][0]]=b;
			//resets clause size
			b=0;
			//increments clause count
			cl++;
		}


		//if read all clauses
		if(cl==nr_clauses+1){
			////////////printf("variables %i\n",variable[0][0]);
			////////////printf(" this is the amount of clauses %d \n ", cl-1);
			break;
		}
	}

}
