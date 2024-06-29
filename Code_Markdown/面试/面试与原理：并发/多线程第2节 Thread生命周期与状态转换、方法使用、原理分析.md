考不常用的、考你不会的、考你忽略的，才是考试！
大部分考试考的，基本都是不怎么用的。例外的咱们不说😄 就像你做程序开发，尤其在 RPC+MQ+分库分表，其实很难出现让你用一个机器实例编写多线程压榨CPU 性能。很多时候是扔出一个 MQ，异步消费了。如果没有资源竞争，例如库表秒杀，那么其实你确实很难接触多并发编程以及锁的使用。
但！凡有例外，比如你需要开发一个数据库路由中间件，那么就肯定会出现在一台应用实例上分配数据库资源池的情况，如果出现竞争就要合理分配资源。如此，类似这样的中间件开发，就会涉及到一些更核心底层的技术的应用。
所以，==有时候不是**没用**，而是你没有用==。

# 一、面试题
谢飞机，小记！ 线程我玩定了，面试也拦不住我，我说的！
谢飞机：嘿，你好哇，我是谢飞机！
面试官：好，今天电话面试，你准备好了？
谢飞机：准备好了，嘿嘿！
面试官：嗯，我看你简历里写了不少线程的东西，看来了解的不错。问你一个线
程吧那就，线程之间状态是怎么转换的？
谢飞机：扒拉扒拉，扒拉扒拉！
面试官：嗯，还不错。那 yield 方法是怎么使用的。
谢飞机：嗯！好像是让出 CPU。具体的没怎么用过！
面试官：做做测试，验证下，下次问你。

# 二、Thread 状态关系
Java 的线程状态描述在枚举类 java.lang.Thread.State 中，共包括如下五种
状态：
```java
public enum State {
    NEW, RUNNABLE, BLOCKED, WAITING, TIMED_WAITING, TERMINATED;
}
```
**这五种状态描述了一个线程的生命周期**，其实这种状态码的定义在我们日常的业务开发中，也经常出现。比如：一个活动的提交、审核、拒绝、修改、通过、运行、关闭等，是类似的。那么线程的状态是通过下图的方式进行流转的，如图：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305202242658.png)

- New：新创建的一个线程，处于等待状态。
- Runnable：可运行状态，并不是已经运行，具体的线程调度各操作系统决定。在Runnable 中包含了 Ready、Running 两个状态，当线程调用了 start() 方法后，线程则处于就绪 Ready 状态，等待操作系统分配 CPU 时间片，分配后则进入 Running 运行状态。此外当调用 yield() 方法后，只是谦让的允许当前线程让出 CPU，但具体让不让不一定，由操作系统决定。如果让了，那么当前线程则会处于 Ready 状态继续竞争 CPU，直至执行。
- Timed_waiting：指定时间内让出 CPU 资源，此时线程不会被执行，也不会被系统调度，直到等待时间到期后才会被执行。下列方法都可以触发：Thread.sleep、Object.wait、Thread.join、LockSupport.parkNanos、LockSupport.parkUntil。
- Wating：可被唤醒的等待状态，此时线程不会被执行也不会被系统调度。**此状态可以通过 synchronized 获得锁，调用 wait 方法进入等待状态**。最后通过notify、notifyall 唤醒。下列方法都可以触发：Object.wait、Thread.join、LockSupport.park。
- Blocked：当发生锁竞争状态下，没有获得锁的线程会处于挂起状态。例如synchronized 锁，先获得的先执行，没有获得的进入阻塞状态。
- Terminated：这个是终止状态，**从 New 到 Terminated 是不可逆的**。一般是程序流程正常结束或者发生了异常。

这里参考枚举 State 类的英文注释了解了每一个状态码的含义，接下来我们去尝试操作线程方法，把这些状态体现出来。

# 三、Thread 状态测试
## 1. NEW
```java
Thread thread = new Thread(() -> {
});
System.out.println(thread.getState());
// NEW
```
这个状态很简单，就是线程创建还没有启动时就是这个状态。
## 2. RUNNABLE
```java
Thread thread = new Thread(() -> {
});
// 启动
thread.start();
System.out.println(thread.getState());
// RUNNABLE
```
创建的线程启动后 start()，就会进入 RUNNABLE 状态。**但此时并不一定在执行，而是说这个线程已经就绪，可以竞争 CPU 资源**。
## 3. BLOCKED
Object.wait()方法，会释放锁资源以及CPU资源。 Thread.sleep()方法，不会释放锁资源,但是会释放CPU资源

sleep()是使线程暂停执行一段时间的方法。wait()也是一种使线程暂停执行的方法。例如，当线程执行wait()方法时候，会释放当前的锁，然后让出CPU，进入等待状态。并且可以调用notify（）方法或者notifyAll（）方法通知正在等待的其他线程。notify（）方法仅唤醒一个线程（等待队列中的第一个线程）并允许他去获得锁。notifyAll（）方法唤醒所有等待这个对象的线程并允许他们去竞争获得锁。具体区别如下：

1)  原理不同。sleep()方法是Thread类的静态方法，是线程用来控制自身流程的，他会使此线程暂停执行一段时间，而把执行机会让给其他线程，等到计时时间一到，此线程会自动苏醒。例如，当线程执行报时功能时，每一秒钟打印出一个时间，那么此时就需要在打印方法前面加一个sleep()方法，以便让自己每隔一秒执行一次，该过程如同闹钟一样。而wait()方法是object类的方法，用于线程间通信，这个方法会使当前拥有该对象锁的进程等待，直到其他线程调用notify（）方法或者notifyAll（）时才醒来，不过开发人员也可以给他指定一个时间，自动醒来。

2)  对锁的 处理机制不同。由于sleep()方法的主要作用是让线程暂停执行一段时间，时间一到则自动恢复，不涉及线程间的通信，因此，调用sleep()方法并不会释放锁。而wait()方法则不同，当调用wait()方法后，线程会释放掉他所占用的锁，从而使线程所在对象中的其他synchronized数据可以被其他线程使用。

3)  使用区域不同。wait()方法必须放在同步控制方法和同步代码块中使用，sleep()方法则可以放在任何地方使用。sleep()方法必须捕获异常，而wait()、notify（）、notifyAll（）不需要捕获异常。在sleep的过程中，有可能被其他对象调用他的interrupt（），产生InterruptedException。由于sleep不会释放锁标志，容易导致死锁问题的发生，因此一般情况下，推荐使用wait（）方法。

 整体的区别其实是有四个：

1、sleep是线程中的方法，但是wait是Object中的方法。

2、sleep方法不会释放lock，但是wait会释放，而且会加入到等待队列中。

3、sleep方法不依赖于同步器synchronized，但是wait需要依赖synchronized关键字。

4、sleep不需要被唤醒（休眠之后推出阻塞），但是wait需要（不指定时间需要被别人中断）。

下面我们就根据这四个区别来分析。

一、sleep是线程方法，wait是Object方法

这个如何验证呢？我们还需要到jdk源码中看看。首先进入到Thread的源码中看一下，然后俺ctrl+O就可以查看方法列表。在最上面可以搜寻，我们输入“s”，就可以查看所有以s开头的方法了。

一、sleep是线程方法，wait是Object方法

这个如何验证呢？我们还需要到jdk源码中看看。首先进入到Thread的源码中看一下，然后俺ctrl+O就可以查看方法列表。在最上面可以搜寻，我们输入“s”，就可以查看所有以s开头的方法了。

![](https://pics2.baidu.com/feed/bd3eb13533fa828b03ad4cd4722bd931970a5a31.jpeg@f_auto?token=f8725a08f3091953b06a6dea92277bbd&s=696A851A199FC0CA4E441CDB020050B0)

我们会发现，slee方法真实的在Thread线程类中。下面我们以同样的方法查看wait。

![](https://pics0.baidu.com/feed/314e251f95cad1c802c62764f10aff0cc93d512a.jpeg@f_auto?token=f807d1b79934074d2702732a71ebc363&s=EDAA851A979969CA546D19DB020050B0)

这是第一个区别很容易验证，下面我们来看第二个。

二、sleep不释放lock，wait会释放

这个如何验证呢？这就需要代码了。先看我们的sleep方法

![](https://pics0.baidu.com/feed/71cf3bc79f3df8dccaf03bb44325ea8e461028fb.jpeg@f_auto?token=620feda7426578e14d579f55c33dd8a0&s=B281B14CD2BC966F5451B50E0000E0C1)

我们看一下运行结果：

![](https://pics2.baidu.com/feed/d4628535e5dde71119dcaf8328db561e9d166177.jpeg@f_auto?token=9e24626063e9313f0707b57cffee75c5)

在上面的结果中，线程2先获取了cpu资源，然后开始执行休眠，在休眠过程中线程1是没法执行的，必须要等待线程2结束之后才可以。这也就是说sleep方法不会释放锁，让其他线程进来。

然后我们测试一下wait方法。

![](https://pics7.baidu.com/feed/279759ee3d6d55fb8ec3901be316d74f20a4dd44.jpeg@f_auto?token=dc5194c00faee583fdda1049fd340a50&s=BA81F14C92BC866F5451B40E0000E0C1)

在上面的例子中，我们使用wait方法等待10秒钟，然后结束。我们看一下结果：

![](https://pics6.baidu.com/feed/72f082025aafa40f85a6060225509b4a79f01943.jpeg@f_auto?token=2e760646475c9dab7d401bfbc58c9224)

这个过程就验证了第二条区别，我们接下来看第三个。

三、sleep不依赖同步方法，wait需要

我们还是一次来验证。首先我们测试sleep方法。

![](https://pics7.baidu.com/feed/279759ee3d6d55fb0c291185e016d74f21a4dd64.jpeg@f_auto?token=848ed5dea33514da3609bcbd36e923f0&s=3281B14CD2B4BE6D5E41950E0000E0C1)

这个方法会依次运行，不会出现任何异常。然后我们主要是看wait方法。

![](https://pics2.baidu.com/feed/f603918fa0ec08fa0ca38855d4daa56854fbda5f.jpeg@f_auto?token=690d44107be3619287df5aacfd84e900&s=B281B14C52B4BE6D1ED1940E0000F0C1)

我们运行以下，看一下结果：

![](https://pics4.baidu.com/feed/d000baa1cd11728b778ec90546c85bcbc3fd2c2b.jpeg@f_auto?token=78b4305f5d5ffd5cf9dee6461331e013&s=7AE6AC1ACD20CC031E7528DB020050B5)

OK，下面我们验证一下第四条区别：

四、sleep不需要被唤醒，wait需要

sleep方法很简单，我们主要关注wait方法。看代码：

首先我们定义两个方法，一个等待方法，一个唤醒方法。

![](https://pics4.baidu.com/feed/9825bc315c6034a83b199f7e4627d1510823760b.jpeg@f_auto?token=a7b56051f42b25dcc3830e0474640067&s=3A81E14E92BE866F0E75981E00007081)

然后再去测试一下：

![](https://pics6.baidu.com/feed/7aec54e736d12f2efcef83f4c2f64d6784356871.jpeg@f_auto?token=0cf296dbdd7556193382356574d147a8&s=3A81B14CD2F4BE6C1CE5DD0F00007081)

如果没有唤醒方法，那第一个线程就会处于一直等待的状态，第二个线程唤醒了之后就不再等待了。
```java
Object obj = new Object();
new Thread(() -> {
    synchronized (obj) {
        try {
            Thread.sleep(10000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}).start();
Thread thread = new Thread(() -> {
    synchronized (obj) {
        try {
            obj.wait();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
});
thread.start();
while (true) {
    Thread.sleep(1000);
    System.out.println(thread.getState());
}
// BLOCKED
// BLOCKED
// BLOCKED
```
- 这段代码稍微有点长，主要是为了让两个线程发生锁竞争。
- 第一个线程，**synchronized 获取锁后休眠，不释放锁**。
- 第二个线程，synchronized 获取不到锁，会被挂起。
- 那么最后的输出结果就会是，BLOCKED

#### 4. WAITING
```java
Object obj = new Object();
Thread thread = new Thread(() -> {
synchronized (obj) {
try {
obj.wait();
} catch (InterruptedException e) {
e.printStackTrace();
}
}
});
thread.start();
while (true) {
Thread.sleep(1000);
System.out.println(thread.getState());
}
// WAITING
// WAITING
// WAITING
```
只要在 synchronized 代码块或者修饰的方法中，调用 wait 方法，又没有被 notify 就会进入 WAITING 状态。

另外 Thread.join 源码中也是调用的 wait 方法，所以也会让线程进入等待状态。
### 5. TIMED_WAITING
```java
Object obj = new Object();
Thread thread = new Thread(() -> {
synchronized (obj) {
try {
Thread.sleep(100000);
} catch (InterruptedException e) {
e.printStackTrace();
}
}
});
thread.start();
while (true) {
Thread.sleep(1000);
System.out.println(thread.getState());
}
```

// TIMED_WAITING
// TIMED_WAITING
// TIMED_WAITING
 
有了上面状态获取的对比，这个状态的获取就没什么难度了。只要改成
Thread.sleep(100000); 就可以了。
#### 6. TERMINATED
```java
Thread thread = new Thread(() -> {
});
thread.start();
System.out.println(thread.getState());
System.out.println(thread.getState());
System.out.println(thread.getState());
// RUNNABLE
// TERMINATED
// TERMINATED
```
这个就比较简单了，只要一个线程运行完，它的生命周期结束了，就进入了 TERMINATED 状态。
# 四、Thread 方法使用
一般情况下 Thread 中最常用的方法就是 start 启动，除此之外一些其他方法可能在平常的开发中用的不多，但这些方法在一些框架中却经常出现，因此只了解它们的概念，但是却缺少一些实例来参考！ 接下来我们就来做一些案例来验证这些方法，包括：yield、wait、notify、join。
## 1. yield
yield 方法让出 CPU，但**不一定**，一定让出！。这种可能会用在一些同时启动的线程中，**按照优先级保证重要线程的执行**；**也可以是其他一些特殊的业务场景**（例如**这个线程内容很耗时，又不那么重要，可以放在后面**）。

为了验证这个方法，我们做一个例子：启动 50 个线程进行，每个线程都进行 1000 次的加和计算。其中 10 个线程会执行让出 CPU 操作。那么，如果让出 CPU 那 10 个线程的计算加和时间都比较长，说明确实在进行让出操作。
```java
案例代码
private static volatile Map<String, AtomicInteger> count = new ConcurrentHashMap<>(
);
static class Y implements Runnable {
private String name;
private boolean isYield;
public Y(String name, boolean isYield) {
this.name = name;
this.isYield = isYield;
}
@Override
public void run() {
long l = System.currentTimeMillis();
for (int i = 0; i < 1000; i++) {
if (isYield) Thread.yield();
AtomicInteger atomicInteger = count.get(name);
if (null == atomicInteger) {
count.put(name, new AtomicInteger(1));
continue;
}
atomicInteger.addAndGet(1);
count.put(name, atomicInteger);
}
System.out.println("线程编号：" + name + " 执行完成耗时：
" + (System.currentTimeMillis() - l) + " (毫秒)" + (isYield ? "让出 CPU--------------
--------" : "不让 CPU"));
}
}
public static void main(String[] args) {
for (int i = 0; i < 50; i++) {
if (i < 10) {
new Thread(new Y(String.valueOf(i), true)).start();
continue;
}
new Thread(new Y(String.valueOf(i), false)).start();
}
}
```
测试结果
线程编号：10 执行完成耗时：2 (毫秒)不让 CPU
线程编号：11 执行完成耗时：2 (毫秒)不让 CPU
线程编号：15 执行完成耗时：1 (毫秒)不让 CPU
线程编号：14 执行完成耗时：1 (毫秒)不让 CPU
线程编号：19 执行完成耗时：1 (毫秒)不让 CPU
线程编号：18 执行完成耗时：1 (毫秒)不让 CPU
线程编号：22 执行完成耗时：0 (毫秒)不让 CPU
线程编号：26 执行完成耗时：0 (毫秒)不让 CPU
线程编号：27 执行完成耗时：1 (毫秒)不让 CPU
线程编号：30 执行完成耗时：0 (毫秒)不让 CPU
线程编号：31 执行完成耗时：0 (毫秒)不让 CPU
线程编号：34 执行完成耗时：1 (毫秒)不让 CPU
线程编号：12 执行完成耗时：1 (毫秒)不让 CPU
线程编号：16 执行完成耗时：1 (毫秒)不让 CPU
线程编号：13 执行完成耗时：1 (毫秒)不让 CPU
线程编号：17 执行完成耗时：1 (毫秒)不让 CPU
线程编号：20 执行完成耗时：0 (毫秒)不让 CPU
线程编号：23 执行完成耗时：0 (毫秒)不让 CPU
线程编号：21 执行完成耗时：0 (毫秒)不让 CPU
线程编号：25 执行完成耗时：1 (毫秒)不让 CPU
线程编号：24 执行完成耗时：1 (毫秒)不让 CPU
线程编号：28 执行完成耗时：0 (毫秒)不让 CPU
线程编号：38 执行完成耗时：0 (毫秒)不让 CPU
线程编号：39 执行完成耗时：0 (毫秒)不让 CPU
线程编号：37 执行完成耗时：1 (毫秒)不让 CPU
线程编号：40 执行完成耗时：0 (毫秒)不让 CPU
线程编号：44 执行完成耗时：0 (毫秒)不让 CPU
线程编号：36 执行完成耗时：1 (毫秒)不让 CPU
线程编号：42 执行完成耗时：1 (毫秒)不让 CPU
线程编号：45 执行完成耗时：1 (毫秒)不让 CPU
线程编号：43 执行完成耗时：1 (毫秒)不让 CPU
线程编号：46 执行完成耗时：0 (毫秒)不让 CPU
线程编号：47 执行完成耗时：0 (毫秒)不让 CPU
线程编号：35 执行完成耗时：0 (毫秒)不让 CPU
线程编号：33 执行完成耗时：0 (毫秒)不让 CPU
线程编号：32 执行完成耗时：0 (毫秒)不让 CPU
线程编号：41 执行完成耗时：0 (毫秒)不让 CPU
线程编号：48 执行完成耗时：1 (毫秒)不让 CPU
线程编号：6 执行完成耗时：15 (毫秒)让出 CPU----------------------
线程编号：7 执行完成耗时：15 (毫秒)让出 CPU----------------------
线程编号：49 执行完成耗时：2 (毫秒)不让 CPU
线程编号：29 执行完成耗时：1 (毫秒)不让 CPU
线程编号：2 执行完成耗时：17 (毫秒)让出 CPU----------------------
线程编号：1 执行完成耗时：11 (毫秒)让出 CPU----------------------
线程编号：4 执行完成耗时：15 (毫秒)让出 CPU----------------------
线程编号：8 执行完成耗时：12 (毫秒)让出 CPU----------------------
线程编号：5 执行完成耗时：12 (毫秒)让出 CPU----------------------
线程编号：9 执行完成耗时：12 (毫秒)让出 CPU----------------------
线程编号：0 执行完成耗时：21 (毫秒)让出 CPU----------------------
线程编号：3 执行完成耗时：21 (毫秒)让出 CPU----------------------

从测试结果可以看到，那些让出 CPU 的，执行完计算已经在 10 毫秒以上，说明我们的测试是效果的。
## 2. wait & notify
wait 和 notify/nofityall，是一对方法，有一个等待，就会有一个叫醒，否则
程序就夯在那不动了。关于这部分会使用到的 synchronized 在之前小傅哥有深入的源码分析，讲到它是怎么加锁在对象头的，如果你忘记了可以往前翻翻。

接下来我们模拟鹿鼎记·丽春院，清倌喝茶吟诗聊风月日常。当有达官贵人来时，需要分配清倌给大老爷。中间会有一些等待、叫醒操作。只为让你更好的记住这样的案例，不要想歪喽。清倌人即是只卖艺欢场人，喊麦的。

案例代码
```java
public class 丽春院 {
public static void main(String[] args) {
老鸨 鸨子 = new 老鸨();
清倌 miss = new 清倌(鸨子);
客官 guest = new 客官(鸨子);
Thread t_miss = new Thread(miss);
Thread t_guest = new Thread(guest);
t_miss.start();
t_guest.start();
}
}
class 清倌 implements Runnable {
老鸨 鸨子;
public 清倌(老鸨 鸨子) {
this.鸨子 = 鸨子;
}
@Override
public void run() {
int i = 1;
while (true) {
try {
Thread.sleep(1000);
} catch (InterruptedException e1) {
e1.printStackTrace();
}
if (i == 1) {
try {
鸨子.在岗清倌("苍田野子", "500 日元");
} catch (InterruptedException e) {
e.printStackTrace();
}
} else {
try {
鸨子.在岗清倌("花田岗子", "800 日元");
} catch (InterruptedException e) {
e.printStackTrace();
}
}
i = (i + 1) % 2;
}
}
}
class 客官 implements Runnable {
老鸨 鸨子;
public 客官(老鸨 鸨子) {
this.鸨子 = 鸨子;
}
@Override
public void run() {
while (true) {
try {
Thread.sleep(1000);
} catch (InterruptedException e1) {
e1.printStackTrace();
}
try {
鸨子.喝茶吟诗聊风月();
} catch (InterruptedException e) {
e.printStackTrace();
}
}
}
}
class 老鸨 {
private String 清倌 = null;
private String price = null;
private boolean 工作状态 = true;
public synchronized void 在岗清倌(String 清
倌, String price) throws InterruptedException {
if (!工作状态)
wait();//等待
this.清倌 = 清倌;
this.price = price;
工作状态 = false;
notify();//叫醒
}
public synchronized void 喝茶吟诗聊风月() throws InterruptedException {
if (工作状态)
wait();//等待
System.out.println("聊风月：" + 清倌);
System.out.println("茶水费：" + price);
System.out.println(" " + " " + " " + " " + " " + " " + " " + " " +
" " + " " + 清倌 + "完事" + "准备 ... ...");
System.out.println("****************************************");
工作状态 = true;
notify();//叫醒
}
}
```
测试结果
```
聊风月：苍田野子
茶水费：500 日元
苍田野子完事准备 ... ...
****************************************
聊风月：花田岗子
茶水费：800 日元
花田岗子完事准备 ... ...
****************************************
聊风月：苍田野子
茶水费：500 日元
苍田野子完事准备 ... ...
****************************************
```
效果效果主要体现 wait、notify，这两个方法的使用。我相信你一定能记住这个例子！
## 3. join
join 是两个线程的合并吗？不是的！
join 是让线程进入 wait ，当线程执行完毕后，会在 JVM 源码中找到，**它执行完毕后，其实执行 notify，也就是 等待 和 叫醒 操作**。
源码：jdk8u_hotspot/blob/master/src/share/vm/runtime/thread.cpp
```c
void JavaThread::exit(bool destroy_vm, ExitType exit_type) {
// Notify waiters on thread object. This has to be done after exit() is called
// on the thread (if the thread is the last thread in a daemon ThreadGroup the
// group should have the destroyed bit set before waiters are notified).
ensure_join(this);
}
static void ensure_join(JavaThread* thread) {
// 叫醒
java_lang_Thread::set_thread(threadObj(), NULL);
lock.notify_all(thread);
}
```
好的，就是这里！lock.notify_all(thread)，执行到这，就对上了。
案例代码
```java
Thread thread = new Thread(() -> {
System.out.println("thread before");
try {
Thread.sleep(3000);
} catch (Exception e) {
e.printStackTrace();
}
System.out.println("thread after");
});
thread.start();
System.out.println("main begin！");
thread.join();
System.out.println("main end！");
```
测试结果
main begin！
thread before
thread after
main end！

首先 **join() 是一个 synchronized 方法， 里面调用了 wait()**，这个过程的目的是**让「持有这个同步锁的线程」进入等待**，那么谁持有了这个同步锁呢？答案是**主线程，因为主线程调用了 threadA.join()方法**，相当于在 `threadA.join()` 代码这块**写了一个同步代码块**，谁去执行了这段代码呢，是主线程，所以主线程被 `wait()` 了。然后在子线程 threadA执行完毕之后，JVM会调用 `lock.notify_all(thread);` ，唤醒持有 threadA 这个对象锁的线程，也就是主线程，会继续执行。
- 这部分验证的主要体现就是加了 `thread.join()` 后，会影响到输出结果。如果不加，`main end!` 会优先 thread after 提前打印出来。
- join() 是一个 synchronized 方法，里面调用了 wait() 方法，**让持有当前同步锁的线程进入等待状态**，也就是主线程。当子线程执行完毕后，我们从源码中可以看到 JVM 调用了 lock.notify_all(thread) 所以唤醒了主线程继续执行。

# 五、总结
线程状态和状态的转换也是面试中必问的问题，但除了面试是我们自己在开发中，如果真的使用线程，是非常有必要了解线程状态是如何转换的。模模糊糊的使用，总会觉得担心，那么你是个好程序员！

**线程的一些深入学习都是在调用本地方法**，也就是需要了解到 JVM 层面，才能更加深刻的见到 c++代码是如何实现这部分逻辑的。

在使用线程的时候一定要让自己有一个类似多核的脑子，*线程一起、生死由你！*

本章节就扯到这了，很多的知识都是为了整套内容体系的全面，为后续介绍其他知识打下根基。感谢！