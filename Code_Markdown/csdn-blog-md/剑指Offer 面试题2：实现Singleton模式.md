@[toc]

# 1. 面试题2：实现Singleton模式
题目：设计一个类，我们只能生成该类的一个实例。

思考：只能生成一个实例的类，即实现了 `Singleton` 单例模式的类型。它是唯一一个能够用短短十几行代码完整实现的模式，因此，考察单例模式的类型是很常见的面试题。


考点：
- 对单例 `Singleton` 模式的理解；
- 对 `C#` 的基础语法的理解，如静态构造函数等；
- 对多线程编程的理解。

---
# 2. 解法1：只能适用于单线程环境
由于要求只能生成一个实例，**就必须把构造函数设为私有函数**，防止其他人创建实例。同时，在 `Singleton` 的静态属性 `Instance` 中，只有 `instance` 是 `null` 时才创建一个实例以避免重复创建。下面定义一个静态的实例，需要时创建该实例：
```csharp
public sealed class Singleton1 {
	private Singleton1() { }
	private static Singleton1 instance = null;
	public static Singleton1 Instance {
		get {
			if (instance == null) 
				instance = new Singleton1();
			return instance;
		}
	}
}
```
不过这个解法在单线程时可以正常工作，但在多线程中就有问题，如果两个线程同时运行到判断 `instance` 为 `null` 的 `if` 语句，并且 `instance` 确实没有创建时，则两个线程都会创建一个实例。此时，单例模式的要求被破坏。

# 3. 解法2：能够在多线程中工作但效率不高
为了保证在多线程环境中，还只能得到类型的一个实例，需要加上一个同步锁，把 `Singleton1` 修改得到如下代码：
```csharp
public sealed class Singleton2 {
	private Singleton2() { }
	private static readonly object syncObj = new object();
	private static Singleton2 instance = null;
	
	public static Singleton2 Instance {
		get {
			lock (syncObj) {
				if (instance == null) 
					instance = new Singleton2();
			}
			return instance;
		}
	}
```
这时，假设有两个线程同时想创建一个实例，但是只有一个线程能够得到同步锁，当它加上锁的时候，第二个线程只能等待。然后，第一个线程发现实例没有创建，它会创建出一个实例，接着释放同步锁。于是，此时第二个线程得到同步锁，运行下面的代码，由于实例已经被创建出来了，第二个线程就不会再创建实例了。**这就保证了在多线程环境中也只能得到一个实例**。

不过，同步锁太耗时了，我们应该尽量避免。

# 4. 解法3：加同步锁前后两次判断实例是否已存在，可行解法
我们只在实例没有创建之前要加锁，以保证只有一个线程创建出实例。**实例已经创建之后，就不需要再加锁了**。

改进上述的代码：
```csharp
public sealed class Singleton3 {
	private Singleton3() { }
	private static object syncObj = new object();
	
	private static Singleton3 instance = null;
	public static Singleton3 Instance {
		get {
			if (instance == null) {
				lock (syncObj) {
					if (instance == null) 
						intance = new Singleton3();
				}
			}
			return instance;
		}
	}
}
```
`Singleton3` 中只有当 `instance` 为 `null` 时才需要加锁，当 `instance` 已经被创建出来后，就不用加锁了，这样 `Singleton3` 的时间效率要好得多。

`Singleton3` 用加锁来确保多线程环境下只创建一个实例，用两个 `if` 判断来提高效率，但是这样的代码实现起来比较复杂，容易出错，更好的方法在后面。

# 5. 解法4：静态构造函数(强烈推荐)
`C#` 的语法有一个函数能够**确保只调用一次**，就是静态构造函数。利用其实现如下：
```csharp
public sealed class Singleton4 {
	private Singleton4() { }
	private static Singleton4 instance = new Singleton4();
	public static Singleton4 Instance {
		get {
			return instance;
		}
	}
}
```
在初始化静态变量 `instance` 时创建一个实例，由于 `C#` 是**在调用静态构造函数时初始化静态变量**，`.NET` 运行时能够确保只调用一次静态构造函数，于是就只初始化一次 `instance` 。

调用静态构造函数的时机：`.NET` 运行时发现**第一次使用一个类型**时，就会自动调用该类型的静态构造函数。因此，第一次用到 `Singleton4` 的时候就会创建 `instance` 。

不过这个做法的小缺点在于：如果 `Singleton4` 中存在静态方法，按理来说，使用该静态函数是不需要创建一个实例的。但是按照 `Singleton4` 的方法实现单例模式，就会**过早创建实例**，**降低内存的使用效率**。

# 6. 解法5：实现按需创建实例(最终版本)
下面的做法可以很好的解决 `Singleton4` 的实例创建时机过早的问题：
```cpp
public sealed class Singleton5 {
	Singleton5() { }
	public static Singleton5 Instance {
		get {
			return Nested.instance;
		}
	}
	class Nested {
		staic Nested { }
		internel static readonly Singleton5 instance = new Singleton5();
	}
}
```
在内部定义了一个私有类型 `Nested` ，第一次用到 `Nested` 这个嵌套类型时，会调用静态构造函数创建 `Singleton5` 的实例 `instance` 。

类型 `Nested` 只有在属性 `Singleton5.Instance` 中用到，由于其是私有的，其他人无法使用 `Nested` 类型。因此当我们第一次试图通过 `Singleton5.Instance` 得到 `Singleton5` 的实例时，会自动调用 `Nested` 的静态构造函数创建实例 `instance` 。

如果我们不调用属性 `Singleton5.Instance` ，就不会触发 `.NET` 运行时调用 `Nested` ，也不会创建实例，这样就真正做到了按需创建实例。


# 7. 解法比较
第一、二种都不是期待的解法。第三种做法**通过两次判断一次加锁**确保能够在多线程环境高效工作。第四种做法利用 `C#` 静态构造函数的特性，确保只创建一个实例。第五种方法利用**私有嵌套类型**的特性，做到只有在真正需要时才会创建实例。

第四种和第五种做法，都是有效的做法。

# 8. 题目扩展
5种单例模式中，都把类型标记为 `sealed` ，表示不能作为其他类型的基类。现在要定义一个表示总统的类型 `President` ，可以从该类型继承出 `FrenchPresident, AmericanPresident` 等类型，这些派生类型都**只能产生一个实例**。

该如何设计实现它们呢？
