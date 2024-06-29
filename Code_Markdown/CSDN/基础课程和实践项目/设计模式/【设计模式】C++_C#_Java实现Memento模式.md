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
# Memento
The `Memento` design pattern **captures and externalizes an object‘s internal state** without violating encapsulation, so that the object can be restored to this state later.

Frequency of use: Low(level 1)

# UML class diagram
A visualization of the classes and objects participating in this pattern.
![](https://img-blog.csdnimg.cn/8212a1cf6c1742dd85ad0306d83f799b.png)

# Participants 
The classes and objects participating in this pattern include:
- `Memento`  (`Memento`)
    -  **stores internal state of the `Originator object`** . The `Memento` may store as much or as little of the `Originator`'s internal state as necessary **at its `Originator`'s discretion**.
  - **protect against access by objects of other than the `Originator` .** `Memento`s have effectively two interfaces. 	 
     - **`Caretaker` sees a narrow interface to the `Memento`** -- it can **only pass the `Memento` to the other objects**.   
     - **`Originator` , in contrast, sees a wide interface**, one that lets it access all the data necessary to restore itself to its previous state. 
     Ideally, only the `Originator` that produces the `Memento` would be permitted to access the memento's internal state.
- `Originator`  (`SalesProspect`)
   - creates a `Memento` containing a snapshot of its current internal state.
    - uses the `Memento` to restore its internal state
- `Caretaker`  (`Caretaker`)
   -  is responsible for the `Memento`'s safekeeping
   -  **never operates on or examines the contents of a `Memento` .**

# Structural code in C++/C#/Java
This structural code demonstrates the `Memento` pattern which **temporarily saves and restores another object's internal state**. 
```csharp
using System;

namespace Memento.Structural
{
    /// <summary>
    /// Memento Design Pattern
    /// </summary>

    public class Program
    {
        public static void Main(string[] args)
        {
            Originator o = new Originator();
            o.State = "On";

            // Store internal state

            Caretaker c = new Caretaker();
            c.Memento = o.CreateMemento();

            // Continue changing originator

            o.State = "Off";

            // Restore saved state

            o.SetMemento(c.Memento);

            // Wait for user

            Console.ReadKey();
        }
    }

    /// <summary>
    /// The 'Originator' class
    /// </summary>

    public class Originator
    {
        string state;

        public string State
        {
            get { return state; }
            set
            {
                state = value;
                Console.WriteLine("State = " + state);
            }
        }

        // Creates memento 

        public Memento CreateMemento()
        {
            return (new Memento(state));
        }

        // Restores original state

        public void SetMemento(Memento memento)
        {
            Console.WriteLine("Restoring state...");
            State = memento.State;
        }
    }

    /// <summary>
    /// The 'Memento' class
    /// </summary>

    public class Memento
    {
        string state;

        // Constructor

        public Memento(string state)
        {
            this.state = state;
        }

        public string State
        {
            get { return state; }
        }
    }

    /// <summary>
    /// The 'Caretaker' class
    /// </summary>

    public class Caretaker
    {
        Memento memento;

        public Memento Memento
        {
            set { memento = value; }
            get { return memento; }
        }
    }
}
```
**Output** 
```csharp
State = On
State = Off
Restoring state:
State = On
```
# Real-world code in C++/C#/Java 
This real-world code demonstrates the `Memento` pattern which temporarily saves and then restores the `SalesProspect`'s internal state. 
```csharp
using System;

namespace Memento.RealWorld
{
    /// <summary>
    /// Memento Design Pattern
    /// </summary>

    public class Program
    {
        public static void Main(string[] args)
        {
            SalesProspect s = new SalesProspect();
            s.Name = "Noel van Halen";
            s.Phone = "(412) 256-0990";
            s.Budget = 25000.0;

            // Store internal state

            ProspectMemory m = new ProspectMemory();
            m.Memento = s.SaveMemento();

            // Continue changing originator

            s.Name = "Leo Welch";
            s.Phone = "(310) 209-7111";
            s.Budget = 1000000.0;

            // Restore saved state

            s.RestoreMemento(m.Memento);

            // Wait for user

            Console.ReadKey();
        }
    }

    /// <summary>
    /// The 'Originator' class
    /// </summary>

    public class SalesProspect
    {
        string name;
        string phone;
        double budget;

        // Gets or sets name

        public string Name
        {
            get { return name; }
            set
            {
                name = value;
                Console.WriteLine("Name:   " + name);
            }
        }

        // Gets or sets phone

        public string Phone
        {
            get { return phone; }
            set
            {
                phone = value;
                Console.WriteLine("Phone:  " + phone);
            }
        }

        // Gets or sets budget

        public double Budget
        {
            get { return budget; }
            set
            {
                budget = value;
                Console.WriteLine("Budget: " + budget);
            }
        }

        // Stores memento

        public Memento SaveMemento()
        {
            Console.WriteLine("\nSaving state --\n");
            return new Memento(name, phone, budget);
        }

        // Restores memento

        public void RestoreMemento(Memento memento)
        {
            Console.WriteLine("\nRestoring state --\n");
            Name = memento.Name;
            Phone = memento.Phone;
            Budget = memento.Budget;
        }
    }

    /// <summary>
    /// The 'Memento' class
    /// </summary>

    public class Memento
    {
        string name;
        string phone;
        double budget;

        // Constructor

        public Memento(string name, string phone, double budget)
        {
            this.name = name;
            this.phone = phone;
            this.budget = budget;
        }

        public string Name
        {
            get { return name; }
            set { name = value; }
        }

        public string Phone
        {
            get { return phone; }
            set { phone = value; }
        }

        public double Budget
        {
            get { return budget; }
            set { budget = value; }
        }
    }

    /// <summary>
    /// The 'Caretaker' class
    /// </summary>

    public class ProspectMemory
    {
        Memento memento;

        public Memento Memento
        {
            set { memento = value; }
            get { return memento; }
        }
    }
}
```
