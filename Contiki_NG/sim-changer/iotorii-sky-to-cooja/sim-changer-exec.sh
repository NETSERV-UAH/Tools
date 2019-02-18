#!/bin/bash


cd ~/contiki-ng/examples/iotorii-n-hlmac/simulation-cooja-mote

#Compile sim changer
gcc -o iotorii-sky-to-cooja.out ~/contiki-ng/examples/iotorii-sky-to-cooja/iotorii-sky-to-cooja.c

#To select each simulation
#for i in `ls -d`; do # because of ./
for i in `ls`; do
	cd ${i}

	#To select each scenario in a simulaion
	for j in `ls *.csc`; do
		printf "Working directory: %s\n" `pwd`;
		.././iotorii-sky-to-cooja.out ${j}
		rm -f ${j}
		mv new-sim/${j} ${j}
	done
	rm -rf new-sim
	cd ..
done
rm iotorii-sky-to-cooja.out
