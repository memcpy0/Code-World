安装完 Wireshark 时，会发现系统中还多了一些程序，例如 Tshark 等。这些程序都采用了命令行的工作方式，虽然体积都很小，但功能却十分强大。围绕以下几点进行讲解：
- 使用 Editcap 对数据包进行**修改**；
- 使用 Mergecap 对数据包进行**合并**；
- Capinfos 的使用方法；
- USBPcapCMD 的使用方法。

打开 Wireshark 的安装目录，然后按照类型对文件进行排序，就可以看到除了Wireshark.exe 之外还有如图 1 所示的一些命令行工具。
图 1 Wireshark 安装目录中的一些命令行工具
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307171624083.png)

首先我们先来简单地看一下这些工具的功能。
- Tshark.exe：这个工具可以看作是 Wireshark 的命令行版本，可以用来捕获数据包，也可以读取保存好的数据包捕获文件。
- editcap.exe：主要用来转换捕获数据包捕获文件的格式。
- dumpcap.exe：和 tshark.exe 一样用来捕获数据包，保存为 libpcap 格式文件。
- mergecap .exe：用来将多个数据包捕获文件合并成一个。
- capinfos.exe：用来将显示数据包捕获文件的信息。
- text2pcap.exe：将十六进制转储文件转换为数据包捕获文件。

## 1. Editcap 的使用方法
在之前例子中，曾经提到过使用 Wireshark 在捕获数据包时得到的文件可能会很大，==Editcap 就可以将这种大文件分割成较小的文件==。另外，==Editcap 也可以通过开始时间和停止时间来获取捕获数据包文件的子集==，删除捕获数据包文件中重复数据等。

同样，了解这个工具最好的办法还是查看它的帮助文件，使用 Editcap -h 可以看到（见图 7）。
图 17 Editcap 的帮助文件
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307171802446.png)

同样这个帮助文件也很长，这里只显示了其中的一部分。下面我们以实例的方式来介绍一下它的应用。
```bash
editcap [options]… <infile> <outfile>[ <packet#> [- <packet#>]…]
```
这里面的 `infile` 和 `outfile` 是必要参数，其中 `infile` 表示要处理的捕获数据包文件，`outfile` 表示经过处理的文件。例如，我们已经将 Wireshark 中捕获的数据包文件保存为 `Traces.pcapng` ，现在需要将里面的前 2000 个数据包单独保存成另一个文件，可以执行如下所示的命令：
```bash
editcap –r Traces.pcapng packetrange.pcapng 1-2000
```
这里面使用了一个参数 r，它的作用是保留要处理的文件 `Traces.pcapng` ，如果不使用这个参数的话，这个文件就会被删除掉。

**将一个文件拆分成多个文件时，需要指定拆分的条件**，例如一个捕获了 100000 个数据包的文件，我们就可以按照每 2000 个数据包为一个新文件的方式作为条件。拆分时使用的参数为 c。
```js
editcap –c 2000 Traces.pcapng SplitTrace.pcapng
```
**当一个文件中包含了重复的数据包时，可以使用参数 d 或者 D 来将重复的数据包去掉**，其中-d 在检测一个数据包是否重复时，只会和当前数据包的前 5 个进行比较，而参数 -D 则可以指定范围（有效值可以是 0～100000）。
```js
editcap –d Traces.pcapng nodupes.pcapng
```
## 2. Mergecap 的使用方法
相比起其他工具，Mergecap 的功能比较单一，它主要的功能就是**将多个文件合并成一个文件**，最基本的语法为
```
mergecap –w <outfile.pcapng> infile1.pcapng infile2.pcapng ...
```
也就是 mergecap 后面跟多个文件名，其中的第一个是其他文件合并生成的。主要参数的作用如下。
- `-a` ：将多个文件拼接成一个文件，默认为按照数据包的时间戳进行合并。
- `-s <snaplen>` ：将文件中的数据包均截断为 `<snaplen>` 字节。
- `-w <outfile>` ：设置保存为文件名。
- `-F <capture type>` ：设置保存的文件类型，默认为 pcapng。
- `-T <encap type>` ：设置保存文件的封装类型，默认和原始文件类型一致。

下面的例子中就将 source1.pcapng、source2.pcapng、source3.pcapng 这 3 个文件合并成了一个 merged.pacap 文件。
```js
mergecap –w merged.pacap source1.pcapng source2.pcapng source3.pcapng
```
另外，我们也可以**只截取目标数据包的一部分**来进行合并，例如截取每个数据包的前 128 个字节，使用的命令如下所示：
```js
mergecap –w merged.pacap -s 128 source1.pcapng source2.pcapng source3.pcapng`
```
# 3. capinfos 的使用方法
capinfos 是一个**显示数据包捕获文件信息**的程序。这个程序最常见的参数如下所示：
- -t 输出包文件的类型
- -E 输出包文件的封装类型
- -c 输出包的个数
- -s 输出包文件的大小（单位：byte）
- -d 输出包所有包的总字节长度（单位：byte）
- -u 输出包文件中包的时间周期(单位：second)
- -a 输出包文件中包的起始时间
- -e 输出包文件中包的结束时间
- -y 输出包文件中包的平均速率 (单位：byte/s)
- -i 输出包文件中包的平均速率(单位：bit/s)
- -z 输出包文件中包的平均字节长度 (单位：byte)
- -x 输出包文件中包的平均速率(单位：packet/s)

如果需要查看这个包的所有信息：
```js
capinfos caps.pcap
```
执行的结果如图 8 所示。
图 8 使用 Capinfos 查看数据包的信息
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307171806574.png)

# 4. USBPcapCMD 的使用方法
USB 技术的应用越来越广泛，常用的 U 盘、鼠标、键盘都是 USB 设备。有时也会遇见要对这种设备进行调试的情形，但很少有人知道其实 Wireshark 也是可以胜任这一任务的。==Wireshark 可以像处理网络中的通信一样来捕获和解析 USB 设备的通信==。

Wireshark2.0 之后就加入了对 USB 协议的支持，USB 协议版本有 USB1.0、USB1.1、USB2.0、USB3.1 等，目前 USB2.0 比较常用。讲解如何使用 Wireshark 来捕获和分析 USB 协议。

**使用 Wireshark 对 USB 进行调试时，需要考虑所使用的操作系统**，默认情况下，Windows 环境中需要安装专门的软件才能完成这个工作。不过 Wireshark2.0 以上的版本提供了一个名为 USBPcap 的工具。这个工具需要管理员的工作权限，这个工具没有提供图形化的操作界面，所以需要在命令行下完成这些工作。

首先将工作目录切换到 USBPcap 的安装目录。
```js
cd c:\program Files\USBPcap
```
使用-h 作为参数来查看这个工具的帮助：
```js
C:\Program Files\USBPcap>USBPcapCMD.exe -h
```
图 9 USBPcap 的帮助文件
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307171807598.png)
==如果现在需要列出当前连接设备的话，我们只需要输入这个工具的名称即可==，无需任何参数。执行之后你就可以看到一个 USB 设备里列表，在这个列表可以找到所需要调试的设备。
图10 查看本机的网卡
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307171808675.png)
图 10 中演示了一个在我的工作环境（Windows 7）下的 USB 设备列表，这里面我的计算机连接了一个无线网卡，一个 USB 鼠标，一个 USB键盘和一个 USB2.0 集线器，它们都连接到了 `\\.\USBPcap1` 上。

最后一行会显示“Select filter to monitor（q to quit）：”，在这里面**输入要捕获信息的控制设备**。这里只有一个设备 `\\.\USBPcap1` ，所以我们输入数字 1。

之后我们还要再输入一个文件的名称，你可以按照自己的习惯来命名。这个文件将用来保存捕获到的 USB 设备信息。

可以使用 Enter 键来开始捕获 USB 流量（见图 11），**当开始捕获之后，这个控制台不会有任何的显示**。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307171809527.png)
图 11 开始捕获 USB 流量

当捕获结束时，可以使用 Ctrl+C 组合键。然后 USBPcap 控制台就会关闭，所有捕获的数据将会保存在 `C:\Program Files\USBPcap\` 下。然后我们就可以使用 Wireshark 来查
看这个捕获文件。
图 12 查看捕获到的 USB 流量
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307171810340.png)

# 1.8 小结
介绍了 Wireshark 中常见的各种工具。包括 Tshark、Dumpcap、Editcap、Mergecap、Capinfo 和 USBPcapCMD 这些工具的功能和使用方法。这些工具在安装Wireshark 时就会自动安装完成，相比起 Wireshark，这些工具体积较小、功能单一。但在一些特殊的场合，这些工具往往可以大展身手。