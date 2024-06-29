当你用 Wireshark 解决了一个又一个难题时，再谦虚的人也会自信心膨胀，以为没有什么问题是解决不了的。可惜这只是错觉，因为 Wireshark 的确有它的应用极限。

我是什么时候意识到这一点的呢？大概两年前我碰到过这样一个问题：接收方不时回复“TCP Window=0”给发送方，导致发送方只能停下来等待。整个传输过程的 Sequence Number 曲线类似于图 1 所示，其中水平部分表明接收方当时正在发“TCP Window=0”。
图 1
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307131730089.png)
为了给客户出一份专业的分析报告，我需要统计出“TCP Window=0”所导致的停滞总共有多少毫秒。通过图 1 的横坐标来统计显然不够精确，所以我不得不把所有的问题包过滤出来，逐段统计停滞的时间。像图 1 这样只有两段停滞时间的情况还好，碰到有几十段的时候就很费时了。

为什么我要人工地去做如此简单的重复劳动呢？这明显更适合由程序来完成，但是 Wireshark 没有提供这项功能。几天后我随口向 Patrick 提起了这个问题，没想到他立即分享给我一段脚本。我只需要运行以下命令，该脚本就可以把总停
滞时间计算出来了。
```java
$ tshark -n -r <tcpdump_name> -z
'proto,colinfo,frame.time_relative,frame.time_relative' -z
'proto,colinfo,tcp.ack && (tcp.srcport == <source_port> && tcp.dstport ==<destination_port>),tcp.ack' -z 'proto,colinfo,tcp.window_size && (tcp.srcport == <source_port> && tcp.dstport == <destination_port>),tcp.window_size'|awk -f <script>
```
`<script>` 指的就是 Patrick 分享的脚本。由于篇幅所限，我就不把脚本内容贴
出来了，这也不是本文的重点。

真正要关注的是上面用到的 tshark 命令，它相当于 Wireshark 的命令行版本。**和图形界面相比，命令行有一些先天的优势**。
- 如上例所示，命令行的输出可以通过 awk 之类的方式直接处理，这是图形界面无法实现的。有一些高手之所以说 tshark 的功能比 Wireshark 强大，也大多出于这个原因。
- 编辑命令虽然费时，但是编辑好之后可以反复使用，甚至可以写成一个软件。比如我经常需要进行性能调优，那就可以写一段程序来完成本书多次提 到 过 的 三 板 斧 （ Summary, Service Response Time 和 Expert Info Composite）。拿到一个性能相关的包之后，直接运行该程序就可以得到三板斧结果，这比起用 Wireshark 快多了。
- tshark 输出的分析文本大多可以直接写入分析报告中，而 Wireshark 生成不了这样的报告。比如说，我想统计每一秒钟里 CIFS 操作的 Service Response Time, 那只要执行以下命令就可以了，如下例所示。
    ```java
    tshark -n –q -r tcpdump.cap -z "io,stat,1.00,AVG(smb.time)smb.time"
    =================================
    IO Statistics
    Interval: 1.000 secs
    Column #0: AVG(smb.time)smb.time
    | Column #0
    Time | AVG
    000.000-001.000 0.008
    001.000-002.000 0.007
    002.000-003.000 0.007
    003.000-004.000 0.007
    004.000-005.000 0.014
    005.000-006.000 0.001
    006.000-007.000 0.003
    007.000-008.000 0.005
    008.000-009.000 0.001
    009.000-010.000 0.001
    010.000-011.000 0.000
    011.000-012.000 0.000
    012.000-013.000 0.001
    =================================
    ```
    这个结果导入 Excel, 又可以生成各种报表。
- 和其他软件一样，命令行往往比图形界面快得多。比如现在有一个很大的包需要用 IP 192.168.1.134 过滤，用 Wireshark 操作的话先得打开包，再用 `ip.addr==192.168.1.134` 过滤，最后保存结果。这三个步骤都很费时，但是tshark 用下面一条命令就可以完成了。`tshark -r tcpdump.log -R "ip.addr==192.168.1.134 " -w tcpdump.log.filtered`

因为上述这些优势，一位工程师可能上手 tshark 之后很快就会舍弃 Wireshark。是的，就是本书所极力推荐的 Wireshark。==学无止境，当你掌握了 足够多的经验时，就完全可以忽略 Wireshark 的友好界面，转而追求更高效，也更复杂的 tshark==。

tshark 的入门并不难。在安装好 tshark 的操作系统上（安装 Wireshark 时也默认安装 tshark），执行“tshark -h”就可以阅读使用说明了。有 Wireshark 经验的读者应该不需要我来解析这些说明。本文要分享的，是一些从使用说明上学不到的技巧。
## 1. 如何在 Windows 命令行中搜索 tshark 的输出？
我建议安装含有 qgrep 的 Windows Resource Kit，然后就可以用 qgrep 来搜索
了。如图 2 所示，我希望搜索 mount.pcap 中含有“code”字符串的一个包，就可以用 qgrep 找出来。
图 2
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307131733735.png)
## 2. 本书介绍过的性能问题三板斧如何通过命令实现？
a．Summary 可以通过 capinfos 命令查询，如图 3 所示。
图 3
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307131733674.png)
注意：安装 Wireshark 的时候，默认会附带 capinfos 和 Editcap 等工具，除非
你手动勾掉它们。
b. 获取 Service Response Time 则要视不同协议而定，比如 NFS 协议可以用图 4 中的命令。
图 4
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307131733360.png)

CIFS 协议只要把图 4 中双引号中的内容改为“smb,rtt,”即可（见图 5）。
图 5
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307131734111.png)

c．重传状况要用到 tcp.analysis.retransmission 命令，注意图 6 中这 384 个 frames 包括了超时重传和快速重传两种情况。
图 6
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307131734621.png)

d．乱序状况则只要把“retransmission”改成”out_of_order”（见图 7）。
图 7
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307131734297.png)

## 3．如何统计一个包里的所有对话？
“conv，xxx”就可以做到，其中 xxx 可以是 tcp、udp、eth 或者 ip（见图 8）。
图 8
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307131734088.png)
## 4．如果一个包大得连 tshark 都无法打开，有没有办法切分成多个？
有办法，可以使用 editcap 命令来做到。我常用 `editcap <input file> <output file> -i <seconds per file>` 或者 `editcap <input file> <output file> -c <packets per file>`  两种方式。图 9 所示的例子以每 8 秒为间隔切分了这个包。
图 9
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307131735863.png)

除了这里介绍的这些，tshark 下的网络分析技巧还有很多。利用管道（Pipeline）还可以结合 awk、sed 等命令实现更为强大的功能，值得每位工程师长期学习。如果学习过程中遇到任何问题，建议查询 Wireshark 的官方说明，地址为 http://www.wireshark.org/docs/man-pages/tshark.html 。就算我这样的老用户还经常能从中学到新知识呢。

## 使用 Tshark 和 Dumpcap 进行数据包的捕获
Tshark.exe 是 Wireshark 的一个组件，可以**用来捕获数据包**，也可以用来**查看之前保存的数据包捕获文件**。Tshark.exe 也提供了**对数据包的解析和保存功能**。虽然没有图形化的工
作界面，但 Tshark.exe 的功能却十分强大。如果你希望查看 Tshark.exe 的全部功能，可以在命令行中输入“tshark –h”就可以查看帮助文件，这个帮助文件很大，图2 只显示了其中与网络接口（网卡）有关的部分。
图2 Tshark 查看帮助文件
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307171626968.png)
首先看一个使用 Tshark.exe 捕获数据包的简单示例，这里**至少需要指定捕获数据包所使用的网卡**，在 Linux 下很容易查看到网卡的名称和编号。但查看 Windows 下网卡的编号则要困难很多，不过在 Tshark 中，可以使用如下的命令查看每个网卡的编号：
```
tshark -D
```
接下来我们使用第 4 块网卡来捕获数据，为了加快捕获的速度，这里**使用-s 参数来表示只捕获数据包的前 512 个字节数据**：
```
tshark -s 512 -i 4
```
和 Wireshark 一样，==Tshark 还支持捕获过滤器和显示过滤器的使用，这两种过滤器的语法也和 Wireshark 中规定的一样==，例如下面就使用了目标端口为 80 的过滤器：
```
tshark -s 512 -i 4 -f 'tcp dst port 80'
```
捕获到的数据包如图 3 所示。
图 3 tshark 捕获到的数据包
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307171636045.png)
需要停止捕获数据包时，可以使用“Ctrl+C”组合键。

Tshark 中还提供了**强大的统计功能，这个功能通过参数-z 来实现**，这个参数后面需要使用 Tshark 所指定的值，可以使用如下命令：
```
tshark -z -h
```
Tshark 所有可以使用的值如图 4 所示。
图 4 tshark 的统计功能
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307171638308.png)
这里面我们**选择使用“io,phs”作为-z 参数的值**，添加了-q 来指定**不显示捕获的数据包信息**：
```
tshark -i 4 -f "port 80" -q -z io,phs
```
执行该命令的结果如图 5 所示。
图 5 tshark 的统计结果
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307171636969.png)
# Dumpcap 的用法
Dumpcap 也是 Wireshark 中自带的一个命令行工具，这种工具的优势就在于对资源的消耗较小。可以使用dumpcap.exe -h 来查看它的帮助文件。
图 6 Dumpcap 的帮助文件
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307171643942.png)
这里首先来介绍几个最为常用的选项。
- `-D` ：列出当前可以的网卡设备。
- `-i <>` ：指定要使用的网卡名字或者序号。
- `-f <capture filter>` ：使用 BPF 语法完成的过滤器表达式。
- `-b filesize` ：指定文件的大小。
- `-w <outfile>` ：指定用来保存文件的名称。

这个工具的使用与 Tshark 很相似，所以本书不再详细介绍。如果希望深入地了解dumpcap 的功能，可以访问 https://www.wireshark.org/docs/man-pages/dumpcap.html 