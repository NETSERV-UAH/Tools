#!/usr/bin/python3

import requests, json, os, datetime


#   XDP GitHub request : https://api.github.com/search/repositories?q=xdp+in:file+in:description+in:name+language:C&per_page=100&page=1
#
#   P4 Github request : https://api.github.com/search/repositories?q=language:p4

URL_XDP = 'https://api.github.com/search/repositories?q=xdp+in:file+in:description+in:name+language:C'
URL_P4 = 'https://api.github.com/search/repositories?q=language:p4'

def getMaxPages(url, size):
    r = requests.get(url + '&per_page='+ str(size) + '&page=1')
    pages = int(r.json()["total_count"] / size)
    if (r.json()["total_count"] % size != 0):
        pages += 1

    return pages

def getListofJsons(url):

    lst = []
    size = 100
    for i in range(1,getMaxPages(url, size)):
        r = requests.get(url + '&per_page='+ str(size) + '&page=' + str(i))
        lst.append(r.json())

    return lst
     

def getYear(strYear):
    return strYear[:4]


def generateActivity():
    
    listTecn = []    
    
    for tecn in [{ "url" : URL_XDP, "data" : 0}, { "url" : URL_P4, "data" : 0}]:

        listJsons = getListofJsons(tecn["url"])
        
        for json in listJsons:
            for item in json["items"]:
                if getYear(item["updated_at"]) == str(datetime.datetime.now().year):
                    tecn["data"] += 1
    
        listTecn.append(tecn) 

    
    with open('02_githubActivity.csv', 'w') as file:
        file.write('xdp,p4\n')
        for item in listTecn:    
            file.write(str(item["data"])+',')
        file.seek(file.tell() - 1, os.SEEK_SET) 
        file.truncate()
        file.write('\n') 


if __name__ == '__main__':

    generateActivity()
