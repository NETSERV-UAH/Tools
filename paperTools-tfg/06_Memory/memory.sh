#!/bin/bash


output_file="06_Memory.csv"
XDP_MEM=$(python3 collectMemoryXDP.py | grep KernelStack | cut -d' ' -f4)
P4_MEM=$(cat bmv2_pmap.1 | grep total | tr ' ' ','| cut -d',' -f13 | sed 's/K$//')


touch $output_file | echo "xdp,p4" > $output_file
echo "${XDP_MEM},${P4_MEM}" >> $output_file


