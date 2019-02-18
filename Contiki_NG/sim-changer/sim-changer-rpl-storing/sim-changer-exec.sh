#!/bin/bash


cd ~/contiki-ng/examples/rpl-storing/sky-testscript/simulation

#Compile sim changer
gcc -o sim-changer-rpl-storing.out ~/contiki-ng/examples/sim-changer-rpl-storing/sim-changer-rpl-storing.c

#To select each simulation
#for i in `ls -d`; do # because of ./
for i in `ls`; do
	cd ${i}

	#To select each scenario in a simulaion
	for j in `ls *.csc`; do
		printf "Working directory: %s\n" `pwd`;
		.././sim-changer-rpl-storing.out ${j}
		rm -f ${j}
		mv new-sim/${j} ${j}
	done
	rm -rf new-sim
	cd ..
done
