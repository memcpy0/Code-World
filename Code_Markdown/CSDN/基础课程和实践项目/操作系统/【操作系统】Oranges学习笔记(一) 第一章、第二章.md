@[toc]


---
# 零、Oranges相关资源
在实验过程中，某一节的程序能成功运行后，我会把相关资源放到GitHub上：[]()。目前已经更新完了书中的第一至第七章。

---
# 一、工作环境和准备工作
**学习环境**：
Windows(原机) + Ubuntu 12.04.5(Oracle VM VirtualBox虚拟机) + Bochs.2.6.8 + NASM + GCC
 
**环境搭建**：
- 我们要在 `windows` 系统下安装 `virtualbox` 软件；
- 之后要用 `virtualbox` 创建一个虚拟机，并为这个虚拟机安装 `ubuntu` 系统；
- 接着启动刚装好的 `ubuntu` 虚拟机，在其中安装`bochs`（注意编译时的配置选项，书中有说明，不然将无法进行调试）；
- 最终运行书中的第一个示例

**软件**& `Linux Distribution` **版本要求**：
- `virtualbox：6.0.12`
- `ubuntu` 镜像：`ubuntu-12.04.5-desktop-i386.iso`
- `bochs`：`bochs-2.6.8.tar.gz`

**简要的参考步骤**：
- 卸载系统中已有的 `virtualbox` ，安装最新版本的
`https://www.virtualbox.org/wiki/Downloads`；
- 用 `virtualbox` 创建并安装 `Ubuntu` 系统，从 `http://mirrors.163.com/ubuntu-releases` 下载 `ubuntu-12.04.5-desktop-i386.iso` 镜像进行安装；
- 从 `http://bochs.sourceforge.net/getcurrent.html` 下载 `bochs` 源码 `bochs-2.6.8.tar.gz`，在 `ubuntu` 中编译安装 `bochs` 软件 

- 使用 `bximage` 创建软驱，调试书上第一个例子。具体书中已经写了。同时参考 `http://bbs.21ic.com/blog-1423056-134746.html`

书的第一章是简要开头，第二章是准备工作。

---
# 二、第一章 马上动手写一个最小的“操作系统”
## 1. 第一章笔记

书中给出的代码如下：
```handlebars
	org	07c00h			; 告诉编译器程序加载到7c00处
	mov	ax, cs			
	mov	ds, ax			; 使ds,es段寄存器指向与cs相同的段
	mov	es, ax
	call	DispStr		; 调用显示字符串例程
	jmp	$				; 无限循环
DispStr:
	mov	ax, BootMessage
	mov	bp, ax			; ES:BP = 串地址
	mov	cx, 16			; CX = 串长度
	mov	ax, 01301h		; AH = 13,  AL = 01h
	mov	bx, 000ch		; 页号为0(BH = 0) 黑底红字(BL = 0Ch,高亮)
	mov	dl, 0
	int	10h				; 10h 号中断
	ret
BootMessage:	db	"Hello, OS world!"
times 510-($-$$) db	0	; 填充剩下的空间，使生成的二进制代码恰好为512字节
dw 	0xaa55				; 结束标志
```
上述代码实际上是一个引导扇区 `boot sector` ，**它直接在裸机上运行，不依赖于其他软件**。当计算机电源被打开后：
- 计算机先进行加电自检 `POST` ；
- 然后寻找启动盘，如果是选择从软盘打开，BIOS会检查软盘的 `0` 面 `0` 磁道 `1` 扇区，如果发现该扇区是以 `0xaa55` 结束，则BIOS认为它是一个 `boot sector`；
- 一旦BIOS发现了引导扇区，就会将这 `512` 字节的内容装载到内存地址 `0000:7c00` 处，然后跳转到 `0000:7c00` 处将控制权彻底交给这段引导代码；
- 此后，计算机不再由BIOS控制，而是由操作系统的一部分来控制。

部分指令说明：
- `org` 伪指令，用来规定程序存放单元的偏移量，如果在源程序的第一条指令使用了这样的指令如 `org 2000h` ，则汇编程序会把 `ip` 设为 `2000h` ，程序的第一个字节存放到 `2000h` 处，然后依次顺序存放后面的内容，直到遇到另一个 `org` 。
- `times 510-($-$$) db 0` 将 `0` 字节重复 `510-($-$$)` 次。

## 2. nasm学习
本书使用的是 `nasm` 的语法(`The Netwide Assembler`) ，NASM是目前唯一开源免费的汇编器，和MASM一样遵从Intel语法，指令集、语法规则**几乎完全相同。**
> 实模式编程：Linux和Windows运行在x86的机器上，**都是运行在保护模式下的**，因此我们需要将编译好的实模式程序写进虚拟硬盘的主引导扇区，然后由虚拟机启动来观察程序运行效果。

和 `masm` 的区别在于：
- `nasm` 中任何不被方括号 `[]` 括起来的**标签和变量名**都是地址；访问标签中的内容必须用 `[]` ；因此不需要 `offset` 关键字；
- `nasm` 中，变量和标签是一样的，下面两条语句是等价的：
	 ```
	 foo dw 1 
	 foo : dw 1
	 ```
- `nasm` 中的 `$` 是当前语句的地址；`$$` 是本节 `section` 的起始语句的地址。
- `nasm` 中使用 `section` 表示一段代码，后面跟一个段的名称(自定义)和段属性。`nasm` 的标准段包括 `section .data` / `section .text` / `section .bss` ，它们默认包含 `vstart = 0` 的语义，即段内的指令和标号的汇编地址都是段内偏移地址。

先安装 `nasm` ，然后使用 `nasm` 编译上述的代码，我们就得到了一个 `512` 字节的 `boot.bin` 代码：
```shell
sudo apt-get install nasm
nasm boot.asm -o boot.bin
```
其他nasm指令参见nasm中文手册。

---

## 3. 实际操作
我的机器没有软驱，所以要创建一个虚拟软盘。由于 `VirtualBox` 不能像创建虚拟空白硬盘一样创建虚拟软盘，而只能**选择一个已有的虚拟软驱** `.img` ，所以要先创建一个基于编译得到的 `boot.bin` 的虚拟软盘。 命令如下：
```powershell
dd if=boot.bin of=floppy.img bs=512 count=1
```
这条命令创建出一个大小为 `512B` ，名为 `floppy.img` ，写入了 `boot.bin` 的虚拟软盘镜像。其中 `boot.bin` 是读取位置，`floppy.img` 是写入位置。

为了实现像书中一样的效果，可以在 `VirtualBox` 里创建一个 `MyOS` 虚拟机。点击**新建**后，过程中要改动的默认设置有：操作系统和版本都选择 `Other` ，内存大小选 `4 mb` ，不创建虚拟硬盘。

建成后不要启动，还需要进行设置：
- 选中建好的 `MyOS` ，点击**设置** ，在**系统**中启动顺序只在**软驱**上打 `√` ；
- **存储**中，选择**添加软盘控制器**，然后添加虚拟软驱，选择文件夹下的 `floppy.img` ，点击**OK**；
<img src="https://img-blog.csdnimg.cn/20200703002338119.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="53%">
-  这时虚拟机 `MyOS` 就完成了，启动后画面如下：
<img src="https://img-blog.csdnimg.cn/20200703002601875.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="52%">
  
这样，第一章就完成了。


---

# 三、第二章 搭建你的工作环境
如果一直使用**虚拟软盘**，太麻烦了，所以我们需要使用 `bochs` 搭建工作环境。

书中分别介绍了 `windows` 和 `linux` 的安装过程，我这里虽然是 `windows` 的机器，但是全部过程都在 **`linux` 虚拟机**中完成。

尽管如此，为了使用实验提供的环境和程序源代码，我开启了**VGA增强功能**，创建了一个共享文件夹，用于主机和虚拟机的信息交换。


## 1. bochs安装和使用步骤
这一部分折腾了我许久，书中的一些配置现在已经不适合了，出现了许多错误。

① 预装相关程序和库：
```shell
sudo apt-get install build-essential xorg-dev libgtk2.0-dev
```
② 安装前的配置命令：(在 `root` 用户下)
```shell
./configure --with-x11 --with-wx --enable-debugger --enable-disasm --enable-all-optimizations --enable-readline --enable-long-phy-address --enable-debugger-gui
```
③ 编译安装
```shell
make
make install
```
④ 将 `bochs2.6.8` 目录下的 `bios` 文件夹中的全部内容复制到 `/usr/share/bochs` 目录中，没有 `bochs` 文件夹的话需要自己创建。

⑤ 设置配置文件。这一步相当重要，如果没有设置好的话，就会出现各种各样的错误：
```py
###############################################################
# Configuration file for Bochs
###############################################################

# how much memory the emulated machine will have
megs: 32

# filename of ROM images
romimage: file=/usr/local/share/bochs/BIOS-bochs-latest
vgaromimage: file=/usr/local/share/bochs/VGABIOS-lgpl-latest

# what disk images will be used 
floppya: 1_44=a.img, status=inserted

# choose the boot disk.
boot: floppy

# where do we send log messages?
log: bochsout.txt

# disable the mouse
mouse: enabled=0

# enable key mapping, using US layout as default.
keyboard:  keymap=/usr/local/share/bochs/keymaps/x11-pc-us.map
```
上面是我的配置文件 `bochsrc` 。如果照着书中的配置的话，会出现一些错误：
- 运行bochsrc时，出现错误 `ROM: couldn't open ROM image file '/usr/share/bochs/BIOS-bochs-latest'` ，要将第 `9` 行的 `romimage: file=/usr/share/bochs/BIOS-bochs-latest` 改为 `romimage: file=/usr/local/share/bochs/BIOS-bochs-latest` ；
- 运行bochsrc时，出现错误 `bochsrc:10: vgaromimage directive malformed` ，要将 `bochsrc` 文件中第 `10` 行的 `vgaromimage: /usr/share/vgabios/vgabios.bin` 改为 `vgaromimage: file=/usr/share/vgabios/vgabios.bin` 或者改为我给出的配置文件中的形式，具体视 `bochs/bios` 中的内容和自己的文件夹组织方式而定；
- 运行时出现关于 `keymap` 的错误如：
<img src="https://img-blog.csdnimg.cn/20200703010634748.png" width="39%">
需要将文件最后一行的 `keyboard_mapping: enabled=1, map=/usr/share/bochs/keymaps/x11-pc-us.map` 改为 `keyboard:  keymap=/usr/local/share/bochs/keymaps/x11-pc-us.map` 。

⑥ 利用 `bximage` 生成软盘映像，在第一步提示中输入 `fd` 即可。具体可以参照书中描述。

⑦ 将引导扇区 `boot.bin` 写入虚拟软盘：
```shell
dd if=boot.bin of=a.img bs=512 count=1 conv=notrunc
```
⑧ 运行 `bochs` ：
- 启动 `bochs` ：
	```shell
	bochs
	```
- 默认选择 `[6]` ，因此回车即可，然后弹出一个黑框，无内容：
	<img src="https://img-blog.csdnimg.cn/20200703005620601.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="55%">
- 此时在终端中键入 `c` 并回车就会执行：
<img src="https://img-blog.csdnimg.cn/20200703005733494.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="55%">
---

## 2. bochs调试操作系统
运行的是一个有调试功能的 `bochs` ，启动后会出现若干选项，一般默认为 `6. Begin simulation` ，直接按回车键就可以了。不过之后 `bochs` 没有直接进入运转，而是给出了一个提示符，等待输入。

此时，如果要调试的话，可以设置断点、查看CPU寄存器、查看某个内存地址的内容等。具体过程见书。

一些常用的命令如下：
<img src="https://img-blog.csdnimg.cn/20200703012900100.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="51%">
 
