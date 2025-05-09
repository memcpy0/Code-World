> 你是个能吃苦的人吗？从前的能吃苦大多指的体力劳动的苦，但现在的能吃苦已经包括太多维度，包括：读书学习&寂寞的苦、深度思考&脑力的苦、自律习惯&修行的苦、自控能力&放弃的苦、低头做人&尊严的苦。
> ==虽然这些苦摆在眼前，但大多数人还是喜欢吃简单的苦==。熬夜加班、日复一日、重复昨天、CRUD，最后身体发胖、体质下降、能力不足、自抱自泣！**所以有些苦能不吃就不吃，要吃就吃那些有成长价值的苦**。今天你写博客了吗？
> **如果一件小事能坚持 5 年以上，那你一定是很了不起的人。是的，很了不起。人最难的就是想清楚了但做不到，或者偶尔做到长期做不到。**
> 其实大多数走在研发路上的伙伴们，都知道自己该努力，但明明下好了的决心就是坚持不了多久。就像你是否也想过要写技术博客，做技术积累。直到有一天被瓶颈限制在困局中才会着急，但这时候在想破局就真的很难了！

# 1. 面试题
谢飞机，小记，飞机趁着周末，吃完火锅。又去约面试官喝茶了！

谢飞机：嗨，我在这，这边，这边。
面试官：你怎么又来了，最近学的不错了？
谢飞机：还是想来大厂，别害羞，面我吧！
面试官：我好像是你补课老师... 既然来了，就问问你吧！volatile 是干啥的？
谢飞机：啊，volatile 是保证**变量对所有线程的可见性的**。
面试官：那 volatile 可以解决原子性问题吗？
谢飞机：不可以！
面试官：那 volatile 的底层原理是如何实现的呢？
谢飞机：...，这！面试官，刚问两个题就甩雷，你是不家里有事要忙？
面试官：你管我！

# 2. volatile讲解
## 2.1 可见性案例
```java
package com.memcpy0.interview.test;  
  
import org.slf4j.Logger;  
import org.slf4j.LoggerFactory;  
  
import java.util.Optional;  
  
/**  
 * -XX:+UnlockDiagnosticVMOptions -XX:+PrintAssembly * <p>  
 * -XX:+UnlockDiagnosticVMOptions -XX:+PrintAssembly -XX:CompileCommand=dontinline,ApiTest.test_volatile  
 */
public class ApiTest {  
    public static void main(String[] args) {  
        Optional.empty();  
        final VT vt = new VT();    
        Thread Thread01 = new Thread(vt);  
        Thread Thread02 = new Thread(new Runnable() {  
            public void run() {  
                try {  
                    Thread.sleep(3000);  
                } catch (InterruptedException ignore) {  
                }  
                vt.sign = true;  
                System.out.println("vt.sign = true 通知 while (!sign) 结束！");  
            }  
        });  
        Thread01.start();  
        Thread02.start();  
    }  
}  
  
class VT implements Runnable {  
    private Logger logger = LoggerFactory.getLogger(ApiTest.class);  
    public boolean sign = false;  
  
    public void run() {  
        while (!sign) { }  
        logger.info("你坏"); 
    }  
}
```
这段代码是两个线程操纵一个VT实例中的变量。程序期望当 `sign` 在线程Thread02设置 `vt.sign = true` 后，Thread01操纵的线程退出死循环，输出“你坏”。**但实际上这段代码永远不会输出 你坏，而是一直处于死循环**。这是为什么呢？接下来我们就一步步讲解和验证。
## 2.2 加入volatile关键字
我们把 sign 关键字加上 volatitle 描述，如下：
```java
public volatile boolean sign = false;
```
测试结果
```java
vt.sign = true 通知 while (!sign) 结束！
03:49:31.758 [Thread-0] INFO  com.memcpy0.interview.test.ApiTest - 你坏
```
==volatile 关键字是 **Java 虚拟机**提供的的**最轻量级的同步机制**，它作为一个修饰符出现，用来修饰变量==，但是这里**不包括局部变量**哦。

在添加 volatile 关键字后，程序就符合预期的输出了 你坏。从我们对volatile 的学习认知可以知道。volatile 关键字是 JVM 提供的最轻量级的同步机制，用来修饰变量，用来**保证变量对所有线程可见性**。修饰后可以让字段在线程变可见，那么**这个属性被修改值后，可以及时的在另外的线程中做出相应的反应**。
# 3. volatile如何保证可见性
## 3.1 无volatile时，内存变化
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305080416874.png)

为什么 `private static SimpleDateFormat f = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");` 这种不是在每个线程都有一个副本呢？而 `sign` 却在每个线程的CPU内存有一个副本？

特地把 `sign` 设为 `private static` 放到 `ApiTest` 中，还是对不同线程没有可见性。

首先是当 sign 没有 volatitle 修饰时 public boolean sign = false;，**线程01 对变量进行操作，线程 02 并不会拿到变化的值**。所以程序也就不会输出结果“你坏”

## 3.2 有 volatile 时，内存变化
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305080424617.png)
当我们把变量使用 volatile 修饰时 public volatile boolean sign = false;，**线程 01 对变量进行操作时，会把变量变化的值强制刷新的到主内存。当线程 02获取值时，会把自己的内存里的 sign 值过期掉，之后从主内存中读取。所以添加关键字后程序如预期输出结果**。

# 4. 反编译解毒可见性
类似这样有深度的技术知识，最佳的方式就是深入理解原理，**看看它到底做了什么才保证的内存可见性操作**。
## 4.1 查看JVM指令
指令：javap -v -p VT
```java
public volatile boolean sign;
	descriptor: Z
	flags: ACC_PUBLIC, ACC_VOLATILE
org.itstack.interview.test.VT();
	descriptor: ()V
	flags:
	Code:
		stack=2, locals=1, args_size=1
			0: aload_0
			1: invokespecial #1 // Method java/lang/Object."<init>":()V
			4: aload_0
			5: iconst_0
			6: putfield #2 // Field sign:Z
			9: return
		LineNumberTable:
			line 35: 0
			line 37: 4
		LocalVariableTable:
			Start Length Slot Name Signature
				0     10    0 this Lorg/itstack/interview/test/VT;
public void run();
	descriptor: ()V
	flags: ACC_PUBLIC
	Code:
		stack=2, locals=1, args_size=1
			0: aload_0
			1: getfield #2 // Field sign:Z
			4: ifne 10
			7: goto 0
			10: getstatic #3 // Field java/lang/System.out:Ljava/io/PrintStream;
			13: ldc #4 // String 你坏
			15: invokevirtual #5 // Method java/io/PrintStream.println:(Ljava/lang/String;)V
			18: return
		LineNumberTable:
			line 40: 0
			line 42: 10
			line 43: 18
		LocalVariableTable:
			Start Length Slot Name Signature
				0     19    0 this Lorg/itstack/interview/test/VT;
		StackMapTable: number_of_entries = 2
			frame_type = 0 /* same */
			frame_type = 9 /* same */
}
```
从 JVM 指令码中只会发现多了，ACC_VOLATILE，并没有什么其他的点。所以，**也不能看出是怎么实现的可见性**。
## 4.2 查看汇编指令
通 过 Class 文 件 查 看 汇 编 ，需要下载 hsdis-amd64.dll 文件，复制到 JAVA_HOME\jre\bin\server 目录下。下载资源如下： 
http://vorboss.dl.sourceforge.net/project/fcml/fcml-1.1.1/hsdis-1.1.1-win32-amd64.zip
http://vorboss.dl.sourceforge.net/project/fcml/fcml-1.1.1/hsdis-1.1.1-win32-i386.zip

另外是执行命令，包括：
1. 基础指令：java -Xcomp -XX:+UnlockDiagnosticVMOptions -XX:+PrintAssembly
2. 指定打印：-XX:CompileCommand=dontinline,类名.方法名
3. 指定打印：-XX:CompileCommand=compileonly,类名.方法名
4. 输出位置：> xxx

最终使用：java -Xcomp -XX:+UnlockDiagnosticVMOptions -XX:+PrintAssembly -XX:CompileCommand=dontinline,ApiTest.main -XX:CompileCommand=compileonly,ApiTest.mian
指令可以在 IDEA 中的 Terminal 里使用，也可以到 DOS 黑窗口中使用。另外，为了更简单的使用，我们把指令可以配置到 idea 的 VM options 里。

配置完成后，不出意外的运行结果如下：
```java
Loaded disassembler from C:\Program Files\Java\jdk1.8.0_161\jre\bin\server\hsdis-
amd64.dll
Decoding compiled method 0x0000000003744990:
Code:
Argument 0 is unknown.RIP: 0x3744ae0 Code size: 0x00000110
[Disassembling for mach='amd64']
[Entry Point]
[Constants]
	# {method} {0x000000001c853d18} 'getSnapshotTransformerList' '()[Lsun/instrument/TransformerManager$TransformerInfo;' in 'sun/instrument/TransformerManager'
	# [sp+0x40] (sp of caller)
	0x0000000003744ae0: mov r10d,dword ptr [rdx+8h]
	0x0000000003744ae4: shl r10,3h
	0x0000000003744ae8: cmp r10,rax
	0x0000000003744aeb: jne 3685f60h ; {runtime_call}
	0x0000000003744af1: nop word ptr [rax+rax+0h]
	0x0000000003744afc: nop
[Verified Entry Point]
	0x0000000003744b00: mov dword ptr [rsp+0ffffffffffffa000h],eax
	0x0000000003744b07: push rbp
	0x0000000003744b08: sub rsp,30h ;*aload_0
									; - sun.instrument.TransformerManager::getSnapshotTransformerList@0 (line 166)
	0x0000000003744b0c: mov eax,dword ptr [rdx+10h]
	0x0000000003744b0f: shl rax,3h ;*getfield mTransformerList
									; - sun.instrument.TransformerManager::getSnapshotTransformerList@1 (line 166)
	0x0000000003744b13: add rsp,30h
	...
```
运行结果就是汇编指令，比较多这里就不都放了。我们只观察🕵重点部分：
```java
0x0000000003324cda: mov 0x74(%r8),%edx ;*getstatic state
										; - VT::run@28 (line 27)
0x0000000003324cde: inc %edx
0x0000000003324ce0: mov %edx,0x74(%r8)
0x0000000003324ce4: lock addl $0x0,(%rsp) ;*putstatic state
										  ; - VT::run@33 (line 27)
```
编译后的汇编指令中，有 volatile 关键字和没有 volatile 关键字，**主要差别在于多了一个 lock addl $0x0,(%rsp)，也就是 lock 的前缀指令**。lock 指令相当于一个**内存屏障**，它保证如下三点：
1. **将本处理器的缓存写入内存**。
2. 重排序时，**不能把后面的指令重排序到内存屏障之前的位置**。
3. **如果是写入动作，会导致其他处理器中对应的内存无效**。

那么，这里的 1、3 就是用来保证被修饰的变量，保证内存可见性。
# 5. 不加volatile也可见吗
有质疑就要有验证
我们现在再把例子修改下，在 while (!sign) 循环体中添加一段执行代码，如下；
```java
class VT implements Runnable {
	public boolean sign = false;
	public void run() {
		while (!sign) {
			System.out.println("你好");
		}
		System.out.println("你坏");
	}
}
```
修改后去掉了 volatile 关键字，并在 while 循环中添加一段代码。现在的运行结果是：
```
...
你好
你好
你好
vt.sign = true 通知 while (!sign) 结束！
你坏
```
 咋样，又可见了吧！这是**因为在没 volatile 修饰时，jvm 也会尽量保证可见性。有 volatile 修饰的时候，一定保证可见性**。但可能并非如此，下章节继续深挖！
# 总结 
最后我们再总结下 volatile，它呢，**会控制被修饰的变量在内存操作上主动把值刷新到主内存**，**JVM 会把该线程对应的 CPU 内存设置过期，从主内存中读取最新值**。

那么，volatile 如何防止指令重排也是内存屏障，volatile 的内存屏故障是在读写操作的前后各添加一个 StoreStore 屏障，也就是四个位置，来**保证重排序时不能把内存屏障后面的指令重排序到内存屏障之前的位置**。

另外 volatile 并不能解决原子性，**如果需要解决原子性问题，需要使用 synchronzied 或者 lock**，这部分内容在我们后续章节中介绍。

# 第 2 节：synchronized
感觉什么都不会，从哪开始呀！

这是最近我总能被问到的问题，也确实是。一个初入编程职场的新人，或是一个想重新努力学习的老司机，这也不会，那也不会，总会犯愁从哪开始。
讲道理，毕竟 Java 涉及的知识太多了，要学应该是学会学习的能力，而不是去背题、背答案，拾人牙慧是不会有太多收益的。

学习的过程要找对方法，遇到问题时最好能自己想想，你有哪些方式学会这些知识。是不感觉即使让你去百度搜，你都不知道应该拿哪个关键字搜！只能拿着问
题直接找人问，这样缺少思考，缺少大脑撞南墙的过程，其实最后也很难学会。所以，你要学会的是自我学习的能力，之后是从哪开始都可以，重要的是开始和坚持！
# 1. 面试题
谢飞机，小记，周末逛完奥特莱斯，回来就跑面试官家去了！
谢飞机：duang、duang、duang，我来了！
面试官：来的还挺准时，洗洗手吃饭吧！
谢飞机：嘿嘿...
面试官：你看我这块鱼豆腐，像不像 synchronized 锁！
谢飞机：啊！？
面试官：飞机，正好问你。synchronized、volatile，有什么区别呀？
谢飞机：嗯，volatile 保证可见性，synchronized 保证原子性！
面试官：那不用 volatile，只用 synchronized 修饰方式，能保证可见性吗？
谢飞机：这...，我没验证过！
面试官：吃吧，吃吧！一会给你个 synchronized 学习大纲，照着整理知识点！
# 2. synchronized 解毒
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305082300195.png)
