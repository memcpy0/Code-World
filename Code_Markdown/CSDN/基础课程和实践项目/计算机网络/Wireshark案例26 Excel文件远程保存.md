当我们在 Notepad 等文本编辑器上单击 File-->Save 的时候，底层的操作非常
简单—编辑器上的内容被直接写入文件了（见图 1）。假如这个文件是被保存到
了网络盘上，我们就可以从 Wireshark 抓包上看到这个过程（见图 2）。
图 1
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307122304249.png)

图 2
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307122305216.png)

包号 58：
客户端：“我要写 6 个字节到/Temp/wireshark.txt 中”。
包号 59：
服务器：“写好了。”

相比之下，微软 Office 保存文件的过程就没有这么简单了，所以微软的老用户
都或多或少经历过保存文件时发生的问题。比如图 3 中的 Excel 提示信息就很常见，它说明该文件被占用，暂时保存不了。这样的问题在 Notepad 上是不会发生的。
图 3
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307122305463.png)

那么，Excel 究竟是如何保存文件的呢？虽然我的手头没有微软的文档，但只要把
文件保存到网络盘上，就可以借助 Wireshark 看到整个过程了。我在实验室中编辑了 Excel 文件“wireshark.xlsx”，然后在保存时抓了个包，我们一起来分析其中比较关键的几个步骤（见图 4）：
图 4
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307122305023.png)
这几个包可以解析为下述过程。
24 号包：
客户端：“/Temp 目录中存在一个叫 DCD652B.tmp 的文件吗？”
25 号包：
服务器：“不存在。”
26 号包：
客户端：“那我要创建一个叫 DCD652B.tmp 的文件。”
27 号包：
服务器：“建好了。”
38 号包：
客户端：“把 Excel 里的内容写到 DCD652B.tmp 里。”
42 号包：
服务器：“写好了。”
异步社区会员 wanlebing(15520495153) 专享 尊重版权
 
从以上过程可见，**Excel 并没有直接把文件内容存到 wireshark.xlsx 上，而是
存到一个叫 DCD652B.tmp 的临时文件上了**。接下来再看（见图 5）。
图 5
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307122306584.png)
47 号包：
客户端：“/Temp 目录里存在一个叫 6AF04530.tmp 的文件吗？”
48 号包：
服务器：“不存在。”
97 号包：
客户端：“那好，把原来的 wireshark.xlsx 重命名成 6AF04530.tmp。”
98 号包：
服务器：“重命名完毕。”
103 号包：
客户端：“再把一开始那个临时文件 DCD652B.tmp 重命名成 wiresahrk.xlsx。”
104 号包：
服务器：“重命名完毕。”
从以上过程可知，**原来的 wireshark.xlsx 被重命名成一个临时文件，叫
6AF04530.tmp 。而 之 前创建的 那个临时 文件 DCD652B.tmp 又 被重命名 成
wireshark.xlsx**。经过以上步骤之后，我们拥有一个包含新内容的 wireshark.xlsx，
 
还有一个临时文件 6AF04530.tmp（也就是原来那个 wireshark.xlsx）。接着往下看，就发现 6AF04530.tmp 被删除了（见图 6）。
图 6
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307122308638.png)

微软把保存过程设计得如此复杂，自然是有很多好处的。不过复杂的设计往
往伴随着更多出问题的概率，因为其中一步出错就意味着保存失败。比如上文提
到的报错信息“…is currently in use. Try again later”，大多数时候的确是文件被占
用才触发的，但也有时候是 Excel bug 或者杀毒软件导致的。无论出于何种原因，
我们只有理解了保存时发生的细节，才可能探究到真相。

==Wireshark 正是获得这些细节的通用法宝，任何经过网络所完成的操作，我们
都可以从 Wireshark 中看到==。有了这样的利器，还有多少问题能难住你？