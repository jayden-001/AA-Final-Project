#!/bin/bash

filelist=$(ls rmfl*)
for filename in $filelist
do
        echo "$filename"
        echo "$filename" >> log
        ./min_cost_flow -f $filename -r 0 >> HL_RMFL.output
done

