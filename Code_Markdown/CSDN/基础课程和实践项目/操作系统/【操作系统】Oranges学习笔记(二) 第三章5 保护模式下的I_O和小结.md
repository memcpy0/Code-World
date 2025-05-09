@[toc]


---
# 3.5 保护模式下的I/O
对IO的控制权限很重要，保护模式对此也做了限制。如果用户进程不被许可，就无法进行IO操作。限制通过两方面进行——IOPL和I/O许可位图。
## 1. IOPL
IOPL是I/O保护机制的关键之一，位于 `eflags` 的 `12,13` 位：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200716004117719.png)
I/O敏感指令 `I/O Sensitive Instructions` 如 `in, ins, out, outs, cli, sti` 只有在 `CPL <= IOPL` 时才可以执行。如果低特权级的指令试图访问这些IO敏感指令将会导致常规保护错误(#GP)。

可以改变IOPL的指令只有 `popf` 和 `iretd` ，但是只有运行在 `ring0` 的程序才能够将其改变。低特权级程序无法改变IOPL，如果试图改变不会产生异常，只是IOPL仍然保持原样。

指令 `popf` 也可以用来改变 `IF` ，就像执行了 `sti, cli` 一样。不过这种情况下，`popf` 也是I/O敏感指令，只有 `CPL <= IOPL` 时改变才会成功，否则像上面一样，IF维持原值，不产生任何异常。

---
## 2. I/O许可位图(I/O Permission Bitmap)
我们回头看，会发现TSS偏移102 Bytes处有个叫做 `I/O位图基址` 的值，它是一个以TSS的地址为基址的偏移，指向的就是I/O许可位图。I/O许可位图中，**每一位表示一个字节的端口地址是否可用**，为零则可用，否则禁用。

由于**每个任务都有单独的TSS，所以每个任务都有单独的I/O许可位图**。比如，一个任务的TSS如下：
- I/O许可位图开始有 `12 Bytes` 内容为 `0FFh` ，即 `12 * 8 = 96` 位为1，所以从端口 `00h` 到 `5Fh` 共 `96` 个端口地址对此任务不可用。同样，接下来的1个字节只有第2位是零，表示对应的端口 `61h` 可用。
- I/O许可位图必须以 `0FFh` 结尾。
- TSS举例的代码如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200716005250852.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

如果I/O位图基址大于等于TSS界限，就等于没有I/O许可位图。如果 `CPL <= IOPL` ，则所有指令都会引起异常。

I/O许可位图的使用，**让同一特权级下的不同任务也可以有不同的I/O访问权限**。

---
# 3.6 保护模式小结
总算初步认识了保护模式，虽然没有涉及保护模式所有的内容。但是现在，我不仅了解了页式存储，知道了中断和异常机制，认识了I/O控制的相关内容，我们对"保护"的认识更加全面了。
> - 在 `GDT` 、`LDT` 以及 `IDT` 中，每一个描述符都有自己的界限和属性等内容，是对描述符所描述对象的一种限定和保护。
> - 分页机制中的 `PDE` 和 `PTE` 都含有 `R/W` 以及 `U/S` 位，提供了页级保护。页式存储的使用使应用程序使用的是线性地址空间而不是物理地址，于是物理内存就被保护起来。
> - 中断不再像实模式下一样使用，也提供特权检验等内容。
> - `I/O` 指令不再随便使用，于是端口被保护起来。
> - 在程序运行过程中，如果遇到不同特权级间的访问等情况，会对 `CPL、RPL、DPL、IOPL` 等内容进行非常严格的检验，同时可能伴随堆栈的切换，这都对不同层级的程序进行了保护。



