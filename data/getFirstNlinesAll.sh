#!/bin/bash 

DATAFILE=testpdb1.txt

for N in $* 
do
    echo ""
    head -$N < $DATAFILE > pat$N.txt
done

#EOF 
 
 
