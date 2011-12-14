#!/bin/bash

filelist=$(ls ./graphs/bi*)
for filename in $filelist
do
        echo "$filename"
        echo "$filename" >> log
        ./min_cost_flow -f $filename -r 0 >> HL_GP_BI.output
done

