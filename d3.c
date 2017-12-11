/******
Todo:

finish sub clause matches

sub_tab can lose the root as this won't change

make sure it's capable for other clause sizes other than 4



*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
int tem=0;
int amount=0;
int loop_count=0;
int variable[132615][6]={0};
int clause[400][400]={0};
int con[400][200][4]={0};
//int connections[400]={0};
int mat_store[1000]={0};
int pos_t[1000]={0};
float stored_v[200]={0};
int prev_add[300][300][300]={0};
// this is the exist
// 0 = exist
// 1 = is the value to be subracted
int ex_clause[100][2]={0};


//change
int s_var[300]={0};
// don't do
int n_var[300]={0};
int st_ch=0;
int var_tab[200];
int var_in_tab=0;
//this adds a clause to the ones being checked
void var_tab_add(int clause_a)
{
var_in_tab=var_in_tab+clause[clause_a][0];
for(int x=1;x<=clause[clause_a][0];x++)
{
	
	var_tab[clause[clause_a][x]]=var_tab[clause[clause_a][x]]+1;


}
}

//this deletes the clauses of the ones being checked
void var_tab_del(int clause_d)
{
var_in_tab=var_in_tab-clause[clause_d][0];
for(int x=1;x<=clause[clause_d][0];x++)
{

	var_tab[clause[clause_d][x]]=var_tab[clause[clause_d][x]]-1;
}
}

int var_tab_check(int clause_d)
{
int var_check=0;
for(int x=1;x<=clause[clause_d][0];x++)
{
	if(var_tab[clause[clause_d][x]]>0)
	{
		var_check++;
	}
}
return(var_check);
}


// this looks through the stored clauses to see how many variables already exist

int indie_var(int root,int k,int i,int sub)
{

//look for how many variables appear more than once
//this table stores the variable in it's own space
int indie_check[30]={0};
int replicant=0;
//search through root
for(int x=1;x<=clause[root][0];x++)
{
indie_check[abs(clause[root][x])]++;
printf("this root variable %i\n",clause[root][x]);	
}

for(int x=1;x<=clause[k][0];x++)
{
printf("k var %i\n",clause[k][x]);

indie_check[abs(clause[k][x])]++;
if(indie_check[abs(clause[k][x])]==2)
{
replicant++;
printf("this k variable %i\n",clause[k][x]);
}

}

for(int x=1;x<=clause[i][0];x++)
{
printf("i var %i\n",clause[i][x]);

indie_check[abs(clause[i][x])]++;
if(indie_check[abs(clause[i][x])]==2)
{


printf("this i variable %i\n",clause[i][x]);
replicant++;

}
//indie_check[abs(clause[i][x])]++;
}
if(sub!=0)
{
for(int x=1;x<=clause[sub][0];x++)
{
printf("sub var %i  amount %i \n",clause[sub][x],indie_check[abs(clause[sub][x])]);

indie_check[abs(clause[sub][x])]++;
if(indie_check[abs(clause[sub][x])]==2)
{

replicant++;
printf("this sub variable %i\n",clause[sub][x]);

}
}
}
return(replicant);
}

//this compares the variables amongst two clauses

int match(int a, int b)
{
int mat=0;
int pol=0;
int one=1;
int ze=-1;
mat_store[0]=0;
//variable one
for(int x=1;x<=clause[a][0];x++)
{
	//variable two
	for(int y=1;y<=clause[b][0];y++)
	{

//printf("clause %i %i \n clause %i %i\n",a,clause[a][x],b, clause[b][y]);
		if(abs(clause[a][x])==abs(clause[b][y]))
		{
			mat++;
//printf("\nin match %i %i\n",mat,clause[b][y]);
			if(clause[a][x]!=clause[b][y])
			{	

				pol++;
			}
		}
	}

//printf("pol %i\nmat %i\n",pol, mat);
}
if(pol==0)
{
int mat2=0;
//	printf("\nmatch %i %i\n",mat,clause[b][0]);

mat2=clause[b][0]-mat;
//printf("mat2 %i\n\n",mat2);
return mat2;
}
else if(pol!=0)
{
return -1;
}
}

//this modifies a branch of the set
//this is to be changed/superseded 

int scale(int root, int i,float replace, long long result_s,float result_scale)
{
int num0=0;
int ce2=var_tab_check(con[root][i][0]);
printf("this is ce2 %i\n",ce2);
//if(root==6){if(j==5){if(i==4){result=result+1;}}}
num0=replace/pow(2,result_s);
int far=2;
printf("num0 %i\n", num0);

printf("this result4 %f\n",replace);
result_scale=result_scale+num0;
result_scale=result_scale*far;// the amount in common
result_scale=result_scale-num0;
result_scale=result_scale/far;//amount in common
//printf("result4 root %i j %i i %i %f %lli\n",root,j,i, result4,result);
return result_scale;

}


long long prev_result[350]={0};
//use a nested loop to solve this 
long int result3=0;

//int basis[300]={0};

//need to track all of the variables that so I can see which variables already have been included for each clause that's to be included.

// There are two difference circumstances where this is needed

// if there are more than 4 connections between the entire included clauses these will all have already been taken with prior clauses and nothing will be taken.

// for this function, I need to mark all the variables to a table
// if the table equates to two after the additon of the variable, this is added more than once
// if there are 4 or more independant variables that clash, this clause would have not taken anything, stipulating that it would take 4 quarters
// not sure if 4 is dependant on the amount of variables with in the clause or if it is based  

int saved[300]={0};
int t_clause[100][100][1]={0};
int st_c=0;
//sub_tab[trunk][branch][primary/secondary]
int sub_tab[200][200][200][3]={0};

long long  in_solve(int k, int j,long long n, long long m,long p,int root,int in_go)
{

//printf("n= %li\n",n);
long long result=0;

long long add=0;
long long d=0;
long int in_s=0;
long int in_k=0;
float result4=0;
long long add2=0;
int count=0;
float result5=0;
loop_count=0;
long long match3=0;
long int typ=0;
int flip=0;
int matt =0;
int rem =0;
int result_r=0;
int result6=0;
int sum=0;
int first_subset=0;
int start=0;
int second_subset=0;
int tem=0;
int num2=0;
int num3=0;
int num4=0;
int num5=0;
int branch_change=0;
int branch=0;
int init_val=0;
int ce =0;

int st_subset=0;
//instruction table
int inst_tab[200][5]={0};

float first_replace=0;

int rem_clause[200]={0};
int t_rem=0;
//printf("k = %i\n",k);
	for (int i=1;i<k;i++)
	{

loop_count++;

		result =match(j, con[root][i][0]);

		result_r=match(root, con[root][i][0]);
//s_var[i]=result;
int sb=0;
int result8=0;
int result9=0;
int flap=0;


//this is for the conflicted variables to be deleted
if(result==-1)
{
	rem_clause[0]++;
	rem_clause[rem_clause[0]]=con[root][i][0];
}

if (result!=-1)
		{
ce=var_tab_check(con[root][i][0]);
printf("root %i j %i i %i\n", root, j, i);
//this needs updating
//what is the 4 for?
result=4-ce;

d=m/pow(2,result);
add=add+d;


int pk=prev_result[i];


if(st_subset==1)
{
//second_subset==1;
//pk = prev_add[root][i][start-1];
//printf("pk %i \n",pk);
}


//stat of second

if(second_subset==1)
{	
//sec_sub[x][z]
int sec_sub[100][100][5]={0};
	int ss_beg=0;
int sim=0;
int sim2=0;
int st2=0;
int st_pk=0;
	for(int z=start;z<i;z++)
	{
		var_tab_add(start);
		printf("this is the second subset  i %i ,z %i \n",i,z);

		if(ss_beg!=0)
		{
			//for each for modification to be made
			// to be finished
			//for(int q=mod_table[0][0]; q<=mod_table[0][1]; q++)
			//{
			//}
				
				sim=var_tab_check(con[root][z][0]);
				 sim2=con[root][z][1]-sim;
				printf("this is the sim %i %i \n",var_in_tab,sim);
				printf("\n this is the second subset lot x %i\n",z);
				//printf("con root %i \n",sub_tab[root][i][t][1]);
				//pk=pk-sub_tab[root][j][z][1];
				st_pk=sec_sub[i][start][start]/pow(2,sim2);
				if(ss_beg==2){st_pk=st_pk-st_pk/16;}	
				sec_sub[i][start][0]=st_pk;
				printf("st_pk %i \n", st_pk);
				//if(z==3){st_pk=st_pk-8;}
				st2=st2+st_pk;
				
				pk=pk-st_pk;
				ss_beg++;
			
		}
	
		if(ss_beg==0)
		{
			//pk=pk+first_replace;
			sim=var_tab_check(con[root][z][0]);
			sim2=con[root][z][1]-sim;
			printf("this is sim1 %i\n", sim2);
			sec_sub[i][start][start]=sub_tab[root][i][start][1];
			pk=pk+sec_sub[i][start][start];
			printf("ss beg pk %i first replace %f \n",pk,first_replace/2);
			ss_beg++;
		}
		var_tab_del(start);

	}
	printf("\n\nsecond subset is a go go  pk=%i\n\n",pk);

}

//end of second

if(first_subset==1)
{
first_subset=0;
second_subset=1;
// add all the previous ones to the test table
//pk = prev_add[root][i][start-1];
printf("fst pk %i \n",pk);
printf("\n fst result under test %lli \n", result);

// check to see how man replicants
int th=0;
indie_var(root,j,i,0);

printf("***th= %i \n",th);

// for the first replace 
pk=pk+first_replace/2;


printf("first_replace == %f \n", first_replace);
//fsa	
if(th==4)
{
	// if there are four conjoined set of variables, this removes zero
	printf("this is the modifier");
	//the modifier is here

	//pk=pk+first_replace*2;

	//pk=pk+450*2;
//	pk=pk+225*2;

	}
}

// for the rest, scale it
//if(root==6){ if(j==5){ if(i==4){ con[root][i][1]=con[root][i][1]-1;}}}
result3=pk*pow(2,con[root][i][1]);
result4=(result3/p/pow(2,result));


printf("result4 %f \n\n",result4);

int scale1=0;
int num1=0;
int clim=1;
int num0=0;
int match4=0;

printf("root %i j %i i %i result %lli result3 %li result4 %f p %lli \n",root,j,i,result, result3, result4,d);




if(st_subset==0)
{
 if(result_r>result)
	{
	s_var[n]=1;
	printf("result under test %i \n", ce);
	start=con[root][i][0];
	st_subset=1;
	first_subset=1;
	//second_subset=1;
	 s_var[k]=root;
	saved[0]++;
	sum=1;
	first_replace=d-result4;
	inst_tab[con[root][i][0]][1]=1;
	printf("initial root %i j %i i %i %f \n",root,j,i,first_replace);
	//saved[saved[0]]=prev_result[i];
	}
}

result5=result5+result4;

prev_add[root][j][i]=add-result5;


sub_tab[root][j][i][0]=pow(2,result);
sub_tab[root][j][i][1]=d-result4;

//printf("prev_result[4] %lli \n", prev_result[4]);
ex_clause[con[root][i][0]][1]=add-result5;

//	stored_v[i]=result4;

//
printf("result 4 END %f \n", result4);
		}
if(i==1)
{
//add2=m;
}

	}
//s_var[k]=result;
//printf("this data! %f\n",result5);
prev_result[k]=add-result5;


count=0;
int pr=0;
	
add2=add-result5;

//printf("\n\n add %i\n\n add2 %i\n\n result! %i \n\n",add,add2,result5);
	//result3=0;


printf(" root %i j%i add2 %lli\n\n", root,j,add2);

return add2;
}

//pass on the table under test, the size of numbers to be tested 
//int n =the previous number


long long solve_p(int j,int f,long int n)
{
long long loop=0;
long long result=0;
long int result2=0;
long int loop2=0;
long int test=0;
loop_count=0;
//for the size of connected
for(int k =1;k<=f;k++)
{

var_tab_add(con[j][k][0]);
result=n/pow(2,con[j][k][1]);
test=test+result;

if(k-1>0)
{
loop=in_solve(k,con[j][k][0],j,result,pow(2,con[j][k][1]),j,con[j][k][1]);
loop2=loop2+loop;
stored_v[k]=result-loop;
}

var_tab_del(con[j][k][0]);
}

loop=0;
result2=test-loop2;

return result2;
}



int  clause_check(int j, int a)
{
int check=0;
//check if the clause appears in the orrignal, return the orriginal connection
int c=0;
c=a;
//printf("a %i %i\n",j,a);
if(con[j][c][0]==a)
{
//printf("this is done before %i\n",c);
 return 1;
}
else if (con[j][c][0]!=a)
{
if (con[j][c][0]>a)
{
for(int f=1;f<=con[j][0][0];f++)
{
//printf("run");
if(con[j][f][0]==a)
check=1;
//break;
}
}
if (con[j][c][0]<a)
{
for(int f=1;f<=con[a][0][0];f++)
{
//printf("run2\n");
if(con[a][f][0]==j)
check=1;
//break;
}
}

}
//printf("b %i %i\n",a,j);
if (check==1)
{
//printf("b_check %i %i\n",a,j);
return 1;
}
if (check==0)
{

return 0;
}

//end of func
}





int con_test(int a, int b)
{
int mat=0;
int pol=0;
int one=1;
int ze=0;
//variable one
for(int x=1;x<=clause[a][0];x++)
{
	//variable two
	for(int y=1;y<=clause[b][0];y++)
	{
		if(abs(clause[a][x])==abs(clause[b][y]))
		{
			mat++;

			if(clause[a][x]!=clause[b][y])
			{	
				pol++;
			}
		}
	}
}
if(pol==0)
{
return one;
}
else if(pol!=0)
{
return ze;
}
}

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
//long int num_check[10000]={0};
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

/////////////////
long long all = pow(2,variable[0][0]);

long long add=0;
long long add2=0;
long int exist=0;
int polarity=0;
int match=0;
int removed=0;
int add5=0;
long long remain=0;
int f_test=0;
long int left=0;
int mat=0;
int note=0;
int ding=0;
//for the cluases
for(int j=1;j<=clause[0][0];j++)
{
loop_count=0;
	exist=all/pow(2,clause[j][0]);
//printf("exist! = %i %li %i\n",variable[0][0], all, clause[j][0]);
	removed=0;
	//compare with all previous clauses
	for(int l=1;l<j;l++)
	{	
		//clause under test
		for (int k=1;k<=clause[j][0];k++)
		{
			//previous clause variables
			for (int m=1;m<=clause[l][0];m++)
			{
				if(abs(clause[j][k])==abs(clause[l][m]))
				{
					match++;

					if(clause[j][k]!=clause[l][m])
					{
//printf("abs cluase %i %i %i\n",clause[j][k],clause[l][m],match);	
						polarity++;
						//break;
					}

				}
			}
			if(k==clause[j][0])
			{
				if(polarity==0)
				{
			
					con[j][0][0]++;
					con[j][con[j][0][0]][0]=l;
					con[j][con[j][0][0]][1]=clause[l][0]-match;
//					removed=removed+exist/pow(2,con[j][con[j][0]//[0]][1]);printf("removed %i\n", removed);


				}
			polarity=0;
			}
			
		}
			//reset variables;
			
			match=0;
	}
//loop_count++;
//printf("exist %li\n",exist);
var_tab_add(j);
add = solve_p(j,con[j][0][0],exist);
var_tab_del(j);
//add2=add2+add;
loop_count++;
//printf(" add %li \n ",add);

	left=exist-add;

	printf("%i possibilities %li\n",j,left);
pos_t[0]++;
pos_t[j]=left;

	remain=remain+left;

}


for(int t=1;t<=9;t++)
{
printf("n_var %i = %i\n",t,prev_add[6][4][t]);
}

for(int t=1;t<=15	;t++)
{
printf("    stored_v[%i] = %f \n",t,stored_v[t]);
}

for(int t=1;t<=6	;t++)
{
printf("    subtab[4][3][%i] = %i \n",t,sub_tab[5][4][t][1]);
}

printf("save %i \n",saved[1]);
printf("variables %i\n", variable[0][0]);
all=all-remain;
printf("possibilities %lli \n",all); 
/*
//printf("con check 8-6 %i \n", clause_check(21,15));
for(int k =1; k<=pos_t[0];k++)
{
printf("pos_t[%i]=%i\n",k,pos_t[k]);
}
*/
printf("st_ch %i\n", st_ch);
return(0);
}


