#!/bin/python

DIFF_FILE_XDP="memory_XDP.diff"
INPUT_FILES = ["kerneltest.1", "kerneltest.2"]

def generateMemoryDiff():
    stats = {"kerneltest.1" : [], "kerneltest.2": []}
    for filediff in INPUT_FILES:
        with open(filediff, 'r') as file:
            for line in file:
                str_mean = line.split(' ')[0]
                tmep = line.split(' ')[item for item in templist if item]
                print(str(mem_mean))
                stats[filediff].append([str_mean, mem_mean])

    #print(str(stats))
if __name__ == '__main__':
    generateMemoryDiff()

