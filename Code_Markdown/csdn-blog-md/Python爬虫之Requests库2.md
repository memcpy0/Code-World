# Requests库2：详解
[TOC]

## 1 文档分析
先看文档：
```python
>>> dir(requests)
['ConnectTimeout', 'ConnectionError', 'DependencyWarning', 
'FileModeWarning', 'HTTPError', 'NullHandler', 'PreparedRequest', 
'ReadTimeout', 'RequestException', 'RequestsDependencyWarning', 
'Timeout', 'TooManyRedirects', 'URLRequired',

'Request', 'Response', 'Session', 
 
'__author__', '__author_email__', '__build__', 
'__builtins__', '__cached__', '__cake__', '__copyright__', 
'__description__', '__doc__', '__file__', '__license__', 
'__loader__', '__name__', '__package__', '__path__',
 '__spec__', '__title__', '__url__', '__version__', '_internal_utils', 

'adapters', 'api', 'auth', 'certs', 'chardet',
'check_compatibility', 'codes', 'compat', 'cookies', 
'delete', 'exceptions', 'get', 'head', 'hooks', 
'logging', 'models', 'options', 'packages', 
'patch', 'post', 'put', 'pyopenssl', 
'request', 'session', 'sessions', 'status_codes', 
'structures', 'urllib3', 'utils', 'warnings']
```
我分了个段，这样看的清楚一些。
```python
>>> print(requests.ConnectTimeout) # 打印了其中一个类，属于异常类
<class 'requests.exceptions.ConnectTimeout'>
```
事实上，异常类有很多：
|   异常名称        |                含义说明         |
|:-----------------------|:---------------------|
|requests.ConnectionError |  网络连接错误异常，如DNS查询失败、拒绝连接等|
|requests.HTTPError     |  HTTP错误异常
|requests.URLRequired   |   URL缺失异常
|requests.TooManyRedirects  |超过最大重定向次数，产生重定向异常
|requests.ConnectTimeout |  连接远程服务器超时异常
|requests.Timeout       | 请求URL超时，产生超时异常
|……|……|
仔细看的话，第一段的文档都是异常和警告。

而我单独列出来的第二段又是什么呢？照样看文档：
```python
# requests.Request
Help on class Request in module requests.models:
class Request(RequestHooksMixin)
 |  A user-created :class:`Request <Request>` object.
 |
 |  Used to prepare a :class:`PreparedRequest <PreparedRequest>`, which is sent to the server.
 |
 |  :param method: HTTP method to use.
 |  :param url: URL to send.
 |  :param headers: dictionary of headers to send.
 |  :param files: dictionary of {filename: fileobject} files to multipart upload.
 |  :param data: the body to attach to the request. If a dictionary is provided, form-encoding will take place.
 |  :param json: json for the body to attach to the request (if files or data is not specified).
 |  :param params: dictionary of URL parameters to append to the URL.
 |  :param auth: Auth handler or (user, pass) tuple.
 |  :param cookies: dictionary or CookieJar of cookies to attach to this request.
 |  :param hooks: dictionary of callback hooks, for internal usage.
 |
 |  Usage::
 |
 |    >>> import requests
 |    >>> req = requests.Request('GET', 'http://httpbin.org/get')
 |    >>> req.prepare()
 |    <PreparedRequest [GET]>
```
上面的是requests库中最重要的对象之一：Request请求对象。准备一个PreparedRequest请求，传递给服务器。而且这里介绍了一些参数。
```python
Help on class Response in module requests.models:

class Response(builtins.object)
 |  The :class:`Response <Response>` object, which contains a
 |  server's response to an HTTP request.
 |
 |  Methods defined here:
 |
 |  __bool__(self)
 |      Returns True if :attr:`status_code` is less than 400.
 |
 |      This attribute checks if the status code of the response is between
 |      400 and 600 to see if there was a client error or a server error. If
 |      the status code, is between 200 and 400, this will return True. This
 |      is **not** a check to see if the response code is ``200 OK``.
 |
 |  __enter__(self)
 |
 |  __exit__(self, *args)
 |
 |  __getstate__(self)
 |
 |  __init__(self)
 |      Initialize self.  See help(type(self)) for accurate signature.
 |
 |  __iter__(self)
 |      Allows you to use a response as an iterator.
 |
 |  __nonzero__(self)
 |      Returns True if :attr:`status_code` is less than 400.
 |
 |      This attribute checks if the status code of the response is between
 |      400 and 600 to see if there was a client error or a server error. If
 |      the status code, is between 200 and 400, this will return True. This
 |      is **not** a check to see if the response code is ``200 OK``.
 |
 |  __repr__(self)
 |      Return repr(self).
 |
 |  __setstate__(self, state)
 |
 |  close(self)
 |      Releases the connection back to the pool. Once this method has been
 |      called the underlying ``raw`` object must not be accessed again.
 |
 |      *Note: Should not normally need to be called explicitly.*
 |
 |  iter_content(self, chunk_size=1, decode_unicode=False)
 |      Iterates over the response data.  When stream=True is set on the
 |      request, this avoids reading the content at once into memory for
 |      large responses.  The chunk size is the number of bytes it should
 |      read into memory.  This is not necessarily the length of each item
 |      returned as decoding can take place.
 |
 |      chunk_size must be of type int or None. A value of None will
 |      function differently depending on the value of `stream`.
 |      stream=True will read data as it arrives in whatever size the
 |      chunks are received. If stream=False, data is returned as
 |      a single chunk.
 |
 |      If decode_unicode is True, content will be decoded using the best
 |      available encoding based on the response.
 |
 |  iter_lines(self, chunk_size=512, decode_unicode=None, delimiter=None)
 |      Iterates over the response data, one line at a time.  When
 |      stream=True is set on the request, this avoids reading the
 |      content at once into memory for large responses.
 |
 |      .. note:: This method is not reentrant safe.
 |
 |  json(self, **kwargs)
 |      Returns the json-encoded content of a response, if any.
 |
 |      :param \*\*kwargs: Optional arguments that ``json.loads`` takes.
 |      :raises ValueError: If the response body does not contain valid json.
 |
 |  raise_for_status(self)
 |      Raises stored :class:`HTTPError`, if one occurred.
 |
 |  ----------------------------------------------------------------------
 |  Data descriptors defined here:
 |
 |  __dict__
 |      dictionary for instance variables (if defined)
 |
 |  __weakref__
 |      list of weak references to the object (if defined)
 |
 |  apparent_encoding
 |      The apparent encoding, provided by the chardet library.
 |
 |  content
 |      Content of the response, in bytes.
 |
 |  is_permanent_redirect
 |      True if this Response one of the permanent versions of redirect.
 |
 |  is_redirect
 |      True if this Response is a well-formed HTTP redirect that could have
 |      been processed automatically (by :meth:`Session.resolve_redirects`).
 |
 |  links
 |      Returns the parsed header links of the response, if any.
 |
 |  next
 |      Returns a PreparedRequest for the next request in a redirect chain, if there is one.
 |
 |  ok
 |      Returns True if :attr:`status_code` is less than 400.
 |
 |      This attribute checks if the status code of the response is between
 |      400 and 600 to see if there was a client error or a server error. If
 |      the status code, is between 200 and 400, this will return True. This
 |      is **not** a check to see if the response code is ``200 OK``.
 |
 |  text
 |      Content of the response, in unicode.
 |
 |      If Response.encoding is None, encoding will be guessed using
 |      ``chardet``.
 |
 |      The encoding of the response content is determined based solely on HTTP
 |      headers, following RFC 2616 to the letter. If you can take advantage of
 |      non-HTTP knowledge to make a better guess at the encoding, you should
 |      set ``r.encoding`` appropriately before accessing this property.
 |
 |  ----------------------------------------------------------------------
 |  Data and other attributes defined here:
 |
 |  __attrs__ = ['_content', 'status_code', 'headers', 'url', 'history', '...
 
```
这是requests库中的另一个绝对重要的对象：Response响应对象。它接受服务器返回的数据包。这里介绍了一些日后会用到的方法和属性，如：raise_for_status。
```python
Help on class Session in module requests.sessions:

class Session(SessionRedirectMixin)
 |  A Requests session.
 |
 |  Provides cookie persistence, connection-pooling, and configuration.
 |
 |  Basic Usage::
 |
 |    >>> import requests
 |    >>> s = requests.Session()
 |    >>> s.get('http://httpbin.org/get')
 |    <Response [200]>
 |
 |  Or as a context manager::
 |
 |    >>> with requests.Session() as s:
 |    >>>     s.get('http://httpbin.org/get')
 |    <Response [200]>
```
而这个Session，是提供cookie，保持登录状态的重要对象。

第四段的一些则是我们日常使用的命令和函数，像其中的['request', 'session']等不是对象，而是具体使用的函数，注意：没有response函数。

***
## 1.5 补充的HTTP协议
**HTTP，Hypertext Transfer Protocol，超文本传输协议**

HTTP是一个基于“请求与响应”模式的、无状态的应用层协议，采用URL作为定位网络资源的标识，URL格式如下：	 
         
		http://host[:port][path]
			host: 合法的Internet主机域名或IP地址
			port: 端口号，缺省端口为80
			path: 请求资源的路径
HTTP URL实例：

   -  http://www.bit.edu.cn
   -  http://220.181.111.188/duty

HTTP URL的理解：URL是通过HTTP协议存取资源的Internet路径，一个URL对应一个数据资源 

HTTP协议对资源的操作方法 |  方法说明|  Requests库方法  | 功能一致性
----------- | :------------------|:-----------|:------------------
GET       | 请求获取URL位置的资源|    requests.get()  |  一致    
HEAD      |  请求获取URL位置资源的响应消息报告，即获得该资源的头部信息|   requests.head()  | 一致    
POST      | 请求向URL位置的资源后附加新的数据|    requests.post() |  一致   
PUT      | 请求向URL位置存储一个资源，覆盖原URL位置的资源|     requests.put()  |  一致
PATCH    | 请求局部更新URL位置的资源，即改变该处资源的部分内容|      requests.patch() |  一致
DELETE   |  请求删除URL位置存储的资源|    requests.delete() |  一致

**理解PATCH和PUT的区别：**
```
假设URL位置有一组数据UserInfo，包括UserID、UserName等20个字段
需求：用户修改了UserName，其他不变，应用PATCH
	• 采用PATCH，仅向URL提交UserName的局部更新请求
    • 采用PUT，必须将所有20个字段一并提交到URL，未提交字段被删除
PATCH的最主要好处：节省网络带宽
```
***
## 2 方法略述
`import requests`引入requests库
### Requests库的7个主要方法
|         方法           |         说明|
|:----------------------|:------------|
|   requests.request()  | 构造一个请求，支撑以下各方法的基础方法
 | requests.get()   |   获取HTML网页的主要方法，对应于HTTP的GET
|requests.head()     |获取HTML网页头信息的方法，对应于HTTP的HEAD
|requests.post()  |   向HTML网页提交POST请求的方法，对应于HTTP的POST
|requests.put()    |  向HTML网页提交PUT请求的方法，对应于HTTP的PUT
| requests.patch()   |  向HTML网页提交局部修改请求，对应于HTTP的PATCH
|requests.delete()  |  向HTML页面提交删除请求，对应于HTTP的DELETE


实际上，这是与requests库中的Request对象连接起来的，这些方法也就是上面看到的第四段的文档中的内容，都出现过了。

而这里看似有7种方法，对应第四段文档的各种方法，其实只有一种，就是requests.request(method, url, **kwargs)方法，它是基础的方法。看[昨天介绍的文档](https://blog.csdn.net/myRealization/article/details/79783514)中的使用示例。

所以，下面的这些方法与上面的是等价的。但我们还是经常用上面的，因为简单一些。
![](http://img.blog.csdn.net/20180403002603278?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbXlSZWFsaXphdGlvbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

### 2.0 方法示例
```python
import requests
# head() 方法
r = requests.head('http://httpbin.org/get')
print(r.headers)
```