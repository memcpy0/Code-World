@[toc]

这是第三章 **保护模式** 第一部分，也是之前我在微机原理与接口技术中学过的一部分知识，但是当时学得不透彻，这里下点功夫吧。

---
# 3.1 认识保护模式
## 1. 实模式跳转到保护模式
### (1) 具体代码和实验
代码1 `chapter3/a/pmtest1.asm` 做了这些工作：
- 定义了一个叫做 `GDT` 的数据结构；
- 后面16位代码段进行了一些和 `GDT` 相关的操作；
- 程序最后跳转到32位代码段，操作了一下显存。
```perl
; ==========================================
; pmtest1.asm
; 编译方法：nasm pmtest1.asm -o pmtest1.bin
; ==========================================
%include	"pm.inc"	; 常量, 宏, 以及一些说明

org	07c00h
jmp	LABEL_BEGIN

[SECTION .gdt]
; GDT
; 段基址, 段界限, 属性
LABEL_GDT:	       Descriptor       0,                0, 0            ; 空描述符
LABEL_DESC_CODE32: Descriptor       0, SegCode32Len - 1, DA_C + DA_32 ; 非一致代码段
LABEL_DESC_VIDEO:  Descriptor 0B8000h,           0ffffh, DA_DRW	      ; 显存首地址
; GDT 结束

GdtLen	equ	$ - LABEL_GDT	; GDT长度
GdtPtr	dw	GdtLen - 1	    ; GDT界限
		dd	0		        ; GDT基地址

; GDT 选择子
SelectorCode32	equ	LABEL_DESC_CODE32 - LABEL_GDT
SelectorVideo	equ	LABEL_DESC_VIDEO  - LABEL_GDT
; END of [SECTION .gdt]

[SECTION .s16]
[BITS	16]
LABEL_BEGIN:
	mov	ax, cs
	mov	ds, ax
	mov	es, ax
	mov	ss, ax
	mov	sp, 0100h

	; 初始化32位代码段描述符
	xor	eax, eax
	mov	ax, cs
	shl	eax, 4
	add	eax, LABEL_SEG_CODE32
	mov	word [LABEL_DESC_CODE32 + 2], ax
	shr	eax, 16
	mov	byte [LABEL_DESC_CODE32 + 4], al
	mov	byte [LABEL_DESC_CODE32 + 7], ah

	; 为加载 GDTR 作准备
	xor	eax, eax
	mov	ax, ds
	shl	eax, 4
	add	eax, LABEL_GDT			; eax <- gdt 基地址
	mov	dword [GdtPtr + 2], eax	; [GdtPtr + 2] <- gdt 基地址

	; 加载 GDTR
	lgdt	[GdtPtr]

	; 关中断
	cli

	; 打开地址线A20
	in	al, 92h
	or	al, 00000010b
	out	92h, al

	; 准备切换到保护模式
	mov	eax, cr0
	or	eax, 1
	mov	cr0, eax

	; 真正进入保护模式
	jmp	dword SelectorCode32:0	; 执行这一句会把SelectorCode32装入cs,
								; 并跳转到Code32Selector:0处
; END of [SECTION .s16]


[SECTION .s32]					; 32位代码段.由实模式跳入.
[BITS	32]

LABEL_SEG_CODE32:
	mov	ax, SelectorVideo
	mov	gs, ax					; 视频段选择子(目的)

	mov	edi, (80 * 11 + 79) * 2	; 屏幕第 11 行, 第 79 列。
	mov	ah, 0Ch					; 0000: 黑底    1100: 红字
	mov	al, 'P'
	mov	[gs:edi], ax

	; 到此停止
	jmp	$

SegCode32Len equ	$ - LABEL_SEG_CODE32
; END of [SECTION .s32]
```

这是第三章第一个代码实验，照着书中给出的代码编译后，`dd if=pmtest1.bin of=a.img bs=512 count=1 conv=notrunc` 了一个 `a.img` ，然后启动 `bochs` 时发现报错：
<img src="https://img-blog.csdnimg.cn/20200703223453700.png">

看了一下书，发现书中说："将第2章中我们用过的软盘映像 `a.img` 和 `Bochs` 的配置文件bochsrc复制过来，并将生成的二进制写入软盘映像"。后者我做到了，前者没有做。

所以，出错的原因是——第二章中的 `a.img` 已经被填充为 `512` 字节并且以 `0xaa55` 结束，BIOS因此认为它是一个引导扇区，就去加载它。第三章中代码没有这样做，才会报错。

解决方法当然不是直接在代码最后面添加上 `times 510-($-$$) db 0` 和 `dw 0xaa55` 两句代码，先按照书上说的做。如下图，可以在屏幕右侧看到一个红色的字母 <font color="red">P</font> ，然后再也不动了。因为程序最后一部分代码写入了两个字节到显存中。
<img src="https://img-blog.csdnimg.cn/20200703225133600.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="54%">
当然，如果注释掉全部类似 `[SECTION ...]` 的语句，然后在代码最后加上那两句话，也可以正常运行：
<img src="https://img-blog.csdnimg.cn/20200703225535847.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="54%">
我用 `ndisasm` 反汇编新的 `pmtest1.bin` 后得到的 `dispmtest1.asm` 最后几个字节如下，足以证明这种方法的可行性：

```powershell
000001F8  0000              add [bx+si],al
000001FA  0000              add [bx+si],al
000001FC  0000              add [bx+si],al
000001FE  55                push bp
000001FF  AA                stosb
```

### (2) 详细分析
代码的具体分析见下面。有一部分内容可能需要看了后面的书才看得懂。

分析 ① GDT段：
```handlebars
[SECTION .gdt]
; GDT
; 段基址, 段界限, 属性
LABEL_GDT:	       Descriptor       0,                0, 0            ; 空描述符
LABEL_DESC_CODE32: Descriptor       0, SegCode32Len - 1, DA_C + DA_32 ; 非一致代码段
LABEL_DESC_VIDEO:  Descriptor 0B8000h,           0ffffh, DA_DRW	      ; 显存首地址
; GDT 结束

GdtLen	equ	$ - LABEL_GDT	; GDT长度
GdtPtr	dw	GdtLen - 1	    ; GDT界限
		dd	0		        ; GDT基地址

; GDT 选择子
SelectorCode32		equ	LABEL_DESC_CODE32	- LABEL_GDT
SelectorVideo		equ	LABEL_DESC_VIDEO	- LABEL_GDT
; END of [SECTION .gdt]
```
这一段 `SECTION .gdt` 中定义了 `3` 个描述符 `Descriptor` ，是个结构体数组，数组名即为 `GDT` ；`GdtLen` 定义为 `GDT` 的长度；`GdtPtr` 则是一个 `6` 个字节的数据结构，前2个字节是 `GdtLen-1` 即 `GDT` 段界限，后面4个字节是 `GDT` 段基址。

`Descriptor` 是 `chapter3/a/pm.inc` 中定义的一个生成描述符的宏，类似于一个结构体：
```handlebars
; 描述符, 共8字节
; usage: Descriptor Base, Limit, Attr 
;        Base:  dd 段基址32位
;        Limit: dd (low 20 bits available) 段限长20位
;        Attr:  dw (lower 4 bits of higher byte are always 0) 段属性 1个半字节
%macro Descriptor 3
	dw	%2 & 0FFFFh				    ; 段界限1(15~0)(Byte1-Byte0)
	dw	%1 & 0FFFFh				    ; 段基址1(15~0)(Byte3~Byte2)
	db	(%1 >> 16) & 0FFh			; 段基址2(23~16)(Byte4)
	dw	((%2 >> 8) & 0F00h) | (%3 & 0F0FFh)	; 属性1 + 段界限2(19~16) + 属性2 (Byte6-Byte5)
	db	(%1 >> 24) & 0FFh			; 段基址3(31~24)(Byte7)
%endmacro
```
这个结构体完美符合全局描述符表中描述符的定义：
<img src="https://img-blog.csdnimg.cn/20200704001024283.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="54%">
`Descriptor` 宏接受3个参数，分别是段基址、段界限和段属性，然后将三个参数加以转换为图中描述符对应的格式，从低字节到高字节。

`SECTION .gdt` 中定义的 `3` 个描述符中，`GDT` **第一个描述符必须是空描述符/哑描述符/NULL描述符**，这是处理器的规定(可能是为了方便定义 `GdtLen, GdtPtr` 和选择子)，所以 `LABEL_GDT` 的段基址、段限长和属性都是零，是一个空的描述符。`LABEL_DESC_CODE32` 是代码段描述符，`LABEL_DESC_VIDEO` 是图形显示段的描述符。属性这里先不过多赘述。

接着是 `3` 个选择子，直观看来，选择子的定义 `LABEL_DESC_CODE32 - LABEL_GDT` 是描述符对应GDT基地址的偏移。但是实际上，选择子的结构如图：
<img src="https://img-blog.csdnimg.cn/20200704001916547.png" width="50%">当 `RPL,TI` 都为零时，选择子才能够说是对应描述符相当于GDT基地址的偏移，由于描述符大小为 `8` 字节，则**选择子必须为** `8` **的倍数**，因此其后三位必然为零。

这一段到此结束，GDT中的描述符和相应选择子都定义完成了。

---
分析 ② 16位代码段：
```handlebars
[SECTION .s16]
[BITS	16]
LABEL_BEGIN:
	mov	ax, cs
	mov	ds, ax
	mov	es, ax
	mov	ss, ax
	mov	sp, 0100h
	; 初始化32位代码段的描述符
	xor	eax, eax						 ; eax=0
	mov	ax, cs
	shl	eax, 4							 ; eax*=16
	add	eax, LABEL_SEG_CODE32			 ; eax=32位代码段的标号地址
	mov	word [LABEL_DESC_CODE32 + 2], ax ; Byte3~Byte2, 段基址15~0
	shr	eax, 16							 ; eax >>= 16
	mov	byte [LABEL_DESC_CODE32 + 4], al ; Byte4, 段基址23~16
	mov	byte [LABEL_DESC_CODE32 + 7], ah ; Byte7, 段基址31~24
```
这一段 `[BITS 16]` 指出其是一个16位代码段，它初始化了 `cs=ds=es=ss` 等段寄存器。同时，它初始化了指向**32位代码段**的描述符 `LABEL_DESC_CODE32` ，由于宏定义时段界限和属性都已经指定如下：
```
LABEL_DESC_CODE32: Descriptor 0, SegCode32Len - 1, DA_C + DA_32 ; 非一致代码段
```
**真正要初始化的是段基址**——这里将 `LABEL_SEG_CODE32` 的物理地址赋给 `eax` ，然后分三个部分赋给描述符 `DESC_CODE32` 的相应位置。至此，这个段描述符初始化全部完成。

---
分析 ③ ：这几句将 `GDT` 的物理地址填充到了 `GdtPtr` 这个6字节的数据结构中。
```handlebars
	; 为加载 GDTR 作准备
	xor	eax, eax
	mov	ax, ds
	shl	eax, 4
	add	eax, LABEL_GDT			; eax<-gdt基地址
	mov	dword [GdtPtr + 2], eax	; [GdtPtr + 2]<-gdt基地址,4个字节
```
**加载** `GdtPtr` 指示的6字节数据(包括GDT段限长和段基址)到 `GDTR` **全局描述符寄存器**中，`GDTR` 结构如图：
<img src="https://img-blog.csdnimg.cn/2020070400355693.png" width="50%">
```handlebars
	; 加载 GDTR
	lgdt [GdtPtr]
```
**关掉中断**。因为**保护模式下的中断机制和实模式不同**，更加复杂和强大，原有的中断向量表不再适用。而且保护模式下，BIOS中断都无法使用，它们是实模式下的代码。重新设置保护模式的中断模式之前，必须先关闭中断：
```handlebars
	; 关中断
	cli
```
**打开地址线A20**。原因在于：实模式下的程序只能够寻址1MB内存，它依赖于16位的段地址左移4位，加上16位的偏移地址访问内存。当**逻辑段地址**达到最大值 `0xFFFFF` 时再加1左移4位，超出了 `20` 位的范畴，进位自然丢失，回到最低地址 `0x00000` 。

后来，到了80286时代，处理器有24根地址线，为了能够在80286机器上运行8086程序而不出错，就用8042键盘控制器强制第21根线 `A20` 为0的做法。下面的代码打开A20，使其为 `1` 。
```perl
	; 打开地址线A20
	in	al, 92h			; 操作端口92
	or	al, 00000010b
	out	92h, al
```
**切换到保护模式**。CR0是处理器内部的控制寄存器，结构如下。打开第0位保护模式允许位 `PE` 使其为 `1` ，系统就运行在保护模式之下了：
<img src="https://img-blog.csdnimg.cn/2020070416533773.png" width="50%">
```handlebars
	; 准备切换到保护模式
	mov	eax, cr0
	or	eax, 1
	mov	cr0, eax
```

**跳转，正式运行保护模式代码**。前面的cs仍然是实模式下16位代码段的值，我们需要装入32位代码段的选择子，用下面的 `jmp` 就可以做到这一点。**这无疑是革命性的一跃**！
```handlebars
	; 真正进入保护模式, 必须加dword, 防止目标地址被截断
	jmp	dword SelectorCode32:0	; 执行这一句会把SelectorCode32装入cs,
								; 并跳转到Code32Selector:0处
; END of [SECTION .s16]
```
总结上述过程，**进入保护模式**的主要步骤：
1. 准备GDT中的描述符、GdtLen、GdtPtr、选择子；
2. 用lgdt加载gdtr；
3. 关中断，打开A20；
4. 置CR0位的PE位；
5. 跳转，进入保护模式。


---


## 2. 保护模式的运行环境
我们把 `pmtest1.bin` 写到了引导扇区运行，不过**引导扇区空间有限**，必须想个更好的方法。

一种是**让引导扇区读取我们的代码并执行**，就像一个操作系统内核，不过这有点难。另外的方法是**把程序编译为COM文件，然后让DOS执行它**。
1. 在网站 <a href="http://bochs.sourceforge.net/guestos/freedos-img.tar.gz">http://bochs.sourceforge.net/guestos/freedos-img.tar.gz</a> 上下载 `FreeDos` ：
2. 采用 `tar vxzf   FreeDos.img.tar.gz ` 解压；
3.  进入文件夹 `freedos-img` ，之后将 `a.img` 的文件重命名为`freedos.img` ，将其复制到 `bochs` 工作的当前文件夹；
4. 采用上一章的方法用 `bximage`  生成一个新的软盘映像，起名为 `pm.img` ，步骤同上次一样，唯一不同的就是上次默认生成的文件名 `a.img` 直接回车了，这次需要输入 `pm.img` ，此时当前工作目录下多了一个 `pm.img` 文件；
5. 修改当前工作目录下的 `bochsrc` 配置文件，增加下面几句话：
	```powershell
	floppya: 1_44="freedos.img", status=inserted
	floppyb: 1_44="pm.img", status=inserted
	boot: a
	```

6. 输入 `bochs  -f  bochsrc` ，启动 `bochs` ，选择 `[6]` ，输入 `c` ，回车：
<img src="https://img-blog.csdnimg.cn/20200704224238964.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="54%">
7. 在 `Bochs` 中，待 `FreeDos` 启动后，使用 `format b:` 格式化 `B:` 盘：
<img src="https://img-blog.csdnimg.cn/20200704224506968.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="54%">
出现了这一句话，说明之前已经格式化过一次了。所以重新再来一次 `format b:` 。得到下来的画面：
<img src="https://img-blog.csdnimg.cn/20200704225723468.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="54%">

8. 把前面的代码 `pmtest1.asm` 复制一份为 `pmtest1b.asm` ，将其中的 `0x7c00` 改为 `0100h` ，重新编译为 `pmtest1b.com` ：
   ```shell
   nasm pmtest1b.asm -o pmtest1b.com
   ```
  
9.  将 `pmtest1b.com` 复制到虚拟软盘 `pm.img` 上：
	```shell
    sudo mount -o loop pm.img /mnt/floppy
    sudo cp pmtest1b.com /mnt/floppy/
    sudo umount /mnt/floppy
    ```
    发现提示 `mount：挂载点/mnt/floppy不存在` ，因此先要在 `/mnt` 下创建一个 `floppy` 目录，然后重新执行上述命令。
10. 在 `FreeDos` 中下达 `B:\pmtest1b.com` ，如图，右边出现了一个红色的字母 `P` ：(不小心关掉了，这里是重新启动的FreeDos)
<img src="https://img-blog.csdnimg.cn/20200704231622460.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="54%">


---

## 3. GDT(Global Descriptor Table)
### (1) 32位PC机的工作模式
以下是《微机原理与接口技术》课程中提到的内容：
> `IA32` 下，CPU有多种工作模式：
① 实模式(`Real-Addressed Mode`) 
<img src="https://img-blog.csdnimg.cn/20200703233529190.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="52%">
② 保护模式(`Protected Mode`) ：应该是不支持并行的多任务。
<img src="https://img-blog.csdnimg.cn/20200703233826597.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="52%">
③ 虚拟86模式(`Virtual 86 Mode`) 
<img src="https://img-blog.csdnimg.cn/20200703234142941.png" width="52%">
④ 系统管理模式(`System Management Mode`)
<img src="https://img-blog.csdnimg.cn/20200703234226945.png" width="52%">
四种模式的关系如下：
<img src="https://img-blog.csdnimg.cn/2020070323444874.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="51%">
我们关注的主要是前两种。PC刚加电打开或系统复位后，工作在实模式下，它为保护模式所需的数据结构做好各种配置和准备。之后，修改控制寄存器CR0中的保护模式允许位PE，使得 `PE=1` ，从而让CPU进入保护模式；当 `PE=0` 时则返回实模式。

上面提到的16位到32位的革命性转换，就是代码中从16位跳转到32位代码段的那个历史性的 `jmp` 。

---
### (2) 从实模式到保护模式
实模式中，`8086` 为16位的CPU、寄存器、数据总线和20位的地址总线(1MB的寻址能力)，一个逻辑地址由段(16位)和偏移(16位)两部分组成，**段地址是地址的一部分，表示以xxxx0h开始的一段内存**，物理地址=段基地址*16+偏移地址。

但是到了32位时代，寻址空间到了4GB，原来的16位寄存器已经不够用了。为此，我们需要保护模式，**目的之一**就是提供更大的寻址能力。

32位时代的地址仍然可以用段值:偏移来表示，**只是段的概念发生了根本性的变化**，虽然段值仍然由原来16位的 `cs,ds` 等段寄存器表示，但是它们已经**变成了一个索引**，指向数据结构GDT的一个表项，表项中详细定义了**段的起始地址、界限、属性**等内容，表项的名字是**描述符**(`Descriptor`)。

即，GDT的作用是提供段式存储机制，这种机制由**段寄存器+GDT中的描述符**共同构成。


---
### (3) 描述符、选择子结构和寻址方式
下面是**代码段和数据段描述符**的结构图：
<img src="https://img-blog.csdnimg.cn/20200704160305453.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="54%">
此外，还有系统段描述符和门描述符。

本节代码 `pmtest1.asm` 中GDT段定义了三个描述符，可以分别称为 `DESC_DUMMY, DESC_CODE32, DESC_VIDEO` 。GDT中每一个描述符都定义了一个段，其中 `DESC_VIDEO` 指向的是显存。

它们如何和16位的 `cs,ds,es,gs` 等段寄存器起来，**使这些段寄存器成为**相对于GDT的一个**索引**呢？在 `[SECTION .s32]` 中有这样的代码：
```handlebars
mov ax, SelectorVideo
mov gs, ax
```
在前面的GDT段中，定义了 `SelectorVideo` ：
```handlebars
; GDT 选择子
SelectorCode32	equ	LABEL_DESC_CODE32 - LABEL_GDT
SelectorVideo	equ	LABEL_DESC_VIDEO - LABEL_GDT
```
由此，段寄存器 `gs` 的值变成了 `SelectorVideo` 标号地址，`SelectorVideo` 则似乎是 `DESC_VIDEO` 相对于GDT段基址 `LABEL_GDT` 的一个偏移，即**选择子**。当然，选择子不完全是偏移，其结构如下：
<img src="https://img-blog.csdnimg.cn/20200704161815310.png" width="50%">

当最低的三位 `TL,RPL` 都为零时，选择子真正成为对应描述符相对于GDT段基址的偏移。

这样，我们明白了这些代码的意义，`gs` 段寄存器值为选择子 `SelectorVideo` ，它指向GDT中对应显存的描述符 `DESC_VIDEO` ，然后下面的32位代码段，将 `ax` 的值写入到显存中**偏移位** `edi` 的位置（段:偏移中，偏移地址的概念没有变化）。
```clike
LABEL_SEG_CODE32:
	mov	ax, SelectorVideo
	mov	gs, ax					; 视频段选择子(目的)

	mov	edi, (80 * 11 + 79) * 2	; 屏幕第 11 行, 第 79 列。
	mov	ah, 0Ch					; 0000: 黑底    1100: 红字
	mov	al, 'P'
	mov	[gs:edi], ax
	...
```
从 `s32` 这部分代码，目前我们知道的段式寻址方式如下，**逻辑地址**(段:偏移)经过段机制(段选择子和段描述符)变成**线性地址**(`Linear Address`)，这里的线性地址可以看做是“物理地址”：
<img src="https://img-blog.csdnimg.cn/20200704162606555.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="54%">
## 4. 描述符属性
下面详细介绍段描述符的几个属性：
- `P` 位 (`Present`) 存在位，为 `1` 表示段存在于内存中，否则段不在内存中；
- `DPL` (`Descriptor Privilege Level`) 描述符特权级位， `0~3` ，数字越小特权级越大；
- `S` 位指明描述符是数据段/代码段(`S=1`)，还是系统段/门描述符(`S=0`) ；
- `TYPE` 描述符，`0~15`：
<img src="https://img-blog.csdnimg.cn/20200704171425171.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="44%">
- `G` 位(`Granularity`)段界限粒度位，当 `G=0` 时段界限粒度为字节，否则为 `4KB` ；
- `D/B` 位：
	- **可执行代码段**描述符中，是 `D` 位，`D=1` 时指令默认使用32位地址及32位/8位操作数；`D=0` 时默认使用16位地址及16位/8位操作数；
	- **向下扩展数据段**描述符中，是 `B` 位，`B=1` 时段的上部界限是 `4GB` ；否则是 `64KB` ；
	- **堆栈段**时，`B=1` 时隐式堆栈访问指令(如 `push,pop,call` )使用32位堆栈指针寄存器 `esp` ；`B=0` 时隐式堆栈访问指令使用16位堆栈指针寄存器 `sp` 。
- `AVL` 保留位，可以被系统软件使用。

这里面，最难理解的是**一致代码段** `Conforming Code Segment`：
- 一致：向特权级更高的**一致代码段**转移时，当前特权级会延续下去；而向特权级更高的**非一致代码段**转移时会报错(`general-protection exception`，常规保护错误)，**除非使用调用门或者任务门**。

	 如果系统代码不访问**受保护的资源和某些类型的异常处理**，可以放入一致代码段中，此时**低特权级的程序可以访问高特权级的一致代码段**；<b><font color="red">为了防止低特权级的程序访问，需要保护的系统代码则应该放入非一致代码段</font></b>；
- <b><font color="blue">目标代码是低特权级，则无论其是否是一致代码段，都不能通过jmp或call转移访问</font></b>。这样也会导致常规保护错误；
- <b><font color="gree">相同特权级的代码，可以直接访问，无论是否是一致代码段。</font></b>
- 特别注意的是，<b><font color="red">所有数据段都是非一致的，即不可能被低特权级的代码访问；但是它可以被更高特权级和同特权级的代码访问，不用使用特定的门</font></b>。
<img src="https://img-blog.csdnimg.cn/202007041743274.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="54%">

下面是节选自 `chapter3/a/pm.inc` 代码的描述符类型定义：
```perl
; 描述符图示 ...
;----------------------------------------------------------------------------
; 在下列类型值命名中：
;       DA_  : Descriptor Attribute
;       D    : 数据段
;       C    : 代码段
;       S    : 系统段
;       R    : 只读
;       RW   : 读写
;       A    : 已访问
;       other: 可按照字面意思理解
;----------------------------------------------------------------------------
; 描述符类型
DA_32		EQU	4000h	; 32 位段
DA_DPL0		EQU	  00h	; DPL = 0
DA_DPL1		EQU	  20h	; DPL = 1
DA_DPL2		EQU	  40h	; DPL = 2
DA_DPL3		EQU	  60h	; DPL = 3
; 存储段描述符类型
DA_DR		EQU	90h	; 存在的只读数据段类型值
DA_DRW		EQU	92h	; 存在的可读写数据段属性值
DA_DRWA		EQU	93h	; 存在的已访问可读写数据段类型值
DA_C		EQU	98h	; 存在的只执行代码段属性值
DA_CR		EQU	9Ah	; 存在的可执行可读代码段属性值
DA_CCO		EQU	9Ch	; 存在的只执行一致代码段属性值
DA_CCOR		EQU	9Eh	; 存在的可执行可读一致代码段属性值
; 系统段描述符类型
DA_LDT		EQU	  82h	; 局部描述符表段类型值
DA_TaskGate	EQU	  85h	; 任务门类型值
DA_386TSS	EQU	  89h	; 可用 386 任务状态段类型值
DA_386CGate	EQU	  8Ch	; 386 调用门类型值
DA_386IGate	EQU	  8Eh	; 386 中断门类型值
DA_386TGate	EQU	  8Fh	; 386 陷阱门类型值
```
 
