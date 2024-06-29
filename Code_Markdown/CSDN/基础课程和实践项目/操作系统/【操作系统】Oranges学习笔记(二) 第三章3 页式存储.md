@[toc]

---
# 3.3 页式存储
常见的几个问题：
- 什么是"段"，什么是"页"：段是信息的逻辑单位，它含有一组其意义相对完整的信息，分段的目的是为了更好地满足用户的需要；页是一块内存，是信息的物理单位，分页是出于系统管理的需要，提高内存的利用率。页的大小在不同的机器上都不同，这里只讨论页大小为 `4KB` 的情况；
- 逻辑地址、线性地址、物理地址：**在未打开分页机制时，线性地址等同于物理地址**，即逻辑地址通过分段机制直接转换成物理地址；**分页开启后，分段机制将逻辑地址转换为线性地址**，线性地址通过分页机制转换为物理地址。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200713215936778.png)
- 为什么分页：分段机制已经提供了很好的保护机制，而分页机制的主要目的在于**实现虚拟存储器**，线性地址中任何一个页都可以映射到物理地址的任何一个页，使内存管理变得更灵活。

## 1. 分页机制概述
分页机制是一个函数，将线性地址映射到物理地址上，`物理地址 = f(线性地址)` 。分页机制中最关键的就是 `PDT` 页目录表和 `PT` 页表这两个数据结构。其中，页目录表的表项 `Page Directory Entry` 简称 `PDE` ，页表表项 `Page Table Entry` 简称 `PTE` 。

**分页机制使用页目录表和页表两级转换**，页目录表大小 `2^12=4KB` ，存储于**一个物理页**中，每个表项PDE长 `4Bytes` ，共有 `1024` 个PDE，每个PDE对应第二级的一个页表。第二级的每个页表也有 `1024` 个表项PTE，每个PTE**对应一个物理页**。因此，**存储页目录表和页表需要的内存为**：$2^{12} + 2^{10} * 2^{10} * 4 = \text{4KB + 4MB}$ 。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200713221710978.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

如上图所示，进行转换时，先是由寄存器 `CR3` 指定的页目录中根据线性地址的高 `10` 位得到页表地址。然后在页表中，根据线性地址的 `21~12` 位得到物理页首地址。最后，将首地址+线性地址第 `12` 位就得到了物理地址。

## 2. PDE和PTE
为了完全理解和使用分页机制，必须先了解PDE和PTE的结构：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200713231425886.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)![在这里插入图片描述](https://img-blog.csdnimg.cn/20200713231511854.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
各位解释如下：
- `P` 存在位，表示当前条目所指向的**页或页表是否在物理内存中**。`P=0` 表示页不在内存中，如果处理器试图访问此页，将会产生页异常（`page-fault exception, #PF`）；`P=1` 表示页在内存中。
- `R/W` 指定**一个页或者一组页**（比如，此条目是指向页表的页目录条目）**的读写权限**。此位与 `U/S` 位和寄存器 `cr0` 中的 `WP` 位相互作用。`R/W=0` 表示**只读**；`R/W=1` 表示**可读并可写**。
- `U/S` 指定**一个页或者一组页**（比如，此条目是指向页表的页目录条目）**的特权级**。此位与 `R/W` 位和寄存器 `cr0` 中的 `WP` 位相互作用。`U/S=0` 表示系统级别（`Supervisor Privilege Level`），如果 `CPL` 为 `0,1,2` ，那么它便是在此级别；`U/S=1` 表示用户级别（`User Privilege Level`），如果 `CPL` 为 `3` ，那么它便是在此级别。
	如果 `cr0` 中的 `WP` 位为 `0` ，那么即便用户级（`User P.L.`）页面的 `R/W=0` ，系统级（`Supervisor P.L.`）程序仍然具备写权限；如果 `WP` 位为 `1` ，那么系统级（`Supervisor P.L.`）程序也不能写入用户级（`User P.L.`）只读页。
- `PWT` 用于控制**对单个页或者页表的缓冲策略**。`PWT=0` 时使用 `Write-back` 缓冲策略；`PWT=1` 时使用`Write-through` 缓冲策略。当 `cr0` 寄存器的 `CD`（`Cache-Disable`）位被设置时会被忽略。
- `PCD` 用于控制**对单个页或者页表的缓冲使能**。`PCD=0` 时页或页表可以被缓冲；`PCD=1` 时页或页表不可以被缓冲。当 `cr0` 寄存器的 `CD`（`Cache-Disable`）位被设置时会被忽略。
- `A` 指示**页或页表是否被访问**。此位往往**在页或页表刚刚被加载到物理内存中时被内存管理程序清零**，处理器会在**第一次访问此页或页面时设置此位**。而且，处理器并不会自动清除此位，只有软件能清除它。
- `D` 指示**页或页表是否被写入**。此位往往在**页或页表刚刚被加载到物理内存中时被内存管理程序清零**，处理器会在**第一次写入此页或页面时设置此位**。而且，处理器并不会自动清除此位，只有软件能清除它。
`A` 位和 `D` 位**都是被内存管理程序用来管理页和页表**，决定其从物理内存中换入和换出的。
- `PS` **决定页大小**。`PS=0` 时页大小为 `4KB` ，`PDE` 指向页表。
- `PAT` 选择 `PAT`（`Page Attribute Table`）条目。`Pentium III` 以后的CPU开始支持此位，在此不予讨论，并在我们的程序中设为 `0` 。
- `G` **指示全局页**。如果此位被设置，同时 `cr4` 中的`PGE` 位被置位，那么**此页的页表或页目录条目不会在** `TLB` **中变得无效**，即便 `cr3` 被加载或者任务切换时也是如此。

另外，为了提高效率，处理器会将经常使用的页目录和页表项放在 `TLB` (`Translation Lookaside Buffer`)的缓冲区中。**只有TLB中找不到被请求页的转换信息后，才会到内存中寻找**。当页目录或者页表项被改变时，必须马上让TLB中对应的条目失效，以便下次用到时及时更新此条目。

**当CR3被加载时，过去的所有TLB都会自动失效**，除非页或页表条目被设为全局页（`G` 位被设置）。

## 3. CR3
CR3 又叫做 `PDBR` (`Page-Directory Base Register`) ，它指向页目录表，结构如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2020071323315548.png)
可见，CR3高20位是页目录表首地址的高20位，页目录表首地址的低12位是零，即页目录表是 `4KB` 对齐的。

类似的，PDE的页表基址 `Page-Table Base Address` 以及PTE中的页基址 `Page Base Address` 也是用高20位表示 `4KB` 对齐的页表和页。

另外，第 `3,4` 位的标志，暂时可以忽略。


---
## 4. 编写代码启动分页机制
为了开启分页机制，需要打开CR0的最高位PG位，如果 `PG=1` ，则开启分页。所以，当我们准备好了页目录表和页表，**将CR3指向页目录表后，需要置CR0的PG位**，就可以启动分页机制了。

下面的代码中，`PageDirBase, PageTblBase` 是两个宏，指定了页目录表和页表在内存中的位置，它们都是4KB对齐的，页目录表位于地址2MB处，有1024个表项，占4KB大小；接着就是页表，位于2MB+4KB处，假定有全部的页表1024个，每个页表占4KB=4096字节，就是4MB空间。总的来说，本程序需要至少6MB以上的内存，图示如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200715182148222.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
另外，我们定义了两个段，分别存放页目录表和页表，大小分别为4KB+4MB。程序将所有线性地址映射为相同的物理地址。于是，`物理地址 = f(线性地址) = 线性地址` ：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200715181222939.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
上面的程序先让 `es` 段寄存器存入段选择子对应到页目录表段，`edi=0` ，所以 `es:edi` 指向页目录表的开始，然后用 `stosd` 第一次将 `eax = PageTblBase | PG_P | PG_USU | PG_RWW` 存入页目录表的第一个 `PDE` ，将让这个PDE对应的页表首地址变为 `PageTblBase` ，属性显示其指向的是存在的 `P` 、可读可写的、用户基本的页表。然后循环，`es:edi` 自动执行下一个PDE，`eax += 4096` 和上一个页表首尾相连。

经过 `1024` 次循环后，页目录表中全部的PDE都初始化了，它们属性相同，并且**所有指向的页表都连续排列在以PageTblBase为首地址的4MB内存中** 。

接着，初始化所有页表中的PTE，由于总共有 $1024^2$ 个PTE，于是 `ecx=1024*1024` 。对 `es, edi` 的处理使其指向页表段的首地址 `PageTblBase` 处。然后，第一个页表的第一个PTE被赋值为 `PG_P | PG_USU | PG_RWW` ，即此PTE指向的页首地址为零，是个可读可写的用户页。同时，**这意味着，第零个页表中第零个PTE指向的页的首地址是0，于是线性地址** `0~0FFFh` **被映射到物理地址** `0~0FFFh` 。即4BG空间的线性地址映射到同样的物理地址中。

接下来的循环初始化了全部的PTE。于是到了启动分页机制的时候，**先让CR3指向页目录表，然后让CR0的PG位为1**。分页机制就启动了。执行结果如下，没有改变，因为我们把所有线性地址映射到完全相同的物理地址：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200715194821932.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

---
## 5. 克勤克俭用内存
上面，我们的页表4MB浪费得太多了，因为我们没有那么大4GB的内存。如果我们的内存总数只有16MB的话，存储页表就花了25%的空间！所以，我们实现了分页，但是还没有得益于分页。 

这里**如果我们想要节省空间，就要对等映射，有多少内存就用多少页表**，如果是16MB或者说`2^24/2^12=2^12` 个页的内存，实际上只需要4个页表就够了。

### (1) 取得内存信息
为此，我们需要知道内存有多大，然后以此来确定页表个数。下面是书上的方法，利用中断15h知道机器的内存：
- 填充 `eax=0E820h` 。`int 15h` 的功能由 `ax` 决定，如果想要得到内存信息，需要给 `ax` 赋这样的值；
- 填充 `ebx=0` 。第一次调用时 `ebx` 必须为零，之后这个寄存器会放置 `后续值` ；
- 用 `es:di` 将其指向一个地址范围描述符结构 `ARDS` (`Address Range Descriptor Structure`) ，BIOS会填充此结构；
- `ecx` ，`es:di` 所指向的**地址范围描述符结构的大小**，以字节为单位。无论 `es:di` 所指向的结构如何设置，BIOS最多将会填充 `ecx` 个字节。不过，通常情况下无论 `ecx` 为多大，BIOS只填充 `20` 字节，有些BIOS忽略 `ecx` 的值，总是填充 `20` 字节；
- `edx` ，`0534D4150h`（`'SMAP'`）──BIOS将会使用此标志，**对调用者将要请求的系统映像信息进行校验**，这些信息会被BIOS放置到 `es:di` 所指向的结构中。

中断调用后，结果放在下面的寄存器中：
- `CF` ，`CF=0` 表示没有错误，否则存在错误。
- `edx` ，`0534D4150h`（`SMAP`）。
- `es:di` ，**返回的地址范围描述符结构指针，和输入值相同**。
- `ecx` ，BIOS**填充在地址范围描述符中的字节数量**，被BIOS所返回的最小值是 `20` 字节。
- `ebx` 这里放置着**为等到下一个地址描述符所需要的后续值**，这个值的实际形势依赖于具体的BIOS的实现，调用者不必关心它的具体形式，只需在下次迭代时将其原封不动地放置到 `ebx` 中，就可以通过它获取下一个地址范围描述符。如果它的值为 `0`，并且 `CF` 没有进位，**表示它是最后一个地址范围描述符**。

ARDS结构如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200715193923669.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
Type取值如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200715193952683.png)
所以，`ax=0E820h` 时调用 `int 15h` 得到的**不仅仅是内存的大小，还包括对不同内存段的一些描述**。而且，**这些描述都被保存在一个缓冲区中**。

所以，在我们调用 `int 15h` 之前，必须先有缓冲区。我们**可以在每得到一次内存描述时都使用同一个缓冲区，然后对缓冲区里的数据进行处理**，也可以**将每次得到的数据放进不同的位置**，比如一块连续的内存，然后在想要处理它们时再读取。

**后一种方式更方便一些**，所以下面的代码第65行定义了一块 `256` 字节的缓冲区），**最多可以存放12个20字节大小的结构体**。现在还不知道它到底够不够用，这个大小仅仅是凭猜测设定。

**我们将把每次得到的内存信息连续写入这块缓冲区，形成一个结构体数组**。这个过程中，`eax, ecx, edx` 的值都不会变，`ebx` 的值先不管，`es:di` 指向 `_MemeChkBuf` 的开始处，每次 `di += 20` ，`_dwMCRNumber += 1` 。最后当 `CF` 被置位或者 `ebx` 为零，则结束循环。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200715200442767.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
然后在保护模式下的32位代码，用双重循环把它们读出来，每次读取一个 `ARDStruct` ，打印其中每个成员的各项；然后根据当前结构的类型，得到可以被操作系统使用的内存的上限，结果存入 `dwMemSize` 中，最后用 `DispInt, DispStr` （和 `DispAL, DispReturn` 一起放入 `lib.inc` 中）显示在屏幕上，**凭借它们得到内存的容量**：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200715201639984.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
`lib.inc` 的内容如下：
- `DispInt` 通过4次调用 `DispAL` 显示一个整数，最后显示一个灰色字母 `h` 。
- `DispStr` 通过循环显示字符串，每次写一个字符到显存，遇到 `\0` 则结束循环，遇到 `\n` 则从下一行开始处继续显示。
- `DispReturn` 通过 `DispStr` 处理回车。
- 另外，这里用变量 `dwDispPos` 保存当前的显存位置而不是用 `edi` ：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200715205319223.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)![在这里插入图片描述](https://img-blog.csdnimg.cn/20200715205410899.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

另外，书中说，现在数据段中每个变量都有类似的两个符号：
```shell
_dwMemeSize : dd   0				; 实模式用_dwMemSize
dwMemSize	  equ  _dwMemSize - $$	; 保护模式用dwMemSize
```
程序中还调用了 `DispMemSize` 函数：
```handlebars
push SzMemChkTitle
call DispStr		; 显示一个字符串作为将要打印的内存信息的表格头
add esp, 4
call DispMemSize	; 显示内存信息
```

`pmtest7.asm` 程序运行结果如下所示：（为了看清楚，先用 `cls` 清屏）
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200715214121796.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
图中共有5段内存，内存情况如下，可以发现，操作系统能使用的最大内存地址为 `01FFFFFFh` （从零开始） ，所以机器有32MB的内存。同时，我们指定的 `256` 字节的 `MemChkBuf` 是够用的：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200715214227436.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
上面的代码不仅得到了内存的大小，还得到了可用内存的分布信息。

### (2) 计算PDE和PTE数量
得到了内存信息后，我们可以节约页目录表和页表占用的空间，不再初始化所有PDE和页表。根据内存大小计算：`内存大小 / 4MB = PDE数量/页表数量` (1个PDE四个字节，指示一个页表，一个页表对应的内存大小是 `4KB * 1024`) 。

然后，初始化页表时，通过刚才计算得到的页表个数乘以1024，每个页表含有1024个PTE，得出要填充的PTE个数，然后通过循环初始化。

这样下来，页表所需的空间就小多了。32MB的内存需要的是 $8$ 个PDE和 $\text{8 * 4KB = 32KB}$ 大小的页表。  下面是代码：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200715223651775.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
然后初始化页表段时：
```handlebars
LABEL_DESC_PAGE_TBL: Descriptor PageTblBase, 4096 * 8 - 1, DA_DRW
```

---
## 6. 进一步体会分页机制
一开始，调用 `ProcPagingDemo` ，向 `LinearAddrDemo` 这个线性地址进行跳转，而 `LinearAddrDemo` 映射到物理地址空间的 `ProcFoo` 处，我们让 `ProcFoo` 打印出红色的字符串 `Foo` ，所以执行时我们应该可以看到红色的 `Foo` 。映射关系转换前的情形如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200715232932137.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
随后我们改变映射关系，实际上就是改变页目录表的指向，页目录表和页表的切换将 `LinearAddrDemo` 映射到 `ProcBar` 处，这时我们调用 `ProgPagingDemo` 时，程序转移到 `ProcBar` 处执行，从而打印出红色字符串 `Bar` ：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200715232937733.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

---
为了实现这个切换，我们看看要对 `pmtest7.asm` 进行怎样的修改。
- 首先，我们用到了另一套页目录表和页表，原来的页目录表和页表段已经不够用了。这里，我们用一个段 `FLAT` 存放两套页目录表和页表。
	```swift
	LABEL_DESC_FLAT_C:  Descriptor 0, 0fffffh, DA_CR|DA_32|DA_LIMIT_4K; 0～4G
	LABEL_DESC_FLAT_RW: Descriptor 0, 0fffffh, DA_DRW|DA_LIMIT_4K     ; 0～4G   
	...  
	SelectorFlatC  equ  LABEL_DESC_FLAT_C  - LABEL_GDT42 
	SelectorFlatRW equ  LABEL_DESC_FLAT_RW - LABEL_GDT
	```
	之所以用两个描述符描述这个段，是因为**我们不仅要读写这段内存，还要执行其中的代码，所以属性要求不一样**。两个段的段基址都是零，长度都是4GB。
- 启动分页的代码也要修改：
	- 我们要存储页表的个数，因为有多个页目录和页表；
	- `es = SelectorFlatRW` ，只需要将地址赋给 `edi` ，`es:edi` 就会指向对应的物理地址；然后，用 `stosd` 将`eax` 的内容持续复制4个字节给 `es:edi` ，并增加 `edi` 4个字节指向下一个表项；
	- 新建一个函数 `PagingDemo` ，将分页的内容放在里面；

	![在这里插入图片描述](https://img-blog.csdnimg.cn/20200716192301827.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
- 此外，开始时 `LinearAddrDemo` 指向 `ProcFoo` 并且**线性地址等于物理地址**，所以 `LinearAddrDemo = ProcFoo` 。而 `ProcFoo, ProcBar` 应该是指定的物理地址，则 `LinearAddrDemo` 也应该是指定的物理地址。所以我们使用它们时**应该确保使用的是FLAT段**，段选择子应该是 `SelectorFlatC / SelectorFlatRW` ；
- 为了将代码放在 `ProcFoo, ProcBar` 这两个地址，我们需要先写两个函数，运行程序时将两个函数的执行码复制过去；
- `ProcPagingDemo` 要调用FLAT段的 `LinearAddrDemo` ，所以如果不想使用段间转移，需要将 `ProcPagingDemo` 也放进FLAT段中。再写一个函数，将代码复制到 `ProcPagingDemo` 处。
- 经过这几步后，`ProcPagingDemo, LinearAddrDemo, ProcFoo, ProcBar` 虽然是函数，但是却**都是内存中指定的地址**，下面将其定为常量：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200716193601866.png)
- 接着，我们需要将代码填充进这些内存地址，用 `PagingDemo` 和 `MemCpy` 复制三个过程到指定的内存地址，`MemCpy` 假设源数据在 `ds`段中，目标在 `es` 段中，所以一开始对 `ds, es` 赋值；
- 最后的4个call指令，启动分页，调用 `ProcPagingDemo`，然后切换页目录，最后调用一次 `ProcPagingDemo` ；
- 由于 `LinearAddrDemo` 和 `ProcFoo` 相等，并且函数 `SetupPaging` 建立起来的是对等的映射关系，所以第一次对 `ProcPagingDemo` 的调用反映的就是图3.34的情况。
![](https://img-blog.csdnimg.cn/20200716222626911.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70) 
- 被复制的三个过程代码如下，其中405行调用 `LinearAddrDemo` 是一个短调用；`foo, bar` 为了简化对段寄存器的使用，仍然选择将单个字符写入显存：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200716222952220.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
 
- 接下来调用的是 `PSwitch` ，我们来看一下这个切换页目录的函数是怎样的：初始化页目录和页表的过程和 `SetupPaging` 差不多；不过接着程序改变了线性地址 `LinearAddrDemo` 对应的物理地址，使其对应 `ProcBar` ；最后将CR3的值改变成 `PageDirBase1` ：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200716224156876.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)![在这里插入图片描述](https://img-blog.csdnimg.cn/20200716224203942.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

对 `ch3/h/pmtest8.asm` 的运行情况如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200715231301383.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
我们看到红色的Foo和Bar，说明**页表切换**成功了。**不同的进程拥有不同地址**，其原理也是类似，**<font color="red">在任务切换时通过改变CR3的值来切换页目录，从而改变地址映射关系。</font>**

这就是分页的妙处！**由于分页机制的存在，程序使用的都是线性地址空间，不再直接是物理地址，所以，应用程序可以不必关心硬件平台和物理内存。** 操作系统全权负责这些工作。

