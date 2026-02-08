**要求**
* 掌握五种单例模式的实现方式（**构造方法都要私有**！！）
* 理解为何DCL实现时**要使用 volatile 修饰静态变量**
* 了解 **jdk 中用到单例的场景**

饿汉式不会出现多线程中的问题：因为饿汉式是类在初始化（类的初始化只有一次）时给静态变量赋值（用静态代码块实例化）的，虚拟机能保证其执行的线程安全。**静态代码的执行处于类生命周期中的初始化阶段，由虚拟机保证其原子且安全执行**。

---
# 1. 饿汉式
**饿汉式**（相对于懒汉式，第一次调用getInstance方法时、即用到时才创建这个实例；而这里**只要类初始化了，实例就会被创建出来**）类先加载、链接再初始化：
- 私有构造方法
- **private static final变量，类型就是单例类型**，用自己的私有构造创建唯一的一个实例(静态变量类加载的时候就初始化，存储在方法区)
- 提供一个公共的静态方法，获得该实例
```java
public class Singleton1 implements Serializable {
    private Singleton1() {
        if (INSTANCE != null) {
            throw new RuntimeException("单例对象不能重复创建");
        }
        System.out.println("private Singleton1()");
    }

    private static final Singleton1 INSTANCE = new Singleton1();

    public static Singleton1 getInstance() {
        return INSTANCE;
    }

    public Object readResolve() {
        return INSTANCE;
    }
    
    //public static void otherMethod() {
    //    System.out.println("otherMethod()");
    //}
}
```
* 构造方法抛出异常是**防止反射破坏单例**
* **类实现了Serializable，可能被反序列化破坏单例**，**`readResolve()` 是防止反序列化破坏单例**

下面调用了Singleton1的静态方法，此时会初始化类，并饿汉式创建类对象
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303232330103.png)

**反射破坏单例**（java的访问修饰符是可以被反射破解的，所以是防君子不防小人）：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303232333184.png)
运行后发现会创建第二个对象：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303232335276.png)

所以Singleton1的构造方法中判断instance是否!=null，**为true说明要第二次构造，就抛出异常**。
> 无论先后调用反射，创建对象都要走这个私有构造，这不是判断了么，有对象就抛异常，不能理解吗

下面是反序列化+序列化：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303232337975.png)
现在又有多个实例了：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303232338253.png)

unsafe对象是Java内部对象，破坏单例好像预防不了。Spring的UnsafeUtils？：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303232340144.png)

> 只有该类被销毁时，静态变量会被回收

# 2. 枚举饿汉式
**枚举饿汉式**（枚举本质上还是被编译成类，可以很方便地控制对象个数）
枚举饿汉式不会出现多线程中的问题：因为==枚举变量同样是类在初始化（类的初始化只有一次）时给静态变量赋值（用静态代码块实例化）的，虚拟机能保证其执行的线程安全==。**静态代码的执行处于类生命周期中的初始化阶段，由虚拟机保证其原子且安全执行**。
> 面试官：那你说一下在jvm虚拟机中怎么保证静态代码块的线程安全
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303232345436.png)

```java
public enum Singleton2 {
    INSTANCE; // 类一加载，在静态代码块中就把枚举对象创造出来

//    private Singleton2() { // 测试时才提供！
//        System.out.println("private Singleton2()");
//    }

    @Override
    public String toString() {
        return getClass().getName() + "@" + Integer.toHexString(hashCode());
    }

    public static Singleton2 getInstance() {
        return INSTANCE;
    }

    public static void otherMethod() {
        System.out.println("otherMethod()");
    }
}
```
* **枚举饿汉式能天然防止反射**（==枚举没有无参构造；反射调用时特殊处理，抛出异常==）、**反序列化**（==ObjectInputStream中对枚举类特殊处理，会把枚举类实例直接返回==，不会从反序列化生成的字节数组中生成）**破坏单例**
* unsafe还是可以破坏
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303232356554.png)

	unsafe突破枚举对象个数限制：
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303232356551.png)

# 3. 懒汉式单例（需要避免在多线程环境下使用！）
**懒汉式**（需要考虑在多线程下用这个类！）
```java
public class Singleton3 implements Serializable {
    private Singleton3() {
        System.out.println("private Singleton3()");
    }

    private static Singleton3 INSTANCE = null;

    // Singleton3.class
    public static synchronized Singleton3 getInstance() {
        if (INSTANCE == null) {
            INSTANCE = new Singleton3();
        }
        return INSTANCE;
    }

    public static void otherMethod() {
        System.out.println("otherMethod()");
    }
}
```
* 其实**只有首次创建单例对象时才需要同步**，但该代码实际上每次调用都会同步
* 因此**有了下面的双检锁改进**

> sync使用的时候要判断你当前调用的方法是属于类还是属于实例对象
> **静态方法可不就是Singleton3.class作为锁对象**
> ==创建对象时需要synchronized==，之后就不需要了
> ![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303240003536.png)

---
# 4. 双检锁懒汉式Double Check Locking
刚开始线程1线程2都竞争内部的锁，线程1占用后线程2等待，线程1创建对象、然后解锁，接着线程2进来发现已经有对象了，就不会重复创建大对象。
```java
public class Singleton4 implements Serializable {
    private Singleton4() {
        System.out.println("private Singleton4()");
    }

    private static volatile Singleton4 INSTANCE = null; // 可见性，有序性

    public static Singleton4 getInstance() {
        if (INSTANCE == null) { // 过了竞争阶段,直接跳过synchronized块 // 外面的if是用来提升性能的，不为null直接return
            synchronized (Singleton4.class) {
                if (INSTANCE == null) { // 内层检查是防止挂起阻塞的线程再次创建单例对象，导致单例被破坏
                    INSTANCE = new Singleton4();
                }
            }
        }
        return INSTANCE;
    }

//    public static void otherMethod() {
//        System.out.println("otherMethod()");
//   }
}
```
为何**必须加 volatile**：
* `INSTANCE = new Singleton4()` 不是原子的，分成 3 步：**创建对象、调用构造、给静态变量赋值**，其中后两步可能被指令重排序优化，变成先赋值、再调用构造，	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303240052653.png)

volatile禁止指令重排，volitile在赋值语句后面加写屏障（其实前面 volatile读/写 后面都加了屏障），保证屏障之前的语句禁止指令重排	
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303240055906.png)

* 如果线程1 先执行了赋值，线程2 执行到第一个 `INSTANCE == null` 时发现 INSTANCE 已经不为 null，此时**就会返回一个未完全构造的对象** 
	> 就和去饭堂打饭刚拿到空盘子就跑掉了，饭还没蹭上呢
* javap -c -v p- Singleton4.class 反编译，invokespecial `<init>` 调用构造方法；putstatic给一个静态变量赋值。指令重排在单线程下没有影响。
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303240047141.png)


---
# 5. 内部类懒汉式
==只要把单例对象的创建放入静态代码块创建，就是线程安全的，由JVM保证。==

内部类在使用时初始化！！！静态内部类的加载是在程序中调用静态内部类时加载的，和外部类的加载没有必然关系，只要外部类中没有使用到内部类，那么外部类加载时内部类不会加载
```java
public class Singleton5 implements Serializable {
    private Singleton5() {
        System.out.println("private Singleton5()");
    }

    private static class Holder {
        static Singleton5 INSTANCE = new Singleton5();
    }

    public static Singleton5 getInstance() { // 调用时才触发Singleton5的初始化
        return Holder.INSTANCE;
    }

    //public Object readResolve() { return Holder.INSTANCE; }
     
    public static void otherMethod() {
        System.out.println("otherMethod()");
    }
}
```
* 避免了双检锁的缺点

---
# 6. JDK 中单例的体现
## 6.1 Runtime 体现了饿汉式单例
System.exit调用的是Runtime.getRuntime().exit；System.gc调用的也是Runtime.getRuntime().gc
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303240109982.png)

* Comparators.NaturalOrderComparator.INSTANCE 枚举饿汉式单例
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303240135338.png)

## 6.2 系统控制台抽象：Console 体现了双检锁懒汉式单例
一看到static volatile就是双检锁！
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303240113153.png)


## 6.3 Collections 集合工具类中获取一个空的集合（**不放元素**，大部分单例模式的应用）的 EmptyNavigableSet 内部类懒汉式单例
就算是public的构造方法，但是个静态私有内部类，所以外面用不到。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303240126058.png)


EmptyEnumeration
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303240132722.png)
* ReverseComparator.REVERSE_ORDER 内部类懒汉式单例 
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303240133117.png)


---
饿汉式：
```java
public class Singleton1 implements Serializable {
	private Singleton1() {
		if (instance != null) {
			throw RuntimeException(...);
		}
	}
	private static final Singleton1 instance = new Singleton1();
	public static Singleton1 getInstance() { return instance; } 
	public Object readResolve() { return instance; }
}
```
枚举饿汉式：
```java
public enum Singleton2 {
	INSTANCE;
	private Singleton2() { ... }
	public static Singleton2 getInstance() { return INSTANCE; }
}
```
懒汉式：
```java
public class Singleton3 {
	private Singleton3() {
		...
	}
	private static Singleton3 instance = null;
	public static synchronized Singleton3 getInstance() { 
		if (instance == null) {
			instance = new Singleton3();
		}	
		return instance;
	}
}
public class Singleton3 {
	private Singleton3() {
		...
	}
	private static Singleton3 instance = null;
	public static Singleton3 getInstance() { 
		Synchronized (Singleton3.class) {
			if (instance == null) {
				instance = new Singleton3();
			}	
		}
		return instance;
	}
}
```
双检锁懒汉式：
```java 
public class Singleton4 {
	private Singleton4() {
		...
	}
	private static volatile Singleton4 instance = null;
	public static Singleton4 getInstance() { 
		if (instance == null) {
			Synchronized (Singleton4.class) {
				if (instance == null) {
					instance = new Singleton4();
				}	
			}
		}
		return instance;
	}
}
```
内部类懒汉式：
```java
public class Singleton5 {
	private Singleton5() {
		...
	}
	private static class Holder {
		static Singleton5 INSTANCE = new Singleton5();
	}
	public static Singleton5 getInstance() { return Holder.INSTANCE; }	
}
```