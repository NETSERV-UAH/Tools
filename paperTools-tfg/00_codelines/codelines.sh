#!/bin/bash

repo_dir="TFG"
repo_url="https://github.com/davidcawork/TFG.git"
packageList="git gcc cloc"
usecases="xdp p4 p4-wireless xdp-wireless"
output_file="00_codelines.csv"


if [ "$EUID" -ne 0 ]
  then echo "Please run as root"
  exit
else
	echo "[+] Running as root"
fi

for pkg in $packageList; do
	dpkg -l | grep -qw $pkg || apt install -y $pkg
done

if [ -d "$repo_dir" ]; then
    echo "[+] Repository $repo_dir exists."
else
    echo "[+] Repository $repo_dir not cloned yet.."
    git clone $repo_url
fi

touch $output_file | echo "case01,case02,case03,case04,case05" > $output_file
for i in $usecases; do
	csv_str=""

	# Collect data from one tecnology-scenario
	for j in $(seq 1 5);
	do
		csv_str+="$(cloc --csv --force-lang=C TFG/src/use_cases/$i/case0$j/*.{c,p4} 2>/dev/null | grep C | cut -d',' -f5),"
	done

	# Save data per CSV line
	echo ${csv_str::-1} >> $output_file
done
