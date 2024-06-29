import requests
from bs4 import BeautifulSoup
import os

def getHTMLText(url):
    try:
        kv={'user-agent':'Mozilla/5.0'}
        r=requests.get(url,headers=kv,timeout=600)
        r.raise_for_status()
        r.encoding=r.apparent_encoding
        return r.text
    except Exception as err:
        print(err)

#def Bookpage(uls,html):
  #  soup=BeautifulSoup(html,'html.parser')
   # for div in soup.find('div','box1',id='book_detail').next_sibling:
      #  for link in soup.find_all('li'):
         #   uls.append(link.a.get['href'])
       # break

def getBOOKTxt(bhtml):
    newsoup=BeautifulSoup(bhtml,'html.parser')
    title=newsoup.h1.string
    
    try:
        root="D://2345下载//"
        path=root+"花千骨.txt"
        #if not os.path.exists(root):
          #  os.mkdir(root)
        if not os.path.exists(path):
             with open (path,'a+') as f:
                f.write(title)
                for p in newsoup('p'):
                    f.write(p.string)
                    f.close()
        #else:
          #  print("existed")
    except Exception as err:
        print(err)
            
def main():
    start_url="http://www.136book.com/huaqiangu/"
    html=getHTMLText(start_url)
    #urls=[]
    #Bookpage(urls,html)
    soup=BeautifulSoup(html,'html.parser')
    for div in soup.find('div','box1',id='book_detail').next_sibling:
        for i in range(0,164):
            for link in soup.find_all('li')[i]:
                url=soup.li.a.attrs['href']
                bhtml=getHTMLText(url)
                getBOOKTxt(bhtml)
                    
main()
