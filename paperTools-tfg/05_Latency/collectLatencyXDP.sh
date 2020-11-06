#!/bin/bash

output_file="Latency_XDP.csv"

touch $output_file
for j in $(seq 1 10);
do
	ping -c10 10.0.0.3 | ./pingparser.py -f "%m %a %M" | tr " " ","  >> $output_file && echo '' >> $output_file &
	pids[${j}]=$!
	sleep 1
done

# wait for all pids
for pid in ${pids[*]}; do
    wait $pid
done
