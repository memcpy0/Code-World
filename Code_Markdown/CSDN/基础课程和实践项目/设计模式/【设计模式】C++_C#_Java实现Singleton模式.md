> 本文属于「设计模式」系列文章之一。这一系列开始时间为2021/10/27，着重于「设计模式的学习与实践」。由于文章内容随时可能发生更新变动，欢迎关注和收藏[设计模式系列文章汇总目录]()一文以作备忘。需要特别说明的是，为了透彻理解和全面掌握设计模式，本系列文章中参考了诸多博客、教程、文档、书籍等资料，限于时间精力有限，这里无法一一列出。部分重要资料的不完全参考目录如下所示，在后续学习整理中还会逐渐补充：
> -  大话设计模式（[C# Design Patterns](https://www.dofactory.com/net/design-patterns)），程杰著，清华大学出版社，
> - （**国外经典书籍**）设计模式：可复用面向对象软件的基础，`Erich Gamm, Richard Helm, Ralph, Johnson, John Vlissides` 著，李英军、马晓星、蔡敏、刘建中译，机械工业出版社
> - *Head First Design Patterns*，`Eric Freeman & Elisabeth Freeman with Kathy Sierra & Bert Bates` ，东南大学出版社
> - 设计模式解析 第2版，`Allan Shalloway, James R. Trott` 著，徐言声译，人民邮电出版社
> - 敏捷软件开发：原则、模式与实践，`Robert C. Martin` 著，邓辉译，清华大学出版社
> - 重构——改善既有代码的设计，`Martin Fowler` 著，侯捷、熊杰译，中国电力出版社
> - 企业应用架构模式，`Martin Fowler` 著，王怀明、周斌译，机械工业出版社
> - Java与模式，阎宏著，电子工业出版社

@[toc]

---
# Singleton
The `Singleton` design pattern **ensures a class has only one instance and provide a global point of access to it.**

Frequency of use: Medium-high(level 4)


# UML class diagram
A visualization of the classes and objects participating in this pattern.
![在这里插入图片描述](https://img-blog.csdnimg.cn/ec37a47a187f4e2e838bf8b20b0da0e6.png)
# Participants
The classes and objects participating in this pattern include:
- `Singleton` (`LoadBalancer`)：
	- defines an instance operation that lets clients access its unique instance. Instance is a class operation.
	- responsible for creating and maintaining its own unique instance.


# Structural code in C++/C#/Java
This structural code demonstrates the `Singleton` pattern which **assures only a single instance (the singleton) of the class can be created**. 

```csharp
using System;

namespace Singleton.Structural
{
    /// <summary>
    /// Singleton Design Pattern
    /// </summary>

    public class Program
    {
        public static void Main(string[] args)
        {
            // Constructor is protected -- cannot use new

            Singleton s1 = Singleton.Instance();
            Singleton s2 = Singleton.Instance();

            // Test for same instance

            if (s1 == s2)
            {
                Console.WriteLine("Objects are the same instance");
            }

            // Wait for user

            Console.ReadKey();
        }
    }

    /// <summary>
    /// The 'Singleton' class
    /// </summary>

    public class Singleton
    {
        static Singleton instance;

        // Constructor is 'protected'

        protected Singleton()
        {
        }

        public static Singleton Instance()
        {
            // Uses lazy initialization.
            // Note: this is not thread safe.
            if (instance == null)
            {
                instance = new Singleton();
            }

            return instance;
        }
    }
}
```
**Output**

```cpp
Objects are the same instance
```

# Real-world code in C++/C#/Java
This real-world code demonstrates the `Singleton` pattern as a `LoadBalancing` object. **Only a single instance (the singleton) of the class can be created** because servers may dynamically come on- or off-line and every request must go throught the one object that has knowledge about the state of the (web) farm. 

```csharp
using System;
using System.Collections.Generic;

namespace Singleton.RealWorld
{
    /// <summary>
    /// Singleton Design Pattern
    /// </summary>

    public class Program
    {
        public static void Main(string[] args)
        {
            LoadBalancer b1 = LoadBalancer.GetLoadBalancer();
            LoadBalancer b2 = LoadBalancer.GetLoadBalancer();
            LoadBalancer b3 = LoadBalancer.GetLoadBalancer();
            LoadBalancer b4 = LoadBalancer.GetLoadBalancer();

            // Same instance?

            if (b1 == b2 && b2 == b3 && b3 == b4)
            {
                Console.WriteLine("Same instance\n");
            }

            // Load balance 15 server requests

            LoadBalancer balancer = LoadBalancer.GetLoadBalancer();
            for (int i = 0; i < 15; i++)
            {
                string server = balancer.Server;
                Console.WriteLine("Dispatch Request to: " + server);
            }

            // Wait for user

            Console.ReadKey();
        }
    }

    /// <summary>
    /// The 'Singleton' class
    /// </summary>

    public class LoadBalancer
    {
        static LoadBalancer instance;
        List<string> servers = new List<string>();
        Random random = new Random();

        // Lock synchronization object

        private static object locker = new object();

        // Constructor (protected)

        protected LoadBalancer()
        {
            // List of available servers
            servers.Add("ServerI");
            servers.Add("ServerII");
            servers.Add("ServerIII");
            servers.Add("ServerIV");
            servers.Add("ServerV");
        }

        public static LoadBalancer GetLoadBalancer()
        {
            // Support multithreaded applications through
            // 'Double checked locking' pattern which (once
            // the instance exists) avoids locking each
            // time the method is invoked

            if (instance == null)
            {
                lock (locker)
                {
                    if (instance == null)
                    {
                        instance = new LoadBalancer();
                    }
                }
            }

            return instance;
        }

        // Simple, but effective random load balancer

        public string Server
        {
            get
            {
                int r = random.Next(servers.Count);
                return servers[r].ToString();
            }
        }
    }
}
```
**Output**
```cpp
Same instance

ServerIII
ServerII
ServerI
ServerII
ServerI
ServerIII
ServerI
ServerIII
ServerIV
ServerII
ServerII
ServerIII
ServerIV
ServerII
ServerIV
```
