#!/bin/bash


#cd ~/contiki-ng/examples/rpl-non-storing/sky-testscript/simulation
cd ~/contiki-ng/examples/rpl-non-storing/cooja-testscript/simulation

#Compile sim changer
gcc -o sim-changer-rpl-non-storing.out ~/contiki-ng/examples/sim-changer-rpl-non-storing/sim-changer-rpl-non-storing.c

#To select each simulation
#for i in `ls -d`; do # because of ./
for i in `ls`; do
	cd ${i}

	#To select each scenario in a simulaion
	for j in `ls *.csc`; do
		printf "Working directory: %s\n" `pwd`;
		.././sim-changer-rpl-non-storing.out ${j}
		rm -f ${j}
		mv new-sim/${j} ${j}
	done
	rm -rf new-sim
	cd ..
done

rm sim-changer-rpl-non-storing.out
