@[toc]


---
# 4.1 突破512字节的限制
引导扇区大小有限，只有512字节。软盘1.44MB的容量则大得多。如果我们可以再建立一个文件，通过引导扇区加载该文件进入内存，然后将控制权交给它，就可以突破512字节的束缚了。

该模块文件要做的事情有：**加载内核；准备保护模式；跳入保护模式等**一系列工作，因此被称为 `Loader` 。引导扇区负责将 `Loader` 加载入内存并交给它控制权，其他工作全部由 `Loader` 完成。

此外，为了操作方便，这里需要把软盘变成 `FAT12` 格式。

## 1. FAT12
### (1) 文件系统层次
这是一种文件系统 `File System` ，从DOS时代到现在仍然在软盘上使用。几乎所有**文件系统划分磁盘的层次**都如下：
- 扇区(`Sector`)：磁盘上的最小数据单元；
- 簇(`Cluster`)：一个或多个扇区；
- 分区(`Partition`)：常指整个文件系统。

### (2) FAT12数据结构
之前的引导扇区是整个软盘的第 `0` 个扇区，这个扇区中最重要的数据结构是 `BPB(BIOS ParameterBlock)` 。下面给出的是 `FAT12` 引导扇区的格式，以 `BPB_` 开头的域属于BPB，以 `BS_` 开头的不属于BPB而是引导扇区的一部分：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2020070516394538.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
引导扇区之后，是两个相同的 `FAT` 表，各占用 `9` 个扇区。再之后，是**根目录区**的第一个扇区；再后面，则是**数据区**。因此，整个 `FAT12` 的结构如下图： 
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200706150542104.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

整个 `FAT12` 文件系统中，根目录区位于第二个FAT表之后，开始的扇区号是 `19` ，存储的最多是 `BPB_RootEntCnt` 个目录条目 `Directory Entry` 。由于根目录区的大小依赖于 `BPB_RootEntCnt` ，因此长度不定。

根目录区中，每一个目录条目占 `32` 字节，格式如下，主要是**文件名称**、**文件属性**、**文件大小**、**日期**、**在磁盘中的位置**等等：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200705221639216.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
### (3) 直观体会FAT12的结构
书上的一个小例子：
- 用 `bximage` 创建一个虚拟软盘 `x.img` 并格式化；
- 在 `FreeDos` 下用 `edit.exe` 创建如下文本文件：
	-  `RIVER.TXT` ，内容是 `riverriverriver`；
	- `FLOWER.TXT` ，内容是300个单词 `flower` (这个不做) ；
	- `TREE.TXT` ，内容是 `treetreetree` 。
- 再添加一个 `HOUSE` 目录，在该目录下添加两个文本文件：
	- `CAT.TXT` ，内容为 `catcatcat`；
	- `DOG.TXT` ，内容为 `dogdogdog` 。
- 用二进制查看器打开 `x.img` 进行查看。

过程中：遇到的问题是，我刚开始使用书中之前虚拟 `FreeDos` 的 `a.img` 和 `bochsrc` ，发现 `EDIT.EXE` **不存在**；和原来解压出来的 `FreeDos` 包中的 `bochsrc` 比较，发现自带的启动的是 `c.img` ，`c.img` 比 `a.img` 也大得多，因此我认为 `c.img` 才是完整的 `freedos` 。于是在 `FreeDos` 包中自带的 `bochsrc` 上进行修改：
```shell
###############################################################
# Configuration file for Bochs
###############################################################

# how much memory the emulated machine will have
megs: 32

# filename of ROM images 
romimage: file=$BXSHARE/BIOS-bochs-latest
vgaromimage: file=$BXSHARE/VGABIOS-lgpl-latest

# what disk images will be used 
floppya: 1_44=x.img, status=inserted
ata0-master: type=disk, path=c.img, cylinders=306, heads=4, spt=17

# choose the boot disk.
boot: c

# where do we send log messages?
# log: bochsout.txt
# disable the mouse
mouse: enabled=0

# enable key mapping, using US layout as default.
keyboard:  keymap=/usr/local/share/bochs/keymaps/x11-pc-us.map
```
然后拷贝了到当前文件夹 `c.img` ，启动后可以使用 `edit` 了：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200706145808392.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
接着格式化 `A` 盘，并进入 `A` 盘：
```shell
format A:
A:
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200706150221929.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
现在可以使用 `EDIT` ：
```shell
edit
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200706150321215.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
用 `ALT` 切换至菜单，创建新文件，保存文件，回到DOS等：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200706150457419.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
创建了 `RIVER.TXT` 和 `FLOWER.TXT` 后，我们回到DOS，创建一个目录 `HOUSE` ：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200706151654537.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
然后发现进不去 `EDIT` 了……所以这里没有创建另外两个文件。

> 后来我试了一下，发现最开始时创建 `HOUSE` 文件，然后进入 `EDIT` 之后，创建其他文件，`SAVE AS` 时选择文件夹是可以的。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200706200814424.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
这样做的话，用 `xxd -a -u -c 16 -g 1 -s +0x2600 -l 0x400 x.img` 查看数据，结果如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200706201003560.png)

---
由于扇区从 `0` 计数，根目录区从 `19` 号扇区开始，每个扇区 `512` 个字节，因此根目录区第一个字节位于偏移 `19*512=0x2600` 处。用 `xxd -u -a -g 1 -c 16 -s +0x2600 -l 512 x.img` 得到 `x.img` 的偏移 `0x2600` 处的数据：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200706153734423.png)
> xxd部分参数：
>   - -a | -autoskip
              toggle autoskip: A single '*' replaces nul-lines.  Default off.
 >  - -c cols | -cols cols
              format  <cols> octets per line. Default 16 (-i: 12, -ps: 30, -b:
              6). Max 256.
>   - -g bytes | -groupsize bytes
              separate the output of every <bytes> bytes (two  hex  characters
              or eight bit-digits each) by a whitespace.  Specify -g 0 to sup‐
              press grouping.  <Bytes> defaults to 2 in normal mode and  1  in
              bits  mode.   Grouping  does  not apply to postscript or include
              style.
>  - -l len | -len len
              stop after writing <len> octets.
>  -  -s [+][-]seek
              start at <seek> bytes abs. (or rel.) infile offset.  + indicates
              that  the  seek  is  relative to the current stdin file position
              (meaningless when not reading from stdin).  - indicates that the
              seek  should  be  that many characters from the end of the input
              (or if combined with +: before the current stdin file position).
              Without -s option, xxd starts at the current file position.
> -  -u     use upper case hex letters. Default is lower case.


以 `RIVER.TXT` 为例，它的各项值为：
- 文件名 `DIR_Name` ：`52 49 56 45 52 20 20 20 54 58 54` 11个字节，对应的ASCII码字符为 `R I V E R [sp] [sp] [sp] T X T` ( `[sp]` 是空格)；
- 文件属性 `DIR_Attr` ：`0x20` 一个字节；
- 保留位：`00 00 00 00 00 00 00 00 00 00` 10个字节；
- 最后一次写入时间 `DIR_WrtTime` ：`F5 7B` 即 `0x7BF5` ；
- 最后一次写入日期 `DIR_WrtDate` ：`E6 50` 即 `0x50E6` ；
- 此条目对应的开始簇号 `DIR_FstClus` ：`02 00` 即 `0x0002` ；
- 文件大小 `DIR_FileSize` ：`11 00 00 00` 即 `0x00000011` ；

这些信息中保留位、时间、日期、属性没有用，忽略；最重要的是 `DIR_FstClus` ，它告诉我们文件存储在磁盘中数据区的哪个位置，让我们可以找到它——注意这里，**一个簇只包含一个扇区**，两者等价；**数据区的第一个簇的簇号是 `2`**。图中，`RIVER.TXT` 的开始簇号是 `2` ，即 `RIVER.TXT` **位于数据区第一个簇**。

不过如果只知道簇号没有用，我们还需要**先计算出根目录区的扇区数**：
- 由于根目录区条目最多有 `BPB_RootEntCnt` 个，且该值的偏移地址为 `0x11` ，长度为 `2` ，因此可以用 `xxd -u -a -g 1 -c 16 -s +0x11 -l 0x2 x.img` 查看，得 `E0 00` 即 `0xE0` 个条目；
- 另外，每个扇区的字节数是 `BPB_BytsPerSec` ，偏移地址为 `0xB` ，长度为 `2` ，可以 `xxd -u -a -g 1 -c 16 -s +0xB -l 0x2 x.img` 查看，得 `00 02` 即 `0x200=512` 个字节；
- 可知，根目录区占据的扇区数：
$$\text{RootDirSectors} = {{\text{(BPB\_RootEntCnt * 32) + (BPB\_BytsPerSec - 1)} }\over {\text{BPB\_BytsPerSec}} }$$  
- 具体计算为：$\text{32 * E0H + 200H - 1 = 1DFFH, 1DFFH / 200H = EH = 14}$ 个扇区；

从而，数据区开始的扇区号 = 根目录区开始扇区号 + 14 = 19 + 14 = 33，第 `33` 扇区的偏移量是 $\text {512 * 33 = 0x4200}$ 。用 `xxd -a -u -g 1 -c 16 -s +0x4200 -l 0x11 x.img` 查看内容，正好是 `RIVER.TXT` 文件的内容：
```shell
0004200: 72 69 76 65 72 72 69 76 65 72 72 69 76 65 72 0D  riverriverriver.
0004210: 0A                                               .
```
同样，如果我们要查看 `TREE.TXT` 文件的内容，由于其长度为 `0xE` ，位于数据区的第 `3` 个簇，即 `RIVER.TXT` 的后一个簇，用 `xxd -a -u -g 1 -c 16 -s +0x4400 -l 0xE  x.img` 查看如下：
```shell
0004400: 74 72 65 65 74 72 65 65 74 72 65 65 0D 0A        treetreetree..
```

---
### (4) FAT表区
前面介绍了引导扇区、根目录区、数据区，还有两个相同的 `FAT` 表没有介绍。

对于小于 `512Bytes` 的文件来说，`FAT` 表没有什么用处。但是更大的文件需要 `FAT` 表来寻找所有的簇(扇区)。

`FAT2` 是 `FAT1` 的备份，因此，我们看 `FAT1` 的结构就可以了：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200706201820903.png)
`FAT` 表像一个位图，其中每 `12` 位称为一个 `FAT` 项，代表一个簇。第 `0,1` 个簇永不使用，从第二个 `FAT` 项开始表示数据区的每个簇，和前文中**数据区的第一个簇的簇号是2**相照应。
![在这里插入图片描述](https://img-blog.csdnimg.cn/2020070620211947.png)
通常，`FAT` 项的值表示的是**文件的下一个簇号**，如果值 `>=0xFF8` 表示**当前簇是本文件的最后一个簇**；如果值 `=0xFF7` 则说明**它是一个坏簇**。

以 `RIVER.TXT` 为例，开始簇号为 `2` ，对应 `FAT` 表中的值为 `0xFFF` ，表明这个簇是最后一个。另外，==一个FAT项可能跨越两个扇区，编码时要考虑在内。==

---
## 2. DOS可识别的引导盘
引导扇区必须要有 `BPB` 等头信息才能够被识别，因此需要在程序开头加上它：
```swift
;%define	_BOOT_DEBUG_	; 做 Boot Sector 时一定将此行注释掉!将此行打开后用 nasm Boot.asm -o Boot.com 做成一个.COM文件易于调试

%ifdef	_BOOT_DEBUG_
	org  0100h				; 调试状态, 做成 .COM 文件, 可调试
%else
	org  07c00h				; Boot 状态, Bios 将把 Boot Sector 加载到 0:7C00 处并开始执行
%endif

	jmp short LABEL_START	; Start to boot.
	nop						; 这个nop不可少

	; 下面是 FAT12 磁盘的头
	BS_OEMName	    DB 'ForrestY'	; OEM String, 必须 8 个字节
	BPB_BytsPerSec	DW 512			; 每扇区字节数
	BPB_SecPerClus	DB 1			; 每簇多少扇区
	BPB_RsvdSecCnt	DW 1			; Boot 记录占用多少扇区
	BPB_NumFATs	    DB 2				; 共有多少 FAT 表
	BPB_RootEntCnt	DW 224			; 根目录文件数最大值
	BPB_TotSec16	DW 2880			; 逻辑扇区总数
	BPB_Media	    DB 0xF0			; 媒体描述符
	BPB_FATSz16	    DW 9			; 每FAT扇区数
	BPB_SecPerTrk	DW 18			; 每磁道扇区数
	BPB_NumHeads	DW 2			; 磁头数(面数)
	BPB_HiddSec		DD 0			; 隐藏扇区数
	BPB_TotSec32	DD 0			; wTotalSectorCount为0时这个值记录扇区数
	BS_DrvNum		DB 0			; 中断 13 的驱动器号
	BS_Reserved1	DB 0			; 未使用
	BS_BootSig		DB 29h			; 扩展引导标记 (29h)
	BS_VolID		DD 0			; 卷序列号
	BS_VolLab	DB 'OrangeS0.02'	; 卷标, 必须 11 个字节
	BS_FileSysType	DB 'FAT12   '	; 文件系统类型, 必须 8 个字节  

LABEL_START:
	mov	ax, cs
	mov	ds, ax
	mov	es, ax
	Call	DispStr			; 调用显示字符串例程
	jmp	$					; 无限循环
DispStr:
	mov	ax, BootMessage
	mov	bp, ax				; ES:BP = 串地址
	mov	cx, 16				; CX = 串长度
	mov	ax, 01301h			; AH = 13,  AL = 01h
	mov	bx, 000ch			; 页号为0(BH = 0) 黑底红字(BL = 0Ch,高亮)
	mov	dl, 0
	int	10h					; int 10h
	ret
BootMessage: db	"Hello, OS world!"
times 	510-($-$$)	db	0	; 填充剩下的空间，使生成的二进制代码恰好为512字节
dw 	0xaa55					; 结束标志
```
将生成的 `Boot.bin` 写入软盘引导扇区，不过这次我们不需要手动编译并使用 `dd` 了，给出的Makefile文件如下：
```shell
##################################################
# Makefile of pmtestx.asm (x=[1,2,3...])
##################################################
SRC:=boot.asm
BIN:=$(subst .asm,.bin,$(SRC))

.PHONY : everything

everything : $(BIN)
	dd if=$(BIN) of=a.img bs=512 count=1 conv=notrunc

$(BIN) : $(SRC)
	nasm $< -o $@
```
执行 `make` ，生成 `Boot.bin` 并写入磁盘引导扇区，得到 `a.img` 。为了实现像书中一样的效果，可以在 `VirtualBox` 里创建一个 `MyOs2` 虚拟机。点击新建后，操作系统和版本都选择 `Other` ，内存大小选 `4 mb` ，不添加虚拟硬盘。建成后不要启动，还需要进行设置：
-  选中建好的 `MyOs2` ，点击设置 ，在系统中启动顺序只在软驱上打 √ ；
-  存储中，选择添加软盘控制器，然后添加虚拟软驱，注册并选择文件夹下的 `a.img` ，点击OK；
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200714001153713.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
-    这时虚拟机 `MyOs2` 就完成了，运行的效果不变，依然是： 
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200714001113843.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

似乎什么都没有变，但是现在的软盘可以被DOS、Linux识别了，我们可以很方便地向其中添加、删除文件。

## 3. 一个最简单的Loader
要写代码加载 `Loader` 进入内存，这时我们需要一个最小的 `Loader` ，它会显示一个字符并进入死循环。这个最简单的 `Loader` 代码如下：`ch4/b/loader.asm`
```swift
org 0100h
mov ax, 0B800h
mov gs, ax
mov ah, 0Fh						 ; 0000: 黑底 1111: 白字
mov al, 'L'
mov [gs:((80 * 0 + 39) * 2)], ax ; 屏幕第0行, 第39列
jmp $							 ; 到处死循环
```
这段代码将在屏幕第一行中央出现字符 `L` ，然后进入死循环。
> 虽然这段代码加载到内存的任何位置都可以正确执行，但是为了将来的执行不出问题，**要保证把它放入某个段内偏移0x100的位置**。

将它编译为 `.COM` 文件在DOS下执行：
```shell
$ nasm loader.asm -o loader.com
$ sudo mount -o loop pm.img /mnt/floppy
$ sudo cp loader.com /mnt/floppy/ -v
$ sudo umount /mnt/floppy/
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200716112252394.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

---
## 4. 加载Loader入内存
加载文件进入内存，需要读软盘，用到BIOS中断 `int 13h` ，用法如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200716120735883.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

中断需要的参数不是**从第0扇区开始的扇区号**，而是**磁头号、柱面号和当前柱面的扇区号**。对于 `1.44MB` 的软盘，有 `2` 个面(磁头号 `0` 和 `1` )，每面 `80` 个磁道(磁道 `0～79` )，每个磁道有 `18` 个扇区(扇区号`1～18` )。所以软盘容量 ` =2×80×18×512=1.44MB` 。
> 存储容量＝`磁头(盘面)数(Head) × 磁道(柱面Cylinder)数 × 每道扇区(Sector)数 × 每扇区字节数` :
> - 硬盘有数个盘片，**每盘片两个面，每个面一个磁头**；
> - 盘片被划分为多个扇形区域即扇区：**磁盘上的每个磁道**被**等分为若干个弧段**，这些弧段便是磁盘的扇区，每个扇区可以存放 `512` 个字节的信息，磁盘驱动器在向磁盘读取和写入数据时，要以扇区为单位。1.44MB3.5英寸的软盘，每个磁道分为18个扇区。
> - **同一盘片不同半径**的**同心圆**为磁道：磁道:当磁盘旋转时，磁头若保持在一个位置上，则每个磁头都会在磁盘表面划出一个圆形轨迹，这些圆形轨迹就叫做磁道。这些磁道用肉眼是根本看不到的，因为它们仅是盘面上以特殊方式磁化了的一些磁化区，磁盘上的信息便是沿着这样的轨道存放的。相邻磁道之间并不是紧挨着的，这是因为磁化单元相隔太近时磁性会相互产生影响，同时也为磁头的读写带来困难。一张1.44MB的3.5英寸软盘，一面有80个磁道，而硬盘上的磁道密度则远远大于此值，通常一面有成千上万个磁道。
> - **不同盘片相同半径**构成的**圆柱面**即柱面：硬盘通常由重叠的一组盘片构成，每个盘面都被划分为数目相等的磁道，并从外缘的 `0` 开始编号，**具有相同编号的磁道形成一个圆柱，称之为磁盘的柱面**。
> - **磁盘的柱面数与一个盘面上的磁道数是相等的**。由于每个盘面都有自己的磁头，因此，**盘面数等于总的磁头数**。
> - 所谓硬盘的 `CHS` ，即 `Cylinder`（柱面）、`Head`（磁头）、`Sector`（扇区），只要知道了硬盘的 `CHS` 的数目，即可确定硬盘的容量，`硬盘的容量=柱面数 * 磁头数 * 扇区数 * 512B`。
 
 
柱面(磁道)号、磁头号、起始扇区号可以用下图的方法来计算：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200716120810516.png)
读软盘需要用到堆栈，先在程序开头初始化 `ss, esp` ：(初始化堆栈 `ch4/b/boot.asm`)

```swift
BaseOfStack  equ 07c00h		; 堆栈基地址 (栈底, 从这个位置向低地址生长)
...
	mov ax, cs
	mov ds, ax
	mov es, ax
	mov ss, ax
	mov sp, BaseOfStack
```
然后，读软盘扇区的函数如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2020071612190521.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
接下来就开始寻找 `Loader` 了。不过还要准备一些宏、变量和字符串(这里其长度都设为9字节，不够则用空格补齐)、显示字符串的函数 `DispStr` ：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2020071622480754.png)![在这里插入图片描述](https://img-blog.csdnimg.cn/20200716224908126.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)![在这里插入图片描述](https://img-blog.csdnimg.cn/20200716225549299.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
现在可以正式修改 `boot.asm` ，让它寻找 `Loader.bin` ：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200716230517760.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)![在这里插入图片描述](https://img-blog.csdnimg.cn/20200716230528144.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

---
先编译生成一个 `Boot.bin` ，还需要软盘和其中的Loader。所以，要用bximage生成软盘 `a.img` ，然后在Linux上执行下面的命令：
```handlebars
nasm boot.asm -o boot.bin
nasm loader.asm -o loader.bin
dd if=boot.bin of=a.img bs=512 count=1 conv=notrunc
sudo mount -o loop a.img /mnt/floppy/
sudo cp loader.bin /mnt/floppy/ -v
sudo umount /mnt/floppy/
```
上面的全部命令都可以用Makefile完成，所以直接执行 `make` 就可以了。然后启动Bochs，进行调试如下：
```shell
<bochs:1> b 0x7c00  <- 开始处设置断点
<bochs:2> c			<- 执行到断点
(0) Breakpoint 1, 0x00007c00 in ?? ()
Next at t=14040251
(0) [0x000000007c00] 0000:7c00 (unk. ctxt): jmp .+60 (0x00007c3e)     ; eb3c
<bochs:3> n			<- 跳过BPB
Next at t=14040252
(0) [0x000000007c3e] 0000:7c3e (unk. ctxt): mov ax, cs                ; 8cc8
<bochs:4> u /45		<- 反汇编,发现书中的jmp .+0xfffe是在0x7cb4处,我的则是在0x7cad处
00007c3e: (                    ): mov ax, cs                ; 8cc8
00007c40: (                    ): mov ds, ax                ; 8ed8
00007c42: (                    ): mov es, ax                ; 8ec0
00007c44: (                    ): mov ss, ax                ; 8ed0
00007c46: (                    ): mov sp, 0x7c00            ; bc007c
00007c49: (                    ): xor ah, ah                ; 30e4
00007c4b: (                    ): xor dl, dl                ; 30d2
00007c4d: (                    ): int 0x13                  ; cd13
00007c4f: (                    ): mov word ptr ds:0x7cb1, 0x0013 ; c706b17c1300
00007c55: (                    ): cmp word ptr ds:0x7caf, 0x0000 ; 833eaf7c00
00007c5a: (                    ): jz .+74                   ; 744a
00007c5c: (                    ): dec word ptr ds:0x7caf    ; ff0eaf7c
00007c60: (                    ): mov ax, 0x9000            ; b80090
00007c63: (                    ): mov es, ax                ; 8ec0
00007c65: (                    ): mov bx, 0x0100            ; bb0001
00007c68: (                    ): mov ax, word ptr ds:0x7cb1 ; a1b17c
00007c6b: (                    ): mov cl, 0x01              ; b101
00007c6d: (                    ): call .+135                ; e88700
00007c70: (                    ): mov si, 0x7cb4            ; beb47c
00007c73: (                    ): mov di, 0x0100            ; bf0001
00007c76: (                    ): cld                       ; fc
00007c77: (                    ): mov dx, 0x0010            ; ba1000
00007c7a: (                    ): cmp dx, 0x0000            ; 83fa00
00007c7d: (                    ): jz .+32                   ; 7420
00007c7f: (                    ): dec dx                    ; 4a
00007c80: (                    ): mov cx, 0x000b            ; b90b00
00007c83: (                    ): cmp cx, 0x0000            ; 83f900
00007c86: (                    ): jz .+37                   ; 7425
00007c88: (                    ): dec cx                    ; 49
00007c89: (                    ): lodsb al, byte ptr ds:[si] ; ac
00007c8a: (                    ): cmp al, byte ptr es:[di]  ; 263a05
00007c8d: (                    ): jz .+2                    ; 7402
00007c8f: (                    ): jmp .+3                   ; eb03
00007c91: (                    ): inc di                    ; 47
00007c92: (                    ): jmp .-17                  ; ebef
00007c94: (                    ): and di, 0xffe0            ; 83e7e0
00007c97: (                    ): add di, 0x0020            ; 83c720
00007c9a: (                    ): mov si, 0x7cb4            ; beb47c
00007c9d: (                    ): jmp .-37                  ; ebdb
00007c9f: (                    ): add word ptr ds:0x7cb1, 0x0001 ; 8306b17c01
00007ca4: (                    ): jmp .-81                  ; ebaf
00007ca6: (                    ): mov dh, 0x02              ; b602
00007ca8: (                    ): call .+48                 ; e83000
00007cab: (                    ): jmp .-2                   ; ebfe
00007cad: (                    ): jmp .-2                   ; ebfe
<bochs:5> disasm 0x7c3e 0x7cb6	<- 对指定内存区域反汇编,进行确认
00007c3e: (                    ): mov ax, cs                ; 8cc8
00007c40: (                    ): mov ds, ax                ; 8ed8
00007c42: (                    ): mov es, ax                ; 8ec0
00007c44: (                    ): mov ss, ax                ; 8ed0
00007c46: (                    ): mov sp, 0x7c00            ; bc007c
00007c49: (                    ): xor ah, ah                ; 30e4
00007c4b: (                    ): xor dl, dl                ; 30d2
00007c4d: (                    ): int 0x13                  ; cd13
00007c4f: (                    ): mov word ptr ds:0x7cb1, 0x0013 ; c706b17c1300
00007c55: (                    ): cmp word ptr ds:0x7caf, 0x0000 ; 833eaf7c00
00007c5a: (                    ): jz .+74                   ; 744a
00007c5c: (                    ): dec word ptr ds:0x7caf    ; ff0eaf7c
00007c60: (                    ): mov ax, 0x9000            ; b80090
00007c63: (                    ): mov es, ax                ; 8ec0
00007c65: (                    ): mov bx, 0x0100            ; bb0001
00007c68: (                    ): mov ax, word ptr ds:0x7cb1 ; a1b17c
00007c6b: (                    ): mov cl, 0x01              ; b101
00007c6d: (                    ): call .+135                ; e88700
00007c70: (                    ): mov si, 0x7cb4            ; beb47c
00007c73: (                    ): mov di, 0x0100            ; bf0001
00007c76: (                    ): cld                       ; fc
00007c77: (                    ): mov dx, 0x0010            ; ba1000
00007c7a: (                    ): cmp dx, 0x0000            ; 83fa00
00007c7d: (                    ): jz .+32                   ; 7420
00007c7f: (                    ): dec dx                    ; 4a
00007c80: (                    ): mov cx, 0x000b            ; b90b00
00007c83: (                    ): cmp cx, 0x0000            ; 83f900
00007c86: (                    ): jz .+37                   ; 7425
00007c88: (                    ): dec cx                    ; 49
00007c89: (                    ): lodsb al, byte ptr ds:[si] ; ac
00007c8a: (                    ): cmp al, byte ptr es:[di]  ; 263a05
00007c8d: (                    ): jz .+2                    ; 7402
00007c8f: (                    ): jmp .+3                   ; eb03
00007c91: (                    ): inc di                    ; 47
00007c92: (                    ): jmp .-17                  ; ebef
00007c94: (                    ): and di, 0xffe0            ; 83e7e0
00007c97: (                    ): add di, 0x0020            ; 83c720
00007c9a: (                    ): mov si, 0x7cb4            ; beb47c
00007c9d: (                    ): jmp .-37                  ; ebdb
00007c9f: (                    ): add word ptr ds:0x7cb1, 0x0001 ; 8306b17c01
00007ca4: (                    ): jmp .-81                  ; ebaf
00007ca6: (                    ): mov dh, 0x02              ; b602
00007ca8: (                    ): call .+48                 ; e83000
00007cab: (                    ): jmp .-2                   ; ebfe
00007cad: (                    ): jmp .-2                   ; ebfe
00007caf: (                    ): push cs                   ; 0e
00007cb0: (                    ): add byte ptr ds:[bx+si], al ; 0000
00007cb2: (                    ): add byte ptr ds:[bx+si], al ; 0000
00007cb4: (                    ): dec sp                    ; 4c
00007cb5: (                    ): dec di                    ; 4f
<bochs:6> b 0x7cad		<- 在jmp $处设断点
<bochs:7> c 			<- 执行到断点
(0) Breakpoint 2, 0x00007cad in ?? ()
Next at t=14086046
(0) [0x000000007cad] 0000:7cad (unk. ctxt): jmp .-2 (0x00007cad)      ; ebfe
<bochs:8> x /32xb es:di - 16 	<- 查看es:di前后的内存
[bochs]:
0x0009011b <bogus+       0>:	0x00	0xff	0xff	0xff	0xff	0x4c	0x4f	0x41
0x00090123 <bogus+       8>:	0x44	0x45	0x52	0x20	0x20	0x42	0x49	0x4e
0x0009012b <bogus+      16>:	0x20	0x00	0x64	0x36	0xbd	0xf0	0x50	0xf0
0x00090133 <bogus+      24>:	0x50	0x00	0x00	0x36	0xbd	0xf0	0x50	0x03
<bochs:9> x /13xcb es:di - 11	<- 发现es:di前面就是我们要找的文件名
[bochs]:
0x00090120 <bogus+       0>:  L    O    A    D    E    R            
0x00090128 <bogus+       8>:  B    I    N        \0 
<bochs:10> sreg					<- 我们查看es
es:0x9000, dh=0x00009309, dl=0x0000ffff, valid=3
	Data segment, base=0x00090000, limit=0x0000ffff, Read/Write, Accessed
cs:0x0000, dh=0x00009300, dl=0x0000ffff, valid=1
	Data segment, base=0x00000000, limit=0x0000ffff, Read/Write, Accessed
ss:0x0000, dh=0x00009300, dl=0x0000ffff, valid=7
	Data segment, base=0x00000000, limit=0x0000ffff, Read/Write, Accessed
ds:0x0000, dh=0x00009300, dl=0x0000ffff, valid=3
	Data segment, base=0x00000000, limit=0x0000ffff, Read/Write, Accessed
fs:0x0000, dh=0x00009300, dl=0x0000ffff, valid=1
	Data segment, base=0x00000000, limit=0x0000ffff, Read/Write, Accessed
gs:0x0000, dh=0x00009300, dl=0x0000ffff, valid=1
	Data segment, base=0x00000000, limit=0x0000ffff, Read/Write, Accessed
ldtr:0x0000, dh=0x00008200, dl=0x0000ffff, valid=1
tr:0x0000, dh=0x00008b00, dl=0x0000ffff, valid=1
gdtr:base=0x000fa1f7, limit=0x30
idtr:base=0x00000000, limit=0x3ff
<bochs:11> r					<- 我们看di
eax: 0x0000004e 78
ecx: 0x00090000 589824
edx: 0x0000000e 14
ebx: 0x00000100 256
esp: 0x00007c00 31744
ebp: 0x00000000 0
esi: 0x000e7cbf 949439
edi: 0x0000012b 299
eip: 0x00007cad
eflags 0x00000046: id vip vif ac vm rf nt IOPL=0 of df if tf sf ZF af PF cf
```
调试过程中，反汇编后对照代码设置断点在 `0x7cad` 处，我这里和书上不一样。程序执行到那里后，`es:di = 0x9000:0x12b` 即内存 `0x9012b` 处，这里正是Loader.bin这个文件名的后面，`di` 刚刚比较完成所有11个字符，找到了Loader.bin。

我们确信可以成功找到文件后，要将 `Loader.bin` 加载入内存。**我们有了这个文件的起始扇区号，可以把起始扇区加载入内存，然后通过它找到FAT中的项，从而找到Loader占用的其他所有扇区。**

我们将Loader装入内存的 `BaseOfLoader:OffsetOfLoader` 中，在代码4.5中根目录区也是装到这个位置，算是资源的回收利用——我们**装入根目录区就是为了找到相关的目录条目**罢了，找到了之后，根目录区就没有用了。所以现在用Loader把它覆盖掉。

下面是一个函数，输入扇区号，输出对应的FAT项的值，这样就可以重复从FAT中找相应的项，省了很多麻烦：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200717113232210.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200717113247834.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
接着就开始加载Loader了，其中的 `DeltaSectorNo equ 17` 是一个常量：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200717121302353.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

## 5. 向Loader交出控制权
上面的代码调试通过后，我们就已经成功地将Loader加载入内存，下面开始跳转，执行Loader： ![在这里插入图片描述](https://img-blog.csdnimg.cn/20200717121634418.png)


## 6. 整理boot.asm
最后对 `boot.asm` 进行整理，以便让执行效果好一点。下面的代码先清屏，然后显示字符串 `Booting` ，加载Loader时打印的圆点则出现在这个字符串的后面。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200717122822827.png)


加载完毕跳入Loader之前打印字符串 `Ready.` ：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200717122840727.png)
执行效果如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200717122529649.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
书中说：`Loader.bin` 本质上是个 `.COM` 文件，最大不可以超过64KB，但是我们已经突破了512字节的限制了，有了长足的进步！

----
# 4.2 保护模式下的操作系统
Linux下的引导扇区代码 `Boot.s` 更简单，它直接把内核移动到目标内存。但是我们想和 `MSDOS` 的磁盘格式兼容，因此多做了些工作。

比如说，可以现在复制 `chapter3/pmtest9.asm` 编译为 `loader.bin` ，复制到刚引导过的软盘中覆盖掉原来的 `loader.bin` ，结果如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200717123517309.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70) 

==其实，只要一个.COM文件中不含有DOS系统调用，它就可以作为一个Loader来使用。== 但是Loader不是操作系统内核。为了加载内核，Loader需要做的事情有：
- **加载内核进入内存**；
- **跳入保护模式**。

此后，内核在保护模式下开始运行。

还有，现在的Loader是.COM文件，放入内存就可以运行。但是**将来的内核是在Linux下编译链接得到的 `ELF` 文件**，需要我们仔细研究。
