@[toc]
> **完美主义者常常因试图努力把一件事做好而放弃对新领域的尝试，从而使做事的机会成本增加**。有时候回头一看才发现，自己在某件事情上已花费了太多时间。而实际上，暂时的妥协可能并不会影响到最终完美结果的呈现。因为不但知识需要沉淀，事情之间也总是有关联的。
> 我们刚刚实现了简单的进程，你现在可能很想把它做得更加完善，比如进一步改进调度算法、增加通信机制等。但是这些工作不但做起来没有尽头，而且有些也是难以实现的，因为进程必须与I/O、内存管理等其他模块一起工作。
> **而且，简单的进程更有利于我们思考和控制。**

现在，我们就来实现简单的I/O系统。

---
# 7.1 键盘
到目前为止，我们的OS一旦启动就不再受控制了，只能等待结果的出现，**我们需要交互**。而交互的手段，**首先当然是键盘**。


## 7.1.1 从中断开始──键盘初体验
说起键盘，你可能想起8259A的IRQ1对应的就是键盘，在第5章中做过一个小小的试验（见代码5.57和图5.16）。那时我们没有为键盘中断指定专门的处理程序，所以当按下键盘时只能打印一行“spurious_irq:0x1”。

现在我们来写一个专门的处理程序。新建一个文件 `keyboard.c`，添加一个非常简单的键盘中断处理程序（代码7.1）。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200718133339928.png)

 结果是每按一次键，打印一个星号，像在输入密码。为了不受其他进程输出的影响，我们把其他进程的输出都注释掉。然后添加指定中断处理程序的代码并打开键盘中断（代码7.2）。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200718133518929.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
不要忘了在 `proto.h` 中声明 `init_keyboard( )` 并调用之（代码7.3）。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200718133550629.png)
修改Makefile之后，就可以 `make` 并运行了，如图所示，敲击一下键盘就可以看到出现星号：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200724020516393.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
但是出现一个星号之后键盘就不再响应了。事情比我们想像的要复杂一些，我们还是要从头说起。

## 7.1.2 AT、PS/2键盘
键盘 `PS/2` 键盘和 `USB` 键盘是当今最流行的两种键盘。现在，看看你的键盘连接计算机的接口，如果看上去很像图7.2左图的样子，那么你用的应该是个 `PS/2` 键盘；如果是一个有棱有角的扁口，那么它很可能是个 `USB` 键盘（如果你居然不熟悉USB口的话）。

再有一种可能，你还在使用一种稍微老一点的 `AT` 键盘，它看上去就像图7.2右图的形状。有人把AT键盘称为大口键盘，而把 `PS/2` 称为小口键盘。因为 `AT` 键盘的接口稍微大一些而 `PS/2` 的稍微小一些。它们的接口分别叫做 `5-pin DIN` 和 `6-pin Mini-DIN` 。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200718133933446.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

在更早时候，IBM还推出过 `XT` 键盘，它也使用 `5-pin DIN` ，不过现在早就不再使用了。如今的主流键盘主要是 `AT、PS/2、USB` 三种。在本书中，我们不讨论 `USB` 键盘，只介绍 `AT` 和 `PS/2` 两种。实际上，`PS/2` 键盘只是在 `AT` 键盘的基础上做了一点点的扩展而已，**在大多数情况下，尤其是在最初接触它们时，你可以认为它们是一样的**。

从设备管理器中可以发现，我的电脑使用的是 `PS/2` 键盘，对应的端口是 `60, 64` ，使用的是8255A的IRQ1：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200718135225540.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)


## 7.1.3 键盘敲击的过程
在键盘中存在一枚叫做键盘编码器 `Keyboard Encoder` 的芯片，它通常是 `Intel 8048` 以及兼容芯片，**作用是监视键盘的输入，并把适当的数据传送给计算机**。另外，在计算机主板上还有一个键盘控制器 `Keyboard Controller` ，用来**接收和解码来自键盘的数据**，并与8259A以及软件等进行通信（如图7.3所示）。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200718134147663.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)


敲击键盘有两个方面的含义：**动作和内容**。动作可以分解成三类：**按下、保持按住的状态以及放开**；内容则是**键盘上不同的键，字母键还是数字键，回车键还是箭头键**。所以，根据敲击动作产生的编码，**8048既要反映哪个按键产生了动作，还要反映产生了什么动作**。

敲击键盘所产生的编码被称作扫描码 `Scan Code` ，它分为 `Make Code` 和 `Break Code` 两类。**当一个键被按下或者保持住按下时，将会产生Make Code；当键弹起时，产生Break Code**。除了Pause键之外，每一个按键都对应一个 `Make Code` 和一个 `Break Code` 。

扫描码总共有三套，叫做 `Scan code set 1、Scan code set 2、Scan code set 3` 。`Scan code set 1` 是早期的 `XT` 键盘使用的，**现在的键盘默认都支持Scan code set 2**，而 `Scan code set 3` 很少使用。

整个键盘输入的过程如下所示：
- **当8048检测到一个键的动作后，会把相应的扫描码发送给8042**；
- **8042会把它转换成相应的** `Scancode set 1` **扫描码，并将其放置在输入缓冲区中**；
- **然后8042告诉8259A产生中断 IRQ1**；
- **如果此时键盘又有新的键被按下，8042将不再接收，一直到缓冲区被清空**，8042才会收到更多的扫描码。

现在，你一定明白了为什么图7.1中只打印了一个字符，因为我们的键盘中断处理例程什么都没做。**只有我们把扫描码从缓冲区中读出来后，8042才能继续响应新的按键**。

那么如何从缓冲区中读取扫描码呢？这就需要我们来看一看8042了。8042包含表7.1所示的一些寄存器。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200718134900100.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
> 注意：由于8042处在8048和系统的中间，所以输入和输出是相对的。比如，**8042从8048输入数据，然后输出到系统**。因此，表7.1中的出和入是相对于系统而言的。

对于(写8042)输入和(读8042)输出缓冲区，可以通过 `in` 和 `out` 指令来进行相应的读取操作。也就是说，一个 `in al, 0x60` 指令就可以读取扫描码了。

修改程序。在 `chapter7/a/kernel/keyboard_handler` 中添加下面一句：`in_byte(0x60);` 。运行，结果如图7.4所示：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200724021014347.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

**按一下键，出现两个星号，再按一下，又出现两个**。因为每次按键都产生一个 `Make Code` 和一个 `Break Code` 。

只打印星号显然不够有趣，我们把收到的扫描码打印一下看看，进一步修改`keyboard_handler`（代码7.4）。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200718135950168.png)
接下来在运行的时候按两个键：字符 `a` 和 `9` 。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200724021309459.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
总共出现4组代码：`0x1E, 0x9E, 0xA, 0x8A` 。对照表7.2发现，它们恰恰就是字符 `a` 和 `9` 的 `MakeCode` 和 `Break Code` 。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200718145311717.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200718145026842.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
> 注意：
> - 由于 `a` 和 `A` 是同一个键，所以它们的扫描码是一样的，事实上它们就是同一个键）。
> - 如果按下 `左Shift+a` ，将得到这样的输出：`0x2A0x1E0x9E0xAA` ，分别是左Shift键的 `Make Code` 、`a` 的 `Make Code` 、`a` 的 `Break Code` 以及左Shift键的 `Break Code` 。
> - 所以，按下 `Shift+a` 得到 `A` 是软件的功劳，键盘和8042是不管这些的，在你自己的操作系统中，甚至可以让 `Shift+a` 去对应 `S` 或者 `T` ，只要你习惯就行。
> - 同理，按下任何的键，不管是单键还是组合键，想让屏幕输出什么，或者产生什么反应，都是由软件来控制的。虽然增加了操作系统的复杂性，但这种机制无疑是相当灵活的。

同时也看到，虽然键盘支持的是 `Scan code set2` ，最终又转化成了 `Scan code set1` 。这是基于为 `XT` 键盘写的程序兼容性而考虑的。又是兼容性问题，你脑海里会飞速闪过A20、内存空洞、被分割得乱七八糟的描述符等一系列令人厌恶的回忆。是啊，有时候，软硬件的设计者对兼容性的考虑的确占去了太多的精力，可是又有什么办法呢？

不过，我们马上就可以根据扫描码来处理键盘输入了，**只要根据表格建立一种对应关系就够了**。因此，忘掉这些不快吧。

---
## 7.1.4 用数组表示扫描码
现在扫描码已被轻松获得，可是该如何将扫描码和相应字符对应起来呢？从表7.2中可以看出，`Break Code` 是 `Make Code | 0x80` 进行**按位或操作**的结果。可是 `Make Code` 和相应键的对应关系好像找不到什么规律。

不过还好，扫描码是一些数字，**我们可以建立一个数组，以扫描码为下标，对应的元素就是相应的字符**。要注意的是，其中以 `0xE0` 以及 `0xE1` 开头的扫描码要区别对待。

我们把这个数组写成如下这个样子。其中每3个值一组（ `MAP_COLS` 被定义成3），分别是**单独按某键、Shift+某键和有0xE0前缀的扫描码**对应的字符。`Esc, Enter` 等被定义成了宏，宏的具体数值无所谓，只要不会造成冲突和混淆，让操作系统认识就可以。 
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200718150421643.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200718150428553.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200718150438782.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
举个例子：
- 如果获得的扫描码是 `0x1F`，我们应该在代码7.5中很容易看到它对应的是字母`s` 。在写程序的时候，应该用 `keymap[0x1F*MAP_COLS]` 来表示 `0x1F` 对应的字符。
- 如果获得的扫描码是 `0x2A0x1E` ，它是左Shift键的 `Make Code` 和字符 `a` 的 `Make Code` 连在一起，说明**按下Shift还没有弹起的时候a又被按下**，因此应该用 `keymap[0x1E*MAP_COLS+1]` 表示行为的结果，即大写字母 `A` 。
- 存在 `0xE0` 的情况也是类似的，如果我们收到的扫描码是 `0xE00x47` ，就应该去找 `keymap[0x47*MAP_COLS+2]` ，它对应的是 `Home` 。

问题是，**8042的输入缓冲区大小只有一个字节，所以当一个扫描码有不止一个字符时，实际上会产生不止一次中断**。也就是说，如果我们按一下 `Shift+A` ，产生的 `0x2A0x1E0x9E0xAA` 是 `4` 次中断接收来的。这就给我们的程序实现带来了困难，因为第一次中断时收到的 `0x2A` 无法让我们知道用户最终会完成什么，说不定是按下Shift又释放，也可能是 `Shift+Z` 而不是 `Shift+A` 。

于是，当接收到类似 `0x2A` 这样的值的时候，需要先把它保存起来，随后慢慢解析用户到底做了什么。

**保存一个字符可以用全局变量来完成**。可是，由于扫描码的值和长度都不一样，这项工作做起来可能并不简单。而且可以想像，键盘操作必将是频繁而且复杂的，**如果把得到扫描码之后相应的后续操作都放在键盘中断处理中，最后keyboard_handler会变得很大**，这不是一个好主意。

在这里，向前辈学习，建立一个缓冲区，让 `keyboard_handler` 将每次收到的扫描码放入这个缓冲区，然后**建立一个新的任务专门用来解析它们并做相应处理**。

## 7.1.5 键盘输入缓冲区
我们先来建立一个缓冲区（这个缓冲区的结构是借鉴来的），用以**放置中断例程收到的扫描码**（代码7.6）。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200718173704130.png)

这个缓冲区的用法如下图所示，**白色框表示空闲字节，灰色框表示已用字节**。在执行写操作的时候要注意，如果已经到达缓冲区末尾，则应将指针移到开头。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200718173857378.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
对照上图，我们可以容易地**对缓冲区进行添加操作**：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200718174142112.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
代码很简单，但要注意，**如果缓冲区已满，这里使用的策略是直接就把收到的字节丢弃**。其中的 `kb_in` ，由于我们只在 `keyboard.c` 中使用，于是把它声明成一个 `PRIVATE` 变量（ `PRIVATE` 定义位于 `const.h` 中，被定义成了 `static` ）。注意，`kb_in` 的成员需要初始化，初始化的代码放在 `init_keyboard( )` 中。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200718175509644.png)


为了保持 `kernel_main` 的整洁，我们把**时钟中断的设定和开启**也放到单独的函数 `init_clock` 中。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200718182356134.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

这样，在 `kernel_main` 中调用这两个函数就可以了。

## 7.1.6 用新加的任务处理键盘操作
添加一个任务很简单，在6.4.6节中已经做过总结。下面要添加的这个任务**将来不仅会处理键盘操作，还将处理诸如屏幕输出等内容，这些操作共同组成同一个任务：终端任务**。现在，可以认为它只处理键盘输入。

为了简化程序，在这个任务中，我们不停地调用 `keyboard.c` 中的函数 `keyboard_read` ：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200718182538285.png)
暂时先把所有对扫描码的处理都写进 `keyboard.c` 中。代码7.10中被 `tty` 使用的 `keyboard_read` 可以如代码7.11这样来定义。函数首先判断 `kb_in.count` 是否为 `0` ，如不为 `0` ，表明缓冲区中有扫描码，就开始读。**读缓冲区开始时关闭了中断，到结束时才打开**，因为 `kb_in` 作为一个整体，对其中的成员的操作应该是不受打扰的。读操作相当于写操作的反过程。 
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200718182833632.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

其中，`disable_int( )` 和 `enable_int( )` 的定义很简单（代码7.12）。 
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200718182915638.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

这样简单的两个汇编函数，完全可以用**C语句中嵌入汇编**的方式来做，而且，由于避免了调用函数的 `call` 指令和返回时的 `ret` 指令，因此更加节省时间。

 
这样我们就完成了**通过任务来处理扫描码**（其实还仅仅是打印数字）的代码，修改Makefile之后就可以 `make` 并运行了，运行结果与过去一样，因为我们没有对扫描码进行解析（扫描码是根据键盘布局来确定的）：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200724021821649.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)


---
## 7.1.7 解析扫描码
对扫描码的解析工作有一点烦琐，所以还是分步骤来完成它。
### 1. 让字符显示出来
虽然已经有了一个数组 `keymap[ ]` ，但是不要低估了解析扫描码的复杂性，因为它不但分为 `Make Code` 和 `Break Code` ，而且有长有短，功能也很多样，比如 `Home` 键对应的是一种功能而不是一个ASCII码，所以要区别对待。先挑能打印的打印一下，看代码7.13。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200718183629765.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
在代码7.11中，总体思想是：
- `0xE0` 和 `0xE1` 单独处理且暂时不加理会。因为从表7.2中知道，**除去以这两个数字开头的扫描码，其余的都是单字节的。** 
- 如果遇到不是以 `0xE0, 0xE1` 开头的，则判断是 `Make Code` 还是 `Break Code` ，如果是后者同样不加理会，如果是前者就打印出来。
- 前文中讲过，`Break Code` 是 `MakeCode | 0x80` 进行按位或操作的结果，代码中的 `FLAG_BREAK` 被定义成了 `0x80` 。
- 从 `keymap[ ]` 中取出字符的时候进行了一个与操作（`scan_code&0x7F` 。一方面，如果当前扫描码是 `Break Code` ，与操作之后就变成 `Make Code` 了；另一方面，这样做也是为了避免越界的发生，因为数组 `keymap[ ]` 的大小是 `0x80` 。

接下来就可以 `make` 并运行了，下图就是初步运行的结果。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200724022143989.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

敲入了 `abc123456789` 共计6个字母，它们被显示在了屏幕上。
### 2. 处理Shift、Alt、Ctrl
现在可以输入简单的字符和数字，但还有更复杂的输入，比如按个Shift组合。

下面就来添加代码，使其能够响应这些功能键。在代码7.14中，我们不但添加了处理 `Shift` 的代码，而且也对 `Alt` 和 `Ctrl` 键的状态进行了判断，只是暂时对它们还没有做任何的处理。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200719150610262.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)![在这里插入图片描述](https://img-blog.csdnimg.cn/20200719150618735.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200719150628677.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

`Shift, Alt, Ctrl` 键左右各3个，**最好不要把左右两个键不加区分，因为有一些软件需要区分对待**，最简单而且经典的一个例子是超级玛丽，其中左右Shift功能是不一样的。为了不把左右键混为一谈，我们声明 `6` 个变量来记录它们的状态。
- 当其中的某一个键被按下时，相应的变量值变为 `true` 。比如，当我们按下左 `Shift` 键，`shift_l` 就变为 `true` ；
- 如果它立即被释放，则 `shift_l` 又变回 `false` 。
- 如果当左 `Shift` 键被按下且未被释放时，又按下 `a` 键，则 `if (shift_l || shift_r)` 成立，于是 `column` 值为 `1` ，`keymap[column]` 的取值就是 `keymap[ ]` 中第二列中相应的值，即大写字母 `A` 。

同时，在这段代码中对以 `0xE0` 开头的扫描码也做了处理。其实它与按下 `Shift` 键类似，甚至还要更简单。
- 当检测到一个扫描码的第一个字节是 `0x E0` 时，将 `code_with_E0` 赋值为 `true` ，**整个函数就返回了**。
- 下一个字节马上进入处理过程，由于 `code_with_E0 = true` ，所以 `column` 值变成 `2` ，于是 `key` 就变成 `keymap[ ]` 中第二列的值了。


在整个过程中，虽然开始变量 `key` 的值是从 `keymap[ ]` 中得到的，但从整个函数执行的角度来看，遵循这样的原则：
- 如果一个完整的操作还未结束，比如一个2字节的扫描码还未完全读入，则 `key` 赋值为 `0` ，等到下一次 `keyboard_read( ` )被执行时再继续处理。
- 目前的情况是，一个完整的操作需要在 `keyboard_read( )` 多次调用时完成。

现在运行一下，结果如图所示：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200724024135781.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
如今我们的OS已经可以识别**大小写字母**，以及 `!, @` 等字符了。

### 3. 处理所有按键
现在我们已经可以处理大部分按键了，但是至少存在两个问题：
1. 如果扫描码更加复杂，比如超过 `3` 个字符，程序还不足以很好地处理。
2. 如果按下诸如 `F1, F2` 这样的功能键，系统会试图把它当做可打印字符来处理，从而打印出一个奇怪的符号。

先来解决第一个问题。目前，一个完整的操作需要在 `keyboard_read( )` 多次调用时完成。这不但增加了许多全局变量，如 `code_with_E0` ，而且让 `keyboard_read( )` 理解起来也更困难。

符合逻辑的方法是，按下一个键会产生一到几字节的扫描码，**最好能够在一个过程中把它们全都读出来**。这其实并不困难，只需将从 `kb_in` 中读取字符的代码单独拿出来作为一个函数，用到时调用它就可以了。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200719152257579.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200719152306343.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200719152313695.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200719152541187.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

这段代码除了把函数 `get_byte_from_kbuf( )` 单独拿出来之外，只是单独处理了 `Pause, PrintScreen` 两个按键。不过这样一来，每一次调用 `keyboard_read( )` 都可以**处理一个相对完整的过程**。比如按下右侧的 `Alt` 键产生的 `0xE0, 0x38` ，不必调用两次 `keyboard_read( )` ，而且所有的按键都已经在处理范围之内了。不过，组合键的情况还是要多次调用 `keyboard_read( )` 。


下面来解决第二个问题——关于非打印字符的处理。`keyboard_read( )` 这个函数只是负责读取扫描码就可以了，如何处理不该是它的职责，**因为只有更高层次的软件才能根据具体情况做出不同的反应**。我们再将代码进行如下修改：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200719153532628.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200719153542311.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

这样，无论是 `Pause, PrintScreen` ，还是其他以 `0xE0` 开头的扫描码或普通的单字符扫描码，都会交给函数 `in_process( )` 来处理。而且，`Shift, Alt, Ctrl` 键的状态会用**设置相应位的方式**通过 `Key` 表现出来。我们马上写一个简单的 `in_process( )`（代码7.17）。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200719153630829.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

注意，这里有一个小技巧。如果打开 `keyboard.h` ，可以看到如下所示的情形：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200719153655921.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

在所有的不可打印字符的定义中，都加了一个 `FLAG_EXT` ，这就使得在程序中可以非常容易地识别出来。当 `(!(key & FLAG_EXT))` 真时，就表明当前字符是一个可打印字符。执行后的效果如图所示：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200724024858248.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
我们成功地输出了 `26` 个字母，包括大写和小写，同时输出了数字，以及其他一些字符。当你按下 `F1, F2` 等功能键时，程序并不做出反应。这些都表明修改是成功的。

不管是单键还是组合键，都使用一个 `32` 位整型数 `key` 来表示。因为可打印字符的ASCII码是 `8` 位，而我们将特殊的按键定义成了 `FLAG_EXT` 和一个单字节数的和，也不超过 `9` 位，这样还剩很多位来表示 `Shift, Alt, Ctrl` 等键的状态，**一个整型记载的信息就足够表示当前的按键情况**。

---
# 7.2 显示器
**随着键盘模块的逐渐完善，我们越来越需要考虑它与屏幕输出之间的关系**。**终端进程不仅处理键盘操作，还将处理诸如屏幕输出等内容**。所以，在彻底完成键盘驱动之前，我们必须了解**终端的概念以及显示器的驱动方式**。 

另外，书中假定系统用的是 `VGA` 以上的视频子系统，并假定不使用单色模式。


## 7.2.1 初始TTY
对于终端最简单而形象的认识是，当按 `Alt+F1, Alt+F2, Alt+F3` 等组合键时，会切换到不同的屏幕。**这些不同的屏幕中可以分别有不同的输入和输出，相互之间彼此独立。** 在某个终端中，如果键入命令 `tty` ，执行的结果将是当前的终端号。

终端当然不仅是 `Alt+Fn` 这么简单，但在目前的操作系统中，**我们暂时只实现这样简单的终端**。对于不同的 `TTY` ，可以理解成下图的样式。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200719154604230.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
不同的 `TTY` 对应的**输入设备虽是同一个键盘，但输出却像是在不同的显示器上**，因为不同的 `TTY` 对应的屏幕画面可能是不同的。**实际上，我们仍在使用同一个显示器，画面的不同只是因为显示了显存的不同位置**。


 `3` 个CONSOLE公用同一块显存，就必须有一种方式，**在切换CONSOLE的瞬间，让屏幕显示显存中某个位置的内容**。通过简单的端口操作相应的寄存器就可以做到这一点。
 
 ---
## 7.2.2 基本概念
"显示器"并不是一个精确的称呼，**因为我们操作的对象可能是显卡，或者仅仅是显存**。下面暂时使用"视频"这个词。

从一开始写简单的 `Boot Sector` 的时候，我们就没有离开过对视频的操作——**如果不是通过屏幕的反馈，我们怎么知道计算机在做些什么呢？**
- 最初的 `Boot Sector` 中，打印字符是通过BIOS中断来实现的；
- 到了保护模式中，BIOS中断不能用，我们就在 `GDT` 中建立了一个段，它的开始地址是 `0xB8000` ，通过段寄存器 `gs` 对它进行写操作，从而**实现数据的显示**。

目前，我们对于视频模块的操作也仅限于此，显示什么就 `mov` 而已。但是视频是一个很复杂的部分——显示适配器可以被设置成不同的模式，用来显示更多的色彩、更华丽的图像和动画。  这实现起来绝非易事。

不过我们没必要搞那么复杂，只要认识**开机看到的默认模式**就够了—— `80×25` 文本模式， 这种模式下：
- 显存大小为 `32KB` ，占用的范围为 `0xB8000～0xBFFFF` ；
- 每 `2` 字节代表一个字符，其中**低字节**表示字符的ASCII码，**高字节**表示字符的属性——包括颜色，我们设置过字符的颜色，还写了一个函数 `disp_color_str()` 显示不同颜色的字符。在默认情况下，**屏幕上每一个字符对应的2字节**的定义如图所示：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200719172246873.png)
- 一个屏幕总共可以显示 `25` 行，每行 `80` 个字符。


总的来说，屏幕字符对应的2个字节中：**低字节表示的是字符本身，高字节用来定义字符的颜色**。
- 颜色分前景和背景两部分，各占4位，其中低三位意义是相同的，表示颜色。
- 如果**前景最高位**为 `1` 的话，字符的颜色会比此位为 `0` 时亮一些；如果**背景最高位**为 `1` ，则显示出的字符将是闪烁的（是**字符闪烁**而不是背景闪烁）。
- 更多细节请参考下表：
![](https://img-blog.csdnimg.cn/20200719173930106.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

现在看第 `3` 章中代码 `3.1` 的这几行，就全明白了：
```c
mov ah, 0Ch ; 0000: 黑底; 1100: 高亮,红字
mov al, 'P'
mov [gs:edi], ax
```
想实际看一下各种颜色的效果，可以通过调用 `disp_color_str( )` 并改变其参数去试一下。


同时，我们已经知道**一个屏幕可以显示几行几列，又知道了一个字符占用几个字节**，易得一个屏幕映射到显存中所占的空间大小：`80×25×2=4000 Bytes` 。

而显存有 `32KB` ，每个屏幕才占 `4KB` ，所以显存中足以存放 `8` 个屏幕的数据。如果我们有 `3` 个 `TTY` ，可以各占 `10KB` 的空间还有剩余，甚至在每一个 `TTY` 内还可以**实现简单的滚屏功能**。

如何能够让系统显示指定位置的内容呢？其实很简单，通过端口操作设置相应的寄存器就可以了。

 
---
## 7.2.3 寄存器
从8259A到8042，再到现在的显示器，都是这样的流程，对端口操作而已。不过每种硬件各不相同，我们要了解其具体细节。

`VGA` 系统有 `6` 组寄存器，比较多，而且有些寄存器读和写端口还不一致。如表7.4所示：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200719182237272.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
使用英语就可以通过 `Register` 这个单词是否复数来判断寄存器是否只有一个。如 `CRT Controller Registers` 这一组，其中的 `Data Registers` 是复数，**说明数据寄存器不止一个**，如表7.5：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2020071918170247.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
这样多的数据寄存器，只有一个端口 `0x3D5` ，操作其中某个时要用到 `Address Register` 。表7.5中每一个寄存器都对应一个索引值，当想要访问其中一个时，只需要先向 `Address Register` 写对应的索引值（通过端口 `0x3D4` ），再通过端口 `0x3D5` 进行的操作就是针对索引值对应的寄存器了。`Data Registers` 相当于一个数组，而 `Address Register` 则相当于数组的下标。


举例，假如想把索引号为 `idx` 的寄存器的值改为 `new_value` ，可以用两次端口操作来做：
```c
out_byte(0x3D4, idx); 
out_byte(0x3D5, new_value);
```
我们试一下。从字面意思可知，`Cursor Location High Register` 和 `Cursor Location LowRegister` 用来**设置光标位置**，索引号分别是 `0Eh, 0Fh` 。

下面的程序中，我们不再让光标停在从Loader中调用BIOS中断显示完第5行的 `Ready` 之后。而是**让它跟随我们敲入的每一个字符**（代码7.19）。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200719183112596.png)
其中，寄存器的端口地址和索引值的定义如代码7.20所示：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200719183120781.png)
之所以 `disp_pos` 被 `2` 除，是因为屏幕上每个字符对应 `2` 字节。`make` 运行，效果如图所示：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200724025410499.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
可以看到，光标开始跟随字符了。进一步做试验，通过设置 `Start Address High Register` 和 `Start Address LowRegister` 来**重新设置显示开始地址**，从而实现滚屏的功能：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200719231521960.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

代码的意义是，检查当前按键是否是 `Shift+↑` ，如果是，则卷动屏幕至 `80×15` 处，即向上卷动 `15` 行。运行，按Shift+↑，呈现出如下样子：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200724025653539.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
`Start Address HighRegister, Start Address Low Register` 两个寄存器可以**用来设置从显存的某个位置开始显示**。这个特性<b><font color="red">允许我们把显存划分成不同的部分，然后只需要简单的寄存器设置就可以显示相应位置的内容。</font></b>

我们通过改变 `VGA` 寄存器的值**实现了光标的移动和屏幕滚动**。不过寄存器太多了！当然，我们暂时用到的没有多少。**了解了它们的访问方式，需要某个功能时，查一下手册就可以了。**
 

---
# 7.3 TTY任务
了解了键盘和显示器的操作，我们就可以实现多个TTY了，让TTY任务这样运行：
- 在TTY任务中执行循环，每次都会轮询每个TTY，处理它的事件——包括从键盘缓冲区读取数据、显示字符等；
- 不是轮询到某个TTY时，箭头对应的全部事件都会发生。**只有当某个TTY对应的控制台是当前控制台时，它才可以读取键盘缓冲区**；
- TTY可以对输入数据做更多的处理，这里简化为"显示"；
- 键盘和显示器是每个TTY的一部分，是公共的。
![在这里插入图片描述](https://img-blog.csdnimg.cn/202007192322365.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

总的来说，每次轮询到某个TTY时要做的：
- 处理输入——如果它是当前TTY，就从键盘缓冲区读取数据；
- 处理输出——如果有要显示的内容，则显示它。

将上面的TTY任务图转换为下面的函数调用图：`task_tty()`  是一个循环，它不断调用 `keyboard_read()` ，而 `key_board()` 从键盘缓冲区得到数据后，会调用 `in_process()` ，将字符直接显示出来。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200719232936923.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
话虽如此，实现起来就不是说的那么简单了：
-  **每一个TTY都应该有自己的读和写的动作**。所以在 `keyboard_read( )` 内部，函数需要了解自己是被哪一个TTY调用。可以通过为函数传入**指向当前TTY的指针**来做到这一点。
- 为了让输入和输出分离，被 `keyboard_read( )` 调用的 `in_process( )` 不应该再直接回显字符，**应该将回显的任务交给TTY来完成**，因此，我们就需要**为每个TTY建立一块缓冲区，用以放置将被回显的字符**。
- 每个TTY回显字符时操作的CONSOLE是不同的，所以它们都应该**有一个成员来记载其对应的CONSOLE信息**。


## 7.3.1 TTY任务框架的搭建
TTY和Console的结体如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200720000040369.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

整个程序流程如下：
- `task_tty( )` 中，**通过循环来处理每一个TTY的读和写操作**；
- 读写操作全都放在 `tty_do_read( ), tty_do_write( )` 两个函数中，这样就让 `task_tty( )` 很简洁，而且逻辑清晰。
- 读操作会调用 `keyboard_read( )` ，此时已经多了一个参数；
- 写操作会调用 `out_char( )` ，它会将字符写入指定的CONSOLE。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200720000547331.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

32KB的显存可以允许同时存在3个控制台，这里声明3个TTY和对应的3个CONSOLE：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200720000722266.png)
`task_tty( )` 框架实现如下，其中 `nr_current_console` 是一个定义在 `const.h`  的全局变量 `EXTERN int nr_current_console;` ，它可以用来记录当前的控制台是哪一个。**只有当某个TTY对应的控制台是当前控制台时，它才可以读取键盘缓冲区**：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200720001010652.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
在这个主循环中，先做了初始化工作。为其中的缓冲区设置初值和指定对应的CONSOLE：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200720012820875.png)
`tty_do_read( )` 要判断 `nr_current_console` 的值，对控制台进行切换时也要改变  `nr_current_console`  ：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200720130148300.png)
`tty_do_read( )` 如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2020072013022031.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
不过，`key_board( )` 改变了，要对其函数体进行修改。同时 `in_process( )` 也要增加参数 `p_tty` 。**往TTY缓冲区写入数据的代码很简单，只把输出字符写入缓冲区，读操作就结束了**；如果遇到 `Alt+Fn` 切换控制台的操作，也要在 `in_process` 中处理掉：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200720130417807.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
看一下写操作，它从TTY缓冲区中中取出值，类似 `get_byte_from_kbuf( )` ，然后用 `out_char( )` 显示在CONSOLE中：
 ![在这里插入图片描述](https://img-blog.csdnimg.cn/20200720130806221.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
`out_char( )` 实现如下：`V_MEM_BASE = 0xB8000` 定义在 `const.h` 中， `V_MEM_BASE + disp_pos` 等同于当前显示位置的地址，我们直接把字符写入特定地址：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200720131203979.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
我们所做的工作如下：
- 当TTY任务开始运行时，初始化所有TTY，全局变量 `nr_current_console = 0` ，开始循环并一直进行；
- 对于每个TTY，首先执行 `tty_do_read( )` ，它调用 `key_board( )` 并把读入的字符交给 `in_process( )` 处理。如果是要输出的字符，`in_process( )` 把它放入当前TTY的缓冲区中；
- 然后执行 `tty_do_write( )` ，如果缓冲区中有数据，就送入 `out_char( )` 进行显示。
- 由于 `nr_current_console` 初始化后没有变过，所以只有初始的TTY在接受处理，其他TTY没有动作。

运行结果如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2020072402592191.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

---
## 7.3.2 多控制台
这里来实现多个CONSOLE。前面，我们还根本没有用到CONSOLE的结构体成员。下图是某时刻显存的使用状态：
- `original_addr, v_mem_limit` 定义控制台所占显存的总体情况，一经初始就不再改变；
- `current_start_addr` 随着屏幕卷动而变化；
- `cursor` 每输出一个字符就更新一次；
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200720132238947.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

为上述成员设置初始值：
- CONSOEL结构体中的成员都是 `int/WORD` 类型，符合对VGA寄存器操作的习惯；
- `init_screen( )` 在 `init_tty( )` 中调用，同时修改原来的 `init_tty( )` ；
- 第一个CONSOLE使用原来的光标位置；其他控制台光标都在左上角，并且显示控制台号和一个字符 `#` 。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200720133950450.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

修改后的 `init_tty( )` 如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200720134230240.png)

`out_char( )` 考虑到多控制台的情况，改变如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200720134324103.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
还需要一个切换控制台的函数：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200720135001644.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

`set_video_start_addr( )` 如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200720135021618.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

另外，惯例是按下 `Alt+Fn` 完成切换工作：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200720135258798.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
有了 `select_console( )` ，我们可以在 `task_tty( )` 中这样初始化 `nr_current_console` ：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200720135348283.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
运行时发现通过 `ALT+F1/F2` 无法切换，可能是Windows快捷键占用或者是Ubuntu的快捷键设置——`Alt + F1` 聚焦到桌面左侧任务导航栏，`Alt + F2` 运行命令导致的。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20200724033419181.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
上面是通过 `Alt + F3` 切换到的控制台2，然后输入字符串。如果切换回控制台0，会发现屏幕快满了。为此，添加屏幕滚动的代码。

这里简化了一下，当屏幕滚动到最下端后，再试图向下滚动时按键不再响应，最上端时也一样。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200720140134572.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
响应 `Shift+↑, Shift+↓` 的代码如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200720140442216.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
再次运行，在控制台0按 `Shift+↑` 多次，如下图：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200724030448594.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)


---
## 7.3.3 完善键盘处理
现在，我们对键盘的支持还是很差的，还不能使用 `CapsLock, BackSpace` 等等。如今任务框架搭好了，我们需要添加处理其他按键的代码。

### 1. 回车键和退格键
敲入回车和退格时，要往TTY缓冲区写入 `"\n"` 和 `"\b"` ，然后在 `out_char( )` 做相应处理：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200720145218779.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
修改 `out_char( )` ：
- 回车键将光标移动到下一行的开头；如果光标已经移出了屏幕，就会触发屏幕滚动；
- 退格键将光标移动到上一个字符的位置，并写一个空格清除原来的字符；
- 输出任何字符，都会做边界检验，防止写到别的控制台或者写到显存之外；
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200720145853121.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
运行效果如下，在控制台0中输出如下按键序列：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200724034309555.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
然后，通过 `ALT+F3` 来到控制台2（这其实是第三个控制台 `^-^` ），输入如下序列：![在这里插入图片描述](https://img-blog.csdnimg.cn/20200724034429610.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

这里我确认，之所以 `ALT+F1/F2` 无法切换是Ubuntu快捷键设置的问题。于是我想到禁用 `ALT+F4` ，然后用 `ALT+F3/F4/F5` 替代 `ALT+F1/F2/F3` ：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200724040123146.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200724040147680.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200724040218809.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)



---
### 2. CapsLock, NumLock, ScrollLock
我的键盘没有 `ScrollLock` ，`NumLock` 也被我用来作为一个截图软件的快捷键了。只有 `CapsLock` ，还像书中说的那样对应一个小灯 (`LED`) 。我们可以通过**敲击键盘或者写入8042的输入缓冲区**控制灯的亮灭。

8042的输入缓冲区和控制寄存器都是可写的，不过**写入输入缓冲区是向8048发送命令，写入控制寄存器是向8042本身发送命令**：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200718134900100.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
先看看对应的端口 `0x60` ，写入这个端口将向8048发送命令，设置LED的命令是 `0xED` 。当键盘接受到命令时，会回复一个ACK `0xFA` 信号，然后等待从端口 `0x60` 写入的LED参数字节：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200720150946888.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

当键盘收到参数字节后，再次回复一个ACK，并设置LED。

注意：向8042输入缓冲区写数据时，要先判断输入缓冲区是否为空——通过端口 `0x64` 读取状态寄存器，第2位如果是 `0` ，表示输入缓冲区是空的，可以写入数据。代码如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200720153803742.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
其中，`LED_CODE, KB_ACK, caps_lock=0, num_lock=1, scroll_lock=0` 分别定义如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200720153854635.png)![在这里插入图片描述](https://img-blog.csdnimg.cn/20200720154147746.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
运行程序，系统启动之后NumLock被点亮。我的键盘这个键不会亮；`num_lock=1` 时可以使用数字功能，为了让功能起效，需要修改 `keyboard_read( )` ——增加了一个 `pad` 变量；在 `key` 中设置了一个相应的位以区分普通数字键和小键盘数字键：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200720172851809.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200720172921188.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)![在这里插入图片描述](https://img-blog.csdnimg.cn/2020072017295474.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)![在这里插入图片描述](https://img-blog.csdnimg.cn/20200720173011145.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

运行如下，出现问题，可能是因为我的键盘没有 `NumLock` 的LED灯和 `ScrollLock` 这个键：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200724041119861.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
这里就不多纠结了。



---
## 7.3.4 TTY任务总结
到此，书中的TTY任务就暂时结束了。

不过问题是，终端任务运行在 `ring1` ，但它与运行在 `ring0` 的 `keyboard_handler` 容易混淆。终端任务可以访问所有内存。`kb_in` 在 `ring0` 下写，在 `ring1` 下读。另外，Minix也是这样做的。

---
# 7.4 区分任务和用户进程
现在，我们有了4个进程——`TTY, A, B, C` ，后三者可有可无，它们是"用户进程"，而TTY是"任务"。具体实现上，让用户进程运行在 `ring3` ，任务运行在 `ring1` ，`ring0` 运行的是进程调度：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2020072017323288.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

现在，增加对 `NR_PROCS` 的定义，同时修改 `NR_TASKS = 1` ：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200720173334138.png)
然后，在所有用到 `NR_TASKS` 的地方进行修改。新声明一个数组 `user_proc_table[ ]` ：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200720173501419.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
然后，修改初始化进程表的代码。这里改变了用户进程的特权级，而且通过 `eflags` 的改变，取消了用户进程所有的IO权限：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200720175445612.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

另外，`protect.c` 中初始化GDT中LDT描述符的代码也进行了修改：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200720175622545.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
区分任务和用户进程后的 `proc.c` 进程调度代码：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200720175724994.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
`make` 运行如下，结果没有多少区别：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200724041453112.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

这次改动标志着Orange's现在已经运行在了3个特权级上。**普通的用户进程和系统任务从此区分开来**。



---

# 7.5 printf
现在，我们有了TTY，还有一个任务和三个用户进程，想看到它们在特定终端运行的情况。为此，我们需要有一个供输出使用的 `printf( )` 。

`printf( )` 进行屏幕输出，需要用到控制台模块的代码，因此，它需要**通过系统调用**才能完成。

## 7.5.1 为进程指定TTY
当某个进程调用 `printf( )` 时，操作系统必须知道往哪个控制台输出才行。而系统调用发生，`ring3` 跳入 `ring0` 时，系统只能够知道当前系统调用是由哪个进程触发的，所以**我们必须为每个进程指定一个与之对应的TTY**。通过在进程表中增加一个成员即可：

![在这里插入图片描述](https://img-blog.csdnimg.cn/20200720180205865.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
用初始化 `PROCESS` 的 `tick, priority` 成员的方法，为 `nr_tty` 设置初值——在 `for` 循环中，所有进程的 `nr_tty = 0` ，即默认与第0个TTY绑定。不过后面，将 `B, C` 两个进程与第1个TTY绑定，它们的输出将同时出现在控制台上1上；而 `A` 的输出出现在控制台 `0` ：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200720180319444.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

---
## 7.5.2 printf()的实现
从学习C的第一天开始，我们就开始用上 `printf( )`  了，但是它的实现并不简单——**它的参数个数和类型都可变；而且还有多种多样的表示格式的参数**，如 `%c, %s` 等，这些都需要加以识别。

整个 `printf( )` 的调用过程如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200720221129518.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)



开始的 `printf( )` 实现一个简单的，它只支持 `%x` 输出十六进制一种格式：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200720200001224.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

`vsprintf( )` 的实现如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200720205025304.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)


可变参数的原理：
> **调用一个函数时，总是先把参数压栈，然后通过call指令转移到被调用者，在完成后清理堆栈。** 但这里遇到两个问题：
> - 如果有多个参数，哪个参数先入栈，是前面的还是后面的？
> - 由谁来清理堆栈，调用者还是被调用者？
> 
> <b></b>
> 这两个方面的问题其实被称为“调用约定”（`Calling Conventions`）……调用约定有若干种，每一种都**规定参数入栈的顺序以及谁来清理堆栈**。我们已经用汇编语言写过不少的函数，都是**后面的参数先入栈(前面的参数就位于栈顶，更容易取出)，并且由调用者清理堆栈**。这种约定被称做C调用约定。
> <b></b>
>C调用约定的好处在处理可变参数函数时得到了充分体现，因为**只有调用者知道此次调用包含几个参数，于是可以方便地清理堆栈**。**C调用约定让使用可变参数的函数成为可能**
> <b></b>
> 可具体怎么做呢？
> - 首先是它的声明，过去我们写的函数，都有确定类型的参数，可现在不同了，**参数的个数和类型都不知道**，于是，省略号就派上了用场，正如代码7.55所示，**一个省略号，表示参数不知道有多少，更不知道是什么。**
> - ...在每一次调用过程中，`printf` 必须有一种方法来使用这些参数才行。从代码7.55可以看到，`printf` 使用了它的第一个参数 `fmt` 作为基准，**得到了后面若干参数的开始地址**，这样，其值也就容易得到了。
>
>  <b></b>
> 举一个例子，假设我们调用 `printf(fmt, var1, var2, var3)` ，则堆栈情况将如图7.25所示：
> ![在这里插入图片描述](https://img-blog.csdnimg.cn/20200720203808394.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
> `&fmt` 表示 `fmt` 地址，`(char*)(&fmt) + 4` 表示跟随在 `fmt` 后面的参数的地址。所以，接下来实际上是将 `var3` 的地址传递给了紧接着调用的 `vsprintf` 。`va_list` 其实就是 `char*` ，它的定义在 `type.h` 中。
> <b></b>
> 函数 `vsprintf` 的实现见代码7.56，虽然它只识别 `%x` 这一种格式，但其他格式的原理也是一样的，即根据%后的格式字符就能判断下一个参数的类型，从而知道从堆栈中取出什么。

---
## 7.5.3 系统调用write()
接下来完成 `write( )` 系统调用，它把 `vsprintf` 输出的字符串打印到屏幕上。

增加一个系统调用 (`foo`) 的过程如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200720205227982.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

系统调用是 `write( )` ，对应的内核部分是 `sys_write( )` ，声明在 `proto.h` 中，对应第4步和第5步：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200720205407775.png)
接下来是 `write( )` 和 `sys_write( )` 两个函数体：由于已有的系统调用没有参数，所以还需要修改 `sys_call( )` ：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200720205620715.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
`sys_write( )` 通过简单函数 `tty_write( )` 实现字符输出，这里 `sys_write( )` 比 `write( )` 多一个参数，需要在修改的 `sys_call( )` 中压栈，然后 `sys_call` 调用 `sys_write` ：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200720205747470.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
修改的 `sys_call` 如下。当前运行的进程是通过设置 `p_proc_ready` 来恢复执行的，所以当进程切换到未发生之前， `p_proc_ready` 的值就是**指向当前进程的指针**。把它压栈就是把当前进程即 `write( )` 的调用者指针传递给了 `sys_write( )` ：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200720220647525.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)




---
## 7.5.4 使用printf()
在3个用户进程中调用它：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200720221606888.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
运行，成功：
① 控制台0的情况：`printf` 打印出字符和数字；
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200724041731373.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
② 控制台1的情况：`printf` 交替打印出两个进程的字母；
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200724042005714.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)


我们有了自己的 `printf` ，**它运行在用户态，可以被普通的用户进程调用**。现在，我们还可以到控制台2的空白屏幕敲击键盘输入字符。这就是一个**多任务多控制台**的操作系统的特性。
