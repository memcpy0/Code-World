@[toc]

进程是操作系统中最重要的概念之一。没有进程，就不能称作操作系统。
> 这一章的概括：
> - 先实现一个简陋的进程，然后模仿它再写一个；
> - 让两个进程同时运行，并用我们的系统试着调度；
> - 最后，尝试扩展进程的功能。

---
# 6.1 迟到的进程
> **进程的切换、调度等内容和保护模式紧密相连**。要深入了解操作系统和进程，至少要接触一种平台上的具体实现。**只有拥有了基于具体平台的感性认识之后，才有可能对形而上的理论有更踏实的理解**。

# 6.2 进程概述
## 6.2.1 进程介绍
什么是进程？书中说：系统中的若干进程就像一个人在一天内要做的若干样工作：
- 总体上：每样工作相对独立，又受控于人，可以产生某种结果；
- 细节上：每样工作都有自己的方法、工具和需要的资源；
- 时间上：每一个时刻只能有一项工作正在处理，各项工作可以轮换来做，对最终结果没有影响。
 
 进程也是一样：
 - 宏观上：每个进程相对独立，有自己的目标/功能，又受控于进程调度模块；
 - 微观上：它可以利用系统的资源，有自己的代码(做事的方法)，有自己的数据和堆栈(做事需要的资源和工具)；
 - 时间上：进程需要被调度轮换，不影响最终结果。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200707130938506.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

## 6.2.2 未雨绸缪——形成进程的必要考虑
为了实现进程的调度(这里我们只有一个CPU，进程有多个，它们看上去像在同时运行；即使有多个CPU，**CPU的个数总是小于进程的个数**)，我们必须有一个数据结构，**记录一个进程的状态**。在进程将被挂起的时候，进程信息写入其中；进程重新启动时，这些信息被读取出来。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200707131535905.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
更复杂的是，**很多情况下进程和进程调度运行在不同的层级上**。简化一下，让所有任务运行在 `ring1` ，进程切换运行在 `ring0` 。

另外，引发进程切换的原因有多种，比如说发生了时钟中断，此时中断处理程序会将控制权交给进程调度模块。这时，如果系统认为应该进行进程切换（也有不应该进行进程切换的时候），进程调度就发生了——**当前进程的状态被保存起来，队列中下一个进程被恢复执行**。如下图：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200722120546961.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
这里先让每次非重入的中断都切换一次进程。

## 6.2.3 参考的代码
这本书的很多代码都是向Minix学习的结果，不过Orange's的进程实现更简单，并不依赖于Minix。

---
# 6.3 最简单的进程
进程切换时的情形：
1. 一个进程 `A` 正在运行；
2. 这时候**时钟中断**发生，特权级从 `ring1` 跳到 `ring0` ，开始执行**时钟中断处理程序**；
3. 中断处理程序这调用**进程调度模块**，指定下一个应该运行的进程 `B` ；
4. 当中断处理程序结束时，下一个进程 `B` 准备就绪并开始运行，特权级又从 `ring0` 跳回 `ring1` ，如图6.4所示。
5. 进程 `B` 运行中。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200722122402364.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

 
要想实现这些功能，必须完成以下几项：
- 时钟中断处理程序
- 进程调度模块
- 两个进程

先来分析一下，进程 `A` 切换到进程 `B` 的过程中，有哪些关键技术需要解决。然后用代码分别实现这几个部分。

## 6.3.1 简单进程的关键技术预测
在实现简单的进程之前，能够想到的关键技术大致包括下面的内容。
### 1. 进程的哪些状态需要被保存
**只有可能被改变的才有保存的必要**。进程要运行需要CPU和内存相互协作，而不同进程的内存互不干涉。但是CPU只有一个，不同进程共用一个CPU的一套寄存器。所以，**我们要把寄存器的值统统保存起来，在进程被恢复执行时使用。**

### 2. 进程的状态何时以及怎样被保存
为了保证**进程状态完整、不被破坏**，要在进程刚刚被挂起时**保存所有寄存器的值**。
用 `push` 或者 `pushad` (一条指令可以保存许多寄存器值)。**这些代码应该写在时钟中断例程的最顶端，以便中断发生时马上被执行。**

### 3. 如何恢复进程B的状态
保存用的是 `push` ，恢复则用 `pop` 。等所有寄存器的值都已经被恢复，执行指令 `iretd` ，就回到了进程 `B` 。

### 4. 进程表的引入
**进程的状态关系到每一次进程挂起和恢复**，对于这样重要的数据结构，我们称之为"进程表"或者进程控制块 `PCB` 。通过进程表，我们可以非常方便地进行进程管理。

这里，中断处理的部分内容必须使用汇编，其他大部分进程管理的内容都可以用C编写——**将进程表定义成一个结构体**；我们有很多个进程，所以会有很多个进程表，**形成一个进程表数组**。如图6.5所示。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200722165326682.png)

**进程表是用来描述进程的，所以它必须独立于进程之外。** 当我们把寄存器值压到进程表内的时候，已经处在进程管理模块之中。

### 5. 进程栈和内核栈
**当寄存器的值已经被保存到进程表内，进程调度模块就开始执行**。但这时 `esp` 指向何处？我们在进程调度模块中会用到堆栈，而寄存器被压到进程表之后，`esp` 是指向进程表某处的。**接下来进行任何的堆栈操作，都会破坏掉进程表的值，从而在下一次进程恢复时产生严重的错误**。

为解决这个问题，必须将 `esp` 指向专门的内核栈区域。这样，在短短的进程切换过程中，esp的位置出现在3个不同的区域（图6.6是整个过程的示意）。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200722170006396.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

其中：
- 进程栈──进程运行时自身的堆栈。
- 进程表──**存储进程状态信息**的数据结构。
- 内核栈──**进程调度模块**运行时使用的堆栈。

在具体编写代码的过程中，一定要清楚当前使用的是哪个堆栈，以免破坏掉数据。

### 6. 特权级变换：ring1→→ring0
在我们以前的代码中，还没有使用过除 `ring0` 之外的其他特权级。对于有特权级变换的转移：
- **如果由外层向内层转移时**，需要从当前 `TSS` 中取出内层 `ss` 和 `esp` 作为目标代码的 `ss` 和 `esp` 。所以，我们必须事先准备好 `TSS` 。
- 由于每个进程相对独立，我们把涉及到的描述符放在局部描述符表 `LDT` 中，所以，我们还需要为每个进程准备 `LDT` 。

### 7. 特权级变换：ring0→→ring1
刚才的分析过程中，我们假设初始状态是“进程 `A` 运行中”。可是到目前为止我们的代码完全运行在 `ring0` 。所以，当我们准备开始第一个进程时，我们面临一个从 `ring0` 到 `ring1` 的转移，并启动进程 `A` 。

这跟我们从进程 `B` 恢复的情形很相似，完全可以在准备就绪之后**跳转到中断处理程序的后半部分**，假装发生了一次时钟中断来启动进程 `A` ，利用 `iretd` 来实现 `ring0` 到 `ring1` 的转移。

---
## 6.3.2 第一步——ring0→→ring1
即便是想像中最简单的进程，仍然需要不少的关键技术。要一次性完成所有列出的关键技术并调试成功是不可能的，所以还是从最容易的做起。

在开始第一个进程时，我们打算使用 `iretd` 来实现由 `ring0` 到 `ring1` 的转移，一旦转移成功，就可以认为已经在一个进程中运行了。为了对实现有一个感性认识，先来看一下第6章最终实现的代码（`chapter6/r/kernel/kernel.asm`) 的一小部分：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200722172131242.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

为了容易理解，先来看一看本章代码的部分内容。因为进程涉及若干方面，如果一开始就下手行动，很可能会无所适从。

在 `/kernel` 中多了一个 `main.c` ，里面有个函数 `kernel_main( )` ，从中有这样一行：    `restart( );` 。它调用的便是代码6.1这一段，**它是进程调度的一部分，同时也是我们的操作系统启动第一个进程时的入口。**


第358行设置了 `esp` 的值，而在下方不远处就是若干个 `pop` 以及一个 `popad` 指令。我们不难推断，`p_proc_ready` 是一个指向进程表的指针，**存放的便是下一个要启动进程的进程表的地址**。而且，其中的内容必然是以图6.7所示的顺序进行存放。这样，才会使 `pop` 和 `popad` 指令执行后各寄存器的内容更新一遍。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200722171109182.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
验证一下。在头文件 `global.h` 中可以找到 `p_proc_ready` ，其类型是一个结构类型指针 `struct s_proc*` 。再打开 `proc.h` ，可以看到 `s_proc` 这个结构体的第一个成员也是一个结构，叫做 `s_stackframe` 。我们找到 `s_stackframe` 这个结构体的声明，它的内容安排与我们的推断完全一致。

现在我们知道了，进程的状态都被存放在 `s_proc` 这个结构体中，而且**位于前部的是所有相关寄存器的值**， `s_proc` 这个结构应该是我们提到过的“进程表”。当要恢复一个进程时，便将 `esp` 指向这个结构体的开始处，然后运行一系列的 `pop` 命令将寄存器值弹出。进程表的开始位置结构如图6.8所示。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200722171419616.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

再来看第359行， `lldt` 设置 `ldtr` 。既然 `esp` 等同于 `p_proc_ready` ，那么 `esp+P_LDT_SEL` 一定是 `s_proc` 的一个成员，通过对比 `sconst.inc` 中 `P_LDT_SEL` 和结构体 `s_proc` 可知， `esp+P_LDT_SEL` 就是 `s_proc` 中的成员 `ldt_sel` 。同时，在执行 `restart( )` 之前，在某个地方一定做了 `ldt_sel` 的初始化工作，以便 `lldt` 可以正确执行。

经过上面的分析，第 `360, 361` 行就很容易理解了，它们的作用是将 `s_proc` 这个结构中第一个结构体成员 `regs` 的末地址赋给TSS中 `ring0` 堆栈指针域（`esp`）。在下一次中断发生时，`esp` 将变成 `regs` 的末地址，然后进程 `ss, esp, eflags, cs, eip` 这几个寄存器值将依次被压栈（参考图3.45），放到 `regs` 这个结构的最后。再回头看 `s_stackframe` 这个结构的定义时，发现最末端的成员果然是这 `5` 个。

至此只剩下两行代码，一行是将 `k_reenter-1` ，而另一行则是将 `esp+4` 。结合 `s_stackframe` 结构定义发现， `esp+4` 跳过了 `retaddr` 这个成员，以便执行 `iretd` 指令。之前堆栈内恰好是 `eip、cs、eflags、esp、ss` 的值。 `retaddr, k_reenter` 起什么作用呢？后面会慢慢说明。

对于进程，我们有了一定程度的感性认识。要想实现进程必须完成这几项：**时钟中断处理程序、进程调度模块和进程体**。下面依次来做这些工作。

---
### 1. 时钟中断处理程序
先来做最简单的。完善时钟中断处理程序并不容易，但这里只实现由 `ring0` 到 `ring1` 的转移，用一个 `iretd` 指令就够了。还不需要关于进程调度的知识，所以时钟中断处理程序在这一步并不重要，我们完全可以做得最简单：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2020072315454665.png)

在这段中断例程中什么也不做，直接返回。等到我们觉得必要的时候再添加新的代码。

### 2. 化整为零：进程表、进程体、GDT、TSS
既然在进程开始之前要用到进程表中各项的值，首先将这些值进行初始化。一个进程开始之前，只要指定好各段寄存器、eip、esp以及eflags，它就可以正常运行，至于其他寄存器是用不到的，所以得出必须初始化的寄存器列表：`cs、ds、es、fs、gs、ss、esp、eip、eflags` 。

在 `Loader` 中就把 `gs` 对应的描述符 `DPL = 3` ，所以进程中的代码是有权限访问显存的；让其他段寄存器对应的**描述符基地址和段界限**与先前的段寄存器对应的**描述符基地址和段界限**相同，只是改变它们的 `RPL, TI` ，以表示它们运行的特权级。

这里 `cs, ds` 等段寄存器**对应的将是LDT中**而不再是GDT中**的描述符**。所以，我们的另一个任务是初始化局部描述符表。由于LDT是进程的一部分，可以把LDT放置在进程表中。同时，我们还必须在GDT中增加相应的描述符，并在合适的时间将相应的选择子加载给 `ldtr` 。

另外，由于我们用到了**任务状态段**，所以还必须初始化一个TSS，并且在GDT中添加一个描述符，对应的选择子将被加载给 `tr` 这个寄存器。其实，TSS中所有能用到的只有两项，便是 `ring0` 的 `ss, esp` ，只需要初始化它们两个就够了。

在第一个进程正式开始之前，准备工作已经做得差不多了，其核心内容便是**一个进程表以及与之相关的TSS等**。对应关系如图所示：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200723221340916.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

这个图将其化整为零，可以分为4个部分，那就是**进程表、进程体、GDT和TSS**，关系大致分为三个部分：
1. 进程表和GDT。进程表内的 `LDT Selector` 对应GDT中的一个描述符，而这个描述符所指向的内存空间就存在于进程表内。
2. 进程表和进程。进程表是进程的描述，进程运行过程中如果被中断，各个寄存器的值都会被保存进进程表中。但在第一个进程开始之前，不需要初始化太多内容，只需要知道进程的入口地址就足够了。另外，由于程序要用到堆栈，而堆栈不受程序本身控制，还需要事先指定 `esp` 。
3. GDT和TSS。GDT中需要有一个描述符来对应TSS，需要事先初始化这个描述符。

这4个部分的相互关系弄清楚了，现在分别来做这4个部分的初始化工作。

第一步，准备一个小的进程体。此刻，并不需要，我们只需要一个极小的进程执行体，它只有不到10行：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200723221605987.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
看到这个"进程"，或者说是个函数，极其简单的函数，但已经可以作为一个进程执行体。在它执行时会不停地循环，每循环一次就打印一个字符 `A` 和一个数字，并且稍停片刻。

注意，这段代码被放置在 `main.c` 这个新建立的文件中。第5章的最后，我们调用指令 `sti` 开中断之后就用 `hlt` 指令让程序停止以等待中断的发生。本章最终将让进程运行起来，而不能仅仅停在那里，所以程序需要继续进行下去。注释掉 `hlt` ，并让程序跳转到 `kernel_main( )` 这个放在 `main.c` 中的函数中，这个函数目前只显示一行字符。不过在完成进程的编写之前，要让程序停在这里，所以用一个死循环作为它的结束。
![在这里插入图片描述](https://img-blog.csdnimg.cn/202007232220259.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
在 `kernel.asm` 的最后，我们跳转到 `kernel_main( )` 中：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200723222110757.png)

进程 `A` 中的函数 `delay( )` 也让它尽量简单，写一个循环：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200723222142394.png)

运行的时候，如果发现两次打印之间的间隔不理想，可以调整循环的次数。

第二步，初始化进程表。首先要有进程表结构 `s_proc` 和结构体 `STACK_FRAME` 的定义：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200723222408317.png)![在这里插入图片描述](https://img-blog.csdnimg.cn/20200723222316572.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
如果完全自己编写这部分代码的话，显然不会在 `STACK_FRAME` 中添加 `retaddr` 作为一个成员。不过，我们已经看过最终的代码，知道它将来是有用的，所以暂时妥协一下，把它加在里面。

现在，结构体的定义有了，在 `global.c中` 声明一个进程表：
```c
PUBLIC PROCESS proc_table[NR_TASKS];
```
其中，`NR_TASKS` 定义了最大允许进程，先把它设为 $1$ 。目前只试验一个进程的运行，但为了以后的扩展，还是声明了一个数组而不是一个变量，当 `NR_TASKS=1` 的时候数组和变量是一样的。

进程表有了，来初始化它。在整个过程中建议同时对照图6.9，有利于理清思路。由于 `kernel_main( )` 是最后一部分被执行的代码，那么初始化进程表的代码理应添加在这里。

从图6.9或者代码6.8可以看出，进程表需要初始化的有3个部分：寄存器、`LDT Selector` 和LDT：
- `LDT Selector` 被赋值为 `SELECTOR_LDT_FIRST` ，这个宏的定义在代码6.10中；
- LDT里面共有两个描述符，为简化起见，分别被初始化成**内核代码段和内核数据段**，只是改变了一下 `DPL` 以让其运行在低的特权级下；
- 要初始化的寄存器比较多，`cs` 指向LDT中第一个描述符，`ds, es, fs, ss` 都设为指向LDT中的第二个描述符，`gs` 仍然指向显存，只是其 `RPL` 发生改变。
- 接下来，`eip` 指向 `TestA` ，这表明进程将从 `TestA` 的入口地址开始运行；
- `esp` 指向了单独的栈，栈的大小为 `STACK_SIZE_TOTAL` ；
- 最后一行设置 `eflags` ，结合图3.46可以知道，`0x1202` 恰好设置了 `IF` 位并把 `IOPL` 设为 $1$ 。这样，进程可以使用 `I/O` 指令，并且中断会在 `iretd` 执行时被打开：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2020072322321361.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)


代码中用到的宏大部分定义在 `protect.h` 中，有 `SELECTOR_LDT_FIRST, SELECTOR_TSS` ，因为从图6.9中可知，我们需要一个用来使用TSS的描述符：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2020072322372797.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

另外，一定要记得LDT跟GDT是联系在一起的，别忘了填充GDT中进程的LDT的描述符：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2020072322391467.png)
这段代码放在 `init_prot( )` 中。`init_descriptor` 和 `init_idt_desc` 有些类似：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200723231446412.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
`seg2phys` 的定义如代码6.13所示：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200723231611771.png)

`vir2phys` 是一个宏，定义在 `protect.h` 中：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2020072323164072.png)

第三步，准备GDT和TSS。再看图6.9，会发现没有初始化的只有TSS和它对应的描述符。来到 `init_prot( )` ，填充TSS以及对应的描述符：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200723231919885.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
如今TSS已经准备好了，需要添加加载 `tr` 的代码。只要在 `kernel.asm` 中添加几行就好了:
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200723232009131.png)

### 3. iretd
我们事先分析过 `restart` 这个函数，可以直接把它复制到 `kernel.asm` 中。不过，由于只是想完成 `ring0` 到 `ring1` 的跳转，`restart` 仍稍嫌复杂，这里简化一下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200723232203205.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

其中，`p_proc_ready` 是指向进程表结构的指针：
```c
EXTERN PROCESS* p_proc_ready;
```
`P_LDT_SEL, P_STACKTOP, TSS3_S_SP0, SELECTOR_TSS` 都定义在新建立的文件 `sconst.inc` 中。注意，这里的选择子必须与 `protect.h` 中的值保持一致。

由于进程的各寄存器值如今在进程表里面保存好了，现在只需要让 `esp` 指向栈顶，然后将各个值弹出就行了。最后一句 `iretd` 执行以后，`eflags` 会被改变成 `pProc->regs.eflags` 的值。我们事先置了 `IF` 位，所以进程开始运行之时，中断其实也已经被打开了，虽然暂时没有意义，但了解这一点对以后很重要。

现在大部分的工作已经完成，只剩下最后几句了。在 `kernel_main( )` 添加几句：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200723232437408.png)

### 4. 进程启动（参考代码 `chapter6/a` ）
仅仅为了一个跳转，就做了如此多的工作，如今检验工作成果，`make` 并运行：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200723235751299.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
进程在运行了，我们看到不断出现的字符 `A` 和不断增加的数字。这意味着 `ring0` 到 `ring1` 的跳转成功了，我们的进程在运行。从此，这个程序已经可以称之为一个"操作系统"了，因为它已经有了"进程"，无论多么简陋。

### 5. 第一个进程回顾
第一个进程的启动过程示意如图：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200723233236900.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
可以看到，此处中提到的进程表、进程体、GDT和TSS这4个部分的初始化工作都已经完成，进程已经开始执行。不过我们虽然用到了进程表，但毫无疑问，这离期望还很远。我们希望**进程表能够保存并恢复进程状态**，而我们的进程开启之后就再不停息，因为还不曾开启时钟中断。

即使打开了时钟中断，时钟中断也只会发生一次，因为没有将中断结束位 `EOI` 置为 $1$ ，告知8259A当前中断结束。不过没关系，这只是第一步。下面就来打开进程调度的大门。

---
## 6.3.3 第二步——丰富中断处理程序
中断在进程实现中扮演着重要的角色，所以不开启中断显然是不行的，现在就慢慢把中断处理模块完善起来。

### 1. 让时钟中断开始起作用
我们还没有打开时钟中断，现在就在 `i8259.c` 的 `init_8259A( )` 中把它打开：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2020072323480689.png)
为了让时钟中断可以不停地发生而不是只发生一次，还需要设置 `EOI` ：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200723234923585.png)

`EOI, INT_M_CTL` 定义在 `sconst.inc` 中：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200723235135555.png)

`make one image` ，运行后发现结果和原来没有任何区别，因为我们只是可以继续接受中断而已，并没有做什么：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200724003523687.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
为了确认中断处理程序的运行，在中断例程中再添加打印语句：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200723235946850.png)

代码6.23通过改变屏幕第 `(0,0)` 字符的方式来说明中断例程正在运行。本来这个位置是 `Boot` 的首字母 `B` ，如果发生中断，它会不断变化。运行一下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200724005838760.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

预期的结果出现了！左上角不断变化的字符按照ASCII码的顺序在跳动，这说明中断处理程序的确是在运行的。 
### 2. 现场的保护与恢复
为什么我们不用 `disp_str` 函数而用 `mov` 指令直接写显存。不仅是因为这样简单，还有其他理由。我们使用进程表是**为了保存进程的状态，以便中断处理程序完成之后需要被恢复的进程能够被顺利地恢复**。在进程表中给每一个寄存器预留了位置，把它们所有的值都保存下来。这样就可以在进程调度模块中尽情地使用这些寄存器，不会对进程产生不良影响。

但在这个很短的中断例程中，我们却在事先没有保存的情况下改变了 `al` 寄存器的值。`al` 很小，但改变它是有风险的。为了不改变更多寄存器的值而产生更大的风险，所以没用更复杂的 `disp_str` 函数。从程序运行的情况来看，对 `al` 的改变并没有影响到进程的运行，但我们仍有些担心，现在就来把程序改进一下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2020072401021652.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
从现在开始，每进行一次代码修改都 `make` 并运行一下，以便看到效果。在这里运行，仍可以看到进程的运行以及跳动的字符：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200724010409592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)



### 3. 赋值tss.esp0
现在的中断处理程序好多了，寄存器先是被保存，后又被恢复，进程被很好地保护起来。不过，中断现在已经被打开，就存在 `ring0` 和 `ring1` 之间频繁的切换。**两个层级之间的切换包含两方面，一是代码的跳转，二是堆栈的切换。**

由 `ring0` 到 `ring1` 时，堆栈的切换直接在指令 `iretd` 被执行时就完成了，目标代码的 `cs、eip、ss、esp` 等都是从堆栈中得到，很简单。但 `ring1` 到 `ring0` 切换时就要用到 `TSS` 。到目前为止，TSS对于我们的用处也只是保存 `ring0` 堆栈信息，而堆栈的信息也就是 `ss` 和 `esp` 两个寄存器。

上一节中，为了搭建一个进程调度的大致框架，我们已经做了一些TSS的初始化工作，并且已经给TSS中用于 `ring0` 的 `ss` 赋了值（代码6.15第101行），那么，`tss.esp0` 应该在什么时候被赋值呢？由于要为下一次 `ring1→ring0` 做准备，所以用 `iretd` 返回之前要保证 `tss.esp0` 是正确的。

**当进程被中断切到内核态，当前的各个寄存器应该被立即保存**（压栈）。每个进程在运行时， `tss.esp0` 应该是**当前进程的进程表中保存寄存器值的地方**，即 `struct s_proc` 中 `structs_stackframe` 的**最高地址处**。这样，进程被挂起后才恰好保存寄存器到正确的位置。我们假设进程 `A` 在运行，那么 `tss.esp0` 的值应该是进程表 `A` 中 `regs` 的最高处，因为我们不可能在进程 `A` 运行时来设置 `tss.esp0` 的值，所以必须在 `A` 被恢复运行之前，即 `iretd` 执行之前做这件事。即在时钟中断处理结束之前做这件事：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200724011129270.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

在这里不仅增加了给 `tss.esp0` 赋值的语句，而且还额外增加了几句代码：
- `sub/add esp` 这两句代码实际上是跳过了4字节，结合进程表的定义知道，这里跳过的就是 `retaddr` ，先不管这个值。
- 另外3行 `mov` 是令 `ds, es` 指向与 `ss` 相同的段。

现在中断例程变成了这样：在中断发生的开始，`esp` 的值是刚刚从TSS里面取到的进程表 `A` 中 `regs` 的**最高地址**，然后各寄存器值被压栈入进程表，最后 `esp` 指向 `regs` 的**最低地址**，然后设置 `tss.esp0` 的值，准备下一次进程被中断时使用。

如今只有一个进程，第二次时钟中断之后对 `tss.esp0` 的赋值就是在重复。但以后实现多个进程，在进程 `B/C` 将要获得CPU之前，`tss.esp0` 的值会被修改成进程表 `B/C` 中相应的地址。

这里可能会想，刚开始添加两行置 `EOI` 位的地址代码时中断就已经打开，从那时起就存在了 `ring0` 到 `ring1` 的切换，可直到现在才把 `tss.esp0` 的值补全。当前面的程序发生 `ring1→ring0` 跳转时，`esp` 一定指向了一个错误且有风险的地方。这里冒了一个险，因为我们在不知道 `esp` 指向何处时就使用了它。

### 4. 内核栈
曾经提到过内核栈的问题，如今这个问题真的出现了。现在 `esp` 指向的是进程表，如果此时执行复杂的进程调度程序呢？最简单的例子，如果想调用一个函数，这时一定会用到堆栈操作，那么，进程表立刻会被破坏掉。所以需要切换堆栈，将 `esp` 指向另外的位置。

在引入内核栈时曾经说过，**在具体编写代码的过程中，一定要清楚当前使用的是哪个堆栈，以免破坏掉不应破坏的数据。** 现在到了该用内核栈的时候：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200724012216875.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
切到内核栈和重新将 `esp` 切到进程表都很简单，一个 `mov` 语句就够了，但是它却**非常关键**。如果没有这个简单的 `mov` ，随着中断例程越来越大，出错的时候可能都不知道错在哪里。

在这里尽可能地把代码放在使用内核栈的过程中来执行，只留下跳回进程所必需的代码。是想暗示，**使用内核栈后我们的中断例程可以变得很复杂**。不妨在这里试一下，把这段打印字符的代码替换成使用 `DispStr` 函数：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200724012550479.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
其中，`clock_int_msg` 仅仅是个字符 `^` 。`make lastone image` 并运行的话，看到如下场景(时钟中断模块运行正常)：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200724013116343.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
看到不断出现的字符 `^` ，说明函数 `disp_str` 运行正常，而且没有影响到中断处理的其他部分以及进程 `A` 。之所以两次字符 `A` 的打印中有多个 `^` ，是因为进程执行体中加入了 `delay( )` 函数，在其执行过程中发生了多次中断。

### 5. 中断重入
从开始只有一句 `iretd` 的中断处理程序到现在，我们已经增加了许多内容。而且知道，在**保存好寄存器的值和使用了内核栈**之后，可以将更加复杂的内容添加进去。

但是，由于中断处理程序的内容变得愈来愈复杂，我们是否应该允许中断嵌套？也就是说，在中断处理过程中，是否应该允许下一个中断发生？**不允许肯定不行**，因为你一定不希望在进程调度时按键就不再响应。因此必须用合适的机制来应付中断嵌套。修改一下代码，让系统可以在时钟中断的处理过程中接受下一个时钟中断，借此来做个试验。

因为CPU在响应中断的过程中会自动关闭中断，我们需要人为打开中断，加入 `sti` 指令；同时，为保证中断处理过程足够长，以至于在它完成之前就会有下一个中断产生，我们在中断处理例程中调用一个延迟函数。具体修改如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200724013717280.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

 
`make one image` 并运行会发现，在打印了一个 `A0x0` 之后就不停打印 `^` ，再也进不到进程里面：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200724014142467.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

**由于中断重入的发生**，在一次中断还未处理完时，又一次中断发生了，程序又跳到中断处理程序的开头，如此反复，永远也执行不到中断处理程序的结尾——**进程挂起后无法再被恢复**。而且，由于压栈操作多而出栈操作少，当堆栈溢出的时候，意料不到的事情就可能发生。

中断处理程序是被动的，它只知道当忠实的中断发生时执行那段代码，完全不理会中断在何时发生。可为了避免这种嵌套现象的发生，我们必须想一个办法让中断处理程序知道自己是不是在嵌套执行。

这个问题并不难解决，只要设置一个全局变量即可，它有一个初值 `-1` ，当中断处理程序开始执行时它自加，结束时自减。在处理程序开头处这个变量需要被检查一下，如果值不是 `0(0=-1+1)` ，则说明在一次中断未处理完之前又发生了一次中断，这时直接跳到最后，结束中断处理程序的执行。

当然，武断结束新的中断并不是一个好的办法，但在这里，我们按照这个思路把程序修改一下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200724014443555.png)
然后在中断例程中加入 `k_reenter` 自加以及判断是否为 `0` 的代码：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200724014550100.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

再 `make two image` 一下，运行，结果如图所示：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200724014657536.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
可以看到，字符 `A` 和相应的数字又在不停出现，这说明修改生效了。而且可以发现，屏幕左上角的字母跳动速度快而字符 `^` 打印速度慢，说明很多时候程序执行了 `inc byte [gs:0]` 之后并没有执行 `disp_str` ，这表明中断重入的确发生了。

如果想做对比的话，可以执行一下 `chapter6/b` 中的程序，会发现打印 `^` 的速度和左上角的字母跳动的速度是一样的。

我们已经有了一个办法来解决中断重入这个问题，那么注释掉刚才的打印字符以及 `Delay` 等语句：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200724014958454.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200724015003942.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
`make three image` 再次运行，结果如图所示：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200724015206416.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

可能非常眼熟，在没有考虑中断重入的时候，画面跟现在是一样的。但是如今我们的代码考虑了更多的情况，可以适应更多的条件变化，它比原先更加成熟。

---
# 6.4 多进程
此时，我们的进程不仅是在运行而已，它可以随时被中断，可以在中断处理程序完成之后被恢复。**进程已经有了两种状态：运行和睡眠**。那么多个进程也是如此：我们让其中一个进程处在运行态，其他进程处于睡眠态。


## 6.4.1 添加一个进程体
在 `main.c` 中进程 `A` 的代码下面添加进程 `B` ，除了打印的字母换成 `B` ，`i = 0x1000` 外，其余都不变：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200723155035839.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

## 6.4.2 相关的变量和宏
进程的4要素：进程表、进程体、GDT、TSS。下一步对进程表初始化。到 `kernel_main( )` 函数中看一下进程 `A` 的初始化工作是怎么做的——将其中的几个关键成员赋值就可以了。

Minix中定义了一个数组叫做 `tasktab` ，这个数组的每一项定义好一个任务（在本章中任务和进程可以互换）的开始地址、堆栈等，在初始化的时候，只要用一个 `for` 循环依次读取每一项，然后填充到相应的进程表项中就可以了。

我们首先在 `proc.h` 中声明一个结构体类型：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200723155403305.png)

其中的 `task_f` 定义为 `type.h` ：
```c
typedef void (*task_f)();
```
一个进程只要有一个进程体和堆栈就可以运行了，所以这个数组只要有前两个成员就够了。不过，这里还定义了 `name` ，以便给每个进程起一个名字。

下面在 `global.c` 中增加定义：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200723155614611.png)

另外同时在 `global.h` 中加入：
```c
extern TASK task_table[];
```
当初就考虑到了以后的扩充，虽然只有一个进程，还是安排了一个进程表数组 `proc_table[NR_TASKS]` ，只是 `NR_TASKS` 为 $1$ 。进程表里有几项 `task_table` 也应该有几项。我们已经有两个进程了，所以先把 `NR_TASKS` 的值修改为 `2` ，并添加 `STACK_SIZE_TESTB` 的定义：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200723163052741.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

最后，在 `proto.h` 中加入 `TestB` 的声明：
```c
void TestB();
```
围绕 `task_table` 与新添加进程相关的定义已经完成，接下来做进程表的初始化工作。

## 6.4.3 进程表初始化代码扩充
用 `for` 循环初始化进程表。每一次循环的不同在于，从 `TASK` 结构中读取不同的**任务入口地址、堆栈栈顶和进程名**，然后赋给相应的进程表项。

需要注意的地方有两点：
- 由于堆栈是从高地址往低地址生长的，所以在给每一个进程分配堆栈空间的时候也是从高地址往低地址进行。
- 这里，为每一个进程都在GDT中分配一个描述符用来对应进程的LDT。在 `protect.h` 中可以看到，`SELECTOR_LDT_FIRST` 是GDT中被定义的最后一个描述符。但是正如它的名字所表示的，它仅仅是"第一个"和"唯一一个"被明白指出来的而已。实际上，我们在 `task_table` 中定义了几个任务，通过上文的 `for` 循环中的代码，GDT中就会有几个描述符被初始化，它们列在 `SELECTOR_LDT_FIRST` 之后。
- 此外，对于进程的名称（`p_proc->p_name`）和序号（`p_proc->pid`）的设置，目前来看并没有什么实际的作用。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200723172655771.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200723172701219.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

## 6.4.4 LDT
每一个进程都会在GDT中对应一个LDT描述符。在 `for` 循环中，我们将每个进程表项中的成员 `p_proc->ldt_sel` 赋值。可是，选择子仅仅是解决了 `where` 问题，通过它，我们能在GDT中找到相应的描述符，但描述符的具体内容是什么，即 `what` 问题还没有解决。

当初只有一个进程时，我们是在 `init_prot( )` 这个函数中通过一个语句解决了 `what` 的问题。现在，我们同样需要把它变成一个循环：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200723173221837.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
另外，每个进程都有自己的LDT，所以当进程切换的时候需要重新加载 `ldtr` 。


## 6.4.5 修改中断处理程序
这里，每次发生时钟中断的时候，被恢复的进程还是原来的 `A` ，我们还没有编写任何进程切换代码。所以，现在运行没有什么效果。 

时钟中断处理程序位于 `kernel.asm` 中，除了保存和恢复进程信息，我们只做了一件简单的事，就是在屏幕上打印一个字符 `^` 。显然，进程切换的代码就应该添加在这个位置才对。

回忆一下，一个进程如何由 `睡眠` 状态变成 `运行` 状态？无非是将 `esp` 指向进程表项的开始处，然后在执行 `lldt` 之后经历一系列 `pop` 指令恢复各个寄存器的值。一切信息都包含在进程表中，所以，要想恢复不同的进程，只需要将 `esp` 指向不同的进程表就可以了。

在离开内核栈的时候，有一个语句是为 `esp` 赋值的：
```swift
mov  esp, [p_proc_ready]
```
全局变量 `p_proc_ready` 是指向进程表结构的指针，**只需要在这一句执行之前把它赋予不同的值就可以了**。

进程切换是一个有点复杂的过程，因为涉及进程调度等内容。一方面，这涉及算法等一些复杂的内容，另一方面，它应该是与硬件无关的，所以我们用C语言编写这个模块。

那么，代码如何组织呢？这块内容既是时钟中断的一部分，又关乎进程调度。所以我们可以创建一个 `clock.c` ，也可以创建一个 `proc.c` 。这里学习Minix，创建一个 `clock.c` 。目前 `clock.c` 里面只有一个函数，且只有一个打印字符 `#` 的语句：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200723173730486.png)

在时钟中断例程中调用这个函数：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200723174023694.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

由于增加了一个文件，所以修改Makefile。 `make` ，运行。图中打印的字符 `#` 说明我们刚刚所增加的代码已经在正确运行。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200723174713390.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
进行进程切换：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200723174200176.png)

每一次我们让 `p_proc_ready` 指向进程表中的下一个表项，如果切换前已经到达进程表结尾则回到第一个表项。最关键的这几行添加完后就可以看到结果了：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200723174921591.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

多进程实现了！我们看到了交替出现的 `A` 和 `B` ，还有各自不断增加的数字。这表明我们的第二个进程运行成功了，我们已经成功实现了多进程。

> 到此为止，一个多进程的框架已经基本完成，在此基础上，你可以**方便地添加任务，并且方便地设计调度算法**对这些任务进行管理。**从此以后，操作系统课本上的调度算法不再是空洞的理论，而变成了你手中可以随意指挥和试验的代码**，任由驱使，近在眼前。


---
## 6.4.6 添加一个任务的步骤总结
现在有了两个进程在运行了，再添加一个进程就是量的积累。
- 先添加一个进程体：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2020072315000348.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
- 在 `task_table` 中添加一个进程：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200723150027842.png)

- 然后是 `proc.h` ，让 `NR_TASKS` 加 $1$ ，定义任务堆栈，修改 `STACK_SIZE_TOTAL`。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200723150311359.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
- 在 `proto.h` 中添加函数声明：`void TestC();` 。
- `make` 并运行，结果如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200723154200977.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

简单几步就成功地添加了一个任务，添加任务的步骤总结如下：
1. 添加一个进程体；
2. 在 `task_table` 中增加一项（`global.c`）。
3. 让 `NR_TASKS` 加 $1$（`proc.h`）。
4. 定义任务堆栈（`proc.h`）。
5. 修改 `STACK_SIZE_TOTAL`（`proc.h`）。
6. 添加新任务执行体的函数声明（`proto.h`）。

除了任务本身的代码和一些宏定义之外，原来的代码几乎不需要做任何改变，代码的自动化程度还是不错的。

---
## 6.4.7 号外：Minix的中断处理
无论是何种系统的中断例程，都会**在开始处保存当前进程的信息，在结束处恢复一个进程**，中间也会遇到**中断重入、内核栈**的问题等。即整个框架有相似性。由于这种性质，所有的中断例程就有一种统一起来的方法，**这种统一充满了美感**。

下面看一下Minix的代码，理解它的中断处理机制：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200723144843857.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)![在这里插入图片描述](https://img-blog.csdnimg.cn/20200723145552262.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

![在这里插入图片描述](https://img-blog.csdnimg.cn/202007231454217.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

代码后部分的 `_hwint00、_hwint01` 等就是中断例程的入口，所有的中断例程都使用 `hwint_master(irq)` 这个宏。跟函数比较起来，**使用宏虽然浪费了一些空间，但是由于避免了使用函数所必需的压栈、出栈，所以节省了时间**。

 `hwint_master` 的整个过程：
 - 首先调用一个函数 `save` 将寄存器的值保存起来；
 - 然后操纵8259A避免在处理当前中断的同时发生同样类型的中断；
 - 紧接着，给8259A的中断命令寄存器发出中断结束命令（ `EOI` ）；
 - 然后，用 `sti` 指令打开中断，调用函数 `(*irq_table[irq])(irq)` ，这是与当前中断相关的一个例程；
 - 再用 `cli` 关中断、`test` 指令判断函数 `(*irq_table[irq])(irq)` 的返回值。如果非零的话就**重新打开当前发生的中断**（比如发生的是时钟中断就重新打开时钟中断）；如果是零的话就直接 `ret` 。 

`save` 的代码如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200723144921242.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
这段中大部分的代码都是我们在中断例程中实现过的，只是需要注意的是，**如果发生中断重入的话，就跳过切换内核栈的代码（因为已经在内核栈了），并且把不同的地址压栈。** 接下来的 `jmp` 指令也有点令人奇怪，跳到了 `[eax+RETADR-P_STACKBASE]` 处，那么 `eax` 是什么呢？向前找到第372行，这一行把 `esp` 的值赋给 `eax` ，那么 `esp` 的值是什么呢？由于当时刚刚做完寄存器的保存工作，所以 `esp` 恰恰指向进程表的起始地址。

关于 `RETADR-P_STACKBASE` ，让我们看一下 `RETADR` 和 `P_STACKBASE` 的定义：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200723145116539.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
原来 `RETADR-P_STACKBASE` 就是执行 `call save` 这条指令时**压栈的返回地址相对于进程表起始地址的偏移**。所以 `[eax+RETADR-P_STACKBASE]` 就是返回地址，即 `inb INT_CTLMASK` 这条指令的地址。

`jmp RETADR-P_STACKBASE(eax)` 实际上是从 `save` 函数返回，继续从 `inb INT_CTLMASK` 向下执行。

另外，中断重入与否的区别**除了是否切换内核栈之外，还有一个push语句也不相同。** 我们拿其中一种情况来看一下，假设非中断重入，将会执行 `push _restart` 这一句。这是什么意思呢？还记得 `hwint_master(irq)` 最后的 `ret` 吗？原来 `ret` 指令是要跳转到 `_restart` 处。好了，我们就来看看 `_restart` 做了些什么：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200723145312273.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
这里就已经明白了一切，因为我们的代码几乎与它相同，不必多说了。相应地，如果发生中断重入的话，就会跳到 `restart1` 处执行，不再进行进程的切换。

虽然这些代码看上去不多，但每一句代码都很重要，甚至有着深意。 我们要理解它的由来。

---
## 6.4.8 代码回顾和整理
Minix这部分代码则不但显得优雅，而且思路更加清晰，我们对代码进行改造。

首先，我们有了 `restart` ，而且和中断例程的最后一部分基本一致，所以合二为一。先修改中断例程：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200723175325899.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)![在这里插入图片描述](https://img-blog.csdnimg.cn/20200723175331633.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

注意，这里不仅仅是形式上的修改，内容也有变化：原先的程序当发生中断重入的时候是不执行 `clock_handler` 的，现在总是在执行。

所以，我们还需要在 `clock_handler` 中稍做修改：当发生中断重入的时候，直接返回，返回前打印字符 `!` ，以便直观看到中断重入的发生。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200723175420743.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
此时，`make` 并运行，没有变化：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200723194407792.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

再来修改 `restart` 。为了将来合二为一，要将它修改得几乎与中断例程中的最后一段一模一样，增加了一行代码和一个标号。注意，既然在进程第一次运行之前执行了 `dec dword [k_reenter]` ，就必须把 `k_reenter` 的初始化值修改一下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200723175654960.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
现在对比代码6.48和代码6.51就会发现，两段代码的最后部分除了标号的名字不同，其余都是相同的，完全可以删掉其中一段，把代码6.48 `hwint00` 中的最后一段代码删掉，同时修改用到标号 `.restart_v2, .restart_reenter_v2` 的地方：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200723175822210.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200723175843109.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

 在这里如果编译并运行，看到的现象与原先别无二致：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200723195056695.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)


但是原来长长的中断例程，如今已经被分离出了一个 `restart` 。现在我们再来分离 `save` 。当准备开始时，注意开头第一个语句：
```swift
sub esp, 4
```
这个语句是跳过了进程表中的一个成员。如今读过了Minix的代码，应该已经明白了，这个成员其实就是由 `call save` 语句产生的、被压栈的返回地址。现在就把开头这部分代码挪进 `save` 函数中：
- 这个 `save` 与我们以前的函数不同，一般的函数最后都是以 `ret` 指令结尾，跳回调用处继续执行，**因为函数所使用的堆栈最后都被释放了**，调用时 `call` 指令的下一条指令地址被压栈，最后 `ret` 指令将这条指令从堆栈中弹出，函数调用前后 `esp` 的值是一样的。
- 可是我们这里的 `save` 函数则不同，调用前后 `esp` 的值是完全不同的，甚至是否发生中断重入也影响着 `esp` 的值。所以**必须事先将返回地址保存起来**，最后用 `jmp` 指令跳转回去。
- 在下面这段代码中，注释被写成了C语言的格式，看起来更清晰：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200723180151120.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
这里运行如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200723194748667.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)


`save` 准备好后，继续修改中断例程，添加了两段代码，在调用 `clock_handler` 之前屏蔽掉时钟中断，在调用之后重又打开。这样，**只打开时钟中断的时候不再会发生中断重入**。但可以预料，当其他中断被打开的时候，中断重入的情况仍然可能出现，我们对它的处理仍有必要：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200723180600924.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
再次运行：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200723195246850.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)


到这里，我们的时钟中断处理程序已经和Minix的 `hwint_master` 差不多了。现在将它替换成一个类似的宏，替换原有的宏并修改中断例程：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200723180931188.png)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20200723180915714.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
这里，引入了一个函数指针数组 `irq_table` （定义在 `global.c` 中）：
```swift
PUBLIC irq_handler irq_table[NR_IRQ];
```
别忘了在 `global.h` 中加入声明：
```swift
extern irq_handler irq_table[ ];
```
其中，`irq_handler` 在 `type.h` 中这样定义：
```swift
typedef void (*irq_handler) (int irq);
```
这与我们的 `clock_handler` 类型是完全一致的。

`NR_IRQ` 的值定义为 `16` ，以对应主从两个8259A（定义在 `const.h` 中）：
```swift
#define NR_IRQ 16
```
现在，虽然已经定义了 `irq_table` ，但它还没有被赋值，我们需要有 `16` 个函数来初始化它，目前只有一个 `clock_handler` ，因此把剩余的元素全部赋值为 `spurious_irq` 。 即，这项工作分为两部分，首先将所有的元素初始化为 `spurious_irq` ，然后写一个函数 `put_irq_handler` 单独为 `irq_table[0]` 时钟中断赋值：
 
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200723193546234.png)![在这里插入图片描述](https://img-blog.csdnimg.cn/20200723193513287.png)

上面，我们新增的一个函数 `disable_irq` 和将要使用的函数 `enable_irq` 都定义在 `klib.asm` 中：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200723193821290.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)![在这里插入图片描述](https://img-blog.csdnimg.cn/20200723193830181.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

现在在 `kernel_main()` 中指定时钟中断处理程序，并让8259A可以接收时钟中断：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200723193918886.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
当然，先要在 `init_8259A( )` 中屏蔽8259A的所有中断：
```c
out_byte(INI_M_CTLMASK, 0xFF);
```
到此为止，代码的修改就告一段落了。编译并运行：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200723195448872.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
结果虽然跟原先大致相同，但是现在的代码**不但更有条理，而且更容易扩展**。现在我们完成的绝不仅仅是一个时钟中断处理程序而已，同时也是**一套方便扩展的中断处理的接口**。若想添加某个中断处理模块，只需要将完成中断处理的函数入口地址赋给irq_table中相应的元素就够了，而且这个函数已经完全可以用C语言编写。

现在的Orange'S已经可以随意地增加进程的数目，已经预留了足够方便的中断处理接口。虽然它仍算不上是完整意义上的操作系统，但是一个粗糙的框架已经形成。回忆一下我们如何走到这一步的：

![在这里插入图片描述](https://img-blog.csdnimg.cn/20200723011715370.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
**顺着这个图表把整个过程重新过一遍的话，涉及的代码并没有感觉上那么多，但是，要彻底把它写出来并不是一件非常容易的事情**，其中最困难的就是**时钟中断处理程序围绕进程表项进行进程切换**的过程，这一点我们已经深有体会。还好我们一步一步走过来了；而且，可以想见，**时钟中断处理程序在以后应该不需要很大的改变**。

不过，这个系统无论如何还是非常幼稚的。原因在于在进程本身方面考虑得还比较少，比如，未曾考虑过**进程优先级、进程间通信**的问题等。

现在让我们对照上图来想像一下，如果增加这些内容的话，大致应该是怎样的情形：
- 切换到 `Kernel` 的GDT的代码通常情况下是不需要改动的；
- `init_prot( )` 中的 `init_8259A( )` 是比较稳定的代码；
- `kernel_main( )` 结构很简单，但由于在这里初始化了进程表，所以若对进程功能进行扩展的话，会有一些改动。

在本来就不多的代码中，只想到一处地方可能会在进程功能扩展时有所改动，这真是一件让人高兴的事情，这意味着，等到下一次想要进一步完善它的时候，上手也会比较容易，因为接口已经足够简单了。

---
# 6.5 系统调用
在Windows中，应用程序通过调用API与操作系统建立联系，比如弹出一个对话框使用 `MessageBoxA` 。在Linux中，我们使用系统调用来完成这样的工作。

我们创造出的3个进程运行在 `ring1` 上，它们不能访问某些权限更高的内存区域，这时只能通过系统调用来完成工作。**系统调用是应用程序和操作系统之间的桥梁**，使用系统调用肯定会涉及到特权级的变换。

不过，进程的切换也是在不停地重复这么一个特权级变换的过程，二者没有多少区别。触发进程切换的是外部中断，这里我们把它变成 `int nnn` ，一切就解决了。
## 6.5.1 实现一个简单的系统调用
我们将实现一个 `int get_ticks( )` 的函数，用来得到当前总共发生了多少次时钟中断。设置一个全局变量 `ticks` ，每发生一次时钟中断就加一。进程可以随时通过这个系统中断得到 `ticks` 。

系统调用的过程是：`问` ，告诉操作系统自己要什么；然后是操作系统 `找` ，即处理；最后是 `回答` ，也就是把结果返回给进程。

用中断可以方便地实现系统调用。我们的 `get_ticks( )` 如下：
- `eax = _NR_get_ticks` ，OS看到当前的 `eax` 是 `_NR_get_ticks` ，就知道问题是"请问当前的 `ticks` 是多少"；
- 系统中断号设为 `0x90` ，只要不和原来的中断号重复即可；

![在这里插入图片描述](https://img-blog.csdnimg.cn/20200722222715213.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

在 `init_prot( )` 中，定义 `INT_VECTOR_SYS_CALL` 对应的中断门，将这号中断和 `sys_call` 对应起来，紧跟着初始化其他中断门的语句：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200722222818990.png)
如何实现 `sys_call` ？模仿 `hwint_master` 宏的做法：先保存寄存器的值，然后调用相关函数，最后返回。下面是 `save` ，原来有一条语句 `mov eax, esp` ，不过这里的 `eax` 中存放着进程询问操作系统的问题，所以将 `eax` 全部改为 `esi` ：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200722223119770.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

写的 `sys_call` 如下，它基本上是 `hwint_master` 的简化，`hwint_master` 中是 `call [irq_table+4*%1]`（即调用了 `irq_table[%1]` ），这里变成了 `call [sys_call_table+eax*4]`（调用的是 `sys_call_table[eax]` ）。类似 `irq_table` ，`sys_call_table` 是一个函数指针数组，每个成员都指向一个函数，用于处理对应的系统调用。
![在这里插入图片描述](https://img-blog.csdnimg.cn/2020072222354825.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
`sys_call_table` 定义在 `global.c` 中，只有一个成员：
```css
PUBLIC system_call sys_call_table[NR_SYS_CALL] = {sys_get_ticks};
```
`system_call` 定义在 `type.h` 中：
```css
typedef void* system_call;
```
前面 `eax` 已被赋值为 `_NR_get_ticks`（即0），而 `sys_call_table[0]` 已经初始化为 `sys_get_ticks` ，所以 `call [sys_call_table+eax*4]` 这一句调用的便是 `sys_get_ticks` 。由于 `ticks` 与进程相关，单独建立一个文件 `proc.c` ，把 `sys_get_ticks` 放在里面。暂时让这个函数最简，打印一个字符 `+` 后就返回，不做其他任何操作：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200722224217598.png)

如果 `mov [esi+EAXREG-P_STACKBASE], eax` 这一句还是不明白，回顾一下 `save` 中的 `jmp [esi+RETADR-P_STACKBASE]` 就知道了，其实它把函数 `sys_call_table[eax]` 的返回值放在进程表中 `eax` 的位置，以便进程 `P` 被恢复执行时 `eax` 中装的是正确的返回值。

下面在 `proto.h` 中添加函数声明：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200722224444567.png)

现在在进程中添加调用 `get_ticks` 的代码了，`TestA` 中添加如下语句：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200722224946630.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
别忘了在 `kernel.asm` 和 `syscall.asm` 中导入和导出相应符号，并且修改 `Makefile`（增加了一个文件 `proc.c` ）。然后就可以 `make` 并运行了，结果如图所示：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200722225500425.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
这里，加号出现在 `'A'` 的前面，所以第一个系统调用成功了。然后改进函数 `sys_get_ticks( )` ，实现真正的效果，返回当前的 `ticks` 。

先在 `global.h` 中定义全局变量：
```cpp
EXTERN int ticks;
```
在 `main.c` 中初始化：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200722231913867.png)

在 `clock_handler(int irq)` 中添加：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200722231910221.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
修改 `sys_get_ticks( )` ：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200722231938882.png)

最后修改 `TestA( )` ，打印当前的 `ticks` ：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200722232043857.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

Linux中接近我们的 `ticks` 的是 `jiffies` ；Minix中，则是 `ticks` 。我们这里更接近Minix。运行，如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200722232750466.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
我们成功增加了一个系统调用，但注意，虽然在学习Minix，但Minix的系统调用并不是这样，它实际上只有3个系统调用：`send, receive, sendrec` ，并以此为基础建立了一套消息机制，需要系统支持的功能都是通过这套消息机制来做到的，所以它是微内核的。

在这里，把 `get_ticks` 这样一个普通的功能直接用系统调用来实现，看上去与Minix不同，有点像Linux的宏内核。之所以这么做，完全是因为在目前的基础上如此实现一个 `get_ticks` 函数最为简单，而且又能说明系统调用的原理。这既不代表作者在微内核、宏内核这个问题上的立场，也不代表Orange'S将来会试图发展成宏内核。

---
## 6.5.2 get_ticks的应用
`get_ticks` 和 `sys_get_ticks` 足够简单，但是却有它的用途——时钟中断发生的时间间隔是一定的，如果我们知道了这个时间间隔，就可以用 `get_ticks` 函数来写一个判断时间的函数，进而替代使用过的丑陋的 `delay( )` 。

### 1. 8253/8254 PIT
时钟中断不是凭空产生的，它是由一个被称做 `PIT, Programmable Interval Timer` 的芯片来触发的。在 `IBM XT` 中，这个芯片用的是 `Intel 8253` ，在 `AT` 以及以后换成了 `Intel 8254` 。8254功能更强一些，但这里并不一定涉及增强的功能。因此我们只称呼它8253。8253有 $3$ 个计数器 `Counter` ，它们都是 $16$ 位的，各有不同的作用：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2020072223341918.png)
时钟中断实际上是由8253的Counter 0产生的。计数器的工作原理是这样的：
- $\text{GATE}_0$ 接 `+5V` ，使计数器 `0` 处于常开状态，有一个输入频率，在PC上是 `1193180Hz` 。8253开机初始化后就一直计数，提供系统时间基准；
- 计数器0用作定时器，初始化时选方式 `3` ，二进制计数，初值 `n=0` (`65536`)，在 $\text{OUT}_0$ 形成方波； 
- 在每一个时钟周期（`CLK cycle`），计数器值会减 `1` ，当减到 `0` 时，就会触发一个输出；
- 因此，默认的时钟中断的发生频率就是 $\text{f/n = 1.19318MHz/65536 = 18.2Hz}$ 。此方波经系统总线 `IRQ0` 送到8259A的 `IR0` ，使CPU每秒产生 `18.2` 次中断( `55ms` /次)。
- CPU以此为时间基准，**在中断服务程序中对中断次数计数，就可形成实时时钟**，例如中断 `100` 次即为 `5.5s` 。适用于对时间精度要求不是很高的场合。

我们可以通过编程来控制 `8253` 。**因为如果改变计数器的计数值，那么中断产生的时间间隔也就相应改变了**。如果想让系统每 `10ms` 产生一次中断，也就是让输出频率为 `100Hz` ，那么需要为计数器0赋值为 `1193180/100≈11931` 。

改变计数器的计数值是通过对相应端口的写操作来实现的。看一下8253的端口情况，如表6.2所示： ![在这里插入图片描述](https://img-blog.csdnimg.cn/20200723002236356.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
改变Counter 0计数值需要操作端口 `40h` 。但是我们需要先通过端口 `43h` 写8253模式控制寄存器。先来看一下它的数据格式：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2020072300263117.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
读/写/锁位如下，锁住当前计数器值不是让计数停止，仅仅是为了方便读取；如果不锁住就读取，才会影响计数：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200723002742188.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
计数器选择位如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200723002857539.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
现在，如何写模式控制寄存器就很简单了。我们要操作的是Counter 0，所以第 `7, 6` 位应该是 `00` ；计数值是 `16` 位的，所以低字节和高字节都要写入，于是第 `5, 4` 位应该是 `11` ；使用模式 `2` ，所以第 `3, 2, 1` 位应该是 `010` ；第 `0` 位设为 `0` 。这样，整个字节就变成 `00110100` ，也就是十六进制的 `0x34` 。

下面来设置计数值使得系统每 `10ms` 产生一次中断，以及相关的宏定义：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200723003438687.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)![在这里插入图片描述](https://img-blog.csdnimg.cn/20200723003708195.png)

---
### 2. 不太精确的延迟函数
上面，我们已经把两次时钟中断的间隔改成了 `10ms` ，如果现在运行程序，将会在很短的时间内打印出很多 `#` ，因为中断发生快了很多。原来一秒钟 `18.2` 次中断，大约 `55ms` 发生一次；现在一秒钟 `100` 次，`10ms` 发生一次，所以区别才会这么明显。
 
现在编写新的延迟函数，因为中断 `10ms` 发生一次，所以 `ticks` 也是 `10ms` 增加一次，延迟函数可以这样来写——函数一开始得到当前的 `ticks` 值，然后开始循环；每次循环的时候看已经过去了多少 `ticks` ，`ticks` 之间的间隔时间是 `1000ms/HZ=1000ms/100Hz=10ms` ，所以 $\Delta t$ 个 `ticks` 相当于 `Δt * 10ms` ，循环会在这个毫秒数大于要求的毫秒数 `milli_sec` 时退出：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200723004405329.png)
然后修改进程 `A` 的进程体，同时让进程 `B, C` 的进程体与此相似但打印的字母不同：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200723004728292.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
`make` 并运行：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200723004056479.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
从图中看出，**发生了很多次中断重入**。由于现在进入内核态**要么是发生了时钟中断，要么是调用了** `get_ticks` ，所以重入发生的唯一情况是调用 `get_ticks` 时发生了时钟中断。不过这倒不是什么大问题，我们已经有了相应的处理机制。

另外，可以根据打印出的 `ticks` 值来计算两次打印 `A` 之间发生了多少此中断，在上图中，第1次和第2次打印 `A` 之间发生了 `0x65 = 0x65-0x0`），也就是 `101` 次中断，这是比较精确的。

虽然中断 `10ms` 发生一次，但通过这种方式写出来的 `milli_delay` 误差却不止 `10ms` ，而是 `10ms级` 的。究其原因：
- 现在有不止一个进程在运行，当时间满足条件之后，CPU控制权可能恰好交给了其他进程，这时其他进程可能耗费掉若干的 `ticks` ；
- 另外，打印这些字符和数字也用掉一些 `ticks` 。

为了排除其他因素的影响，我们把进程数减为 `1`（可以通过修改 `NR_TASKS, task_table[NR_TASKS]` 来实现），然后把中断例程中打印 `#, !` 的代码也去掉，再运行一次，会发现每一次的间隔都是精确的 `0x64` ，也就是 `100` 个 `ticks` ：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200723011036649.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)


> 虽然存在误差的可能，精度不够高，但比起原来那个野蛮的循环，却已经有很大进步了。而且**我们把第一个系统调用派上了用场，同时还掌握了如何操作8253。** 这些收获无疑让这个不完美的函数价值大增。
> <b></b>
> 在不同的甚至是同一个操作系统中，都会有不同的延迟函数，这些函数实现的机制各不相同，而且有一些还很精妙：
> - `Linux` 中的 `udelay` ，是通过计算循环次数和时间之间的关系，用一定次数的循环来延迟一定的时间（我们当初的delay也是循环，不过太简陋了）；
> - `Minix` 中的 `milli_delay` 通过读取8253的计数值来得到比较精确的延迟时间，但是它只能运行在核心态。
> - 我们的延迟函数是新的发明，它不够精妙，但是足够简单，`milli_delay` 的函数体只有两行这是其他延迟函数做不到的。而且**它运行在用户态，使用十分方便**。

---
# 6.6 进程调度
## 6.6.1 避免对称——进程的节奏感
3个进程都延迟同样的时间，这里让它们延迟不同的时间—— `A,B,C` 分别延迟 `300,900,1500ms` ： 
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200722172927104.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
运行结果如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200722191204602.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

延迟的时间越长，输出就越少。这里进程干活的时间长短不一，暗合优先级的概念。**我们似乎是通过设置延迟时间的长短，给不同的进程赋予了不同的优先级。** 当然，这种方法不值得提倡。但是，既然延迟可以通过得到 `ticks` 实现，那么把延迟的过程放到进程调度模块中实现，不就实现了进程的优先级了吗？我们只需要稍微修改调度算法就可以了。
- 过去，我们在发生时钟中断选择下一个执行的进程时，直接选择进程表中的下一个进程，**这种时间片轮转的方式给了每个进程均等的机会**。
- 现在不再给每个进程以相等的机会了。具体的方法是，给每一个进程都添加一个变量（可以放在进程表中），在一段时间的开头，这个变量的值有大有小，**进程每获得一个运行周期，这个变量就减1，当减到0时，此进程就不再获得执行的机会，直到所有进程的变量都减到0为止。** 这样，每个进程获得的执行时间就不一样了。

我们现在修改一下代码，看看执行的结果怎么样。先修改进程表（代码6.77）。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200722192022347.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

在进程表中添加了两个成员：`ticks` 递减，从某个初值到 `0` 。为了记住 `ticks` 的初值，我们另外定义一个变量 `priority` ，它是恒定不变的。当所有的进程 `ticks` 都变为0之后，再把各自的 `ticks` 赋值为 `priority` ，然后继续执行。

`ticks` 和 `priority` 最初赋值如代码6.78所示。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200722192118708.png)
**进程调度**可以单独写一个函数 `schedule( )` ，放在 `proc.c` 中（代码6.79）：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200722192322291.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
同时，修改时钟中断处理函数：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200722192416571.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

然后，**我们将所有进程的延迟时间全改为相同的值**  `milli_delay = 200` 。`make` ，运行，出现如图6.29所示的情形。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200722194832668.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

我们看到，**虽然各个进程延迟的时间都相同，但由于改变了它们的优先级，运行的时间明显不同**，这说明优先级策略生效了！不过可以发现，当前的 `A、B、C` 三个字母的个数之比是 `139:71:54` ，大体相当于 `2.57:1.31:1` ，与进程优先级 `5:1.67:1（15:5:3）` 相差比较大。为什么会出现这样的情形呢？打印更多的信息来研究一下。

首先修改各个进程，用 `disp_color_str( )` 把 `A,B,C` 打印成红色，然后各自打印一个当前的 `ticks`（代码6.81）。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200722193730386.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

接着修改 `proc.c` 中的 `schedule( )` ，加上几条打印语句，注释掉为进程表中的成员 `ticks` 重新赋值的代码，让进程不至于永远执行（当所有进程的 `ticks = 0` 时程序停止），这样有利于观察（代码6.82）。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200722194012897.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

由于打印的东西比较多，在 `kernel_main( )` 中添加清空屏幕的函数，让输出从屏幕左上角开始，否则无法看到所有的输出（代码6.83）。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200722194058405.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
再次运行，结果如图6.30所示。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200722195329699.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

分析会发现，整个执行过程可以划分成 `3` 个阶段：最开始只有进程 `A` 自己在运行，后来 `A, B` 同时运行，再后来 `A, B, C` 同时运行，如表6.6所示。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200722195700287.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
表中，除了最右边一列"结果的产生原因"外，都是图6.30所示执行结果的真实记录。由于进程的每一次循环都延迟 `200ms`（`20ticks`），所以，在最开始的 `100ticks` 中，进程A循环 `5` 次，在后面的 `20×2ticks` 中每个进程循环 `2` 次，最后的 `30×3ticks` 中每个进程循环 `4/5` 次都很容易理解。


现在已经很明白了，在3个阶段中，最初阶段的时间跨度为 `100ticks` ，之后，由于进程 `A` 的 `ticks` 值已经小于 `50` ，已经与进程B的 `ticks` 值相当，所以以后就同时有 `A, B` 受到调度。在最后一个阶段，就变成 `A, B, C` 三个进程同时受到调度。

由于每一次进程调度的时候只有某一个进程的 `ticks` 会减 `1` ，所以，总共调度的次数应该是 `3` 个进程的 `ticks` 之和（`150+50+30=230`）。这个规律放在中间某个过程中也是适用的，比如到最后阶段，当 `A` 和 `B` 的 `ticks` 都减到 `30` 时，`3` 个进程同时运行，总共运行的时间将是 `30×3=90ticks` 。所以我们总结出：
- 进程A执行的循环数= `(100 + 20×2 + 30×3) / 20 = 230 / 20 = 11.5` 次进程
- B执行的循环数= `(0 + 20×2 + 30×3) / 20 = 130 / 20 = 6.5` 次进程
- C执行的循环数= `(0 + 0×2 + 30×3) / 20 = 90 / 20 = 4.5` 次

这个结论与我们的试验结果 `11:7:5` 是相吻合的。根据这个分析也可以知道，基于现在的调度算法，`A, B, C` 三个进程的执行时间之比，理论值应该是 `230:130:90` ，即 `2.56:1.44:1` 。我们两次的试验结果（图6.29和图6.30）结论都与此相吻合。

现在，从实践到理论，我们第一阶段的调度算法试验就算是结束了。可以看到，**虽然这种算法能分出定性的优先级关系**，但是从数字上不容易一下子看出各自执行的时间定量关系（`150:50:30` 和 `11:7:5` 是很不相同的）。

这就意味着，当我们给予一个进程某个优先级，需要经过计算才能知道它们各自运行的时间比例是多少。有必要在此基础上改进一下程序。其实，只要在 `clock_handler( )` 中添加一个判断，问题便告解决（代码6.84）。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200722200212966.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
这样，在一个进程的 `ticks` 还没有变成 `0` 之前，其他进程就不会有机会获得执行，结果如图6.31所示。
![在这里插入图片描述](https://img-blog.csdnimg.cn/2020072222091646.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

从上图可以明显看出，进程 `A` 先执行，然后是 `B` ，再然后是 `C` ，与原先有了很大的差别。原因在于 `A` 的 `ticks` 从 `150` 递减至 `0` 之后，才把控制权给 `B` ，`B` 用完它的 `ticks (50)` 之后再给 `C` ，然后各自的 `ticks` 被重置，继续下一个类似的过程。

可以看到，进程 `A` 在 `150ticks` 内执行 `8` 次循环，`B` 在 `50ticks` 内执行 `3` 次循环，`C` 在 `30ticks` 内执行 `2` 次循环。这样就很直观了。不过进程各自运行的时间有点长，把它们的优先级改小一些。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200722200533651.png)
然后把各个进程的延迟时间改成 `10ms` 。 
![在这里插入图片描述](https://img-blog.csdnimg.cn/2020072220024286.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
运行一下，结果如图所示。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200722221218654.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
可以看出，打印出的字符的个数之比非常接近 `15:5:3` 。

## 6.6.2 优先级调度
总结至此，**基于简单优先级的进程调度算法已经完成了**。它很简单，目前看来运行得还是可以的。

计算机世界跟现实世界在很多时候都是类似的，优先级调度也是来源于现实世界，所谓"轻重缓急"四个字，恰好可以用来表达优先级调度的思想。最重要的事情总是应该被赋予更高的优先级，应该给予更多的时间，以及尽早地进行处理。

在Minix中，进程分为任务（`Task`）、服务（`Server`）和用户进程（`User`）三种，进程调度也据此设置了 `3` 个不同的优先级队列，**目前并没有使用优先级队列来实现调度策略**，是因为一方面那样会使程序实现的复杂度大大增加，另一方面，目前的算法是在系统调用 `get_ticks( )` 的使用中顺理成章地形成，虽然它很简陋，但在"更早地处理"和"更多的时间"这两方面，都已经给予了高优先级的进程以很大的照顾。

毫无疑问，我们已经通过它进入了进程调度算法这个领域的大门。

