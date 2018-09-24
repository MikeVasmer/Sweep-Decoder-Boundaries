#!/bin/bash -l

# Batch script to run an array job with the upgraded
# software stack under SGE.

# 1. Force bash
#$ -S /bin/bash

# 2. Request ten minutes of wallclock time (format hours:minutes:seconds).
#$ -l h_rt=0:01:0

# 3. Request 1 gigabyte of RAM.
#$ -l mem=1G

# 4. Request 15 gigabyte of TMPDIR space (default is 10 GB)
#$ -l tmpfs=10G

# 5. Set up the job array.  In this instance we have requested 1000 tasks
# numbered 1 to 1000.
#$ -t 1-120

# 6. Set the name of the job.
#$ -N array-params

# 7. Set the working directory to somewhere in your scratch space.  This is
# a necessary step with the upgraded software stack as compute nodes cannot
# write to $HOME.
# Replace "<your_UCL_id>" with your UCL user ID :)
#$ -wd /home/ucapjgv/Scratch/output

# 8. Parse parameter file to get variables.
number=$SGE_TASK_ID
paramfile=/home/ucapjgv/Scratch/input/params.txt
 
index=`sed -n ${number}p $paramfile | awk '{print $1}'`
N=`sed -n ${number}p $paramfile | awk '{print $2}'`
d=`sed -n ${number}p $paramfile | awk '{print $3}'`
p=`sed -n ${number}p $paramfile | awk '{print $4}'`
runs=`sed -n ${number}p $paramfile | awk '{print $5}'`

# 9. Automate transfer of output to Scratch from $TMPDIR
#Local2Scratch

# 10. Run the program (replace echo with your binary and options).

module load python3/recommended 
python3 legion_data_generator.py $d $p $p $N $runs $index
