#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
int amount=0;

int variable[132615][5]={0};
int clause[400][40]={0};


int main(int argc, char *argv[])
{
//int ch [132615]={0};
// variable for current clause 
int current_clause=1;



//opens file for reading
FILE *fp;
	if (argc > 1) {
		fp = fopen(argv[1], "r");
		
	} else {
		printf("fail \n");
	}

unsigned int nr_variables=1;
unsigned int nr_clauses=1;


// reads the amount of variables and clauses
fscanf(fp, "p cnf %i	 %u\n", &nr_variables, &nr_clauses);
printf("number of variables %u \n Number of clauses %u \n", nr_variables, nr_clauses);
int cant=0;
//stores temporary literals
int tmp[35]={0};

//stores all the ones
//int ones[10000]={0};
//stores variable number in clause
int b=0;
int end2=0;
int cl=1;
//scans file
int literal;
int unload=0;
//17.39.7
long int num_check[1000000]={0};
while (1){
			
	if (fscanf(fp, "%d", &literal) != 1) {
		fprintf(stderr, "error: expected literal\n");
		exit(EXIT_FAILURE);
	}
			
	if (literal!=0)
		{
			//stores which literal
			tmp[b]= literal;
			b++;			
		}


//at the end of each line
	if (literal == 0)
		{
clause[0][0]++;
			for (int unload=0;unload<b;unload++)
				{

					//stores where each variable is store
					
					clause[clause[0][0]][0]=b;
//convert variable to progessive number
					if(variable[abs(tmp[unload])][1]==0)
					{
					//printf("variable status %i %i \n",abs(tmp[unload]), variable[abs(tmp[unload])][0]);
					variable[0][0]++;
					variable[abs(tmp[unload])][0]=variable[0][0];
variable[abs(tmp[unload])][1]=1;
					
					}

//printf("temp %i\n",abs(tmp[unload]));

					if(tmp[unload]>0)
					{
					clause[clause[0][0]][unload+1]=variable[abs(tmp[unload])][0];
					}
					if(tmp[unload]<0)
					{
					clause[clause[0][0]][unload+1]=-variable[abs(tmp[unload])][0];
					}

									

			}
			//resets clause size
			b=0;
			//increments clause count
			cl++;
		}


	//if read all clauses
	if(cl==nr_clauses+1)
		{
printf("variables %i\n",variable[0][0]);
		printf(" this is the amount of clauses %d \n ", cl-1);

			break;
		}
}

int removed[100]={0};
int taken2 [100][100][1]={0};
//-clause -variable - taken first

int test=0;
int bin_t=0;
int rem=0;
int n=0;
long int taken=0;
long int *num= malloc(536870912*sizeof(long int));;
long int *flip= malloc(536870912*sizeof(long int));;

for(int d=0;d<pow(2,variable[0][0]);d++)
{
//num2[d]=0;
n=d<<1;
//printf("\nremoved %i ",d );
//for the cluases
	for(int j=1;j<=clause[0][0];j++)
	{

//for the variables
rem=0;

		for (int k=1;k<=clause[j][0];k++)
		{
			test=1<<abs(clause[j][k]);
			bin_t=n&test;
//printf("rem %i %i %i %i\n",n,clause[j][k], bin_t,test);
			if(clause[j][k]<0)
			{
				if(bin_t>0)
				{
				rem++;
				}
			}
			if(clause[j][k]>0)
			{
				if(bin_t==0)
				{
				rem++;
				}
			}
		}
//	printf("rem %i\n",rem);

		if(rem==clause[j][0])
		{
if(j==cl-1){if(flip[d]!=0){taken2[flip[d]][0][0]++;}}
if(flip[d]==0){flip[d]=j;}
//taken2[flip[d]][0][0]++;

	//	printf(" %i ",j );
		if(num[d]==0)
		{
taken2;
		removed[j]++;
		}

num[d]=1;
		
		}
		
		

	}
}//printf("\n");
for(int f=1;f<=clause[0][0];f++)
{
printf("this clause removes %i\n", removed[f]);
}
int fa=0;
fa=0;
for(int k=0;k<pow(2,variable[0][0]);k++)
{
fa=fa+num[k];
}
for(int f=1;f<=clause[0][0];f++)
{
printf("this clause was in %i with %i\n", f,taken2[f][0][0]);
}
int fa2=0;
fa2=pow(2,variable[0][0])-fa;
printf("variables %i\n",variable[0][0]);
printf("taken %li	%i\n",taken,fa2);

return(0);
}
