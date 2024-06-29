# Python网络爬虫知识整合
[TOC]

## Requests常用命令整合
[Requests中文文档之快速上手](http://docs.python-requests.org/zh_CN/latest/user/quickstart.html#post)
[Requests中文文档之高级用法](http://docs.python-requests.org/zh_CN/latest/user/advanced.html)

### Requests之发送请求
- `r = requests.get(url, ……）`
```python
>>> r = requests.get('https://api.github.com/user', 
auth=('user', 'pass'))
```
现在，我们有一个名为`r`的`Response`对象。我们可以从这个对象中获取所有我们想要的信息。Requests简便的API(应用编程接口)意味着所有 HTTP请求类型都是显而易见的。
```python
>>> r = requests.post('http://httpbin.org/post', 
data = {'key':'value'})
>>> r = requests.put('http://httpbin.org/put', 
data = {'key':'value'})
>>> r = requests.delete('http://httpbin.org/delete')
>>> r = requests.head('http://httpbin.org/get')
>>> r = requests.options('http://httpbin.org/get')
```
### Requests之定制请求
#### 超时-timeout参数
可以告诉requests 在经过以`timeout`参数设定的秒数时间之后停止等待响应。基本上**所有的生产代码都应该使用这一参数**。如果不使用，你的程序可能会永远失去响应：
```python
>>> requests.get('http://github.com', timeout=0.001)
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
requests.exceptions.Timeout: HTTPConnectionPool(host='github.com', port=80): 
Request timed out. (timeout=0.001)
```

#### 定制请求头-headers参数
如想为请求添加 HTTP 头部，只要简单地传递一个dict给headers参数就可以了。
```python
>>> url = 'https://api.github.com/some/endpoint'
>>> headers = {'user-agent': 'my-app/0.0.1'}
>>> r = requests.get(url, headers=headers)
```
Requests不会基于定制header的具体情况改变自己的行为。只不过在最后的请求中，所有的header信息都会被传递进去。

#### 传递URL参数-params参数
为URL的查询字符串(query string)传递URL参数，手工构建URL，数据会以键/值对的形式置于URL中，跟在一个问号的后面：
我们可以使用`params`关键字参数，以一个字符串字典来提供这些参数。
```python
>>> payload = {'key1': 'value1', 'key2': 'value2'}
>>> r = requests.get("http://httpbin.org/get", params=payload)
>>> print(r.url)
http://httpbin.org/get?key2=value2&key1=value1
>>> payload = {'key1': 'value1', 'key2': ['value2', 'value3']}
# 将一个列表作为值传入
# 字典里值为None的键都不会被添加到URL的查询字符串里
>>> r = requests.get('http://httpbin.org/get', params=payload)
>>> print(r.url)
http://httpbin.org/get?key1=value1&key2=value2&key2=value3
```

#### 更加复杂的POST请求-data参数
如果想要发送一些编码为表单形式的数据——非常像一个HTML表单。要实现这个，只需简单地传递一个字典给 data 参数。你的数据字典在发出请求时会自动编码为表单形式：
```python
>>> payload = {'key1': 'value1', 'key2': 'value2'}
>>> r = requests.post("http://httpbin.org/post", 
data=payload)
>>> print(r.text)
{
  ...
  "form": {
    "key2": "value2",
    "key1": "value1"
  },
  ...
}
```
还可以为`data`参数传入一个元组列表。在表单中多个元素使用同一 key 的时候，这种方式尤其有效：
```python
>>> payload = (('key1', 'value1'), ('key1', 'value2'))
>>> r = requests.post('http://httpbin.org/post', 
data=payload)
>>> print(r.text)
{
  ...
  "form": {
    "key1": [
      "value1",
      "value2"
    ]
  },
  ...
}
```
很多时候你想要发送的数据**并非编码为表单形式**的。如果你传递一个string而不是一个dict，那么数据会被直接发布出去。

例如，Github API v3 接受编码为 JSON 的 POST/PATCH 数据：
```python
>>> import json
>>> url = 'https://api.github.com/some/endpoint'
>>> payload = {'some': 'data'}
>>> r = requests.post(url, data=json.dumps(payload))
```
此处除了可以自行对dict进行编码，你还可以使用`json参数`直接传递，然后它就会被自动编码。这是 2.4.2 版的新加功能：
```
>>> url = 'https://api.github.com/some/endpoint'
>>> payload = {'some': 'data'}
>>> r = requests.post(url, json=payload)
```
#### 代理-proxies参数
如果需要使用代理，你可以通过为任意请求方法提供`proxies参数`来配置单个请求:
```python
import requests

proxies = {
  "http": "http://10.10.1.10:3128",
  "https": "http://10.10.1.10:1080",
}

requests.get("http://example.org", proxies=proxies)
```
你也可以通过环境变量`HTTP_PROXY和HTTPS_PROXY`来配置代理。
```python
$ export HTTP_PROXY="http://10.10.1.10:3128"
$ export HTTPS_PROXY="http://10.10.1.10:1080"

$ python
>>> import requests
>>> requests.get("http://example.org")
```
若你的代理需要使用`HTTP Basic Auth`，可以使用`http://user:password@host/`语法：
```python
proxies = {
    "http": "http://user:pass@10.10.1.10:3128/",
}
```
要为某个特定的连接方式或者主机设置代理，使用`scheme://hostname`作为key， 它会针对指定的主机和连接方式进行匹配。
```python
proxies = {'http://10.20.1.128': 'http://10.10.1.10:5323'}
```
注意，代理URL必须包含连接方式。

### Requests之响应内容查看和处理
读取服务器响应的内容，包括:
#### 响应状态码
- `r.status_code`：要检查请求是否成功，使用`r.raise_for_status()`或者检查`r.status_code`是否和期望相同。
```python
>>> r = requests.get('http://httpbin.org/get')
>>> r.status_code
200
# 为方便引用，Requests还附带了一个内置的状态码查询对象
>>> r.status_code == requests.codes.ok
True
```
#### 响应头(HTTP头部是大小写不敏感的)
- `r.headers`: 可以查看以一个Python字典形式展示的服务器响应头:
```python
>>> r.headers
{
    'content-encoding': 'gzip',
    'transfer-encoding': 'chunked',
    'connection': 'close',
    'server': 'nginx/1.0.4',
    'x-runtime': '148ms',
    'etag': '"e1ca502697e5c9317743dc078f67693f"',
    'content-type': 'application/json'
}
```
因此，我们可以使用任意大写形式来访问这些响应头字段：
```python
>>> r.headers['Content-Type']
'application/json'
>>> r.headers.get('content-type')
'application/json'
```
- `r.encoding`：Requests会基于**HTTP头部**对响应的编码作出有根据的推测。当你访问`r.text`之时，Requests会使用其推测的文本编码。你可以找出Requests使用了什么编码，并且能够使用`r.encoding`属性来改变它。如果你改变了编码，每当你访问 `r.text`，Request都将会使用`r.encoding`的新值。
```python
>>> r.encoding
'utf-8'
>>> r.encoding = 'ISO-8859-1'
```
#### 响应内容
- `r.text`
- `r.json()`：JSON响应内容。Requests中也有一个内置的JSON解码器，助你处理JSON数据。如果JSON解码失败，r.json()就会抛出一个异常。
```python
>>> import requests
>>> r = requests.get('https://api.github.com/events')
>>> r.json()
[{u'repository': {u'open_issues': 0, u'url': 'https://github.com/...
```

- `r.content`：二进制响应内容。以字节的方式访问请求响应体，对于非文本请求二进制响应内容。Requests会自动为你**解码gzip和deflate传输编码**的响应数据。
```python
>>> r.content
b'[{"repository":{"open_issues":0,"url":"https://github.com/...
```
- `r.raw()`：原始响应内容。在罕见的情况下，可能想获取来自服务器的原始套接字响应，那么可以访问r.raw。 如果确实想这么干，那请你确保在初始请求中设置了`stream=True`。具体这么做：
```python
>>> r = requests.get('https://api.github.com/events', stream=True)
>>> r.raw
<requests.packages.urllib3.response.HTTPResponse object at 0x101194810>
>>> r.raw.read(10)
'\x1f\x8b\x08\x00\x00\x00\x00\x00\x00\x03'
```
但一般情况下，你应该以下面的模式**将文本流保存到文件**：
```python
with open(filename, 'wb') as fd:
    for chunk in r.iter_content(chunk_size):
        fd.write(chunk)
```
使用`Response.iter_content`将会处理大量你直接使用 Response.raw 不得不处理的。当流下载时，上面是**优先推荐**的获取内容方式。

#### Cookie-cookies参数
如果某个响应中包含一些`cookie`，你可以快速访问它们：
```python
>>> url = 'http://example.com/some/cookie/setting/url'
>>> r = requests.get(url)
>>> r.cookies['example_cookie_name']
'example_cookie_value'
```
要想发送你的cookies到服务器，可以使用cookies参数：
```python
>>> url = 'http://httpbin.org/cookies'
>>> cookies = dict(cookies_are='working')
>>> r = requests.get(url, cookies=cookies)
>>> r.text
'{"cookies": {"cookies_are": "working"}}'
```

#### 重定向与请求历史
默认情况下，除了HEAD，Requests会自动处理所有重定向。
可以使用响应对象的`history`方法来追踪重定向。`Response.history`是一个 Response 对象的列表，为了完成请求而创建了这些对象。这个对象列表按照**从最老到最近**的请求进行排序。

例如，Github将所有的HTTP请求重定向到 HTTPS：
```python
>>> r = requests.get('http://github.com')

>>> r.url
'https://github.com/'

>>> r.status_code
200

>>> r.history
[<Response [301]>]
```
如果你使用的是GET、OPTIONS、POST、PUT、PATCH 或者 DELETE，那么你可以通过`allow_redirects`参数禁用重定向处理：
```python
>>> r = requests.get('http://github.com', 
allow_redirects=False)
>>> r.status_code
301
>>> r.history
[]
```
如果你使用了HEAD，你也可以启用重定向：
```python
>>> r = requests.head('http://github.com', 
allow_redirects=True)
>>> r.url
'https://github.com/'
>>> r.history
[<Response [301]>]
```
********
## bs4常用命令整合
[bs4 4.4.0中文文档](http://beautifulsoup.readthedocs.io/zh_CN/v4.4.0/#id27)
Beautiful Soup支持Python标准库中的HTML解析器,还支持一些第三方的解析器,其中一个是`lxml`，另一个可供选择的解析器是纯Python实现的`html5lib`, html5lib的解析方式与浏览器相同,可以选择下列方法来安装html5lib。

### bs4导入解析
- `from bs4 import BeautifulSoup`
- `soup = BeautifulSoup(html_doc, 'html.parser')`
- `print(soup.prettify())`  用户友好格式输出

### 几个简单的浏览结构化数据的方法:


## XPATH常用命令整合
