20 世纪 80 年代初，一家神奇的公司在硅谷诞生了，它就是 Sun Microsystems。
这个名字与太阳无关，而是源自互联网的伊甸园—Stanford University Network的首字母。在不到 30 年的时间里，SUN 公司创造了无数传世作品。其中，Java、Solaris 和基于 SPARC 的服务器至今还闻名遐迩。后来，人们总结 SUN 公司衰落的原因时，有一条竟然是技术过剩。

Network File System（NFS）协议也是 SUN 公司设计的。顾名思义，**NFS 就
是网络上的文件系统**。它的应用场景如图 1 所示，**NFS 服务器提供了/code 和
/document 两个共享目录，分别被挂载到多台客户端的本地目录上**。当用户在这些本地目录读写文件时，**实际是不知不觉地在 NFS 服务器上读写**。
图 1
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307122344013.png)

NFS 自 1984 年面世以来，已经流行 30 年。理论上它适用于任何操作系统，
不过因为种种原因，**一般只在 Linux/UNIX 环境中存在**。我在很多数据中心见到过 NFS 应用，其中不乏通信、银行和电视台等大型机构。无论 SUN 的命运如何多舛，NFS 始终处乱不惊，这么多年来只出过 3 个版本，即 1984 年的 NFSv2、
1995 年的 NFSv3 和 2000 年的 NFSv4。目前，大多数 NFS 环境都还是 NFSv3，本文介绍的也是这个版本。NFSv2 还在极少数环境中运行（我只在日本见到过），可以想象这些环境有多老了。而 **NFSv4 因为深受 CIFS 影响，实施过程相对复杂，所以普及速度较慢**。

如何深入学习 NFS 协议呢？其实所有权威资料都可以在 RFC 1813 中找到，
不过这些文档读起来就像面对一张冷冰冰的面孔，令人望而却步。《鸟哥的 Linux
私房菜》中对 NFS 的介绍虽称得上友好，但美中不足的是不够深入，出了问题也
不知道如何排查。我曾经为此颇感苦恼，因为工作中碰到的 NFS 问题太多了，走
投无路时就只能硬啃 RFC—既然网络协议都那么复杂，我也不指望有捷径了。

直到有一天偶然打开挂载时抓的包，才意识到 Wireshark 可以改变这一切：**它使
整个挂载过程一目了然，所有细节都一览无遗**。分析完每个网络包，再回顾 RFC
1813 便完全不觉得陌生。

如果你对 NFS 有兴趣，不妨一起来分析这个网络包。在我的实验室中，NFS
客户端和文件服务器的 IP 分别是 10.32.106.159 和 10.32.106.62。我在运行挂载命
令（mount）时抓了包，然后用 “portmap || mount || nfs”进行过滤（见图 2）。
```java
[root@shifm1 tmp]# mount 10.32.106.62:/code /tmp/code
```
图 2
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307122346927.png)
从图 2 中的 Info 一栏可以看到，Wireshark 已经提供了详细的解析。不过我们还
可以翻译成更直白的对话（为了方便第一次接触 NFS 的读者，我还作了一些注释）。
包号 112 和 113（见图 3）：
图 3
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307122351577.png)
客户端：“我想连接你的 NFS 进程，应该用哪个端口呀？”
服务器：“我的NFS端口是 2049。”①
> ① 在这一步，**客户端找到服务器的 portmap 进程，向它查询 NFS 进程的端口号**。然后服务器的 portmap 进程回复了 2049。**portmap 的功能是维护一张进程与端口号的对应关系表，而它自己的端口号 111 是众所周知的，其他进程都能找到它**。这个角色类似很多公司的前台，她知道每个员工的分机号。当我们需要联系公司里的某个人（比如 NFS）时，可以先拨前台(111)，查询到其分机号(2049)，然后就可以拨这个分机号了。==其实大多数文件服务器都会使用 2049 作为 NFS 端口号，所以即便不先咨询 portmap，直接连 2049 端口也不会出问题==。

包号 123 和 124（见图 4）：
图 4
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307122351731.png)
客户端：“那我试一下 NFS 进程能否连上。”
服务器：“收到了，能连上。”②
> ② **客户端尝试连接服务器的 NFS 进程**，由此判断 2049 端口是否被防火墙拦截，还有 NFS 服务是否已经启动。

包号 128 和 129（见图 5）：
图 5
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307122352757.png)
客户端：“我想连接你的 mount 服务，应该用哪个端口呀？”
服务器：“我的mount的端口号是 1234。”①
> ① 客户端**再次联系服务器的 portmap**，询问 mount 进程的端口号。与 NFS 不同的是，**mount的端口号比较随机，所以这步询问是不能跳过的**。

包号 132 和 133（见图 6）：
图 6
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307122354443.png)
客户端：“那我试一下 mount 进程能否连上。”
服务器：“收到了，能连上。”②
> ② 客户端尝试连接服务器的 mount 进程，由此判断 1234 端口是否被防火墙拦截，还有 mount 进程是否已经启动。

包号 134 和 135（见图 7）：
图 7
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307130009267.png)
客户端：“我要挂载/code 共享目录。”
服务器：“你的请求被批准了。以后请用file handle 0x2cc9be18 来访问本目录。"③
> ③ 这一步真正挂载了/code 目录。**挂载成功后，服务器把该目录的 file handle 告诉客户端**（要点开详细信息才能看到 File handle）。

包号 140 和 141（见图 8）：
图 8
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307130009190.png)
客户端：“我试一下 NFS 进程能否连上。”
服务器：“收到了，能连上。”①
> ① 在我看来这一步没有必要，因为之前已经试连过 NFS 了，再测试一次有何意义？我猜是开发人员不小心重复调用了同一函数，但因为没有抓包，所以测试人员也没有发现这个问题。

包号 143 和 144（见图 9）：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307130017900.png)
图 9
客户端：“我想看看这个文件系统的属性。”
服务器：“给，都在这里。”②
>  ② 客户端获得了该文件系统的大小和空间使用率等属性。我们在客户端上执行 df 就能看到这些信息


包号 145 和 146（见图 10）：
图 10
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307130017769.png)
客户端：“我想看看这个文件系统的属性。”
服务器：“给，都在这里。”③
> ③ 这一步又是重复操作，更让我怀疑是开发人员的疏忽。这个例子也说明了 Wireshark 在辅助开发中的作用。

以上便是 NFS 挂载的全过程。**细节之处很多**，所以在没有 Wireshark 的情况
下很难排错，经常不得不盲目地检查每一个环节，比如**先用 rpcinfo 命令获得服务器上的端口列表（见图 11），再用 Telnet 命令逐个试探（见图 12）**。即使这样也只能检查几个关键进程能否连上，排查范围非常有限。
图 11
![Uploading file...nt6vx]()
```java
[root@shifm1 tmp]# telnet 10.32.106.62 2049
[root@shifm1 tmp]# telnet 10.32.106.62 1234
[root@shifm1 tmp]# telnet 10.32.106.62 111
```
图 12

用上 Wireshark 之后就可以很有针对性地排查了。例如，**看到 portmap 请求没
有得到回复**，就可以考虑防火墙对 111 端口的拦截；如果发现 mount 请求被服务
器拒绝了，就应该**检查该共享目录的访问控制**。

既然说到访问控制，我们就来看看 NFS 在安全方面的机制，包括**对客户端的
访问控制**和**对用户的权限控制**。
- **NFS 对客户端的访问控制**是通过 IP 地址实现的。创建共享目录时可以指定哪
些 IP 允许读写，哪些 IP 只允许读，还有哪些 IP 连挂载都不允许。==虽然配置不难，但这方面出的问题往往很“诡异”，没有 Wireshark 是几乎无法排查的==。比如，我碰到过一台客户端的 IP 明明已经加到允许读写的列表里，结果却只能读。这个
问题难住了很多工程师，因为在客户端和服务器上都找不到原因。后来我们在
服务器上抓了个包，才知道**在收到的包里，客户端的 IP 已经被 NAT 设备转换成
别的了**。
- **NFS 的用户权限也经常让人困惑**。比如在我的实验室中，客户端 A 上的用户
admin 在/code 目录里新建一个文件，该文件的 owner 正常显示为 admin。但是在
客户端 B 上查看该文件时，owner 却变成 nasadmin，过程如下所示。

客户端 A（见图 13）：
```java
[admin@shifm1 /tmp]$ cp abc.txt code/abc.txt
[admin@shifm1 /tmp]$ ls -l code/abc.txt
-rw-r--r-- 1 admin adm 491292 Jul 28 2013 code/abc.txt
```
图 13
客户端 B（见图 14）：
```java
[root@shifm2 /tmp]# ls -l code/abc.txt
-rw-r--r-- 1 nasadmin adm 491292 Jul 28 2013 code/abc.txt
```
图 14

这是为什么呢？借助 Wireshark，我们很容易就能看到原因。图 15 显示了用
户 admin 在创建/tmp/code/abc.txt 时的包。
图 15
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307130022820.png)
由图 15 中的 Credentials 信息可知，**用户在创建文件时并没有使用 admin 这个
用户名，而是用了 admin 的 UID 501 来代表自己的身份**（用户名与 UID 的对应关系是由客户端的/etc/passwd 决定的）。也就是说 **NFS 协议是只认 UID 不认用户名**。当 admin 通过客户端 A 创建了一个文件，其 UID 501 就会被写到文件里，成为 owner 信息。

而当客户端 B 上的用户查看该文件属性时，看到的其实也是“UID: 501”。但因为客户端 B 上的/etc/passwd 文件和客户端 A 上的不一样，其 UID 501 对应的用户名叫nasadmin，所以文件的 owner 就显示为 nasadmin 了。同样道理，当客户端 B 上的用户 nasadmin 在共享目录上新建一个文件时，客户端 A 上的用户看到的文件 owner 就会变成 admin。==为了防止这类问题，建议用户名和 UID 的关系在每台客户端上都保持一致==。

弄清楚了 NFS 的安全机制后，我们再来看看读写过程。经验丰富的工程师都
知道，**性能调优是最有技术含量的**。借助 Wireshark，**我们可以看到 NFS 究竟是如何读写文件的，这样才能理解不同 mount 参数的作用，也才能有针对性地进行
性能调优**。图 16 展示了读取文件 abc.txt 的过程。
```java
[root@shifm1 tmp]# cat /tmp/code/abc.txt
```
图 16
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307130026558.png)

包号 2 和 3（见图 17）：
图 17
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307130026720.png)
客户端：“我可以进入 0x2cc9be18（也就是/code 的 file handle）吗？”
服务器：“你的请求被接受了，进来吧。”

包号 5 和 6（见图 18）：
图 18
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307130026059.png)
客户端：“我想看看**这个目录里的文件**及其 file handle。”
服务器：“文件名及file handle的信息在这里。其中abc.txt的file handle是
0x531352e1。”①
> ① 这个 file handle 也需要从包的详细信息里才能看到。就如之前提到过的，==**NFS 操作文件时使用的是 file handle**，所以要先通过文件名找到其 file handle，而不是直接读其文件名==。如果一个目录里文件数量巨大，获取 file handle 可能会比较费时，所以**建议不要在一个目录里存放太多文件**。

包号 8 和 9（见图 19）：
图 19
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307130027434.png)
客户端：“0x531352e1（也就是 abc.txt）的文件属性是什么？“
服务器：“权限、uid、gid, 文件大小等信息都给你。”

包号 11 和 12（见图 20）：
图 20
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307130027655.png)
客户端：“我可以打开 0x531352e1（也就是 abc.txt）吗？”
服务器：“你的请求被允许了。你有**读、写、执行等权限**。”

包号 13、14、152、292（见图 21）：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307130028994.png)
图 21
客户端：
“从 0x531352e1 的偏移量为 0 处（即从 abc.txt 的开头位置）读 131072
字节。”
客户端：“从 0x531352e1 的偏移量为 131072 处（即接着上一个请求读完的位
置）再读 131072 字节。”
服务器：“给你 131072 字节。”
服务器：“再给你 131072 字节。”
（继续读，直到读完整个文件。）

就这样，NFS 完成了文件的读取过程。**从最后几个包可见，Linux 客户端读 NFS
共享文件时是多个 READ Call 连续发出去的**（本例中是连续两个）。这个方式跟
Windows XP 读 CIFS 共享文件有所不同。Windows XP 不会连续发 READ Call，而是先发一个 Call，等收到 Reply 后再发下一个。相比之下，==Linux 这种读方式比 Windows XP 更高效，尤其是在高带宽、高延迟的环境下==。这就像叫外卖一样，如果你今晚想吃鸡翅、汉堡和可乐三样食物，那合理的方式应该是打一通电话把三样都叫齐了。而不是先叫鸡翅，等鸡翅送到了再叫汉堡，等汉堡送到后再叫可乐。除了读文件的方式，每个 READ Call 请求多少数据也会影响性能。这台 Linux 默认每次读 131072 字节，我的实验室里还有默认每次读 32768 字节的客户端。==在高性能环境中，要手动指定一个比较大的值==。比如在我的 Isilon 实验室中，常常要调到 512KB。**这个值可以在 mount 时通过 rsize 参数来定义**，比如“mount -o rsize=524288 10.32.106.62:/code /tmp/code”。

分析完读操作，接下来我们再看看**写文件的过程**。把一个名为 abc.txt 的文件
写到 NFS 共享的过程如下（见图 22）。
```java
[root@shifm1tmp]# cp abc.txt code/abc.txt
```
图 22
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307130031557.png)

包号 1 和 2（见图 23）：
图 23
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307130032487.png)
客户端：“我可以进入 0x2cc9be18（即/code 目录）吗？”
服务器：“你的请求被接受了，进来吧。”

包号 4 和 5（见图 24）：
图 24
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307130032976.png)
客户端：“请问这里有叫 abc.txt 的文件么？”
服务器：“没有。”①
> ① **在创建一个文件之前，要先检查一下是否有同名文件存在**。如果没有才能继续写，**如果有，要询问用户是否覆盖原文件**。

包号 6 和 7（见图 25）：
图 25
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307130033297.png)
客户端：“那我想创建一个叫 abc.txt 的文件。”
服务器：“没问题，这个文件的 file handle 是 0x531352e1。”

包号 64、104、130、190（见图 26）：
图 26
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307130035569.png)
客户端：“从 0x531352e1 的偏移量为 0 处（即 abc.txt 的文件开头）写 131072
字节。”
服务器：“第一个 131072 字节写好了。”
客户端：“从 0x531352e1 的偏移量为 131072 处（即接着上一个写完的位置）
再写 131072 字节。”
客户端：“从 0x531352e1 的偏移量为 262144 处（即接着上一个写完的位置）
再写 131072 字节。”
（继续写，直到写完整个文件。）

包号 306 和 307（见图 27）：
图 27
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307130047861.png)
客户端：“我刚才往 0x531352e1（也就是 abc.txt）写的数据都存盘了吗？”
服务器：“都存好了。”①
> ① 这是 COMMIT 操作。对于 async 方式的 WRITE Call，**服务器收到 Call 之后会在真正存盘前就回复 WRITE Reply**，这样做是为了提高写性能。那么，==客户端怎么知道哪些 WRITE Call 已经真正存盘了呢？COMMIT 操作就是为此而设计的。只有 COMMIT 过的数据才算真正写好==。

包号 308 和 309（见图 28）：
图 28
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307130048773.png)
客户端：“那我看看 0x531352e1（也就是 abc.txt）的文件属性。”
服务器：“文件的权限、uid、gid、文件大小等信息都给你。”
 
**这个例子的写操作也是多个 WRITE Call 连续发出去的**，这是因为我们在挂载
时没有指定任何参数，所以**使用了默认的 async 写方式**。和 async 相对应的是 sync方式。假如 mount 时使用了 sync 参数（见图 29），客户端会先发送一个 WRITE Call，等收到 Reply 后再发下一个 Call，也就是说 WRITE Call 和 WRITE Reply 是交替出现的。除此之外，还有什么办法在包里看出一个写操作是 async 还是 sync 呢？
答案就是**每个 WRITE Call 上的“UNSTABLE”和“FILE_SYNC”标志**，前者表
示 async，后者表示 sync。图 30 显示了用 sync 参数后的网络包。
```java
[root@shifm1 tmp]# mount -o sync 10.32.106.62:/code /tmp/code
[root@shifm1 tmp]# cp abc.txt /tmp/code/abc.txt
```
图 29

图 30
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307130049216.png)
从图 30 中**不仅可以看到 FILE_SYNC标志，还可以看到 WRITE Call 和 WRITE
Reply 是交替出现的**（也就是说没有连续的 Call）。

不难想象，**每个 WRITE Call 写多少数据也是影响写性能的重要因素**，我们可以在 mount 时用 wsize 参数来指定每次应该写多少。不过在有些客户端上启用 sync 参数之后，无论 wsize 定义成多少都会被强制为 4KB，从而导致写性能非常差。那为什么还有人用 sync 方式呢？答案是==有些特殊的应用要求服务器收到 sync 的写请求之后，一定要等到存盘才能回复 WRITE Reply，sync 操作正符合了这个需求。由此我们也可以推出 COMMIT 对于 sync 写操作是没有必要的==。

非常值得一提的是，**经常有人在 mount 时使用 noac 参数，然后发现读写性能
都有问题**。而根据 RFC 的说明，**noac 只是让客户端不缓存文件属性而已**，为什么会影响性能呢？**光看文档也许永远发现不了原因**。抓个包吧，Wireshark 会告诉我们答案。

先看写文件的情况（见图 31）：
```java
[root@shifm1 tmp]# mount -o noac 10.32.106.62:/code /tmp/code
[root@shifm1 tmp]# cp abc.txt /tmp/code/abc.txt
```
图 31

在图 32 中，从 Write Call 里的 FILE_SYNC 可以知道，==虽然在 mount 时并没
有指定 sync 参数，但是 noac 把写操作强制变成 sync 方式了，性能自然也会下降==。
图 32
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307130051961.png)

再看读文件时的情况（见图 33）：
```java
[root@shifm1 tmp]# mount -o noac 10.32.106.62:/code /tmp/code
[root@shifm1 tmp]# cat /tmp/code/abc.txt
```
图 33
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307130051543.png)
 
从图 33 中可以看到，==在读文件过程中，客户端频繁地通过 GETATTR 查询文
件属性，所以读性能也受到了影响，在高延迟的网络中影响尤为明显==。

纵观全文，我们分析了**挂载过程的每个步骤**，理清了 **NFS 的安全机制**，还研
究了**读写过程的各种细节**，几乎把 NFS 协议的方方面面都覆盖了。如果你认真读完本文，可以说对 NFS 的理解已经达到很高的境界，以后碰到类似 noac 这般隐蔽
的问题也难不倒你。

==假如真能遇到棘手的难题，我建议用 Wireshark 分析。一旦用它解决了第一个问题，恭喜你，很快就会中毒上瘾的==。中毒之后会有什么症状呢？你可能碰到什么问题都想抓个包分析，就像小时候刚学会骑车一样，到小区门口打个酱油都要骑车去。