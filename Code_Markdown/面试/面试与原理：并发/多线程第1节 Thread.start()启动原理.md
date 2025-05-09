> 有句话：正因为你优秀，所以难以卓越！刚开始听这句话还在上学，既不卓越、也不优秀，甚至可能还有点笨！但突然从某次爬到班级的前几名后，开始喜欢上了这种感觉，原来前面的风景是如此灿烂😜！
> 优秀和卓越差的不是一个等级，当你感觉自己优秀后，还能保持空瓶的心态开始，才能逐步的像卓越迈进，并漫漫长！
> 是不小时候更容易学会更多的知识，但越大越笨了！人可能很容易被自己的年纪大了，当成长者。却很少能保持一个低姿态谦卑的心态，不断的学习。所以最后，放不下自己，也拾不起能力。
> 喜欢一句话，蓝是天的颜色、红是火的象征，我不学大海抄袭天的蓝、也不学晚霞模拟火的红。我就是我，生命是我的、命运是我的。健身也是你的、学习也是你的，只要你有一个好心态，自然会走到前面卓越那里！

# 一、面试题
谢飞机，小记！ 码德，年轻人写代码好猖狂，不遵守规范还喷我，你要耗子尾汁！
谢飞机骂骂咧咧的下班后，找面试官聊心得。
谢飞机：我感觉天天就像活在粪堆，代码都是乱糟糟，我有心无力！
面试官：怎么，想跳槽了？
谢飞机：想去写代码有规范的公司，想提升！
面试官：嗯！确实，有些大公司的代码质量要好一些。但是你也要自身能力强的。
谢飞机：是的，我一直在努力学习！准备跑路！
面试官：那我顺便考你个题，看看你进大厂的几率大不。嗯... Java 线程如何启动的？
谢飞机：如何启动的？**start 启动的！**
面试官：还有吗？
谢飞机：嗯...，没了！
面试官：嗯，可能会与不会这一个题并不会让你代码有多牛、有多好，但是你的技术栈深度和广度，决定你的编程职业生涯是否有一条康庄大道。还是要多努力！

# 二、线程启动分析
```java
new Thread(() -> {
	// todo
}).start();
```
Java 的线程创建和启动非常简单，但如果问==一个线程是怎么启动起来的==往往并不清楚，甚至不知道为什么启动时是==调用 start()==，而不是==调用 run()==方法呢？
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305201948046.png)
那么，为了让大家有一个更直观的认知，我们先站在上帝视角。把这段 Java 的线程代码，到 **JDK 方法使用**，以及 **JVM 的相应处理过程**，展示给大家，以方便后续逐步分析。
- 线程的启动会涉及到本地方法（JNI）的调用，也就是那部分 C++ 编写的代码。**JVM 的实现中会有不同操作系统对线程的统一处理**，比如：Win、Linux、Unix。
- 线程的启动会涉及到线程的生命周期状态（RUNNABLE），以及唤醒操作，所以**最终会有回调操作**。也就是**调用我们的 run() 方法**

# 三、线程启动过程
接下来，我们就开始逐步分析每一步源码的执行内容，从而了解线程启动过程。
## 1. Thread start UML 图
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305202001018.png)
如图 19-2 是线程的启动过程时序图，整体的链路较长，会涉及到 JVM 的操作。

核心源码如下：
1. Thread.c：https://github.com/unofficial-
openjdk/openjdk/blob/jdk/jdk/src/java.base/share/native/libjava/Thread.c
2. jvm.cpp：
https://github.com/JetBrains/jdk8u_hotspot/blob/master/src/share/vm/prims/jvm.cpp
3. thread.cpp：
https://github.com/JetBrains/jdk8u_hotspot/blob/master/src/share/vm/runtime/thread.cpp
4. os.cpp：
https://github.com/JetBrains/jdk8u_hotspot/blob/master/src/share/vm/runtime/os.hpp
5. os_linux.cpp：
https://github.com/JetBrains/jdk8u_hotspot/blob/master/src/os/linux/vm/os_linux.cpp
6. os_windows.cpp：
https://github.com/JetBrains/jdk8u_hotspot/blob/master/src/os/windows/vm/os_windows.cpp
7. vmSymbols.hpp：
https://github.com/JetBrains/jdk8u_hotspot/blob/master/src/share/vm/classfile/vmSymbols.hpp

### 2. Java 层面 Thread 启动
#### 2.1 start() 方法
```java
new Thread(() -> {
// todo
}).start();

// JDK 源码
public synchronized void start() {
    if (threadStatus != 0)
        throw new IllegalThreadStateException();
    group.add(this);
    boolean started = false;
    try {
	start0();
	started = true;
    } finally {
        try {
            if (!started) {
                group.threadStartFailed(this);
            }
        } catch (Throwable ignore) {}
    }
}
```
- 线程启动方法 start()，在它的方法英文注释中已经把核心内容描述出来。Causes this thread to begin execution; the Java Virtual Machine calls the run method of this thread. 这段话的意思是：**由 JVM 调用此线程的 run 方法，使线程开始执行**。其实这就是一个 JVM 的回调过程，下文源码分析中会讲到

- 另外 start() 是一个 synchronized 方法，但为了避免多次调用，在方法中会由线程状态判断。threadStatus != 0。
- group.add(this)，是把当前线程加入到线程组，ThreadGroup。
- **start0()，是一个本地方法，通过 JNI 方式调用执行**。这一步的操作才是启动线程的核心步骤。

### 2.2 start0() 本地方法
```java
// 本地方法 start0
private native void start0();
// 注册本地方法
public class Thread implements Runnable {
    /* Make sure registerNatives is the first thing <clinit> does. */
    private static native void registerNatives();
    static {
        registerNatives();
    }
    // ...
}
```
- start0()，是一个本地方法，用于启动线程。
- registerNatives()，这个方法是用于**注册线程执行过程中需要的一些本地方法**，比如：start0、isAlive、yield、sleep、interrupt0 等。registerNatives，本地方法定义在 Thread.c 中，以下是定义的核心源码：
```c
static JNINativeMethod methods[] = {
{"start0", "()V", (void *)&JVM_StartThread},
{"stop0", "(" OBJ ")V", (void *)&JVM_StopThread},
{"isAlive", "()Z", (void *)&JVM_IsThreadAlive},
{"suspend0", "()V", (void *)&JVM_SuspendThread},
{"resume0", "()V", (void *)&JVM_ResumeThread},
{"setPriority0", "(I)V", (void *)&JVM_SetThreadPriority},
{"yield", "()V", (void *)&JVM_Yield},
{"sleep", "(J)V", (void *)&JVM_Sleep},
{"currentThread", "()" THD, (void *)&JVM_CurrentThread},
{"interrupt0", "()V", (void *)&JVM_Interrupt},
{"holdsLock", "(" OBJ ")Z", (void *)&JVM_HoldsLock},
{"getThreads", "()[" THD, (void *)&JVM_GetAllThreads},
{"dumpThreads", "([" THD ")[[" STE, (void *)&JVM_DumpThreads},
{"setNativeName", "(" STR ")V", (void *)&JVM_SetNativeThreadName},
};
```
源码：https://github.com/unofficial-
openjdk/openjdk/blob/jdk/jdk/src/java.base/share/native/libjava/Thread.c

从定义中可以看到，**start0 方法会执行 &JVM_StartThread 方法，最终由
JVM 层面启动线程**。
## 3. JVM 创建线程
### 3.1 JVM_StartThread
源码：https://github.com/JetBrains/jdk8u_hotspot/blob/master/src/share/vm/
prims/jvm.cpp
```java
JVM_ENTRY(void, JVM_StartThread(JNIEnv* env, jobject jthread))
    JVMWrapper("JVM_StartThread");
    JavaThread *native_thread = NULL;
    // 创建线程
    native_thread = new JavaThread(&thread_entry, sz);
    // 启动线程
    Thread::start(native_thread);
    JVM_END ...
```
这部分代码比较多，但**核心内容主要是创建线程和启动线程**，另外
**&thread_entry 也是一个方法**，如下：
#### thread_entry，线程入口
```java
static void thread_entry(JavaThread* thread, TRAPS) {
    HandleMark hm(THREAD);
    Handle obj(THREAD, thread->threadObj());
    JavaValue result(T_VOID);
    JavaCalls::call_virtual(&result,
        obj,
        KlassHandle(THREAD, SystemDictionary::Thread_klass()),
        vmSymbols::run_method_name(),
        vmSymbols::void_method_signature(),
        THREAD);
}
```
**重点**，在创建线程引入这个**线程入口的方法**时，thread_entry 中包括了 Java 的回调函数 **JavaCalls::call_virtual**。这个回调函数会由 JVM 调用。
**vmSymbols::run_method_name()，就是那个被回调的方法**，源码如下：
源码：
https://github.com/JetBrains/jdk8u_hotspot/blob/master/src/share/vm/classfile/vmSymbols.hpp
```java
#define VM_SYMBOLS_DO(template, do_alias)
template(run_method_name, "run")
```
这个 run 就是我们的 Java 程序中会被调用的 run 方法。接下来我们继续按照代码执行链路，**寻找到这个被回调的方法在什么时候调用的**。
### 3.2 JavaThread
```java
native_thread = new JavaThread(&thread_entry, sz);
```
接下来，我们继续看 JavaThread 的源码执行内容。

源码：
https://github.com/JetBrains/jdk8u_hotspot/blob/master/src/share/vm/runtime/thread.cpp
```c
JavaThread::JavaThread(ThreadFunction entry_point, size_t stack_sz) :
    Thread()
#if INCLUDE_ALL_GCS
    , _satb_mark_queue(&_satb_mark_queue_set),
    _dirty_card_queue(&_dirty_card_queue_set)
#endif // INCLUDE_ALL_GCS
{
    if (TraceThreadEvents) {
        tty->print_cr("creating thread %p", this);
    }
    initialize();
    _jni_attach_state = _not_attaching_via_jni;
    set_entry_point(entry_point);
    // Create the native thread itself.
    // %note runtime_23
    os::ThreadType thr_type = os::java_thread;
    thr_type = entry_point == &compiler_thread_entry ? os::compiler_thread :os::java_thread;
    os::create_thread(this, thr_type, stack_sz);
}
```
- ThreadFunction entry_point，就是我们上面的 thread_entry 方法。
- size_t stack_sz，表示进程中已有的线程个数。

这两个参数，都会传递给 os::create_thread 方法，**用于创建线程使用**。
### 3.3 os::create_thread
源码： 
- os_linux.cpp: https://github.com/JetBrains/jdk8u_hotspot/blob/master/src/os/linux/vm/os_linux.cpp
- os_windows.cpp： https://github.com/JetBrains/jdk8u_hotspot/blob/master/src/os/windows/vm/os_windows.cpp

众所周知，JVM 是个啥！，所以它的 OS 服务实现，Liunx 还有 Windows 等，**都会实现线程的创建逻辑**。这有点像适配器模式
```c
os_linux -> os::create_thread
bool os::create_thread(Thread* thread, ThreadType thr_type, size_t stack_size) {
    assert(thread->osthread() == NULL, "caller responsible");
    // Allocate the OSThread object
    OSThread* osthread = new OSThread(NULL, NULL);
    
    // Initial state is ALLOCATED but not INITIALIZED
    osthread->set_state(ALLOCATED);
    
    pthread_t tid;
    int ret = pthread_create(&tid, &attr, (void* (*)(void*)) java_start, thread);
    return true;
}
```
- osthread->set_state(ALLOCATED)，初始化已分配的状态，但此时并没有
初始化。
- **pthread_create，是类 Unix 操作系统（Unix、Linux、Mac OS X 等）的创建线程的函数**。
- **java_start，重点关注类，是实际创建线程的方法**。

### 3.4 java_start
源码：https://github.com/JetBrains/jdk8u_hotspot/blob/master/src/os/linux/vm/os_linux.cpp
```java
static void *java_start(Thread *thread) {
    // 线程 ID
    int pid = os::current_process_id();
    
    // 设置线程
    ThreadLocalStorage::set_thread(thread);
    
    // 设置线程状态：INITIALIZED 初始化完成
    osthread->set_state(INITIALIZED);

    // 唤醒所有线程
    sync->notify_all();
    
    // 循环，初始化状态，则一致等待 wait
    while (osthread->get_state() == INITIALIZED) {
        sync->wait(Mutex::_no_safepoint_check_flag);
    }
    
    // 等待唤醒后，执行 run 方法
    thread->run();
    return 0;
}
```
- JVM 设置线程状态，INITIALIZED 初始化完成。
- sync->notify_all()，唤醒所有线程。
- osthread->get_state() == INITIALIZED，while **循环等待**
- ==thread->run()，是等待线程唤醒后，也就是状态变更后，才能执行到==。这在我们的线程执行 UML 图中，也有所体现

## 4. JVM 启动线程
```java
JVM_ENTRY(void, JVM_StartThread(JNIEnv* env, jobject jthread))
JVMWrapper("JVM_StartThread");
JavaThread *native_thread = NULL;
// 创建线程
native_thread = new JavaThread(&thread_entry, sz);
// 启动线程
Thread::start(native_thread);

JVM_END
```
JVM_StartThread 中有两步，创建（new JavaThread）、启动
（Thread::start）。创建的过程聊完了，接下来我们聊启动。

### 4.1 Thread::start
源码：
https://github.com/JetBrains/jdk8u_hotspot/blob/master/src/share/vm/runtime/thread.cpp
```c
void Thread::start(Thread* thread) {
    trace("start", thread);
    
    if (!DisableStartThread) {
        if (thread->is_Java_thread()) {
            java_lang_Thread::set_thread_status(((JavaThread*)thread)->threadObj(),
            java_lang_Thread::RUNNABLE);
        }
        // 不同的 OS 会有不同的启动代码逻辑
        os::start_thread(thread);
    }
}
```
- 如果**没有禁用线程** DisableStartThread 并且是 Java 线程 thread->is_Java_thread()，那么**设置线程状态为** RUNNABLE。
- os::start_thread(thread)，调用线程启动方法。不同的 OS 会有不同的启动代码逻辑

### 4.2 os::start_thread(thread)
源码：
https://github.com/JetBrains/jdk8u_hotspot/blob/master/src/share/vm/runtime/os.hpp
```c
void os::start_thread(Thread* thread) {
    // guard suspend/resume
    MutexLockerEx ml(thread->SR_lock(), Mutex::_no_safepoint_check_flag);
    
    OSThread* osthread = thread->osthread();
    osthread->set_state(RUNNABLE);
    
    pd_start_thread(thread);
}
```
- osthread->set_state(RUNNABLE)，设置线程状态 RUNNABLE
- pd_start_thread(thread)，启动线程，**这个就由各个 OS 实现类，实现各自系统的启动方法了**。比如，windows 系统和 Linux 系统的代码是完全不同的。

### 4.3 pd_start_thread(thread)
源码：
https://github.com/JetBrains/jdk8u_hotspot/blob/master/src/os/linux/vm/os_linux.cpp
```c
void os::pd_start_thread(Thread* thread) {
    OSThread * osthread = thread->osthread();
    assert(osthread->get_state() != INITIALIZED, "just checking");
    
    Monitor* sync_with_child = osthread->startThread_lock();
    MutexLockerEx ml(sync_with_child, Mutex::_no_safepoint_check_flag);
    sync_with_child->notify();
}
```
这部分代码 notify() 最关键，它可以唤醒线程。

线程唤醒后，3.4 中的 thread->run(); 就可以继续执行了。
## 5. JVM 线程回调
### 5.1 thread->run()[JavaThread::run()]
源码：
https://github.com/JetBrains/jdk8u_hotspot/blob/master/src/share/vm/runtime/thread.cpp
```c
// The first routine called by a new Java thread
void JavaThread::run() {
    // ... 初始化线程操作
    thread_main_inner();
}
```
os_linux.cpp 类中的 java_start 里的 thread->run()，最终调用的就是 thread.cpp 的 JavaThread::run() 方法。

**这部分还需要继续往下看**，thread_main_inner(); 方法。
### 5.2 thread_main_inner
源码：
https://github.com/JetBrains/jdk8u_hotspot/blob/master/src/share/vm/
runtime/thread.cpp
```java
void JavaThread::thread_main_inner() {
    if (!this->has_pending_exception() &&
    !java_lang_Thread::is_stillborn(this->threadObj())) {
        {
            ResourceMark rm(this);
            this->set_native_thread_name(this->get_thread_name());
        }
        HandleMark hm(this);
        this->entry_point()(this, this);
    }
    DTRACE_THREAD_PROBE(stop, this);
    this->exit(false);
    delete this;
}
```
- 这里有你熟悉的设置的线程名称，this->set_native_thread_name(this->get_thread_name())。
- this->entry_point()，实际调用的就是 3.1 中的 thread_entry 方法。

- thread_entry，方法最终会调用到 JavaCalls::call_virtual 里的vmSymbols::run_method_name()。也就是 run() 方法，**至此线程启动完成**。终于串回来了！
# 四、总结
线程的启动过程涉及到了 JVM 的参与，所以如果没有认真了解过，确实很难从一个本地方法了解的如此透彻。

整个源码分析可以结合着代码调用 UML 时序图进行学习，基本核心过程包括：
1. Java 创建线程和启动
2. 调用本地方法 start0()
3. JVM 中JVM_StartThread 的创建和启动、设置线程状态等待被唤醒、根据不同的 OS 启动线程并唤醒
4. 最后回调 run() 方法启动 Java 线程

有时候可能只是一步很简单的方法，也会有它的深入之处，当真的懂了以后，就不用死记硬背