# 一、抓包
==拿到一个网络包时，我们总是希望它尽可能小。因为操作一个大包相当费时，
有时甚至会死机==。如果让初学者分析 1GB 以上的包，估计会被打击得信心全无。所以抓包时应该尽量只抓必要的部分。有很多方法可以实现这一点。
1．**只抓包头**。一般能抓到的每个包（称为“帧”更准确，但是出于表达习惯，本书可能会经常用“包”代替“帧”和“分段”）的最大长度为 1514 字节，启用了 Jumbo Frame（巨型帧）之后可达 9000 字节以上，而大多数时候我们只需要 IP 头或者 TCP 头就足够分析了。**在 Wireshark 上可以这样抓到包头：单击菜单栏上的 Capture-->Options，然后在弹出的窗口上定义“Limit each packet to”的值。我一般设个偏大的数字：80 字节**，也就是说每个包只抓前 80 字节。这样 TCP 层、网络层和数据链路层的信息都可以包括在内（见图 1）。
 
图 1
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307122319098.png)
**如果问题涉及应用层，就应该再加上应用层协议头的长度**。如果你像我一样经常忘记不同协议头的长度，可以输入一个大点的值。即便设成 200 字节，也比 1514 字节小多了。

以上是使用 Wireshark 抓包时的建议。用 tcpdump 命令抓包时可以用“-s”参
数达到相同效果。比如以下命令只抓 eth0 上每个包的前 80 字节，并把结果存到
/tmp/tcpdump.cap 文件中。
```java
[root@server_1 /]# tcpdump -i eth0 -s 80 -w /tmp/tcpdump.cap
```
2．**只抓必要的包**。服务器上的网络连接可能非常多，而我们只需要其中的一
小部分。Wireshark 的 Capture Filter 可以在抓包时过滤掉不需要的包。比如在成百上千的网络连接中，我们只对 IP 为 10.32.200.131 的包感兴趣，那就可以在
Wireshark 上这样设置：单击菜单栏上的 Capture-->Options，然后在 Capture Filter输入“host 10.32.200.131”（见图 2）。
图 2
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307122321763.png)

如果对更多 filter 表达式感兴趣，请参考 http://wiki.wireshark.org/CaptureFilters。
用 tcpdump 命令抓包时，也可以用“host”参数达到相同效果。比如以下命
令只抓与 10.32.200.131 通信的包，并把结果存到/tmp/tcpdump.cap 文件中。
```java
[root@server_1 /]# tcpdump-i eth0 host 10.32.200.131-w /tmp/tcpdump.cap
```
注意：设置 Capture Filter 之前务必三思，以免把有用的包也过滤掉，尤其
是容易被忽略的广播包。当然有时候再怎么考虑也会失算，比如我有一次把对方
的 IP 地址设为 filter，结果一个包都没抓到。最后只能去掉 filter 再抓，才发
现是 NAT（网络地址转换）设备把对方的 IP 地址改掉了。

**抓的包除了要小，最好还能为每步操作打上标记**。这样的包一目了然，赏心
悦目。比如要在 Windows 上抓一个包含三步操作的问题，我会这样抓。
（1）`ping <IP> -n 1 -l 1`
（2）操作步骤 1
（3）`ping <IP> -n 1 -l 2`
（4）操作步骤 2
（5）`ping <IP> -n 1 -l 3`
（6）操作步骤 3
如图 3 所示， ==如果我需要分析步骤 1，则只要看 146～183 之间的包即可==。注意到 146 号包最底下的“Data（1 byte）”了吗？byte 的数目表示是第几步，这样就算在步骤很多的情况下也不会混乱。

抓包的技巧还有很多，比如**可以写一个脚本来循环抓包**，等侦察到某事件时
自动停止。**一位工程师即便不懂网络分析，但如果能抓得一手好包，也是一项很
了不起的技能了**。
图 3
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307122323837.png)

# 二、个性化设置
Wireshark 的默认设置堪称友好，但不同用户的从事领域和使用习惯各有不
同，所以有时需要根据自己的情况对配置略作修改。
1．我经常需要参照服务器上的日志时间，找到发生问题时的网络包。所以就
把 Wireshark 的时间调成跟服务器一样的格式。单击 Wireshark 的 View-->Time
Display Format-->Date and Time of Day，就可以实现此设置（见图 4）。
图 4
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307122324311.png)

2．不同类型的网络包可以自定义颜色，比如网络管理员可能会把 **OSPF 等协
议或者与 Spanning Tree Protocol（生成树协议）相关的网络包**设成最显眼的颜色。而文件服务器的管理员则更关心 **FTP、SMB 和 NFS 协议的颜色**。我们可以通过View -->Coloring Rules 来设置颜色。如果同事已经有一套非常适合你工作内容的配色方案，可以请他从 Coloring Rules 窗口导出，然后导入到你的 Wireshark 里（见图 5）。记得下次和他吃饭时主动买单，要知道配一套养眼的颜色可要花不少时间。
图 5
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307122324644.png)

3．更多的设置可以在 Edit-->Preferences 窗口中完成。**这个窗口的设置精度可
以达到一些协议的细节**。比如在此窗口单击 Protocols-->TCP 就可以看到多个 TCP相关选项，将鼠标停在每一项上都会有详细介绍。**假如经常要对 Sequence Number 做加减运算，不妨选中 Relative sequence numbers**（见图 6），这样会使 Sequence number 看上去比实际小很多。
图 6
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307122325504.png)
4．如果你在其他时区的服务器上抓包，然后下载到自己的电脑上分析，最好
把自己电脑的时区设成跟抓包的服务器一样。这样，Wireshark 显示的时间才能匹
配服务器上日志的时间。比如说，服务器的日志显示 2/13/2014 13:01:32 有一个错
误信息。那我们要在自己电脑上调整时区之后，才能到 Wireshark 上检查 2/13/2014 13:01:32 左右的包，否则就得先换算时间。
# 三、过滤
很多时候，**解决问题的过程就是层层过滤，直至找到关键包**。前面已经介绍
过抓包时的 Capture Filter 功能了。其实在包抓下来之后，还可以进一步过滤，而
且**这一层的过滤功能更加强大**。图 7 表示一个“IP 为 10.32.106.50，且 TCP 端口为 445”的过滤表达式。
图 7
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307122327686.png)
要说过滤的作用与技巧，就算专门写一本小册子都不为过。
1．如果已知某个协议发生问题，**可以用协议名称过滤一下**。以 Windows
Domain 的身份验证问题为例，如果已知该域的验证协议是 Kerberos，那么就在
Filter 框输入 Kerberos 作为关键字过滤。除了纯粹的 Kerberos 包，你还将得到
Session Setup 之类包含 Kerberos 的包（见图 8）。
图 8
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307122327352.png)
用协议过滤时务必考虑到协议间的依赖性。比如 NFS 共享挂载失败，问题可
能发生在挂载时所用的 mount 协议，也可能发生在 mount 之前的 portmap 协议。这种情况下就**需要用“portmap || mount”来过滤了**（见图 9）。如果不懂协议间的依赖关系怎么办？我也没有好办法，只能暂时放弃这个技巧，等熟悉了该协议后再用。
图 9
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307122328533.png)
2．**IP 地址加 port 号是最常用的过滤方式**。除了手工输入 ip.addr eq <IP 地址>
&&tcp.port eq <端口号>之类的过滤表达式，Wireshark 还提供了更快捷的方式：右键单击感兴趣的包，**选择 Follow TCP/UDP Stream（选择 TCP 还是 UDP 要视传输层协议而定）就可以自动过滤**（见图 10）。而且该 Stream 的对话内容会在新弹出的窗口中显示出来。
图 10
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307122329726.png)
经常有人在论坛上问，Wireshark 是按照什么过滤出一个 TCP/UDP Stream
的？答案就是：两端的 IP 加 port。单击 Wireshark 的 Statistics-->Conversations， 再单击 TCP 或者 UDP 标签就可以**看到所有的 Stream**（见图 11）。
图 11
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307122329965.png)
3．用鼠标帮助过滤。我们有时因为 Wireshark 而苦恼，并不是因为它功能不够，
而是强大到难以驾驭。比如在过滤时，有成千上万的条件可供选择，但怎么写才是
合乎语法的？虽然 http://www.wireshark.org/docs/dfref/ 提供了参考，但经常查找毕竟太费时费力了。

**Wireshark 考虑到了这个需求，右键单击 Wireshark 上感兴趣的内容，然后选择 Prepare a Filter-->Selected**，就会在 Filter 框中自动生成过滤表达式。在有复杂需求的时候，还可以选择 And、Or 等选项来生成一个组合的过滤表达式。

**假如右键单击之后选择的不是 Prepare a Filter，而是 Apply as Filter-->Selected，则该过滤表达式生成之后还会自动执行**。图 12 显示了在一个 SMB 包的 SMB Command：Read AndX 上右键单击，并选择 Selected 之后，所有的 Read 包都会被过滤出来。
图 12
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307122330318.png)
4．我们可以把过滤后得到的网络包存在一个新的文件里，因为小文件更方便操作。单击 Wireshark 的 File-->Save As，选中 Displayed 单选按钮再保存，得到的新文件就是过滤后的部分（见图 13）。
图 13
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307122331903.png)
有时候你会发现，保存后的文件再打开时会显示很多错误。这是因为过滤后
得到的不再是一个完整的 TCP Stream，就像抓包时漏抓了很多一样。所以选择
Displayed 选项时要慎重考虑。
注意：有些 Wireshark 版本把这个功能移到了菜单 File-->Export Specified
Packets…选项中，如图 14 所示。
图 14
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307122331066.png)
总体来说，**过滤是 Wireshark 中最有趣，最难，也是最有价值之处，值得我
们用心学习**。

# 四、让 Wireshark 自动分析
有些类型的问题，我们根本不需要研究包里的细节，直接交给 Wireshark 分
析就行了。
1．单击 Wireshark 的 Analyze-->Expert Info Composite，就可以在不同标签下看到不同级别的提示信息。比如**重传的统计**、连接的建立和**重置统计**，等等。在分析网络性能和连接问题时，**我们经常需要借助这个功能**。图 15 是 TCP 包的重传统计。
图 15
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307122332397.png)

2．单击 Statistics-->Service Response Time，再选定协议名称，可以得到响应
时间的统计表。我们在衡量服务器性能时经常需要此统计结果。图 16 展示的是
SMB2 读写操作的响应时间。
图 16
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307122332014.png)
3．单击 Statistics-->TCP Stream Graph，可以生成几类统计图。比如我曾经用
Time-Sequence Graph (Stevens)生成了图 17。
图 17
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307122333634.png)
从图 17 中可以看出 25～40 秒，以及 65～75 秒之间没有传输数据。进一
步研究，发现发送方内存不足，所以偶尔出现暂停现象，添加内存后问题就
解决了。

为什么 Wireshark 要把这个图称为“Stevens”呢？我猜是为了==向《TCP/IP
Illustrated》的作者 Richard Stevens 致敬。这也是我非常喜欢的一套书，在此推荐
给所有读者==。

4．单击 Statistics-->Summary，可以看到一些统计信息，比如平均流量等，这
有助于我们推测负载状况。比如图 18 中的网络包才 1.594Mbit/s，说明流量低
得很。
图 18
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307122334520.png)

五、最容易上手的搜索功能
与很多软件一样，Wireshark 也可以通过“Ctrl+F”搜索关键字。假如我们怀疑包
里含有“error”一词，就可以按下“Ctrl+F”之后选中“String”单选按钮，然后在 Filter
中输入“error”进行搜索（见图 19）。很多应用层的错误都可以靠这个方法锁定问题包。
图 19
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307122334293.png)

一篇文章不可能涵盖所有技巧，本文就到此为止。最后要分享的，是我认为
最“笨”但也是最重要的一个技巧——勤加练习。只要练到这些技巧都变成习惯，
就可以算登堂入室了。