#!/usr/bin/python3

from bs4 import BeautifulSoup
import requests, json, time


#   XDP Google Scholar request : https://scholar.google.es/scholar?as_sdt=0,5&q=XDP+BPF+Linux&hl=es&as_ylo=2017&as_yhi=2018
#
#                               - As we seen it enables Patents: as_sdt=0
#                               - Main query: q=XDP+BPF+Linux
#                               - Language: hl=es
#                               - To configure year range: as_ylo=2017&as_yhi=2018
#
#   P4 Github request : https://api.github.com/search/repositories?q=language:p4

URL_XDP = 'https://scholar.google.es/scholar?as_sdt=0,5&q=XDP+BPF+Linux&hl=es'
URL_P4 = 'https://scholar.google.es/scholar?q=P4+language+sdn+Programming+protocol-independent+packet+processors&hl=es&as_sdt=0'
TIMEOUT = 30


def getProxies():
    url = 'https://free-proxy-list.net/'
    proxies = [] 
    
    r = requests.get(url)
    soup = BeautifulSoup(r.content, 'html.parser')
    
    for proxy in soup.select('#proxylisttable > tbody > tr'):
        if proxy.select('td.hx')[0].get_text() == 'yes':
            proxies.append({"ipAddr" : proxy.select('td:nth-child(1)')[0].get_text(), "port" : proxy.select('td:nth-child(2)')[0].get_text(), "full" : proxy.select('td:nth-child(1)')[0].get_text()+':'+ proxy.select('td:nth-child(2)')[0].get_text()})
         
    return proxies

if __name__ == '__main__':

    years = ["2014","2015","2016","2017","2018","2019","2020"]
    listPapers = []
    proxies = getProxies()

   
    for tecn in [{ "url" : URL_XDP, "data" : {}}, { "url" : URL_P4, "data" : {}}]:
        for year in years:
            for proxy in proxies:
                try:
                    r = requests.get(tecn["url"]+'&as_ylo='+year+'&as_yhi='+str(int(year)+1), proxies={"http": 'http://'+proxy["full"], "https": 'https://'+proxy["full"]}, timeout=TIMEOUT, verify=False)
                    soup = BeautifulSoup(r.content, 'html.parser')
                    print(soup.prettify())
                    if soup.find_all('div', class_='gs_ab_mdw'):
                        print(tecn["url"]+'&as_ylo='+year+'&as_yhi='+str(int(year)+1)+ ' : ' + soup.find_all('div', class_='gs_ab_mdw')[1].get_text())
                        break
                except  Exception as e: 
                    print(e)
                    print('Skipping. Connnection error')
                    time.sleep(0.2)

