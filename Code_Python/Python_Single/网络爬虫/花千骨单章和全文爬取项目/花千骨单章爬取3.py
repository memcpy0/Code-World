import requests
from bs4 import BeautifulSoup

def getHTMLText(url,kv):
    try:
        r=requests.get(url,headers=kv,timeout=11)
        r.raise_for_status()
        r.encoding=r.apparent_encoding
        return r.text
    except:
        print("getHTMLTexterror!")

def printTxt(html):
    import bs4
    soup=BeautifulSoup(html,'html.parser')
    print(soup.h1.string)
    #for div in soup.find('div',id='content').children:
        #if isinstance(p,bs4.element.Tag):
            # Ps=div('p')
    for p1 in soup('p'):
          print(p1.string)
         # break 不能跳出！


def main():
    url="http://www.136book.com/huaqiangu/ebxeeql/"
    kv={'user-agent':'Mozilla/5.0'}
    
    html=getHTMLText(url,kv)
    printTxt(html)

main()
