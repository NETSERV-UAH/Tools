#!/usr/bin/python3

import requests, json, os


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


def generateTrendByYear():
    
    listTecn = []    
    years = ["2015","2016","2017","2018","2019","2020"]
    
    for tecn in [{ "url" : URL_XDP, "data" : {}}, { "url" : URL_P4, "data" : {}}]:

        listJsons = getListofJsons(tecn["url"])
        
        for json in listJsons:
            for item in json["items"]:
                if getYear(item["created_at"]) in tecn["data"]:
                    tecn["data"][getYear(item["created_at"])] += 1
                else:
                    tecn["data"][getYear(item["created_at"])] = 1
    
        listTecn.append(tecn) 

        print(str(tecn["data"]))
    
    with open('01_githubTrendYear.csv', 'w') as file:
        file.write('2015,2016,2017,2018,2019,2020\n')
        for item in listTecn:
            for year in years:
                file.write(str(item["data"][year])+',')
            file.seek(file.tell() - 1, os.SEEK_SET) 
            file.truncate()
            file.write('\n')


if __name__ == '__main__':

    generateTrendByYear()
