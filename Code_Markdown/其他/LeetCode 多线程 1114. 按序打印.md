> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个类：

public class Foo {
  public void first() { print("first"); }
  public void second() { print("second"); }
  public void third() { print("third"); }
}

三个不同的线程 A、B、C 将会共用一个 `Foo` 实例。

-   线程 A 将会调用 `first()` 方法
-   线程 B 将会调用 `second()` 方法
-   线程 C 将会调用 `third()` 方法

请设计修改程序，以确保 `second()` 方法在 `first()` 方法之后被执行，`third()` 方法在 `second()` 方法之后被执行。

**提示：**

-   尽管输入中的数字似乎暗示了顺序，但是我们并不保证线程在操作系统中的调度顺序。
-   你看到的输入格式主要是为了确保测试的全面性。

**示例 1：**

**输入：**nums = [1,2,3]
**输出：**"firstsecondthird"
**解释：**
有三个线程会被异步启动。输入 [1,2,3] 表示线程 A 将会调用 first() 方法，线程 B 将会调用 second() 方法，线程 C 将会调用 third() 方法。正确的输出是 "firstsecondthird"。

**示例 2：**

**输入：**nums = [1,3,2]
**输出：**"firstsecondthird"
**解释：**
输入 [1,3,2] 表示线程 A 将会调用 first() 方法，线程 B 将会调用 third() 方法，线程 C 将会调用 second() 方法。正确的输出是 "firstsecondthird"。

**提示：**

-   `nums` 是 `[1, 2, 3]` 的一组排列

> 为了防止出现并发竞争状态，需要一种具有两种功能的机制：
> 1. 关键部分的访问控制：需要某些关键部分代码具有排他性，即在给定的时间内，只有一个线程可以进入关键部分代码。
> 2. 通知阻塞线程：唤醒其他等待线程。

这是一个典型的执行屏障的问题，可以通过构造屏障来实现。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304230147509.png)
> 如果最先调度的是second线程，那second使用lock后，发现isFinished为false就进入死循环状态，并且不释放lock，导致其它线程无法执行，这样不是死锁了吗？ 望大佬们解答，谢谢。
> 调用lock.wait()方法是会释放锁的。

如下图，我们需要构造 222 道屏障，second 线程等待 first 屏障，third 线程等待 second 屏障。：{:align=center}

first 线程会释放 first 屏障，而 second 线程会释放 second 屏障。

Java 中，我们使用线程等待的方式实现执行屏障，使用释放线程等待的方式实现屏障消除。具体代码如下：

代码：
Java
class Foo {
    
    private boolean firstFinished;
    private boolean secondFinished;
    private Object lock = new Object();

    public Foo() {
        
    }

    public void first(Runnable printFirst) throws InterruptedException {
        
        synchronized (lock) {
            // printFirst.run() outputs "first". Do not change or remove this line.
            printFirst.run();
            firstFinished = true;
            lock.notifyAll(); 
        }
    }

    public void second(Runnable printSecond) throws InterruptedException {
        
        synchronized (lock) {
            while (!firstFinished) {
                lock.wait();
            }
        
            // printSecond.run() outputs "second". Do not change or remove this line.
            printSecond.run();
            secondFinished = true;
            lock.notifyAll();
        }
    }

    public void third(Runnable printThird) throws InterruptedException {
        
        synchronized (lock) {
           while (!secondFinished) {
                lock.wait();
            }

            // printThird.run() outputs "third". Do not change or remove this line.
            printThird.run();
        } 
    }
}
我们使用一个 Ojbect 对象 lock 实现所有执行屏障的锁对象，两个布尔型对象 firstFinished，secondFinished 保存屏障消除的条件。


题目要求按顺序依次执行三个方法，且每个方法都在单独的线程中运行。为了保证线程的执行顺序，可以在方法之间创建一些依赖关系，即第二个方法必须在第一个方法之后执行，第三个方法必须在第二个方法之后执行。

方法对之间的依赖关系形成了所有方法的特定的执行顺序。例如 A < B, B < C，则所有方法的执行顺序为 A < B < C。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304230125893.png)


依赖关系可以通过并发机制实现。使用一个共享变量 firstJobDone 协调第一个方法与第二个方法的执行顺序，使用另一个共享变量 secondJobDone 协调第二个方法与第三个方法的执行顺序。

算法

首先初始化共享变量 firstJobDone 和 secondJobDone，初始值表示所有方法未执行。

方法 first() 没有依赖关系，可以直接执行。在方法最后更新变量 firstJobDone 表示该方法执行完成。

方法 second() 中，检查 firstJobDone 的状态。如果未更新则进入等待状态，否则执行方法 second()。在方法末尾，更新变量 secondJobDone 表示方法 second() 执行完成。

方法 third() 中，检查 secondJobDone 的状态。与方法 second() 类似，执行 third() 之前，需要先等待 secondJobDone 的状态。
 ![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304230126021.png)

多线程中执行设置执行屏障的问题.说白了,就是保证线程内部的可见性.在java中,解决这一题有如下几种方式

两个volatile变量
    class Foo {
        volatile boolean firstFinished = false;
        volatile boolean secondFinished = false;

        public Foo() {

        }

        public void first(Runnable printFirst) throws InterruptedException {

            // printFirst.run() outputs "first". Do not change or remove this line.
            printFirst.run();
            firstFinished = true;
        }

        public void second(Runnable printSecond) throws InterruptedException {

            // printSecond.run() outputs "second". Do not change or remove this line.

            if (!firstFinished) {
                while (!firstFinished) {
                }
            }
            printSecond.run();
            secondFinished = true;
        }

        public void third(Runnable printThird) throws InterruptedException {

            // printThird.run() outputs "third". Do not change or remove this line.
            if (!secondFinished) {
                while (!secondFinished) {
                }
            }
            printThird.run();
        }
在程序运行期间,由于有两个线程在死循环,这一点对CPU不是特别友好,使用两个CountDownLatch也能搞定
    class Foo {

        private CountDownLatch count1=new CountDownLatch(1);
        private CountDownLatch count2=new CountDownLatch(1);

        public Foo() {

        }

        public void first(Runnable printFirst) throws InterruptedException {

            printFirst.run();
            count1.countDown();
        }

        public void second(Runnable printSecond) throws InterruptedException {

            count1.await();
            printSecond.run();
            count2.countDown();

        }

        public void third(Runnable printThird) throws InterruptedException {
            count2.await();
            printThird.run();
        }
    }
如果使用最原始的wait和notify呢?
    class Foo {

        final private Object lock1 = new Object();
        final private Object lock2 = new Object();

        public Foo() {

        }

        public void first(Runnable printFirst) throws InterruptedException {

            synchronized (lock1) {
                printFirst.run();
                lock1.notify();
            }
        }

        public void second(Runnable printSecond) throws InterruptedException {
            synchronized (lock1) {
                lock1.wait();
                printSecond.run();
                synchronized (lock2){
                    lock2.notify();
                }
            }
        }

        public void third(Runnable printThird) throws InterruptedException {
            synchronized (lock2) {
                lock2.wait();
                printThird.run();
            }

        }
    }
其实吧,运行过才知道,wait会自动释放锁,但是notify却是不会释放锁.

 
内置锁已经足以保证单个变量在多线程下的可见性了
```java
public class Foo {
    private volatile int flag = 1;
    private final Object object = new Object();

    public Foo() {

    }

    public void first(Runnable printFirst) throws InterruptedException {
        synchronized (object) {
            while (flag != 1) object.wait();
            printFirst.run();
            flag = 2;
            object.notifyAll();
        }
    }

    public void second(Runnable printSecond) throws InterruptedException {
        synchronized (object) {
            while (flag != 2) object.wait();
            printSecond.run();
            flag = 3;
            object.notifyAll();
        }
    }

    public void third(Runnable printThird) throws InterruptedException {
        synchronized (object) {
            while (flag != 3) object.wait();
        }
        printThird.run();
    }
}
```
### 原子变量
```java
class Foo {

  private AtomicInteger firstJobDone = new AtomicInteger(0);
  private AtomicInteger secondJobDone = new AtomicInteger(0);

  public Foo() {}

  public void first(Runnable printFirst) throws InterruptedException {
    // printFirst.run() outputs "first".
    printFirst.run();
    // mark the first job as done, by increasing its count.
    firstJobDone.incrementAndGet();
  }

  public void second(Runnable printSecond) throws InterruptedException {
    while (firstJobDone.get() != 1) {
      // waiting for the first job to be done.
    }
    // printSecond.run() outputs "second".
    printSecond.run();
    // mark the second as done, by increasing its count.
    secondJobDone.incrementAndGet();
  }

  public void third(Runnable printThird) throws InterruptedException {
    while (secondJobDone.get() != 1) {
      // waiting for the second job to be done.
    }
    // printThird.run() outputs "third".
    printThird.run();
  }
}
```
### 信号量Semaphore
0就是没有资源可以获取，只能阻塞，当有别的线程调用release时，才会给信号量加一个资源
```java
class Foo {
    private Semaphore s12;
    private Semaphore s23;
    public Foo() {
        s12 = new Semaphore(0);
        s23 = new Semaphore(0);
    }
    public void first(Runnable printFirst) throws InterruptedException {
        printFirst.run();
        s12.release();
    }
    public void second(Runnable printSecond) throws InterruptedException {
        s12.acquire();
        printSecond.run();
        s23.release();
    }

    public void third(Runnable printThird) throws InterruptedException {
        s23.acquire();
        printThird.run();
    }
}
```
### CountDownLatch 
两个 CountDownLatch，避免自旋浪费 cpu
```java
 class Foo {

    private final CountDownLatch firstDone;
    private final CountDownLatch secondDone;

    public Foo() {
        firstDone = new CountDownLatch(1);
        secondDone = new CountDownLatch(1);
    }

    public void first(Runnable printFirst) throws InterruptedException {
        // printFirst.run() outputs "first". Do not change or remove this line.
        printFirst.run();
        firstDone.countDown();
    }

    public void second(Runnable printSecond) throws InterruptedException {
        firstDone.await();
        // printSecond.run() outputs "second". Do not change or remove this line.
        printSecond.run();
        secondDone.countDown();
    }

    public void third(Runnable printThird) throws InterruptedException {
        secondDone.await();
        // printThird.run() outputs "third". Do not change or remove this line.
        printThird.run();
    }
}
```


方法四： 阻塞队列
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.SynchronousQueue;

class Foo {
    BlockingQueue<String> blockingQueue12, blockingQueue23;

    public Foo() {
        //同步队列,没有容量，进去一个元素，必须等待取出来以后，才能再往里面放一个元素
        blockingQueue12 = new SynchronousQueue<>();
        blockingQueue23 = new SynchronousQueue<>();
    }

    public void first(Runnable printFirst) throws InterruptedException {

        // printFirst.run() outputs "first". Do not change or remove this line.
        printFirst.run();
        blockingQueue12.put("stop");
    }

    public void second(Runnable printSecond) throws InterruptedException {
        blockingQueue12.take();
        // printSecond.run() outputs "second". Do not change or remove this line.
        printSecond.run();
        blockingQueue23.put("stop");
    }

    public void third(Runnable printThird) throws InterruptedException {
        blockingQueue23.take();
        // printThird.run() outputs "third". Do not change or remove this line.
        printThird.run();
    }
} 


import java.util.concurrent.CountDownLatch;

class Foo {

    CountDownLatch countDownLatch12, countDownLatch23;

    public Foo() {
        countDownLatch12 = new CountDownLatch(1);
        countDownLatch23 = new CountDownLatch(1);
    }

    public void first(Runnable printFirst) throws InterruptedException {

        // printFirst.run() outputs "first". Do not change or remove this line.
        printFirst.run();
        countDownLatch12.countDown();
    }

    public void second(Runnable printSecond) throws InterruptedException {
        //等待计数器归零再向下执行
        countDownLatch12.await();
        // printSecond.run() outputs "second". Do not change or remove this line.
        printSecond.run();
        countDownLatch23.countDown();
    }

    public void third(Runnable printThird) throws InterruptedException {
        //等待计数器归零再向下执行
        countDownLatch23.await();
        // printThird.run() outputs "third". Do not change or remove this line.
        printThird.run();
    }
}

#### 方法三：CountDownLatch(减法计数器)

方法一：lock锁
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

class Foo {

    int num;
    Lock lock;
    //精确的通知和唤醒线程
    Condition condition1, condition2, condition3;

    public Foo() {
        num = 1;
        lock = new ReentrantLock();
        condition1 = lock.newCondition();
        condition2 = lock.newCondition();
        condition3 = lock.newCondition();
    }

    public void first(Runnable printFirst) throws InterruptedException {
        lock.lock();
        try {
            while (num != 1) {
                condition1.await();
            }
            // printFirst.run() outputs "first". Do not change or remove this line.
            printFirst.run();
            num = 2;
            condition2.signal();
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            lock.unlock();
        }
    }

    public void second(Runnable printSecond) throws InterruptedException {
        lock.lock();
        try {
            while (num != 2) {
                condition2.await();
            }
            // printSecond.run() outputs "second". Do not change or remove this line.
            printSecond.run();
            num = 3;
            condition3.signal();
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            lock.unlock();
        }
    }

    public void third(Runnable printThird) throws InterruptedException {
        lock.lock();
        try {
            while (num != 3) {
                condition3.await();
            }
            // printThird.run() outputs "third". Do not change or remove this line.
            printThird.run();
            num = 1;
            condition1.signal();
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            lock.unlock();
        }
    }
}
方法一有问题，如果是first先被调用，在first执行完后唤醒second时，此时second还未被调用因此未进入等待队列，唤醒了个寂寞，等到second被线程调用时，线程将会一直等待
线程A、B、C共用一把lock锁，创建三个condition监视器用来精准通知和唤醒线程，每个监视器对应一个线程，标志位num来决定哪个线程被唤醒。开始时num=1只有调用first()的线程可以执行，调用second()和third()的线程会阻塞，直到first()执行完num=2，调用second()的线程被唤醒，以此类推，不知道有没有说清楚...

num = 2;//此时只有2可以执行 condition2.signal();//如果2正在等待，让2结束等待，进入可执行状态