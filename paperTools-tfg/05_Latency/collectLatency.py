#!/usr/bin/python3

import csv


LATENCY_FILE_XDP="Latency_XDP.csv"
LATENCY_FILE_P4="Latency_P4.csv"


def generateLatency():
    
    min_l=float(0)
    avg_l=float(0)
    max_l=float(0)
    stats = []

    for csvs in [LATENCY_FILE_XDP, LATENCY_FILE_P4]:
        with open(csvs, 'r') as csvfile:
            doc = csv.reader(csvfile, delimiter=',')
            for row in doc:
                min_l += float(row[0])            
                avg_l += float(row[1])            
                max_l += float(row[2])

        stats.append({"min": float(min_l/10), "avg": float(avg_l/10), "max": float(max_l/10)})

    with open('05_Latency.csv', 'w') as file:
        file.write('min,avg,max\n')
        for item in stats:
            file.write(str(item["min"])+','+str(item["avg"])+','+str(item["max"])+'\n')
    
        
if __name__ == '__main__':
    generateLatency()
