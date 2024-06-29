# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 
program     : 
"""

import pymysql
import requests
from scrapy.selector import Selector

__author__ = 'demon'
"""
我们最后需要的ip代理是这种格式的{'http': 'http://113.105.146.77:8086'}，这里面包含了协议ip地址以及端口号
我们需要在页面中提取这些内容，我还提取的速度，因为有些ip地址的速度太TMD慢了，拿来也并没有什么卵用！
我们爬取前1000页中的免费ip地址，然后在代码中实现了获取当前的免费ip地址，端口，以及协议及速度，
然后把提取到的每页中的我们需要的信息到放到一个元祖中，循环每页中的列表，把和免费代理ip相关的信息放到数据库中
"""

conn = pymysql.connect(host='localhost', user='root', passwd='wdMySQLmm123', db='xc_proxy', charset='utf8')
cursor = conn.cursor()

def crawl_ips():
    headers = {
        "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) "
                      "Chrome/59.0.3071.115 Safari/537.36"
    }
    for i in range(1, 11):
        url = 'http://www.xicidaili.com/wt/{0}'.format(i)
        req = requests.get(url=url, headers=headers)
        selector = Selector(text=req.text)
        all_trs = selector.xpath('//*[@id="ip_list"]//tr')

        ip_lists = []
        for tr in all_trs[1:]:
            speed = 0.0
            speed_str = tr.xpath('td[7]/div/@title').extract()[0]
            if speed_str:
                speed = float(speed_str.split('秒')[0])
            ip = tr.xpath('td[2]/text()').extract()[0]
            port = tr.xpath('td[3]/text()').extract()[0]
            proxy_type = tr.xpath('td[6]/text()').extract()[0].lower()
            ip_lists.append((ip, port, speed, proxy_type))

        for ip_info in ip_lists:
            cursor.execute(
                f"INSERT proxy_ip(ip,port,speed,proxy_type) VALUES('{ip_info[0]}','{ip_info[1]}',{ip_info[2]},"
                f"'{ip_info[3]}') "
            )
            conn.commit()

class GetIP(object):
    def delete(self,ip):
        delete_sql = 'DELETE FROM proxy_ip WHERE ip="{0}"'.format(ip)
        cursor.execute(delete_sql)
        conn.commit()
        return True

    def valid_ip(self, ip, port, proxy_type):
        headers = {
            "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) "
                          "Chrome/59.0.3071.115 Safari/537.36"}
        try:
            proxies = {proxy_type: proxy_type + '://' + ip + ':' + port}
            req = requests.get('http://ip.chinaz.com/getip.aspx', headers=headers, proxies=proxies, timeout=3)
        except:
            print('invalid ip and port')
            self.delete(ip)
            return False
        else:
            if 200 <= req.status_code < 300:
                # print('{0} effective ip~'.format(proxies))
                print(req.text)
                return True
            else:
                print('invalid ip and port')
                self.delete(ip)
                return False

    @property
    def get_random_ip(self):
        random_ip = 'SELECT proxy_type,ip, port FROM proxy_ip  ORDER BY RAND()  LIMIT 1;'
        cursor.execute(random_ip)
        proxy_type, ip, port = cursor.fetchone()
        valid_ip = self.valid_ip(ip, port, proxy_type)
        if valid_ip:
            return {proxy_type: proxy_type + '://' + ip + ':' + port}
        else:
            return self.get_random_ip

if __name__ == '__main__':
    proxy = GetIP()
    print(proxy.get_random_ip)