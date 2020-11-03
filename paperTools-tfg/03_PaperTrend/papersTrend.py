#!/usr/bin/python3

from bs4 import BeautifulSoup
import requests, json


#   XDP Google Scholar request : https://scholar.google.es/scholar?as_sdt=0,5&q=XDP+BPF+Linux&hl=es&as_ylo=2017&as_yhi=2018
#
#                               - As we seen it enables Patents: as_sdt=0
#                               - Main query: q=XDP+BPF+Linux
#                               - Language: hl=es
#                               - To configure year range: as_ylo=2017&as_yhi=2018
#
#   P4 Github request : https://api.github.com/search/repositories?q=language:p4

URL_XDP = 'https://scholar.google.es/scholar?as_sdt=0,5&q=XDP+BPF+Linux&hl=es'
URL_P4 = 'https://scholar.google.es/scholar?q=P4+language+sdn+Programming+protocol-independent+packet+processors&hl=es&as_sdt=0%2C5'

if __name__ == '__main__':

    years = ["2014","2015","2016","2017","2018","2019","2020"]
    listPapers = []

    for tecn in [{ "url" : URL_XDP, "data" : {}}, { "url" : URL_P4, "data" : {}}]:
        for year in years:
            r = requests.get(tecn["url"]+'&as_ylo='+year+'&as_yhi='+str(int(year)+1))
            soup = BeautifulSoup(r.content, 'html.parser')
            #tecn["data"][year] = soup.find_all('div', class_='gs_ab_mdw')[1].split(' ')[0]
            print(soup.find_all('div', class_='gs_ab_mdw')[1].get_text().split(' ')[0])
        print(str(tecn["data"]))
    
