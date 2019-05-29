#!/bin/bash


#cd ~/contiki-ng/examples/rpl-storingHC/sky-testscript/simulation
cd ~/contiki-ng/examples/rpl-storingHC/cooja-testscript/simulation
#cd ~/contiki-ng/examples/rpl-non-storingHC/sky-testscript/simulation
#cd ~/contiki-ng/examples/rpl-non-storingHC/cooja-testscript/simulation


#Compile sim changer
gcc -o sim-changer-rpl-hopcount.out ~/contiki-ng/examples/sim-changer-rpl-hopcount/sim-changer-rpl-hopcount.c

#To select each simulation
#for i in `ls -d`; do # because of ./
for i in `ls`; do
	cd ${i}

	#To select each scenario in a simulaion
	for j in `ls *.csc`; do
		printf "Working directory: %s\n" `pwd`;
		.././sim-changer-rpl-hopcount.out ${j}
		rm -f ${j}
		mv new-sim/${j} ${j}
	done
	rm -rf new-sim
	cd ..
done

rm sim-changer-rpl-hopcount.out
