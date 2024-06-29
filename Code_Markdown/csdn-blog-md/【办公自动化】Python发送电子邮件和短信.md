@[toc]


对我而言，这是个很新奇的领域。用Python来发送电子邮件和短信什么的……有什么用呢？检查和答复电子邮件/短信会占用很多时间，只是这不是理所当然的吗？很多邮件/短信都需要自己阅读、要有自己的回应，**不可能只写一个程序来应付所有的电子邮件/短信**！不过如果掌握了这一技能，确实可以**自动化一些与电子邮件和短信相关的任务**：
- **通过自己编写程序来批量发送电子邮件，能够节省大量复制和粘贴电子邮件的时间**。比如说，根据客户或其他人的记录信息（如年龄和位置），批量发送不同格式的电子邮件。
- **通过编程发送邮件和短信来通知自己，从而在离开计算机时能专注于更重要的事情**。比如说，执行某个自动化任务需要蛮久，不想时刻等在计算机前或者每过几分钟就看一眼计算机，就可以编写程序，使其在完成时向手机发送邮件或短信。

---
# 1. SMTP和IMAP、IMAP与POP的区别
`SMTP` 简单邮件传输协议是**用于发送电子邮件的协议**，类似于 `HTTP` 是**计算机用来通过因特网发送网页的协议**。SMTP规定电子邮件如何**格式化**、**加密**、**在邮件服务器之间传递**、以及其他发送细节。不过这些都不重要，Python的 `smtplib` 模块已经将它们简化为几个函数。

SMTP只**负责向他人发送电子邮件**，另一个协议 `IMAP` **负责取回发送给你的电子邮件**。IMAP，即互联网消息访问协议 `Internet Message Access Protocol`，它规定了**如何与Email服务提供商的服务器通信**、**取回发送到个人电子邮件地址的邮件**，通过这种协议就可以从邮件服务器上**获取邮件信息、下载邮件**等等。

IMAP与POP类似，都是一种邮件获取协议，但两者也存在很大的区别：
- POP允许电子邮件客户端下载服务器上的邮件，但**在电子邮件客户端的操作**（如移动邮件、标记已读等）**是不会反馈到服务器上的**，比如通过Email客户端收取了QQ邮箱中的3封邮件、并移动到了其他文件夹，这些动作不会反馈到服务器，即QQ邮箱服务器上的这些邮件没有同时被移动。因此，**POP是单向的**。
IMAP就不同，**电子邮件客户端的操作都会反馈到服务器上**，对邮件进行操作后（如：移动邮件、标记已读等），服务器上的邮件也会做相应的动作。因此，**IMAP是双向的**。
- IMAP还可以只下载邮件的主题，只有当真正需要的时候，才会下载邮件的所有内容。POP做不到这一点。

Python自带了一个 `imaplib` 模块，只是第三方的  `imapclient` 模块更易用，IMAPClient的完整文档在[http://imapclient.readthedocs.org/](http://imapclient.readthedocs.org/)。`imapclient` 模块从IMAP服务器下载Email，格式相当复杂，可以使用 `pyzmail` 模块来解析这些邮件，PyzMail的完整文档在[http://www.magiksys.net/pyzmail/](http://www.magiksys.net/pyzmail/)。

安装PyzMail时可能会出错，在Stack Overflow上面有解答：
![在这里插入图片描述](https://img-blog.csdnimg.cn/0a8d4c7482bb4f84a2dd7435690a24a4.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
因此运行命令 `pip install pyzmail36` 即可：
![在这里插入图片描述](https://img-blog.csdnimg.cn/216ab8b0b0d34e809673c01186793516.png)

---
# 2. 用SMTP发送电子邮件
在GMail、QQ邮箱、网易邮箱等网站/软件中发送电子邮件是一件很轻松的事，不过Python没有提供这样的一个GUI，取而代之的是调用函数来执行SMTP的每个步骤。如下（不可用的）代码示例展示了用Python发送电子邮件的过程：
```py
>>> import smtplib 
>>> smtpObj = smtplib.SMTP('smtp.example.com', 587) 
>>> smtpObj.ehlo() 
(250, b'mx.example.com at your service, [216.172.148.131]\nSIZE 35882577\ n8BITMIME\nSTARTTLS\nENHANCEDSTATUSCODES\nCHUNKING') 
>>> smtpObj.starttls() 
(220, b'2.0.0 Ready to start TLS') 
>>> smtpObj.login('bob@example.com', 'MY_SECRET_PASSWORD') 
(235, b'2.7.0 Accepted') 
>>> smtpObj.sendmail('bob@example.com', 'alice@example.com', 'Subject: So long.\nDear Alice, so long and thanks for all the fish. Sincerely, Bob') 
{}
>>> smtpObj.quit() 
(221, b'2.0.0 closing connection ko10sm23097611pbd.52 - gsmtp')
```
## 2.1 连接到SMTP服务器
首要的事情是，**用Python连接到你的电子邮件账户**，为此需要**配置SMTP服务器和端口**，不过这些设置因电子邮件服务提供商而不同。**SMTP服务器的域名通常是 `smtp.` 加上EMail服务提供商的域名**，端口则是一个整数值、且几乎总是 `587` ，该端口由命令加密标准 `TLS` 使用。下面列出本人常用的SMTP服务器域名及其提供商：
| 提供商 | SMTP服务器域名
|:---|:---
| Gmail | `smtp.gmail.com` 
| QQ | `smtp.qq.com`  
| Outlook.com/Hotmail.com  | `smtp-mail.outlook.com` 

特别注意，如要使用QQ邮箱的SMTP/IMAP服务，需要自行在**邮箱设置->帐户**中开启**IMAP/SMTP服务**，之后会得到一个授权码，用于后面的登录：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2ca677f8ac0e467b8242bb693ac1c2f9.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
得到SMTP服务器域名和端口信息后，调用 `smtplib.SMTP()` 并传入域名作为字符串参数、传入端口作为整数参数，创建一个SMTP对象，**SMTP对象表示与SMTP邮件服务器的连接**。我们需要调用它的方法来登录和发送电子邮件。下面分别连接到Gmail和QQ邮箱，`type(smtpObj)` 表明 `smtpObj` 中保存了一个SMTP对象：
```python
>>> import smtplib
>>> smtpObj = smtplib.SMTP('smtp.gmail.com', 587)
>>> type(smtpObj)
<class 'smtplib.SMTP'>
>>> smtpObj2 = smtplib.SMTP('smtp.qq.com', 587)
>>> type(smtpObj2)
<class 'smtplib.SMTP'>
```
如果 `smtplib.SMTP()` 调用不成功，可能是SMTP服务器不支持TLS端口 `587` ，此时需要使用 `smtplib.SMTP_SSL()` 和 `465` 端口来创建SMTP对象。对个人程序来说，TLS和SSL之间的区别不重要，**只要知道目标SMTP服务器使用哪种加密标准，就知道如何连接它**：

```haskell
>>> smtpObj3 = smtplib.SMTP_SSL('smtp.gmail.com', 465)
```
如果此时没有连接到互联网，Python将抛出 `socket.gaierror: [Errno 11004] getaddrinfo
failed` 或类似的异常。


如果您的邮件客户端不在上述列出的范围内，您可以尝试如下通用配置：
接收邮件服务器：imap.qq.com
发送邮件服务器：smtp.qq.com
账户名：您的QQ邮箱账户名（如果您是VIP邮箱，账户名需要填写完整的邮件地址）
密码：您的QQ邮箱密码
电子邮件地址：您的QQ邮箱的完整邮件地址
 
如何设置IMAP服务的SSL加密方式？
使用SSL的通用配置如下：
接收邮件服务器：imap.qq.com，使用SSL，端口号993
发送邮件服务器：smtp.qq.com，使用SSL，端口号465或587
账户名：您的QQ邮箱账户名（如果您是VIP帐号或Foxmail帐号，账户名需要填写完整的邮件地址）
密码：您的QQ邮箱密码
电子邮件地址：您的QQ邮箱的完整邮件地址
 
![在这里插入图片描述](https://img-blog.csdnimg.cn/76e7b185562d4882a76cd96c5bfc1324.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/0f1a9651fe584d519f8cbc78ef426855.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/790083a823ff46bb96849de1cb9d1250.png)

## 2.2 发送SMTP的"Hello"消息
得到SMTP对象后，调用古怪的 `ehlo()` 方法向SMTP邮件服务器打招呼，**这是SMTP的第一步**，对于建立到服务器的连接是很重要的。细节无关紧要，**只要确保得到SMTP对象后第一件事就是调用 `ehlo()` 方法**（否则后续的方法调用会出现错误）：

```haskell
>>> import smtplib
>>> gmail = smtplib.SMTP('smtp.gmail.com', 587)
>>> qq = smtplib.SMTP('smtp.qq.com', 587)
>>> gmail.ehlo()
(250, b'smtp.gmail.com at your service, [47.57.140.157]\nSIZE 35882577\n8BITMIME\nSTARTTLS\nENHANCEDSTATUSCODES\nPIPELINING\nCHUNKING\nSMTPUTF8')
>>> qq.ehlo()
(250, b'newxmesmtplogicsvrszb6.qq.com\nPIPELINING\nSIZE 73400320\nSTARTTLS\nAUTH LOGIN PLAIN XOAUTH XOAUTH2\nAUTH=LOGIN\nMAILCOMPRESS\n8BITMIME')
```
如果返回的元组中，第一项是整数 `250` 即SMTP中的成功状态码，则问候成功了。

## 2.3 开始TLS加密
如果之前连接到SMTP服务器的 `587` 端口（即使用TLS加密），现在就要调用 `starttls()` 方法，**这是为连接实现加密的必要步骤**，能让SMTP连接处于TLS模式，调用的返回值是 `220` 就表示服务器已充分准备。如果已连接到 `465` 端口（使用SSL），加密就已经设置完成了，应该跳过这一步。
```python
>>> gmail = smtplib.SMTP('smtp.gmail.com', 587)
>>> gmail.ehlo()
(250, b'smtp.gmail.com at your service, [47.57.140.157]\nSIZE 35882577\n8BITMIME\nSTARTTLS\nENHANCEDSTATUSCODES\nPIPELINING\nCHUNKING\nSMTPUTF8')
>>> gmail.starttls()
(220, b'2.0.0 Ready to start TLS')

```

## 2.4 登录到SMTP服务器
建立了到SMTP服务器的加密连接后，调用 `login()` 方法，传入用户名（即你的电子邮件地址）字符串作为第一个参数、传入密码字符串作为第二个参数。返回值 `235` 表示认证成功。如果密码错误，Python会抛出 `smtplib. SMTPAuthenticationError` 异常。**注意不要将密码硬编码在源代码中**！

一开始登录Gmail时，(⊙o⊙)…，解决办法是右图**开启安全性较低的应用的访问权限**，更安全的方法是**开启两步验证、设置应用程序专用密码**：
<img src="https://img-blog.csdnimg.cn/046e99b2e6b64e25bb2dca0793bfefd6.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16" width="50%"><img src="https://img-blog.csdnimg.cn/364a533e65dd4d76b36d85a07dd9debb.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16" width="50%">

另外在登录QQ时注意，Python作为第三方客户端，应当使用“授权码”替代密码来登陆，类似于Gmail的专用密码。然后就成功了：
```haskell
>>> gmail.login('my_email_address@gmail.com', 'MY_SECRET_PASSWORD')
(235, b'2.7.0 Accepted')
>>> qq.login('my_email_address@qq.com', 'MY_Authorization_Code') # 不用输入授权码中间的空格
(235, b'Authentication successful')
```
## 2.5 发送电子邮件
登录到EMail服务提供商的SMTP服务器后，可以调用 `sendmail()` 方法来发送电子邮件。`sendmail()` 需要三个参数：
- 你的电子邮件地址字符串，即电子邮件的 `from` 地址；
- 收件人的电子邮件地址字符串或多个收件人的地址字符串列表，作为 `to` 地址；
- 电子邮件正文字符串，**必须以 `'Subject: \n'` 开头**作为电子邮件的主题行，`'\n'` 换行符将主题行与电子邮件正文分隔开来。

`sendmail()` 的返回值是一个字典，对于电子邮件传送失败的每个收件人，字典中将有一个键值对。空字典表示**对所有收件人均成功发送电子邮件**。
```python
>>> smtpObj.sendmail('my_email_address@gmail.com', 'recipient@example.com', 'Subject: So long.\nDear Alice, so long and thanks for all the fish. Sincerely, Bob') 
{}
```
## 2.6 从SMTP服务器断开
确保完成发送电子邮件后，调用 `quit()` 方法，将程序从SMTP服务器断开。返回值 `221` 表示会话结束：
```python
>>> smtpObj.quit() 
(221, b'2.0.0 closing connection ko10sm23097611pbd.52 - gsmtp')
```

---
# 3. 用IMAP获取和删除电子邮件
在Python中查找和获取电子邮件也有多个步骤，这里有一个完整的例子，包括登录IMAP服务器、搜索电子邮件、获取邮件、提取电子邮件的文本。

```python
>>> import imapclient 
>>> imapObj = imapclient.IMAPClient('imap.gmail.com', ssl=True) 
>>> imapObj.login('my_email_address@gmail.com', 'MY_SECRET_PASSWORD') 
'my_email_address@gmail.com Jane Doe authenticated (Success)' 
>>> imapObj.select_folder('INBOX', readonly=True) 
>>> UIDs = imapObj.search(['SINCE 05-Jul-2014']) 
>>> UIDs
[40032, 40033, 40034, 40035, 40036, 40037, 40038, 40039, 40040, 40041] 
>>> rawMessages = imapObj.fetch([40041], ['BODY[]', 'FLAGS']) 
>>> import pyzmail 
>>> message = pyzmail.PyzMessage.factory(rawMessages[40041]['BODY[]']) 
>>> message.get_subject() 
'Hello!' 
>>> message.get_addresses('from') 
[('Edward Snowden', 'esnowden@nsa.gov')] 
>>> message.get_addresses('to') 
[(Jane Doe', 'jdoe@example.com')] 
>>> message.get_addresses('cc') []
>>> message.get_addresses('bcc') []
>>> message.text_part != None True 
>>> message.text_part.get_payload().decode(message.text_part.charset) 
'Follow the money.\r\n\r\n-Ed\r\n' 
>>> message.html_part != None True 
>>> message.html_part.get_payload().decode(message.html_part.charset) 
'<div dir="ltr"><div>So long, and thanks for all the fish!<br><br></div>Al<br></div>\r\n' 
>>> imapObj.logout()
```

## 3.1 连接到IMAP服务器
**首先需要一个IMAPClient对象，连接到IMAP服务器并接收电子邮件**。同样的，也需要Email服务提供商的IMAP服务器域名，这和SMTP服务器的域名不同。下面列出本人常用的IMAP服务器域名及其提供商：
| 提供商 | SMTP服务器域名
|:---|:---
| Gmail | `imap.gmail.com` 
| QQ | `imap.qq.com`  
| Outlook.com/Hotmail.com  | `imap-mail.outlook.com` 

现在调用 `imapclient().IMAPClient()` 函数，创建一个IMAPClient对象，作为连接到IMAP服务器的客户端对象。由于多数Email服务提供商要求SSL加密，要传入 `SSL = True` 关键字参数：

```haskell
>>> import imapclient
>>> gmail = imapclient.IMAPClient('imap.gmail.com', ssl=True)
>>> qq = imapclient.IMAPClient('imap.qq.com', ssl=True)
```

注意，QQ邮箱的IMAP服务目前仍存在功能限制，暂时还不支持删除文件夹和重命名文件夹的操作。其他关于IMAP的已知问题有，不能在命名文件夹时使用特殊符号（如全角的空格），否则会导致在部分手机上接收邮件时出现“检查信息失败”的情况。

## 3.2 登录到IMAP服务器
有了IMAPClient对象后，调用 `login()` 方法，传入用户名（即电子邮件地址）和密码字符串（或者**应用专用密码**或**授权码**）。同样记得，不要硬编码账户名和密码，使用 `input()` 输入、配置文件或数据库：

```haskell
>>> imapObj.login('my_email_address@gmail.com', 'MY_SECRET_PASSWORD')
'my_email_address@gmail.com Jane Doe authenticated (Success)'
```

## 3.3 搜索电子邮件
登录后，实际获取感兴趣的电子邮件分为两步，首先选择要搜索的文件夹，然后调用IMAPClient对象的 `search()` 方法，传入IMAP搜索关键字字符串。
## 3.3.1 选择文件夹

## 3.3.2 执行搜索
## 3.4 大小限制
## 3.5 取邮件并标记为已读
## 3.6 从原始消息中获取电子邮件地址
## 3.7 从原始消息中获取正文
## 3.8 删除电子邮件
## 3.9 从IMAP服务器断开

---
# 4. 练手：向会员发送会费提醒
## 4.1 打开Excel文件
## 4.2 查找所有未付成员
## 4.3 发送定制的电子邮件提醒

---
# 4. 用Twilio发送短信
## 4.1 注册Twilio账号
## 4.2 发送短信

---
## 5. 练手：只给我发短信模块

---
小结

习题

# 8. 实践项目
## 8.1 随机分配家务活的电子邮件程序
## 8.2 伞提醒程序

## 8.3 自动退订

## 8.4 通过电子邮件控制你的电脑
