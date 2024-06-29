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
# Observer
The `Observer` design pattern defines **a one-to-many dependency between objects** so that when one object changes state, all its dependents are notified and updated automatically.

Frequency of use: high(level 5)

# UML class diagram
A visualization of the classes and objects participating in this pattern.
![在这里插入图片描述](https://img-blog.csdnimg.cn/841efb37622541af98114a39c379c678.png)

# Participants
 The classes and objects participating in this pattern include: 
 - `Subject` (`Stock`)
 	 - knows its observers. Any number of `Observer` objects may observe a subject.
 	 - provides an interface for attaching and detaching `Observer` objects.
 - `ConcreteSubject` (`IBM`)
 	 - stores state of interest to `ConcreteObserver` 
 	 - sends a notification to its observers when its state changes.
 - `Observer` (`IInvestor`)
 	- defines an updating interface for objects that should be notified of changes in a subject.
- `ConcreteObserver` (`Investor`)
	- maintains a reference to a `ConcreteSubject` object.
	- stores state that should stay consistent with the subject's
	- implements the `Observer` updating interface to keep its state consistent with the subject's

# Structural code in C++/C#/Java
This structural code demonstrates the `Observer` pattern in which **registered objects are notified of and updated with a state change**. 
```csharp
using System;
using System.Collections.Generic;

namespace Observer.Structural
{
    /// <summary>
    /// Observer Design Pattern
    /// </summary>

    public class Program
    {
        public static void Main(string[] args)
        {
            // Configure Observer pattern

            ConcreteSubject s = new ConcreteSubject();

            s.Attach(new ConcreteObserver(s, "X"));
            s.Attach(new ConcreteObserver(s, "Y"));
            s.Attach(new ConcreteObserver(s, "Z"));

            // Change subject and notify observers

            s.SubjectState = "ABC";
            s.Notify();

            // Wait for user

            Console.ReadKey();
        }
    }

    /// <summary>
    /// The 'Subject' abstract class
    /// </summary>

    public abstract class Subject
    {
        private List<Observer> observers = new List<Observer>();

        public void Attach(Observer observer)
        {
            observers.Add(observer);
        }

        public void Detach(Observer observer)
        {
            observers.Remove(observer);
        }

        public void Notify()
        {
            foreach (Observer o in observers)
            {
                o.Update();
            }
        }
    }

    /// <summary>
    /// The 'ConcreteSubject' class
    /// </summary>

    public class ConcreteSubject : Subject
    {
        private string subjectState;

        // Gets or sets subject state

        public string SubjectState
        {
            get { return subjectState; }
            set { subjectState = value; }
        }
    }

    /// <summary>
    /// The 'Observer' abstract class
    /// </summary>

    public abstract class Observer
    {
        public abstract void Update();
    }

    /// <summary>
    /// The 'ConcreteObserver' class
    /// </summary>

    public class ConcreteObserver : Observer
    {
        private string name;
        private string observerState;
        private ConcreteSubject subject;

        // Constructor

        public ConcreteObserver(
            ConcreteSubject subject, string name)
        {
            this.subject = subject;
            this.name = name;
        }

        public override void Update()
        {
            observerState = subject.SubjectState;
            Console.WriteLine("Observer {0}'s new state is {1}",
                name, observerState);
        }

        // Gets or sets subject

        public ConcreteSubject Subject
        {
            get { return subject; }
            set { subject = value; }
        }
    }
}
```
**Output**

```cpp
Observer X's new state is ABC
Observer Y's new state is ABC
Observer Z's new state is ABC
```

# Real-world code in C++/C#/Java
This real-world code demonstrates the `Observer` pattern in which **registered investors are notified every time a stock changes value.** 
```csharp
using System;
using System.Collections.Generic;

namespace Observer.RealWorld
{
    /// <summary>
    /// Observer Design Pattern
    /// </summary>

    public class Program
    {
        public static void Main(string[] args)
        {
            // Create IBM stock and attach investors

            IBM ibm = new IBM("IBM", 120.00);
            ibm.Attach(new Investor("Sorros"));
            ibm.Attach(new Investor("Berkshire"));

            // Fluctuating prices will notify investors

            ibm.Price = 120.10;
            ibm.Price = 121.00;
            ibm.Price = 120.50;
            ibm.Price = 120.75;

            // Wait for user

            Console.ReadKey();
        }
    }

    /// <summary>
    /// The 'Subject' abstract class
    /// </summary>

    public abstract class Stock
    {
        private string symbol;
        private double price;
        private List<IInvestor> investors = new List<IInvestor>();

        // Constructor

        public Stock(string symbol, double price)
        {
            this.symbol = symbol;
            this.price = price;
        }

        public void Attach(IInvestor investor)
        {
            investors.Add(investor);
        }

        public void Detach(IInvestor investor)
        {
            investors.Remove(investor);
        }

        public void Notify()
        {
            foreach (IInvestor investor in investors)
            {
                investor.Update(this);
            }

            Console.WriteLine("");
        }

        // Gets or sets the price

        public double Price
        {
            get { return price; }
            set
            {
                if (price != value)
                {
                    price = value;
                    Notify();
                }
            }
        }

        // Gets the symbol

        public string Symbol
        {
            get { return symbol; }
        }
    }

    /// <summary>
    /// The 'ConcreteSubject' class
    /// </summary>

    public class IBM : Stock
    {
        // Constructor

        public IBM(string symbol, double price)
            : base(symbol, price)
        {
        }
    }

    /// <summary>
    /// The 'Observer' interface
    /// </summary>

    public interface IInvestor
    {
        void Update(Stock stock);
    }

    /// <summary>
    /// The 'ConcreteObserver' class
    /// </summary>

    public class Investor : IInvestor
    {
        private string name;
        private Stock stock;

        // Constructor

        public Investor(string name)
        {
            this.name = name;
        }

        public void Update(Stock stock)
        {
            Console.WriteLine("Notified {0} of {1}'s " +
                "change to {2:C}", name, stock.Symbol, stock.Price);
        }

        // Gets or sets the stock

        public Stock Stock
        {
            get { return stock; }
            set { stock = value; }
        }
    }
}
```
**Output**

```cpp
Notified Sorros of IBM's change to $120.10
Notified Berkshire of IBM's change to $120.10

Notified Sorros of IBM's change to $121.00
Notified Berkshire of IBM's change to $121.00

Notified Sorros of IBM's change to $120.50
Notified Berkshire of IBM's change to $120.50

Notified Sorros of IBM's change to $120.75
Notified Berkshire of IBM's change to $120.75
```

