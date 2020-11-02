
import random
import os
import subprocess
import sys
import time

def unique_count(this):
    unique=[]
    count=0
    for i in range (0, len(this)):
        for x in range( 0, len(this[i]) ):
            if abs(this[i][x]) not in unique:
                count=count+1
                unique.append(abs(this[i][x]))
    return(len(unique))

def export(title,formula):
    count = unique_count(formula)
    file = open(title, "w")
    file.write("p cnf 15 ")
    file.write(str(count))
    file.write("\n")
    for i in formula:
        lst = i
        for x in lst:
            file.write(str(x))
            file.write(" ")
        file.write("0 \n")
    file.write("0 \n")
    

def generate_cnf(char):
    '''requires name'''
    #file = open(char,"w")
    formula = []
    #number of clauses
    indx_rand = random.randrange(1, 16)

    set =[]

    for indx in range(0,15):
        c_size = random.randrange(1, 4)
        this=[]
        used=[]
        for i in range(0,c_size):

            variable = random.randrange(1,10);

            if abs(variable) in set :
                if c_size==1:
                    break
            if c_size==1:
                set.append(variable)
            if random.randrange(0,2)==0:
                variable=variable*-1

            if  abs(variable) not in used :
                this.append( variable )
                used.append( abs(variable))

        if(len(this)!=0):
            formula.append(this)
    
    export(char,formula)
    return formula
    
def process(cmd, file):
    process = subprocess.run([cmd, file], check=True, stdout=subprocess.PIPE, universal_newlines=True)
    output = process.stdout
    res = [int(i) for i in output.split() if i.isdigit()] 
    return res

title = 'test.cnf'
while 1:
	first =0
	second=0
	generate_cnf(title)
	rop = process('./bin', title)
	first=rop[len(rop)-1] 
	print( first )

	rop = process('../CountSat', title)
	second=rop[1] 
	#second = rop[len(rop)-1] 
	print( second )
	if first!=second:
		break


