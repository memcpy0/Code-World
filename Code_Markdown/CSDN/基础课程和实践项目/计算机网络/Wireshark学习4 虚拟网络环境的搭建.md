经过前面的学习，相信已经熟悉 Wireshark 的基本操作了，并且也掌握了
如何在成千上万的数据包中找到目标，以及如何将捕获到的数据包保存起来。接下来就可以在实践中应用这些技能了。==为了能模拟出各种常见的网络情况，首先需要建立一个实验室，它将包含各种常见的网络设备和操作系统，这样就可以模拟出各种场景==，这将帮助我们积累更多的 Wireshark 使用经验。建立一个能模拟出大部分网络环境的实验室，需要花费大量的金钱，不过好在现在已经有了很多模拟软件，所以需要的只是一台计算机。

后面会涉及一些如何使用 Wireshark 来分析网络攻击行为的实例，所以这个实验室除了包含**路由器、交换机**和**安装了各种操作系统的设备**之外，我们还会包含**一台安装有Kali Linux 2 操作系统的设备**。围绕如下主题进行讲解：
- 虚拟网络设备的构建工具 eNSP；
- 虚拟 PC 的工具 VMware；
- 在虚拟环境中安装 Kali Linux 2；
- 在虚拟环境中安装其他操作系统；
- eNSP 与 VMware 的连接。

# 1. 虚拟网络设备的构建工具 eNSP
一个完整的网络结构，通常都是由各种网络设备和计算机共同构成的。图1就给出了一个常见的网络结构图，其中包含交换机、路由器和服务器等。
图 4 一个典型的单位网络结构图
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307101743190.png)
对于大多数学习者来说，购买全套的交换机、路由器以及防火墙等设备是不现实的。

不过，现在有很多工具软件都提供了网络设备的虚拟功能，其中最著名的包括 GNS3 和eNSP 等。GNS3 主要提供了对思科设备的模拟，而 eNSP 则主要提供了对华为设备的模拟，**这两个模拟器都提供了虚拟设备与真实网络的连接功能，这也是它们成为目前最受欢迎的模拟器的原因**。下面采用了 eNSP 作为实例，主要是因为当前的 GNS3 没有直接提供交换机的模拟功能，这会让初学者感到不便。

## 1.1 eNSP 的下载与安装
华为技术有限公司在自己的官方网站提供了 eNSP 的下载，而且该工具可以免费使用。

如果想要下载文件的话，则需要在华为网站登录，如果你之前没有该网站账户的话，可以使用手机很简单地进行注册。成功地下载了 eNSP 之后，就可以双击该文件开始进行安装了，如图 3 所示。 
图 2 eNSP 的下载页面
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307101745825.png)
图 3 eNSP 的安装向导
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307101745496.png)

整个安装过程很简单，在中间会出现一个“选择安装其他程序”的提示，如图 4-4 所示。这里给出了 eNSP 正常运行时所需要的 3 个组件，分别是 WinPcap、Wireshark 和 VirtualBox，为了保证 eNSP 各项功能的正常使用，这 3 个组件建议也安装上。不过 eNSP 安装包中集成的组件未必是最新版的，如果你的电脑中已经有了更新的版本，这里就可以不必安装。按照进度，在前面已经安装了最新版的 Wireshark，所以此处可以不必勾选 WinPcap 和 Wireshark。
图 4 eNSP 的选择安装其他程序窗口
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307101747087.png)

如果你选择了这 3 个组件的话，在安装过程中会弹出相应的安装窗口，按照提示完成即可。在安装过程结束之后，可以打开 eNSP。**有了这个 eNSP 之后，我们就可以在没有真实的路由器、交换机等设备的情况下进行模拟实验，学习网络技术**。eNSP 的启动界面如图5 所示。
图 5 eNSP 的启动界面
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307101835299.png)
这个图形化的界面包含了 3 个部分，其中最上方的一行为工具栏，如图 4-6 所示，这里面包含了 eNSP 中常用的操作，例如新建和保存等。
图 6 eNSP 的工具栏
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307101835826.png)
如图 7 所示，界面的左侧则列出了所有可以模拟的设备。
图 7 eNSP 中提供的设备
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307101840074.png)
如图 8 所示，界面的右侧列出了 eNSP 中自带的拓扑实例。
图 8 eNSP 中自带的拓扑实例
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307101840767.png)

## 1.2 使用 eNSP 创建一个实验环境
我们现在就来构建一个包含一个交换机和两个 PC 机的网络实验环境。
（1）单击工具栏最左侧的新建拓扑按钮，创建一个新的实验环境。
（2）**向网络中添加一个交换机**，如图 4-9 所示，在左侧设备分类面板中单击交换机“”图标，在左侧显示的交换机中，左键点击 S3700 图标，将其拖动右侧的拓扑界面中。
图 9 向 eNSP 中添加一台交换机
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307101842996.png)
（3）**向网络中添加两个 PC 机**：在左侧设备分类面板中单击终端图标，在左侧显示的终端中，左键点击 PC 图标，将其拖动右侧的拓扑界面中。按照相同的步骤，添加两个 PC 机。添加完的界面如图 4-10 所示。
图 10 向 eNSP 中添加完设备的操作界面
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307101842148.png)
（4）**连接设备**：在左侧设备面板，单击连接图标，在显示的连接中，选中图标，单击设备选择端口完成连接。其中使用交换机的 Ethernet 0/0/1 端口连接 PC1 的 Ethernet 0/0/1 端口，交换机的 Ethernet 0/0/2 端口连接 PC2 的 Ethernet 0/0/1 端口。**在图 11 中显示连接设备的连线两端显示的都是红点，这表示该连线所连接的端口都处于未开启状态**。
图 11 完成连接的拓扑图
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307101908146.png)
（5）**对终端进行配置**：在 PC 设备上单击鼠标右键，然后在弹出的菜单中选中“设置”选项，查看该设备系统配置信息。如图 4-12 所示，在弹出的设置属性窗口包含多个标签页，我们可以在这里设置包括 IP 地址在内的各种信息。
图 12 PC 设备的配置信息
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307101909948.png)
（6）**启动终端**：在 PC 设备上单击鼠标右键，然后在弹出的菜单中选中“启动”选项（见图 4-13）。
图 13 在 eNSP 中启动 PC 设备
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307101909637.png)
（7）**对交换机进行配置**：在交换机上单击鼠标右键，然后在弹出的菜单中选中“设置”选项（见图 4-14）。
图 14 交换机的视图界面
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307101910748.png)
（8）**启动交换机**：在交换机上单击鼠标右键，然后在弹出的菜单中选中“启动”选项。设备的启动需要一些时间，启动了所有设备的拓扑如图 4-15 所示。
图 4-15 启动了所有设备的拓扑
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307101910051.png)
（9）**对交换机进行配置**：在交换机上单击鼠标右键，然后在弹出的菜单中选中“CLI”选项。**在这个命令行中就可以如同操作真实设备一样**，配置界面如图 4-16 所示。
图 16 交换机的配置界面
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307101911016.png)

这样我们就完成了**一个简单的网络拓扑环境**。
# 2. 虚拟 PC 的工具 VMware
因为虚拟机最大的好处，就在于**可在一台计算机上同时运行多个操作系统**，所以你可以获得的其实不只是双系统，而是多个系统。这些操作系统之间是独立运行的，跟实际上的多台计算机并没有区别。但模拟操作系统的时候会造成很大的系统开销，因此最好加大电脑的物理内存。

目前最为优秀的虚拟机软件包括 VMware workstation 和 Virtual Box，这两款软件的操作都很简单。我们以 VMware workstation 为例，截至本书写作期间，VMware workstation的最新版本为 12.5.7，建议大家在使用时尽量选择最新的版本。
> 如果要在虚拟环境中学习路由交换，eNSP云不要桥接物理网卡，而是桥接虚拟网卡，即最好和VMWare一起使用。

首先，可以在 VMware workstation 的官方网站下载安装程序。国内很多下载网站也都提供了 VMware workstation 的下载。开始运行 VMware workstation 的安装程序，安装的过程很简单，安装完成之后的 VMware 界面如图 17 所示。
图 17 VMware workstation 的工作界面
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307101913488.png)
不过现在 VMware 中还没有载入任何一个镜像，还需要像在真实环境中安装操作系统那样在虚拟机中完成系统的安装。本书中主要用到 3 个操作系统 Kali Linux2、Windows 7 和 Metasploitable。
# 3. 在虚拟环境中引入 Kali Linux 2
Kali Linux 2 是一个为专业人士所提供的**渗透测试和安全审计操作系统**，它是由之前大名鼎鼎的 Back Track 系统发展而来的。Back Track 系统曾经是世界上极为优秀的渗透测试操作系统，取得了巨大的成功。之后，Offensive Security 对 Back Track 进行了升级改造，并在 2013 年 3 月推出了崭新的 Kali Linux 1.0，相比起 Back Track，Kali Linux 提供了更多更新的工具。之后，Offensive Security 每隔一段时间都会对 Kali 进行更新，在 2016 年又推出了功能更为强大的 Kali Linux 2。目前最新的版本是 2017 年推出的 Kali Linux2017.1 。在这个版本中包含了 13 个大类 300 多个程序，几乎已经涵盖了当前世界上所有优秀的渗透测试工具。如果你之前没有使用过 Kali Linux 2，那么我相信在你打开它的瞬间，绝对会被里面数量众多的工具所震撼。

本书使用 Kali Linux 2 的目的，**是为了模拟出网络中可能会出现的各种威胁**。目前 **Offensive Security 提供了已经安装完毕的 Kali Linux 2 操作系统镜像**，我们可以直接下载使用，具体的过程如下。

Offensive security 提供虚拟机镜像文件kali.org/get-kali/#kali-platforms  


 
 
  
  
  
  
  
  
  
  
  
  、、、、是在所下载的 Kali Linux 32 bit VM PAE 下进行调试的，经测试这也是最为稳定的一个版本。所以学习过程中，建议你选择相同的版本。
图 18 Kali Linux 的下载页面
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307101914007.png)
下载之后得到的是一个压缩文件，将这个文件解压到指定目录中。例如我将这个文件解压到 `E:\Kali-Linux-2017.1-vm-i686` 目录。那么启动 VMware 之后，在菜单选项中依次选中“文件”→“打开”，如图 19 所示。
图 19 在菜单选项中依次选中“文件”→“打开”
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307102351833.png)
然后在弹出的文件选择框中选中“Kali-Linux-2017.1-vm-i686.vmx”，如图 4-20 所示。
图 20 选中“Kali-Linux-2017.1-vm-i686.vmx”
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307102352604.png)
双击打开该文件之后，在 VMware 的左侧列表中，就多了一个 Kali-Linux-2017.1-vm-i686 系统，如图 21 所示。
图 21 VMware 的左侧列表中的 Kali-Linux-2017.1-vm-i686
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307102352499.png)
单击这个系统就可以看到相关的详细信息，如图 4-22 所示。
图 22 Kali-Linux-2017.1-vm-i686 系统的详细信息
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307102353812.png)
接下来，我们只需要启动这个操作系统。**这里有一个好消息就是 Kali Linux 2 虚拟机镜像文件中已经自带了 VMware Tools，有了这个工具之后，你就可以实现在虚拟机和宿主机之间拖拽文件、共享文件等功能**。

如图 23 所示，在登录界面，Kali Linux 虚拟机的登录用户名为 root，密码为 toor。
图 23 Kali Linux 虚拟机的登录界面
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307102355443.png)

启动了 Kali Linux 2 之后，可以看到一个和 Windows 类似的图形化操作界面，这个界面的上方有一个菜单栏，左侧有一个快捷的工具栏。单击菜单上的“应用程序”，可以打开一个下拉菜单，**所有的工具按照功能的不同分成了 13 种（菜单中是有 14 个选项，但是最后的“系统服务”并不是工具分类）**。当我们选中其中一个种类的时候，这个种类所包含的软件就会以菜单的形式展示出来，如图 24 所示。
图 24 Kali Linux 虚拟机的登录界面
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307102354442.png)

好了，现在我们就可以使用 Kali Linux 虚拟机了。
# 4. 在虚拟环境中安装其他操作系统 
Metasploitable2 是一个专门用来进行渗透测试的靶机，这个靶机上存在着大量的漏洞，这些漏洞正好是我们学习 Kali Linux 2 是最好的练习对象。这个靶机的安装文件是一个 VMware 虚拟机镜像，我们可以将这个镜像下载下来使用，使用的步骤如下：
（1）首先从 Metasploitable 网站https://sourceforge.net/projects/metasploitable/下载 Metasploitable2 镜像的压缩包，并将其保存到你的计算机中。
（2）下载完成后，将下载下来的 metasploitable-linux-2.0.0.zip 文件解压缩。
（3）接下来启动 VMWare，在菜单栏上单击“文件”/“打开”，然后在弹出的文件选择框中选中刚解压缩的文件夹中的 Metasploitable.vmx。
（4）现在这个 Metasploitable2 就会出现在左侧的虚拟系统列表中了，单击就可以打开这个系统。
（5）对虚拟机的设置不需要更改，但是要注意的是，网络连接处要选择“NAT 模式”（见图 25）。
图 25 Metasploitable2 的网络连接方式
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307102355339.png)
（6）现在 Metasploitable2 就可以正常使用了。我们在系统名称上单击鼠标右键，然后依次选中“电源”→“启动客户机”，就可以打开这个虚拟机了。系统可能会弹出一个菜单，选择“I copied it”即可。
（7）使用“msfadmin”作为用户名，使用“msfadmin”作为密码登录这个系统。
（8）登录成功以后，**VMware 已经为这个系统分配了 IP 地址**。现在我们就可以使用这个系统了。

==上面那个充满漏洞的靶机是一个 Linux 系统，但我们平时进行渗透测试的目标是以 Windows 为主的。所以我们还应该搭建一个 Windows 操作系统作为靶机==。这里我们有两个选择，如果你有一张 Windows 7 的安装盘的话，那么就可以在虚拟机中安装这个系统。另外我建议你最好到微软官网下载微软官方提供的测试镜像。在其官网，微软提供了如图 26 所示的各种系统的虚拟机镜像，**利用这些镜像，渗透测试者可以极为方便地对各种系统和浏览器进行测试**。
图 26 微软提供的操作系统镜像列表
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307102356432.png)
我们下载其中的“IE8 on Win7（x86）”作为靶机，使用的方法和之前的一样。
# 5. eNSP 与 VMware 的连接
## 5.1 VMware 中的网络连接
==我们可以按照自己的想法在 VMware 中建立任意的网络拓扑==。在之前的章节中，已经提过 NAT 的概念了，实际上，**VMware 中使用了一个名为 VMnet 的概念，在 VMware 中每一个 VMnet 就相当于一个交换机，连接到了同一个 VMnet 下的设备就都同处于一个子网内**，你可以在菜单栏点击“编辑”/“虚拟网络编辑器”来查看 VMnet 的设置，如图 27 所示。
图 27 VMware 中的虚拟网络编辑器
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307110049390.png)
这里面只有 VMnet0、VMnet1、VMnet8 这 3 个子网，当然我们还可以添加更多的网络，**这 3 个子网分别对应 VMware 虚拟机软件提供的 3 种进行设备互联的方式**，分别是桥接模式、NAT 模式、仅主机模式。**这些连接方式与 VMvare 中的虚拟网卡是相互对应的**。
- VMnet0：这是 VMware 用于**虚拟桥接网络**下的虚拟交换机。
- VMnet1：这是 VMware 用于**虚拟仅主机模式网络**下的虚拟交换机。
- VMnet8：这是 VMware 用于**虚拟 NAT 网络**下的虚拟交换机。

另外，当我们安装完 VMware 软件之后，**系统中就会多出两块虚拟的网卡**，分别是 VMware Network Adapter VMnet1 和 VMware Network Adapter VMnet8，如图 28 所示。
图 28 安装完 VMware 之后的两块虚拟网卡
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307110052243.png)
- VMware Network Adapter VMnet1：这是 Host 用于与 **Host-Only 虚拟网络**进行通信的虚拟网卡。
- VMware Network Adapter VMnet8：这是 Host 用于与 **NAT 虚拟网络**进行通信的虚拟网卡。

我们来看一下这 3 种连接方式的不同之处。
- NAT 网络：这是 VMware 中一种最为常用的联网模式，这种连接方式使用的是 VMnet8 虚拟交换机。**同处于 NAT 网络模式下的系统通过 VMnet8 交换机进行通信**。NAT 网络模式下的 IP地址、子网掩码、网关和 DNS 服务器==都是通过 DHCP 分配的==，而==该模式下的系统在与外部通信时使用的是虚拟的 NAT 服务器==。
- 桥接网络：这种模式很容易理解，**凡是选择使用桥接网络的系统就好像是局域网中的一个独立的主机**，就是和你真实的计算机一模一样的主机，并且**它也连接到了这个真实的网络**。因此如果我们需要这个系统联网的话，就需要将这个系统和外面的真实主机采用相同的设置方法。
- 仅主机模式：这种模式和 NAT 模式差不多，**同处于这种联网模式下的主机是相互联通的，但是默认是不会连接到外部网络的**，这样我们在进行网络实验（尤其是蠕虫病毒）时就不会担心传播到外部。

在本书中，所使用的虚拟机都采用了 NAT 联网模式，这样**既可以保证虚拟系统的互联，也能保证这些系统连接到外部网络**。
## 5.2 通过 eNSP 中的云与 VMware 相连
**在 eNSP 中存在一种特殊的设备：==云，利用它就可以将虚拟设备和外部真实网络连接到一起==**。这个功能是相当有用的，我们以此可以建立和真实世界网络一摸一样的实验环境。

==eNSP 中的云是通过连接到物理机上的网卡（无论真实网卡和虚拟网卡）完成工作的==，例如如果希望将 VMware 中的 Kali Linux 2 虚拟机连接到 eNSP 中，最简单的做法就是**将 Kali Linux 2 虚拟机的连接方式设置为 VMnet8（NAT），然后将云连接到 VMnet8 上**。具体的连接方式如下所示。
（1）首先在 VMware 中 Kali Linux 2 虚拟机的网络连接方式设置为 NAT，然后启动。
（2）启动 eNSP，**新建一个网络拓扑**，并在其中添加一个交换机和两个 PC 机。
（3）如图 29 所示，向网络拓扑中添加一个云设备。
图 29 完成的网络拓扑
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307110107488.png)
（4）在拓扑图中的云设备上单击鼠标右键，然后在弹出的菜单中选中设置，打开云设备的配置界面，如图 30 所示。
图 30 云设备的配置界面
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307110108558.png)
（5）在这个界面中的“端口创建”部分添加两个端口，添加的方法是首先单击“增加”按钮，添加一个 UDP 端口（见图 31）。
图 31 添加两个端口
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307110108777.png)
（6）然后在绑定信息下拉菜单处选择“VMware Network Adapter Vmnet8”，然后单击“增加”（见图 32）。
图 32 选择正确的绑定信息
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307110110631.png)

（7）添加完的两个端口如图 4-33 所示。
图 33 添加完的两个端口
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307110110717.png)

（8）然后在图 34 所示的端口映射设置中，将入端口编号和出端口编号分别设置为 1和 2，并勾选下方的“双向通道”，然后单击“增加”按钮。
图 34 端口映射设置
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307110111243.png)

（9）将云设备连接到交换机，并启动所有设备（见图 4-35）。
图 35 启动所有设备
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307110111712.png)

（10）默认情况下，Kali Linux 2 的 IP 地址是自动分配的，具体可以 ifconfig 命令查看（见图 36）。
图 36 ifconfig 命令查看到的地址
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307110111803.png)
（11）如图 4-37 所示，我们将 PC1 和 PC2 的 IP 地址都设置在 192.168.169.0/24 内，例如本例中 PC1 设置为 192.168.169.101，PC2 设置为 192.168.169.102，子网掩码设置为255.255.255.0。
图 37 PC 的设置界面
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307110112679.png)

（12）在 PC1 的设置窗口中切换到“命令行”，使用 ping 命令测试与虚拟机 Kali Linux 2 的连接。如果不通的话可以重新启动虚拟机 Kali Linux 2 再进行测试（见图 38）。
图 38 测试与虚拟机 Kali linux 2 的连接
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307110112468.png)

（13）将这个拓扑图保存起来，我们在后面的例子中还会用到，单击工具栏的保存按钮，然后在弹出的窗口中输入保存的名字并选择保存的路径，我们在这里将其命名为“Basic_net.topo”（见图 4-39）。
图 39 拓扑图的保存
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307110113445.png)
好了，**现在已经搭建了一个既包含 eNSP 的设备，又可以连接到VMware 虚拟机的拓扑结构**，在这里面我们几乎可以完成大部分的实验。
# 6. 小结
前面的 3 章我们已经学习了 Wireshark 的使用基础，但单单这些还不足以熟练地应对网络的复杂问题。因此特地讲解了 eNSP 和 VMWare 这两种工具的使用，在它们的帮助下我们可以模拟各种和真实环境一模一样的网络结构，并以此来进行练习。

**eNSP 是一款专门模拟华为交换机和路由器的工具，而 VMware 则可以模拟各种不同的操作系统**，通过对这两者的介绍和讲解，我们建立了一个常见的网络环境，在后面将会多次用到它。

下一章对这些网路设备的原理和作用进行讲解。