#!/bin/python

DIFF_FILE_XDP="memory_XDP.diff"
INPUT_FILES = ["kerneltest.1", "kerneltest.2"]

def generateMemoryDiff():
    stats = {"kerneltest.1" : [], "kerneltest.2": []}
    for filediff in INPUT_FILES:
        with open(filediff, 'r') as file:
            for line in file:
                str_mean = line.split(' ')[0]
                mem_mean = [item for item in line.split(' ') if item]
                if len(mem_mean) == 3:
                    stats[filediff].append([str_mean, mem_mean[1]])
                
    # Print or save data
    for items in range(0,len(stats[INPUT_FILES[0]])):
        print('[+] '+stats[INPUT_FILES[0]][items][0]+' = '+ str(int(stats[INPUT_FILES[1]][items][1])-int(stats[INPUT_FILES[0]][items][1])))



if __name__ == '__main__':
    generateMemoryDiff()

