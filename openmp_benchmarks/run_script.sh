#!/bin/bash

set -uex

num_cpus=$(grep -c ^processor /proc/cpuinfo)

benchmarks=(activation batchnorm conv ctfft emptyloop matmul quicksort wave)


for b in ${benchmarks[@]}
do
   #create results folders
   result_folder=gcc_results/$b
   mkdir -p $result_folder

   for (( c=1; c<=$num_cpus; c++ ))
   do
      if [[ $c -eq 1 ]];
      then
         OMP_NUM_THREADS=$c ./build/OpenMPBench descriptors/$b.json $result_folder/out_$c.csv
      else
         OMP_NUM_THREADS=$c ./build/OpenMPBench descriptors/$b.json $result_folder/out_$c.csv skipserial
      fi
   done
done