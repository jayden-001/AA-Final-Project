#!/bin/bash

filelist=$(ls rmfr*)
for filename in $filelist
do
        echo "$filename"
        echo "$filename" >> log
        ./min_cost_flow -f $filename -r 1 >> FI_RMFR.output
done


filelist=$(ls rmfl*)
for filename in $filelist
do
        echo "$filename"
        echo "$filename" >> log
        ./min_cost_flow -f $filename -r 1 >> FI_RMFL.output
done

filelist=$(ls rmfw*)
for filename in $filelist
do
        echo "$filename"
        echo "$filename" >> log
        ./min_cost_flow -f $filename -r 1 >> FI_RMFW.output
done

filelist=$(ls com*)
for filename in $filelist
do
        echo "$filename"
        echo "$filename" >> log
        ./min_cost_flow -f $filename -r 1 >> FI_COM.output
done

filelist=$(ls rmfr*)
for filename in $filelist
do
        echo "$filename"
        echo "$filename" >> log
        ./min_cost_flow -f $filename -r 0 >> HL_RMFR.output
done

filelist=$(ls rmfl*)
for filename in $filelist
do
        echo "$filename"
        echo "$filename" >> log
        ./min_cost_flow -f $filename -r 0 >> HL_RMFL.output
done

filelist=$(ls rmfw*)
for filename in $filelist
do
        echo "$filename"
        echo "$filename" >> log
        ./min_cost_flow -f $filename -r 0 >> HL_RMFW.output
done


filelist=$(ls com*)
for filename in $filelist
do
        echo "$filename"
        echo "$filename" >> log
        ./min_cost_flow -f $filename -r 0 >> HL_COM.output
done



