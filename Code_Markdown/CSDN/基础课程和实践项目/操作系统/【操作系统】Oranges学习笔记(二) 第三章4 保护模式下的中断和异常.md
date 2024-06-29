@[toc]

---
# 3.4 保护模式下的中断和异常
以前，我们使用中断都是在实模式下进行的，如 `int 15h` 得到内存信息后在保护模式下把它们显示出来。原因在于，**在保护模式下，中断机制发生了很大变化**。

保护模式下，**原来的中断向量表已经被IDT所代替，实模式下能用的BIOS中断在保护模式下已经不能用了**。IDT跟GDT、LDT一样也是个描述符表，即中断描述符表 `Interrupt Descriptor Table` 。IDT中的描述符有三种：
- 中断门描述符；
- 陷阱门描述符；
- 任务门描述符（任务门不是每个操作系统都有的，比如Linux就没有，因此不做关注）

IDT的作用是：**将每个中断向量和一个描述符对应起来**。很像实模式下的中断向量表。在"调用门初体验"中提过，**中断门和陷阱门是特殊的调用门**，只不过调用门是 `call` 指令，这里使用 `int` 指令。因此本节内容只是原有内容上的一点改变。

中断向量对应到中断处理程序的过程如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200715235302794.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

中断门和陷阱门的结构如下，对比调用门发现，下面BYTE4的低5位变成了保留位而不是 `ParamCount` ，`TYPE` 的4位也将变成 `0xE` 中断门或 `0xF` 陷阱门，`S` 位依然是零：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200715235529570.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)


## 1. 中断和异常机制
中断不仅涉及到处理器和指令，还包括和硬件的接口，比如8259A中断处理芯片。

**中断通常在程序执行时因为硬件而随机发生，它们通常用来处理CPU外部的事件**，比如外围设备的请求。软件通过执行 `int   n` 指令也可以产生中断。**异常**则通常在处理器执行指令过程中检测到错误时发生，比如遇到零除的情况。处理器检测的错误条件有很多，比如保护违例、页错误等。 

总之，中断和异常都是**硬件或软件**在发生某种情形后通知处理器的行为。保护模式下处理器可以处理的中断或异常如下表3.8：  
- `Fault`、`Trap` 和 `Abort` 是异常的三种类型。
- `Fault` 错误，是一种可被更正的、任务不失连续性的异常。当一个 `Fault` 发生时，处理器会把产生 `Fault` 的指令之前的状态保存起来。异常处理程序的返回地址将会是产生 `Fault` 的指令，**而不是其后的那条指令**。
- `Trap` 陷阱，是一种在发生 `Trap` 的指令执行之后立即被报告的异常，同样允许程序不失连续性地执行。注意：**异常处理程序的返回地址将会是产生** `Trap` **的指令之后的那条指令**。图中，`Trap` 类型的异常只有2个，最常用的应该就是**设置和调试断点**。
- `Abort` 终止，**不允许继续执行程序**，是**用来报告严重错误**的。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200716013111923.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

## 2. 外部中断
这里看一下中断。中断产生的原因有两种，外部中断(硬件产生)和内部中断(`int n` 产生的软中断)。

 `int n` 产生中断，`n` 为向量号，类似于调用门的使用，图见3.37 中断向量到中断处理程序的对应过程；外部中断的情况更复杂，分为不可屏蔽中断 `NMI` 和可屏蔽中断两种，分别由CPU的两根引脚 `NMI` 和 `INTR` 接受：
- `NMI` 不可屏蔽，与 `IF` 是否被设置无关，`NMI` 中断对应的中断向量号是 `2` ；
- 为了**建立可屏蔽中断和向量号间的对应关系**，我们需要通过可编程中断控制器8259A——它是**中断机制中所有外围设备的代理**，不仅可以根据优先级在同时发生中断的设备中选择应该处理的请求，还可以通过设置寄存器来屏蔽和打开相应的中断。和CPU的连接如下图： ![在这里插入图片描述](https://img-blog.csdnimg.cn/20200716014927288.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0HM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
和CPU相连的有两片级联8259A，每个8259A有8根中断信号，总共可以挂15个不同的外部设备。通过对8259A的设置，我们可以将这些设备的中断请求和中断向量对应起来。

BIOS初始化8259A时，`IRQ0 ~ IRQ7` 被设置为对应向量号 `08h ~ 0Fh` ，但是保护模式下向量号 `08h ~ 0Fh` 已经被占用了，我们必须重新设置主从8259A。

---
## 3. 设置和编程操作8259A
8259A是可编程中断控制器，通过向端口写入特定的初始化控制字 `ICW` (`Initialization Command Word`)进行设置，ICW共有4个，每个都是具有特定格式的字节。主8259A对应端口是 `20h ~ 21h` ，从8259A对应的端口地址 `A0h ~ A1h` 。初始化过程如下(顺序不可变化)：
1. 往端口20h（主片）或A0h（从片）写入ICW1。
2. 往端口21h（主片）或A1h（从片）写入ICW2，这里涉及到与中断向量的对应。
3. 往端口21h（主片）或A1h（从片）写入ICW3。
4. 往端口21h（主片）或A1h（从片）写入ICW4。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200716151504381.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

此外，还有 `OCW` (`Operation Control Word`) ，共有3个，其中 `OCW1` 是最常用的，它被写入中断屏蔽寄存器 `IMR` (`Interrupt Mask Register`) 中。我们用它**屏蔽或打开外部中断**，屏蔽时将对应位设为1即可：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2020071615471364.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
如果我们想要通知8259A中断处理结束了，就需要使用 `OCW2` 。实模式下我们写汇编，当每一次中断处理结束时，需要发送一个 `EOI` 给8259A，通知它中断处理工作结束，继续接受其他中断。发送 `EOI` 需要往端口 `20h/A0h` 写入 `OCW2` 实现：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2020071615512290.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
发送 `EOI` 给8259A可以使用下面的代码完成：
```handlebars
mov al, 20h
out 20h/A0h, al
```

---
编程时实际要做的工作就是设置8259A和建立IDT两个部分，以 `pmtest8.asm` 为基础进行修改得到 `pmtest9.asm` 。

这里将设置8259A的代码写成函数，它分别向主从8259A中写入了4个ICW。
- 主8259A写入ICW2时，由于IRQ0对应中断向量号0x20，于是IRQ0~IRQ7对应中断向量 `20h ~ 27h` ；从片中，`IRQ8 ~ IRQ15` 对应中断向量 `28h ~ 2Fh` 。这些中断向量号都处于用户定义中断的范围中：
- 后半段，我们设置了OCW1，仅开启定时器中断；
- 另外，每次I/O操作后，我们调用了延迟函数 `io_delay` 等待操作完成，这个延迟函数实际上就是4个nop指令：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200716153938462.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

## 4. 建立IDT
我们将IDT放入一个单独的段，用NASM的 `%rep` 预处理，设置255个同样的描述符，它们都指向 `SelectorCode32:SpuriousHandler` 中断门。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200716161448462.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
`SpuriousHandler` 将在屏幕右上角打印红色字符 `!` ，然后进入死循环：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200716161756259.png)
加载IDT的代码和对GDT的代码很像，执行 `lidt` 前用 `cli` 指令清 `IF` 位，暂时不响应可屏蔽中断：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200716162006633.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
到这里我们的中断机制就初始化完了，运行会发现无法正常回到实模式。如果**要顺利回到实模式**还需要将 `IDTR` 和8259A等恢复原样，这些代码参见 `pmtest9.asm` 。

---
## 5. 实现一个中断
初始化保护模式下的中断异常处理机制后，我们要实现一个中断。在此之前，我们试一下 `int` 指令的效果：`ch3/i/pmtest9b.asm` 
```swift
[SECTIOIN .s32]
...
	call Init8259A
	int 080h
	...
```
由于IDT中所有描述符都指向 `SelectorCode32:SpuriousHandler` 处，所以调用几号中断都是打印红色字符 `!` 再进入死循环。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200716170846167.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

---
修改后得到 `ch3/i/pmtest9c.asm` ，将 `80h` 号中断单独列出来，并新增一个函数 `UserIntHandler` 来处理这个中断，它通过 `iretd` 返回，而不是进入死循环：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200716171313278.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

运行 `pmtest9c.asm` 结果如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200716171708411.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
 
 ---
## 6. 时钟中断试验
上面的中断和调用门没有太大差别。不过下面我们将打开时钟中断 `IRQ0` (对应 `020h` 号端口)，学点新的东西。
> 可屏蔽中断与NMI的区别在于是否受到IF位的影响，而8259A的中断屏蔽寄存器（IMR）也影响着中断是否会被响应。
> 
> 所以，**外部可屏蔽中断**的发生就受到两个因素的影响，只有当 `IF` 位为1，并且 `IMR` 相应位为 `0` 时才会发生。

如果我们要打开时钟中断的话，需要设计一个中断处理程序，还要设置 `IMR` 和 `IF` 位，前者写 `OCW2` 实现，后者用 `STI` 完成。

简单的中断处理程序如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200716172428124.png)然后，开启时钟中断：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200716172457639.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
修改IDT，将 `20h` 号单独写成一个中断门，对应 `IRQ0` ：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200716172630263.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
现在调用 `80h` 号中断后用 `sti` 打开中断，进行死循环(不然可能第一个时钟中断都没发生，程序就结束了)：
```handlebars
	int 080h
	sti
	jmp $
```

看到效果如下，时钟中断的实现证明我们对8259A的设置是正确无误的：`pmtest9.asm` 
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200716191633535.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)


--- 
## 7. 几点额外说明
### (1) 特权级变换
上面的代码始终运行在 `ring0` ，实际应用中，中断的产生大多带有特权级变换。**通过中断门和陷阱门的中断就相当于用call指令调用一个调用门，涉及的特权级变换的规则完全一样**。

### (2) 中断或异常发生时的堆栈变化
中断或异常发生时，以及相应的处理程序结束时，堆栈都会发生变化：
- 没有特权级变换时，`eflags, cs, eip` 依次入栈，有出错码则最后入栈； 
- 有特权级变换时，发生堆栈切换，`ss, esp` 被压入堆栈，然后是 `eflags, cs, eip, error code` ；
- 从中断或异常返回时必须使用指令 `iretd` ，和 `ret` 相似，不过 `iretd` 同时会改变 `eflags` 的值。注意：只有当 `CPL = 0` 时，`eflags` 的 `IOPL` 域才会改变，且只有当 `CPL <= IOPL` 时，`IF` 才会改变；

	此外，`iretd` 执行 `Error Code` 不会被自动从堆栈中弹出，所以执行它之前需要先将它从栈中清除。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200716140114862.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

### (3) 中断门和陷阱门的区别
有中断门向量引起的中断会复位 `IF` ，可以避免其他中断干扰当前中断的处理，随后的 `iretd` 指令会从堆栈上恢复 `IF` 的原值。通过陷阱门产生的中断不会改变 `IF` 。

