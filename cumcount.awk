#!/bin/bash 
BEGIN{
    N=0;
}

{
    N=N+1;
    print $1, $2, N; 
} 

