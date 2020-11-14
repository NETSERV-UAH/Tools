#!/usr/bin/python3

import requests, json, os, datetime


#   XDP GitHub request : https://api.github.com/search/repositories?q=xdp+in:file+in:description+in:name+language:C&per_page=100&page=1
#
#   P4 Github request : https://api.github.com/search/repositories?q=language:p4

URL_XDP = 'https://api.github.com/search/repositories?q=xdp+in:file+in:description+in:name+language:C'
URL_P4 = 'https://api.github.com/search/repositories?q=language:p4'

# Put here your Token ID :)
URL_COMMITS = 'https://api.github.com/repos/'
TOKEN = ''


def getMaxPages(url, size):
    r = requests.get(url + '&per_page='+ str(size) + '&page=1', headers={'Authorization':'token '+TOKEN})
    pages = int(r.json()["total_count"] / size)
    if (r.json()["total_count"] % size != 0):
        pages += 1

    return pages

def getMaxPagesCommits(url, size):
    pages = 0
    while True:
        pages += 1
        r = requests.get(url + '?per_page='+ str(size) + '&page='+str(pages),headers={'Authorization':'token '+TOKEN})
        if not r.json():
            break
    return pages

def getListofCommits(url):

    lst = []
    size = 100
    pages = 0
    while True:
        pages += 1
        r = requests.get(url + '?per_page='+ str(size) + '&page='+str(pages), headers={'Authorization':'token '+TOKEN})
        if not r.json():
           break
        else:
            lst.append(r.json())

    return lst

def getListofJsons(url):

    lst = []
    size = 100
    for i in range(1,getMaxPages(url, size)):
        r = requests.get(url + '&per_page='+ str(size) + '&page=' + str(i), headers={'Authorization':'token '+TOKEN})
        lst.append(r.json())

    return lst
     

def getYear(strYear):
    return strYear[:4]


def generateActivity():
    
    listTecn = []    
    years = ["2015","2016","2017","2018","2019","2020"]

    for tecn in [{ "url" : URL_XDP, "data" : {}}, { "url" : URL_P4, "data" : {}}]:
        listJsons = getListofJsons(tecn["url"])
        for json in listJsons:
            for repo in json["items"]:
                listCommits = getListofCommits(URL_COMMITS + repo["full_name"] + '/commits')
                for year in years:
                    if isActive(year, listCommits):
                        if year in tecn["data"]:
                            tecn["data"][year] += 1
                        else:
                            tecn["data"][year] = 1
        listTecn.append(tecn) 

    
    generateCSV(listTecn)

def isActive(year, listCommits):
    for listCommit in listCommits:
        for commit in listCommit:
            if getYear(commit["commit"]["author"]["date"]) == year:
                return True
    return False


def generateCSV(listTech):
    with open('02_githubActivityPerYear.csv', 'w') as file:
        file.write('xdp,p4\n')
        for item in listTecn:    
            file.write(str(item["data"])+',')
        file.seek(file.tell() - 1, os.SEEK_SET) 
        file.truncate()
        file.write('\n') 


if __name__ == '__main__':

    generateActivity()
