#!/bin/bash

input="input/20_05_20.csv"

i=12
line=$(sed -n ${i}p $input)
# echo $line

# Set "," as the field separator using $IFS 
while IFS=',' read -ra params 
do 
    python data_generator.py "rhombic_toric" ${params[0]} ${params[1]} ${params[1]} ${params[2]} ${params[3]} --sweep_schedule "alternating_YZ" --job ${params[4]}
done <<< "$line"
