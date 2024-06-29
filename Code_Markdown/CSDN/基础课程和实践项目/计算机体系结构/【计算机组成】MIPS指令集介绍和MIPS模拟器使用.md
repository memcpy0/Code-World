@[toc]

学习计算机组成，不学汇编语言，等于白学。对我来说，相当于把以后学的汇编课程移到现在开始学。
学习汇编的好处在于：
1. 有助于深刻理解计算机的软硬件接口，理解编程语言的底层；
2. 汇编的助记语法差不多，因此学了一门，其他种类就很容易吸收了。

我的目标是学3门汇编语言，分别是Computer Organization and Design: The Hardware/Software Interface(第五版)中的MIPS，32位指令集；汇编语言(清华大学出版社 王爽 第3版)介绍的Intel x86 8086指令集，16位；计算机程序设计艺术中的虚拟指令集MMIX，32位。

> 学习不能在一台抽象的计算机上来进行，必须针对一台具体的计算机来完成学习过程。 ——王爽

王爽老师的说法我很认可，事实上，MIPS有模拟器；8086机尽管不存在了，但是和Intel兼容的系列都可以用8086的方式工作，而且也有模拟器；MMIX是虚拟的，也有志愿者开发了模拟程序。这些都充分说明了学习环境的重要，只在纸上面写汇编没多少用，缺点在于执行编写的任何程序的困难。

下面简单介绍两个MIPS的模拟器，MIPSsim和Mars。
# MIPSsim下载和使用
[MIPS模拟器_x64](https://download.csdn.net/download/xph110/10439473)  

双击MIPSsim.exe，即可启动该模拟器。MIPSsim是在Windows操作系统上运行的程序，它需要用.NET运行环境。如果你的机器没有该环境，请先下载和安装“Microsoft .NET Framework 2.0 版可再发行组件包”。

[计算机组成原理之MIPS指令系统和MIPS体系结构](https://blog.csdn.net/qq_35260622/article/details/51657000)这篇文章介绍了MIPSsim的基本操作，加载了样例程序 alltest.asm。


# Mars下载和使用
MARS是一个4MB左右的.Jar可执行文件，需要使用Java环境，如果有Java运行环境(JRE)可直接使用，否则应先下载JRE或JDK。

[MARS（MIPS汇编程序和运行时模拟器）](https://blog.csdn.net/y_universe/article/details/82875244)这篇文章介绍了MARS下载和使用入门，编写了一个MIPS程序，写得相对详细。

