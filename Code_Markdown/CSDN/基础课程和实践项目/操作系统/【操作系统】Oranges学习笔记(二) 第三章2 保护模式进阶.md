@[toc]

---


从汇编源码编译成执行文件，**所有变量和地址都是按照线性地址编译出来的**。虽然实模式和保护模式的寻址方法不一样，但是**只要他们计算的线性地址结果是一样的，那获取的内容也就是一样的**。保护模式的优势就是**通过分段的方法（即段式存储机制）与汇编源码的SECTION可以对应起来**，从汇编源码的编写角度来看就简单了很多。

从汇编源码生成的执行程序是一个纯二进制文件，就是程序执行的内存影像和二进制文件是一样的。并且**执行程序文件的汇编指令是按照线性地址一直排下去的**。执行程序**在内存的起始线性地址是按照约定放置的**，例如com文件起始地址是0100h。执行程序的数据段地址可以是通过源码设置的。


汇编程序可以**选择2种执行方式**：
1. 在虚拟机直接执行：按照起始地址 `org    07c00h` 编译成二进制程序，复制到引导软盘下，在虚拟机下开机执行:
	```c
	nasm pmtest1.asm -o pmtest1.bin
	dd  if=pmtest1.bin  of=a.img  bs=512  count=1  conv=notrunc
	```

2. 在DOS环境下执行：按照起始地址 `org    0100h` 编译成二进制程序，直接在DOS下执行即可。
	```c
	nasm pmtest1b.asm -o pmtest1.com 
	```

---
# 3.2 保护模式进阶
为了充分利用保护模式带给我们的便利——更大的寻址空间，我们做了如下的实验。


## 1. 体会超过1MB内存的访问
这一节中我们将体验更大的内存：
- 在前面的基础上，新建一个段，以 `5MB` 为基址，远超出实模式下 `1MB` 的界限。
- 然后，我们读取开始处 `8` 字节的内容，再写入一个字符串(已经保存在一个**新增的数据段**中)，最后从中读出 `8` 字节。
- 如果成功的话，**两次读出的内容应该是不同的**；第二次读出的应该是我们写入的字符串。

首先是GDT段的定义 (`chapter3/b/pmtest2.asm`) ：
```clike
; ==========================================
; pmtest2.asm
; 编译方法：nasm pmtest2.asm -o pmtest2.com
; ==========================================
%include	"pm.inc"	; 常量, 宏, 以及一些说明

org	0100h
jmp	LABEL_BEGIN

[SECTION .gdt]
; GDT
;                           段基址,         段界限, 属性
LABEL_GDT:         Descriptor    0,              0, 0            ; 空描述符
LABEL_DESC_NORMAL: Descriptor    0,         0ffffh, DA_DRW       ; Normal 描述符
LABEL_DESC_CODE32: Descriptor    0, SegCode32Len-1, DA_C+DA_32   ; 非一致代码段, 32
LABEL_DESC_CODE16: Descriptor    0,         0ffffh, DA_C         ; 非一致代码段, 16
LABEL_DESC_DATA:   Descriptor    0,      DataLen-1, DA_DRW    	 ; Data
LABEL_DESC_STACK:  Descriptor    0,     TopOfStack, DA_DRWA+DA_32; Stack, 32 位
LABEL_DESC_TEST:   Descriptor 0500000h,     0ffffh, DA_DRW
LABEL_DESC_VIDEO:  Descriptor  0B8000h,     0ffffh, DA_DRW       ; 显存首地址
; GDT 结束

GdtLen	equ	$ - LABEL_GDT ; GDT长度
GdtPtr	dw	GdtLen - 1	  ; GDT界限
		dd	0			  ; GDT基地址

; GDT 选择子
SelectorNormal		equ	LABEL_DESC_NORMAL	- LABEL_GDT
SelectorCode32		equ	LABEL_DESC_CODE32	- LABEL_GDT
SelectorCode16		equ	LABEL_DESC_CODE16	- LABEL_GDT
SelectorData		equ	LABEL_DESC_DATA		- LABEL_GDT
SelectorStack		equ	LABEL_DESC_STACK	- LABEL_GDT
SelectorTest		equ	LABEL_DESC_TEST		- LABEL_GDT
SelectorVideo		equ	LABEL_DESC_VIDEO	- LABEL_GDT
; END of [SECTION .gdt]
```
其中，有一部分需要说明：
- `LABEL_DESC_CODE32` ：`Descriptor` 为32位代码段(保护模式) `[SECTION .32]` 的描述符；
- `LABEL_DESC_DATA` ：`Descriptor` 是新增的数据段 `[SECTION .data1]` 的描述符；数据段
- `LABEL_DESC_STACK` ：`Descriptor` 是全局堆栈段 `[SECTION .gs]` 的描述符；
- `LABEL_DESC_TEST` ：`Descriptor` 是用来测试大内存和数据读写的段的描述符；

然后是两个新的段，数据段和全局堆栈段：
```clike
; 数据段
[SECTION .data1]	 
ALIGN	32
[BITS	32]
LABEL_DATA:
SPValueInRealMode	dw	0
; 字符串
PMMessage:		db	"In Protect Mode now. ^-^", 0	; 在保护模式中显示
OffsetPMMessage	equ	PMMessage - $$
StrTest:		db	"ABCDEFGHIJKLMNOPQRSTUVWXYZ", 0
OffsetStrTest	equ	StrTest - $$
DataLen			equ	$ - LABEL_DATA
; END of [SECTION .data1]

; 全局堆栈段
[SECTION .gs]
ALIGN	32
[BITS	32]
LABEL_STACK:
	times 512 db 0
	TopOfStack	equ	$ - LABEL_STACK - 1
; END of [SECTION .gs]
```
接着是32位代码段 `[SECTION .s32]` ，它做了如下操作：
- 初始化 `ds,es,gs,ss` ，让 `ds` 指向新增的数据段(作为选择子)，`es` 指向测试段(作为选择子)，`gs` 指向显存段(作为选择子)；
- 保护模式中我们用到了堆栈，则需要建立堆栈段；其描述符有 `DA_32` 的属性，表示是32位的堆栈段。同时，还设置了 `ss` 指向堆栈段(作为选择子)，`esp` 作为栈底指针。**所有堆栈操作都会在新增的堆栈段中进行**；
- 接着显示一行黑底红字的字符串；
- 显示完毕后，开始读写大内存：读内存的过程为 `TestRead` ，写内存的过程为 `TestWrite` ；
- 读内存的过程中，调用了 `DispAL, DispReturn` 两个函数，前者以十六进制数和红字显示 `al` 中的内容，**后者模拟回车换行，让下一个字符显示在下一行的开头**。

```clike
[SECTION .s32]; 32 位代码段. 由实模式跳入.
[BITS	32]

LABEL_SEG_CODE32:
	mov	ax, SelectorData
	mov	ds, ax			; 数据段选择子
	mov	ax, SelectorTest
	mov	es, ax			; 测试段选择子
	mov	ax, SelectorVideo
	mov	gs, ax			; 视频段选择子

	mov	ax, SelectorStack
	mov	ss, ax			; 堆栈段选择子
	mov	esp, TopOfStack

	; 下面显示一个字符串
	mov	ah, 0Ch			; 0000: 黑底    1100: 红字
	xor	esi, esi
	xor	edi, edi
	mov	esi, OffsetPMMessage	; 源数据偏移
	mov	edi, (80 * 10 + 0) * 2	; 目的数据偏移,屏幕第 10 行, 第 0 列。
	cld
.1:
	lodsb
	test	al, al
	jz	.2
	mov	[gs:edi], ax
	add	edi, 2
	jmp	.1
.2:	; 显示完毕
	call	DispReturn
	call	TestRead
	call	TestWrite
	call	TestRead

	; 到此停止
	jmp	SelectorCode16:0
; ------------------------------------------------------------------------
TestRead:
	xor	esi, esi
	mov	ecx, 8
.loop:
	mov	al, [es:esi]
	call	DispAL
	inc	esi
	loop	.loop

	call	DispReturn

	ret
; TestRead 结束-----------------------------------------------------------
; ------------------------------------------------------------------------
TestWrite:
	push	esi
	push	edi
	xor	esi, esi
	xor	edi, edi
	mov	esi, OffsetStrTest	; 源数据偏移
	cld
.1:
	lodsb
	test	al, al
	jz	.2
	mov	[es:edi], al
	inc	edi
	jmp	.1
.2:

	pop	edi
	pop	esi

	ret
; TestWrite 结束----------------------------------------------------------
; ------------------------------------------------------------------------
; 显示 AL 中的数字
; 默认地:
;	数字已经存在 AL 中
;	edi 始终指向要显示的下一个字符的位置
; 被改变的寄存器:
;	ax, edi
; ------------------------------------------------------------------------
DispAL:
	push	ecx
	push	edx

	mov	ah, 0Ch			; 0000: 黑底    1100: 红字
	mov	dl, al
	shr	al, 4
	mov	ecx, 2
.begin:
	and	al, 01111b
	cmp	al, 9
	ja	.1
	add	al, '0'
	jmp	.2
.1:
	sub	al, 0Ah
	add	al, 'A'
.2:
	mov	[gs:edi], ax
	add	edi, 2

	mov	al, dl
	loop	.begin
	add	edi, 2

	pop	edx
	pop	ecx

	ret
; DispAL 结束-------------------------------------------------------------


; ------------------------------------------------------------------------
DispReturn:
	push	eax
	push	ebx
	mov	eax, edi
	mov	bl, 160
	div	bl
	and	eax, 0FFh
	inc	eax
	mov	bl, 160
	mul	bl
	mov	edi, eax
	pop	ebx
	pop	eax

	ret
; DispReturn 结束---------------------------------------------------------

SegCode32Len	equ	$ - LABEL_SEG_CODE32
; END of [SECTION .s32]
```

**保护模式下字符串寻址**：写内存的过程中，使用了一个常量 `OffsetStrTest = StrTest - $$` ，等价于字符串 `StrTest` 相对于本节开始处 `LABEL_DATA` 处的偏移。

如果我们看下面初始化段描述符的过程，就会发现数据段的基址就是 `LABEL_DATA` 的物理地址，因此，`OffsetStrTest` **既是该字符串相对于** `LABEL_DATA` **的偏移，也是在数据段中的偏移**。<b><font color="red">保护模式下，我们使用的就是这个偏移而不再是实模式下的地址</font></b>。

```clike
[SECTION .s16]
[BITS	16]
LABEL_BEGIN:
	mov	ax, cs
	mov	ds, ax
	mov	es, ax
	mov	ss, ax
	mov	sp, 0100h

	mov	[LABEL_GO_BACK_TO_REAL+3], ax
	mov	[SPValueInRealMode], sp

	; 初始化 16 位代码段描述符
	mov	ax, cs
	movzx	eax, ax
	shl	eax, 4
	add	eax, LABEL_SEG_CODE16
	mov	word [LABEL_DESC_CODE16 + 2], ax
	shr	eax, 16
	mov	byte [LABEL_DESC_CODE16 + 4], al
	mov	byte [LABEL_DESC_CODE16 + 7], ah

	; 初始化 32 位代码段描述符
	xor	eax, eax
	mov	ax, cs
	shl	eax, 4
	add	eax, LABEL_SEG_CODE32
	mov	word [LABEL_DESC_CODE32 + 2], ax
	shr	eax, 16
	mov	byte [LABEL_DESC_CODE32 + 4], al
	mov	byte [LABEL_DESC_CODE32 + 7], ah

	; 初始化数据段描述符
	xor	eax, eax
	mov	ax, ds
	shl	eax, 4
	add	eax, LABEL_DATA
	mov	word [LABEL_DESC_DATA + 2], ax
	shr	eax, 16
	mov	byte [LABEL_DESC_DATA + 4], al
	mov	byte [LABEL_DESC_DATA + 7], ah

	; 初始化堆栈段描述符
	xor	eax, eax
	mov	ax, ds
	shl	eax, 4
	add	eax, LABEL_STACK
	mov	word [LABEL_DESC_STACK + 2], ax ;设置段界限
	shr	eax, 16
	mov	byte [LABEL_DESC_STACK + 4], al
	mov	byte [LABEL_DESC_STACK + 7], ah

	; 为加载 GDTR 作准备
	xor	eax, eax
	mov	ax, ds
	shl	eax, 4
	add	eax, LABEL_GDT		; eax <- gdt 基地址
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
	jmp	dword SelectorCode32:0	; 执行这一句会把 SelectorCode32 装入 cs, 并跳转到 Code32Selector:0  处

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

LABEL_REAL_ENTRY:		; 从保护模式跳回到实模式就到了这里
  ; ...从保护模式回到实模式, 具体代码见下文
; END of [SECTION .s16]
```

---
## 2. 从保护模式到实模式
`chapter3/b/pmtest2.asm` 中从实模式到保护模式，需要初始化GDT中的描述符，准备GdtPtr和加载GDTR，关中断，打开A20，修改CR0的PE位，最后一个跳转就可以了。

而从保护模式返回实模式，也需要做许多工作：
- 需要**加载一个合适的描述符的选择子到段寄存器**，以包含合适的段界限和属性；
- 另外，不能从32位代码段中返回实模式，**只能从16位代码段中返回**。因为无法实现从32位代码段返回时，cs高速缓冲寄存器中的属性符合实模式的要求（实模式不能改变段属性）；
- 结果是新增了 `LABEL_DESC_NORMAL` 的描述符，对应段 `[SECTION .s16code]` ，返回实模式之前将该段对应的选择子 `SelectNormal` 加载到 `ds,es,fs,gs,ss` 。
- 然后，关闭CR0的 `PE` 位，进行跳转。

下面的16位代码段，是由 `[SECTION .s32]` 最后一句 `jmp SelectorCode16:0` 跳转过来的：
```handlebars
; 16 位代码段. 由 32 位代码段跳入, 跳出后到实模式
[SECTION .s16code]
ALIGN	32
[BITS	16]
LABEL_SEG_CODE16:
	; 跳回实模式:
	mov	ax, SelectorNormal
	mov	ds, ax ;ds=es=fs=gs=ss=SelectorNormal
	mov	es, ax
	mov	fs, ax
	mov	gs, ax
	mov	ss, ax
	; 关闭CR0的PE位
	mov	eax, cr0
	and	al, 11111110b
	mov	cr0, eax

LABEL_GO_BACK_TO_REAL: 		; 跳转,段地址是0
	jmp	0:LABEL_REAL_ENTRY	; 段地址会在程序开始处被设置成正确的值

Code16Len	equ	$ - LABEL_SEG_CODE16
; END of [SECTION .s16code]
```
最后的跳转看起来不太对劲，段地址是0 ？这里需要看前面，最开始实模式的代码段 `[SECTION .s16]` ，有下面几句话：
```cpp
mov ax, cs
...
mov	[LABEL_GO_BACK_TO_REAL+3], ax ; 为回到实模式的跳转指令指定正确的段地址
mov	[SPValueInRealMode], sp		  ; 保存以在恢复到实模式后重新设定sp的值
...
```
由于实模式下的 `cs` 代码段地址是程序自动填写的，我们保存到了 `ax` 中，然后存入 `LABEL_GO_BACK_TO_REAL+3` 处，此后该处就是实模式下代码Segment的地址。然后前面的JMP变成了：
```cpp 
jmp	cs_real_mode:LABEL_REAL_ENTRY	
```
从而跳转回到开始的**从实模式进入保护模式**的那个16位代码段 `[SECTION .s16]` 中的 `LABEL_REAL_ENTRY` 处，*从哪里开始，从哪里结束*。

再之后，和实模式=>保护模式相反，我们需要重新设定各个段寄存器的值，恢复 `sp` 的值，关闭A20，打开中断：
```handlebars
LABEL_REAL_ENTRY:		; 从保护模式跳回到实模式就到了这里
	mov	ax, cs
	mov	ds, ax
	mov	es, ax
	mov	ss, ax

	mov	sp, [SPValueInRealMode] ; 重新设定sp的值

	in	al, 92h		 
	and	al, 11111101b	; 关闭 A20 地址线
	out	92h, al		 

	sti					; 开中断

	mov	ax, 4c00h	   
	int	21h				; 回到DOS
; END of [SECTION .s16] 
```

---

实际操作。编译这个程序：
```shell
nasm pmtest2.asm -o pmtest2.com
```
然后用 `保护模式的运行环境` 中的过程得到：
```shell 
b:\pmtest2.com
```
<img src="https://img-blog.csdnimg.cn/20200704234019461.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="54%">

从图中，我们可以看到，程序打印出了“In Protect Mode now.”，后面第二行全是零，说明内存地址 `5MB` 处都是零；下一行变成 `41 42 ...` ，说明**写操作成功**——这些数字正是 `ABCDE...` 的十六进制形式。

最后，程序结束后不再死循环，而是重新出现了 `DOS` 提示符，**我们回到了实模式下的DOS**。
 


---
## 3. LDT(Local Descriptor Table)
同GDT一样，LDT简单来说也是一种描述符表，只不过它的选择子的 `TI` 位必须置为 `1` 。

### (1) 代码和分析
下面的代码来自 `chapter3/c/pmtest3.asm` ：
- 增加了两个新的节，一个是新的描述符表LDT `[SECTION .ldt]` ，其中有一个描述符对应 `[SECTION .la]` ；`[SECTION .la]` 是该LDT的代码段，在GDT中无定义；
- `[SECTION .ldt]` 在GDT中有对应的描述符 `LABEL_DESC_LDT` 和选择子，以及描述符的初始化代码；
- `[SECTION .la]` 中将打印字符 `L` ，实现时调用了GDT中的 `SelectorVideo` ；
![在这里插入图片描述](https://img-blog.csdnimg.cn/2020071117543229.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)![在这里插入图片描述](https://img-blog.csdnimg.cn/20200711175446977.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)![在这里插入图片描述](https://img-blog.csdnimg.cn/20200711175502819.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
 
在运用LDT时，需要先用 `lldt` 指令加载 `ldtr` ，`lldt` 的操作数是 `Selector` ，对应的是 `GDT` 中用来描述 `LDT` 的描述符 `LABEL_DESC_LDT`  (`lgdt	[GdtPtr]` 加载 `gdtr` ，其操作数是一个 `GdtPtr` 的数据结构)。**也就等同于LDT是GDT中描述的一个段，对应特别的寄存器ldtr，该段中又有描述符描述一些LDT段，只属于这个LDT段**。
 

另外，此处的LDT有一个描述符 `LABEL_LDT_DESC_CODEA` ，和GDT中的描述符没有区别；**但是选择子却有不同**，`SelectorLDTCodeA` 多了一个属性 `SA_TIL` —— 定义在 `pm.inc` ，`SA_TIL EQU 4` 。加上这个属性，会使得选择子 `SelectorLDTCodeA` 的 `TI` 位为 $1$ 。
```handlebars
jmp SelectorLDTCodeA:0  
```
上一句中，系统将从LDT中寻找相应描述符，并跳转到对应的LDT段中。完成了显示字符的任务后，就会 `jmp SelectorCode16:0` 回到GDT中描述的16位代码段，然后返回实模式。
 

---
### (2) 实际运行
我们发现，如果什么时候都要自己编译链接文件，输入命令，……太麻烦了。所以接下来，都会使用Makefile来完成大部分实际的运行过程。下面是源代码中给出的Makefile文件：
```shell
##################################################
# Makefile of pmtestx.asm (x=[1,2,3...])
##################################################
# 简单赋值定义了两个变量SRC,BIN
SRC := pmtest2.asm             
BIN := $(subst .asm, .com, $(SRC)) # 字符串替换函数,将字符串$(SRC)中的.asm替换成.com

# 用.PHONY定义了一个伪目标
.PHONY : everything
# make everything 依赖于 $(BIN)
everything : $(BIN)
	sudo mount -o loop pm.img /mnt/floppy/
	sudo cp $(BIN) /mnt/floppy/ -v
	sudo umount /mnt/floppy/
# $(BIN)是pmtest2.com, 依赖于pmtest2.asm,
# 执行编译命令nasm pmtest2.asm -o pmtest2.com
$(BIN) : $(SRC)
	nasm $< -o $@
```
执行上面的Makefile文件，就能一次完成编译出 `pmtest2.com` 然后复制到 `pm.img` 中的所有工作。我第一次执行时结果如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2020071223333753.png)
虽然编译出了 `pmtest3.com` ，但是后面的执行出了错。可能是没有遵照书上的步骤先 `bochs` 然后格式化B盘的原因。为此，我先进入Bochs然后 `format B:` 。接着执行 `make` ：
![在这里插入图片描述](https://img-blog.csdnimg.cn/202007122346129.png)
成功了。然后在Bochs中 `B:\pmtest3.com` 执行程序，完美：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200712234810848.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

---
### (3) 本节总结

上例的LDT很简单，只有一个代码段。我们完全可以在其中增加更多的段，比如数据段、堆栈段等，**我们甚至可以把一个单独任务用到的所有东西都封装在一个LDT中**，这种思想是后面章节中的多任务处理的一个雏形。

增加一个用LDT描述的任务的整个步骤如下：
1. **增加一个32位的代码段**；本节代码中原本的那个32位的代码段 `[SECTION .32]` 用来从实模式跳入保护模式，然后从该段中跳入LDT代码段 `[SECTION .la]` ；如果有更多的任务，就需要增加新的LDT代码段；最后一个LDT代码段，负责跳回到GDT中描述的16位代码段，然后返回实模式。
2. **增加一个LDT段**，内容是LDT描述符表，可以有多个描述符描述多个段；注意，使用选择子的时候 `TI` 位为 $1$ ；
3. **在GDT中新增一个描述符，用来描述这个新的LDT**，同时定义其选择子；
4. **增加GDT中新的描述符的初始化代码**，主要用来设置段基址；
5. 用新加的LDT描述的局部任务准备完毕；
6. 先用 `lldt` 加载 `ldtr` ，用 `jmp` **指令跳转**等运行。

从这几个实验，我们对保护模式有了初步的认知：
- 描述符中的段基址、段界限定义了一个段的范围，禁止了越出段界限的访问，是对段的保护；
- 复杂的段属性，限制了段的行为和性质，也是保护。

 
---


## 4. 特权级概述
描述符属性中的 `DPL` (`Descriptor Privilege Level`)和选择子中的 `RPL` (`Requested Privilege Level`) 都是用来表示特权级的。前面所有代码都运行在最高特权级下——`DPL=RPL=0` 。

在 `IA32` 的段式内存机制中，特权级从高到低是 `0~3` ，如下图。核心代码和数据，被放置在高特权级中，用以**防止低特权级任务在不被允许的情况下访问高特权级的段**：
<img src="https://img-blog.csdnimg.cn/20200704181955919.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="54%">
### (1) CPL、DPL、RPL
通过识别 `CPL` (`Current Privilege Level`) 和 `DPL,RPL` 三种特权级，处理器进行特权级检验。

1.CPL **当前执行的程序或任务的特权级**：
-  它存储于 `CS和SS` 的第 `0,1` 两位上，通常情况下**CPL等于代码所在段的特权级**；
- **程序转移到不同特权级的代码段**时，CPL会被处理器改变；
- **转移到一致代码段时，CPL则会延续不变**，因为一致代码段可以被相同或低特权级的代码访问。

2.DPL(`Descriptor Privilege Level`) **段或者门的特权级**：
- 它被存储在 `段描述符或者门描述符` 的 `DPL` 字段中；
- **当前代码段**试图访问一个段或者门的时候，`DPL` 将会和 `CPL` 以及段/门选择子的 `RPL` 进行比较。访问的段或门类型的不同，DPL将会被区别对待：
	- **数据段**：`DPL` 规定了可以访问该段的*最低特权级*。比如某数据段 `DPL=1` ，则只有运行在 `CPL=0/1` 的程序可以访问它；
	- **非一致代码段**(不使用调用门的情况下)：`DPL` 规定访问此段的*特权级*。比如，一个非一致代码段的特权级是 `0` ，则只有 `CPL=0` 的程序可以访问它；
	- **调用门**：`DPL` 规定了当前执行的程序或任务可以访问此调用门的*最低特权级*；
	- **一致代码段**和**通过调用门访问的非一致代码段**：`DPL` 规定了访问此段的*最高特权级*，如果一致代码段的 `DPL` 是 `2` ，则 `CPL=0/1` 的程序将无法访问此段；
	- **TSS**：`DPL` 规定了访问此 `TSS` 的最低特权级。(`Task State Segment`)
- 总的来说，数据段、调用门、TSS三者的DPL规则是一致的。

3.RPL (`Requested Privilege Level`) 
- 它存在于**段选择子**的 `0,1` 位，根据代码中不同段的跳转来确定，以动态刷新 `CPL` ；
- 处理器通过检查 `RPL,CPL` 确认一个访问请求是否合法，不仅提出访问请求的段需要有足够的CPL特权级，RPL也要够高。**如果RPL>CPL(RPL特权级更低)，RPL对访问合法性其决定作用，反之亦然**；
- 操作系统用RPL**避免低特权级程序访问高特权级的数据**：
	- (操作系统过程)被调用过程从一个(应用程序)调用过程中接受到一个选择子时，会将选择子的RPL设置为调用者的特权级；
	- 然后，操作系统用这个选择子访问特殊的段时，处理器会用调用过程的RPL（已存储到CPL中），而**不是更高的操作系统过程的特权级CPL**进行特权检验。

### (2) 小实验
数据访问的特权级检验比较简单，只要 `CPL,RPL` 都小于被访问的数据段的  `DPL` 就可以了。我们可以把前面代码中的数据段描述符的 `DPL` 修改一下：
```handlebars
LABEL_DESC_DATA: Descriptor 0, DataLen - 1, DA_DRW + DA_DPL1
```
编译链接并运行，和原来一样。说明我们对这个段的数据访问是合法的。如果改变上面改过的数据段的选择子 `RPL` 为 $3$ :
```handlebars
SelectorData equ LABEL_DESC_DATA - LABEL_GDT + SA_RPL3
```
这次运行Bochs会崩溃。因为我们违反了特权级规则，用 `RPL=3` 访问 `DPL=1` 的段，引发异常；而且我们没有对应的异常处理程序，所以最严重的情况就发生了。

不过与书中不同的是，我没有看到 `load_seg_reg(DS)` 这样的字样，而是Bochs陷入停滞，然后可以在 `<bochs:1>` 中输入新的命令。

为了确认没有执行问题，我将上面的 `SelectorData` 改为了 `+ SA_RPL1` 。由于数据段的DPL规定了可以访问此段的最低特权级，因此现在的选择子应该可以访问该段。执行程序，可以运行：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200713000239389.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)



### (3) 不同特权级代码段之间的转移
这里，我们会看一下*不同特权级代码段之间的转移情况*。
- 从一个代码段转移到另一个代码段之前，目标代码段的选择子会被加载到 `cs` 中；
- 然后，处理器将检查段描述符的界限、类型、特权级等；
- 如果检验成功，`cs` 会被加载，程序控制权转移到新的代码段中，从 `eip` 指示的位置开始执行。

程序控制转移的发生，常常由 `jmp, call, ret, sysenter, sysexit, int n, iret` 引起，亦可能是**中断和异常处理机制**引起。其中，使用 `jmp,call` 可以实现4种转移：
1. 目标操作数**包含**目标代码段的段选择子；
2. 目标操作数**指向**一个包含目标代码段选择子的**调用门描述符**；
3. 目标操作数**指向**一个包含目标代码段选择子的**TSS**；
4. 目标操作数**指向**一个**任务门**，该任务门指向一个包含目标代码段选择子的**TSS**。

其中，第一种是通过 `jmp,call` 的**直接转移**，是一类；另外三种是通过某个描述符的**间接转移**，是第二类。下面将开始详细的阐述。

---

## 5. 特权级转移
### (1) 通过jmp或者call进行直接转移
通过前面的讨论，我们可以总结出下面的规则：
- 目标代码段是**非一致代码段**，则要求 `CPL` 必须等于目标段的 `DPL` ，同时 `RPL <= DPL` ；
- 目标代码段是**一致代码段**，则要求 `CPL` 必须小于等于目标段的 `DPL` ，`RPL` 不做检查。转移到目标段后，`CPL` 不会变成目标代码段的 `DPL` 。
- 这样，jmp和call进行的代码段间直接转移很有限：
	- 对于非一致代码段，**只能够在相同特权级代码段间转移**；
	- 对于一致代码段，**最多能从低到高**，而且 `CPL` 不会改变。
### (2) 调用门体验
门也是一种**描述符** `Gate Descriptor` ，其结构如下，和之前提到的描述符很不相同：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200713001239272.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
直观来看，一个门描述符定义了目标代码对应段的**一个选择子**、**入口地址偏移**指定的线性地址(程序通过这个地址进行转移)、一些属性，属性中 `BYTE5` 和以前的描述符完全相同，`S` 位固定为零。门描述符的类型有四种：
- 调用门 `Call Gates`
- 中断门 `Interrupt Gates` 
- 陷阱门 `Trap Gates`
- 任务门 `Task Gates`

中断门和陷阱门是特殊的，先不介绍，而是先介绍调用门。下面的例子用到调用门**但先不涉及特权级转换**。在 `pmtest3.asm` 的基础上增加一个代码段，**作为通过调用门转移的目标段**：(`ch3/d/pmtest4.asm`)
![在这里插入图片描述](https://img-blog.csdnimg.cn/2020071301374537.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
我们注意到，代码末尾是 `retf` 指令，因为我们要用call指令调用这个建立的调用门。下面加入代码段的描述符、选择子、及初始化这个描述符的代码：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200713111545273.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
调用门的代码，门描述符的属性为 `DA_386CGate` ，表明是一个调用门；选择子是 `SelectorCodeDest` ，表明目标代码段是刚刚添加的代码段；偏移地址是 `0` ，即跳转到目标段的开头；另外，`DPL=0` ：
```handlebars
; 门
LABEL_CALL_GATE_TEST: Gate SelectorCodeDest, 0, 0, DA_386CGate + DA_DPL0
```
用到的宏如下：(`ch3/d/pm.inc`)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200713112000587.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
调用门对应的选择子如下：
```swift
SelectorCallGateTest equ LABEL_CALL_GATE_TEST - LABEL_GDT
```
上面，我们完成了准备调用门的工作，门指向 `SelectorCodeDest:0` 即标号 `LABEL_SEG_CODE_DEST` 处的代码。然后，用call使用它：
```swift
	...
    ; 测试调用门(无特权级变换), 打印字符'C'
	call SelectorCallGateTest:0
	...
	jmp  SelectorLDTCodeA:0	; 跳入局部任务, 打印字符'L'
	...
```
由于新的代码段以 `retf` 结尾，所以代码会跳回到call指令的下一句继续执行。所以程序效果如下，在 `pmtest3.com` 的基础上多一个红字符 `C` ：
 ![在这里插入图片描述](https://img-blog.csdnimg.cn/20200713144638520.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
调用门本质上是一个入口地址，只是增加了一些属性罢了。上面的例子中调用门完全等同于一个地址，可以将**使用调用门进行跳转**的指令改为：
```swift
call SelectorCodeDest:0
```
运行一下，效果完全一样：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200713144815626.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

---
如果我们想要在不同的特权级代码间转移的话，还需要学习**使用调用门进行转移时特权级检验**的规则：
- 调用一个调用门G，从代码A转移到代码B(调用门G中目标选择子指向的段)，中间涉及到了 `CPL` 、`RPL` 、G的DPL `DPL_G` 、B的DPL `DPL_B` ；
- 代码A访问G调用门时，其规则等同于访问一个数据段，要求 `CPL,RPL <= DPL_G` 。即 `CPL,RPL` 要在更高的特权级上；
- 此外，系统还要比较 `CPL` 和 `DPL_B` 。如果是一致代码段，则 `DPL_B <= CPL` 即 `CPL` 特权级要么相等要么较低；如果是非一致代码段，则jmp和call有所不同，call时要求 `DPL_B <= CPL` ；jmp时只能是 `DPL_B = CPL` 。
- 也就是说，通过调用门和CALL，无论目标是一致还是非一致代码段，都可以**实现从低特权级到高特权级**的跨越。

总的来说，**调用门使用时特权级检验**规则如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200713143913154.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
在真正写程序实现一个特权级变换之前，我们发现，**<font color="red">调用门只能实现特权级从低到高的转移</font>**，但是现在的程序一直都是在最高特权级的！如何才能先降到低特权级呢？别急，除了严格的特权级检验外，我们还要学习的是——特权级变换时堆栈的变化情况。

---
### (3) 长短调用时的堆栈变化


首先，我们需要回忆起长跳转/调用 `far jmp/call` 和短跳转/调用 `near jmp/call` 的不同：
- 对于jmp来说，长跳转对应段间，短跳转对应段内，结果没什么不同；
- 对于call来说，由于**call指令会影响堆栈**，长短调用对堆栈的影响也不同。
	- 对于短调用，先是将参数依次入栈，call执行将下一条指令 `nop` 的地址——调用者`eip` 压入栈，对应下图的 `esp` (指向当前堆栈的栈顶)的变化：
	![在这里插入图片描述](https://img-blog.csdnimg.cn/20200713150000549.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)	
	然后，到ret执行时，这个 `eip` 会从堆栈被弹出，执行前后的 `esp` 变化如下：
	![在这里插入图片描述](https://img-blog.csdnimg.cn/20200713151158530.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
	- 长调用的情况类似，不过由于跨了段，因此在call指向时压入栈的不仅有 `eip` (下一条指令的地址)，还有 `cs` 
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200713151402894.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)	
	ret执行后返回，需要调用者的 `cs` 和 `eip` ，因此弹出两者：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200713151531685.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

---
### (4) 特权级变换时的堆栈变化
联系起通过调用门的转移，我们很容易想到，call一个调用门也是长调用。但是不同的是，特权级变化的时候，堆栈也要发生切换，即**call执行前后的堆栈不再是同一个**。

处理器的这种机制**避免了高特权级的过程由于栈空间不足而崩溃**；另外，如果不同特权级共享同一个堆栈的话，高特权级程序可能因此受到有意或无意的干扰。无疑，这也是一种**保护**。

但是这种变化也给我们带来了困扰，如果我们压入参数和返回时地址，需要使用的时候却发现堆栈已经变成了另外一个，该怎么办呢？Intel提供了一种机制——将堆栈A的内容复制到堆栈B中，如下图：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2020071315254220.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

上面仅仅涉及到两个堆栈，但是，由于每个任务最多可能在4个特权级间切换，因此每个任务实际需要4个堆栈。无奈我们只有一个 `ss` 和一个 `esp` ，如果发生堆栈切换，该从哪里得到其他堆栈的 `ss,esp` 呢？这里涉及到 `TSS` (`Task-State Segment`)，它是一个数据结构，包含多个字段。32位TSS如下图：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200713153737723.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
在这里，我们只关注偏移4到偏移27的3个 `ss` 和3个 `esp` 。当发生堆栈切换时，内层的 `ss` 和 `esp` 就是从这里取得的。如果我们从 `ring3->ring1` ，堆栈将自动切换到 `ss1,esp1` 指定的位置。由于**只是从低特权级到高特权级切换时**新堆栈才会从TSS中取得，所以TSS中没有位于最外层的 `ring3` (最低特权级) 的堆栈信息。

书上总结了CPU在整个转移过程中做的工作，即调用门从外层到内层的全过程：
1. 根据目标代码段的 `DP ` (新的 `CPL`) 从 `TSS` 中选择应该切换到哪个 `ss,esp` ；
2. 从 `TSS` 中读取新的 `ss,esp` 。如果发现 `ss,esp` 或者 `TSS` 界限错误都会报无效TSS异常(#TS)；
3. 对 `ss` 描述符进行检验，如果错误，同样发生 #TS 异常；
4. 暂时保存当前 `ss,esp` 的值；
5. 加载新的 `ss,esp` ；
6. 将刚刚保存下来的 `ss,esp` 的值压入新的 `ss,esp` 指向的新栈；
7. 从调用者堆栈中将参数复制到被调用者堆栈(新堆栈)中，复制参数的数目由调用门中 `ParamCount` 来决定，如果是零的话，不复制参数；`ParamCount` 有5个字节，最大可以表示31个参数，更大时需要**让其中的一个参数变成指向一个数据结构的指针**，或者**通过保存在新堆栈中的** `ss,esp` 访问旧堆栈中的参数；
8. 将当前的 `cs,eip` 压入新栈；
9. 加载调用门中指定的新的 `cs,eip` ，开始执行被调用者过程。

反过来，ret是call的反过程，只是**带参数的ret指令会同时释放事先被压栈的参数**。ret不仅可以实现长短返回，而且可以实现带有特权级变换的长返回。由被调用者到调用者的返回过程如下：
1. 检查保存的 `cs` 上的 `RPL` 以判断返回时是否需要变换特权级；
2. 加载被调用者堆栈上的 `cs, eip` ，进行代码段描述符和选择子类型与特权级检验；
3. 如果ret指令含有参数，则增加 `esp` 的值跳过参数。然后 `esp` 指向被保存过的调用者的 `ss, esp` 。注意，ret的参数必须对应调用门中的 `ParamCount` 的值；
4. 加载 `ss, esp` ，切换到调用者堆栈，被调用者的 `ss, esp` 被丢弃。这里将进行  `ss` 描述符、`esp` 以及 `ss` 段描述符的检验；
5. 如果ret指令含有参数，增加 `esp` 的值跳过参数，此时已经处于调用者堆栈中；
6. 检查 `ds, es, fs, gs` 的值，如果哪个寄存器指向的段的 `DPL  < CPL` ，则加载一个空描述符到对应寄存器。**此规则不适用于一致代码段**。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200713171632753.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

综上，使用调用门的过程分为：从低特权级到高特权级，通过调用门和call实现；另一部分是从高特权级到低特权级，通过ret指令实现。

---
### (5) 进入ring3
在ret执行前，堆栈中应该准备好了目标代码段的 `cs,eip` ，以及 `ss, esp` 和参数等。我们的例子中，ret前的参数如下：![在这里插入图片描述](https://img-blog.csdnimg.cn/2020071319074421.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
执行完ret之后，就可以**转移到低特权级代码中**了。在`pmtest4.asm` 基础上做一下修改，形成 `pmtest5a.asm` 。

首先添加一个 `ring3` 的代码段 `[SECTION .ring3]` 和一个 `ring3` 的堆栈段 `[SECTION .s3]` ，代码段很简单，同样是打印一个字符，会在"In Protect Mode now."下方显示。不过由于其运行在 `ring3` ，但是写显存要访问 `VIDEO` 段，为了避免错误，我们把 `VIDEO` 的 `DPL` 改为3。同时，新的代码段对应描述符的属性加上 `DA_DPL3` ，相应选择子的 `SA_RPL3` 也将 `RPL` 设为了3:
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200713191600966.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
这样，代码段和堆栈段都准备好了，现在将 `ss, esp, cs, eip` 依次压栈，执行 `retf` 指令：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200713192642131.png)

如果我们看到屏幕上出现红色的 `3` ，并停止不再返还DOS，说明转移成功。编译，运行：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200713193556288.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
这表明我们由ring0到ring3、从高特权级到低特权级的历史性转移完成！
### (6) 通过调用门进行有特权级变换的转移——实践
在 `[SECTION .ring3]` 中增加了使用调用门的代码，修改调用门的描述符和选择子使其满足 `CPL, RPL` 都小于等于 `DPL` 的条件：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200713201258113.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
接着，从低特权级到高特权级转移时，需要用到TSS，下面准备一个TSS：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200713212336409.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
添加初始化TSS描述符的代码后，开始加载TSS：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200713212501633.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
`pmtest5c.asm` 执行结果如下，出现数字 `3` 和字符 `C` ，说明在ring3下对调用门的使用也是成功的：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200713213917138.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

---
为了让我们的程序能够回到实模式，需要将调用局部任务的代码加入到调用门的目标代码 `[SECTION .sdest]` 中，程序从这里进入局部任务，然后由原路返回实模式：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200713214506461.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
`pmtest5.asm` 运行结果如下，程序各部分都输出了：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200713214745744.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

  
