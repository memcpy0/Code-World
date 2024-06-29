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
# Factory Method
The `Factory Method` design pattern **defines an interface for creating an object**, but let subclasses decide which class to instantiate. This pattern **lets a class defer instantiation to subclasses.**

Frequency of use: high(level 5)
# UML class diagram
A visualization of the classes and objects participating in this pattern.
![在这里插入图片描述](https://img-blog.csdnimg.cn/695c80865b3a45028ca4b748e4d27e56.png)
# Participants
The classes and objects participating in this pattern include: 
- `Product` (`Page`)
	- defines the interface of **objects the factory method creates**.
- `ConcreteProduct` (`SkillsPage, EducationPage, ExperiencePage`)
	- implements the `Product` interface
- `Creator` (`Document`)
	- declares the `Factory Method` , which returns an object of type `Product` . `Creator` may also define a default implementation of the `Factory method` that returns default `ConcreteProduct` object.
- `ConcreteCreator` (`Report, Resume`)
	- overrides the `Factory Method` to return an instance of a `ConcreteProduct` .

# Structural code in C++/C#/Java
This structural code demonstrates the `Factory method` offering great flexibility in **creating different objects**. The Abstract class may provide a default object, but **each subclass can instantiate an extended version of the object**.

```csharp
using System;

namespace DoFactory.GangOfFour.Factory.Structural
{
    /// <summary>
    /// MainApp startup class for Structural 
    /// Factory Method Design Pattern.
    /// </summary>

    class MainApp
    {
        /// <summary>
        /// Entry point into console application.
        /// </summary>

        static void Main()
        {
            // An array of creators

            Creator[] creators = new Creator[2];

            creators[0] = new ConcreteCreatorA();
            creators[1] = new ConcreteCreatorB();

            // Iterate over creators and create products

            foreach (Creator creator in creators)
            {
                Product product = creator.FactoryMethod();
                Console.WriteLine("Created {0}",
                  product.GetType().Name);
            }

            // Wait for user

            Console.ReadKey();
        }
    }

    /// <summary>
    /// The 'Product' abstract class
    /// </summary>

    abstract class Product
    {
    }

    /// <summary>
    /// A 'ConcreteProduct' class
    /// </summary>

    class ConcreteProductA : Product
    {
    }

    /// <summary>
    /// A 'ConcreteProduct' class
    /// </summary>

    class ConcreteProductB : Product
    {
    }

    /// <summary>
    /// The 'Creator' abstract class
    /// </summary>

    abstract class Creator
    {
        public abstract Product FactoryMethod();
    }

    /// <summary>
    /// A 'ConcreteCreator' class
    /// </summary>

    class ConcreteCreatorA : Creator
    {
        public override Product FactoryMethod()
        {
            return new ConcreteProductA();
        }
    }

    /// <summary>
    /// A 'ConcreteCreator' class
    /// </summary>

    class ConcreteCreatorB : Creator
    {
        public override Product FactoryMethod()
        {
            return new ConcreteProductB();
        }
    }
}
```

**Output**

```cpp
Created ConcreteProductA
Created ConcreteProductB
```
# Real-world code in C++/C#/Java
This real-world code demonstrates the `Factory method` offering flexibility in creating different documents. The derived `Document` classes `Report` and `Resume` instantiate extended versions of the `Document` class. Here, the `Factory Method` is called in the constructor of the `Document` base class. 
```csharp
using System;
using System.Collections.Generic;

namespace DoFactory.GangOfFour.Factory.RealWorld
{
    /// <summary>
    /// MainApp startup class for Real-World 
    /// Factory Method Design Pattern.
    /// </summary>

    class MainApp
    {
        /// <summary>
        /// Entry point into console application.
        /// </summary>

        static void Main()
        {
            // Note: constructors call Factory Method

            Document[] documents = new Document[2];

            documents[0] = new Resume();
            documents[1] = new Report();

            // Display document pages

            foreach (Document document in documents)
            {
                Console.WriteLine("\n" + document.GetType().Name + "--");
                foreach (Page page in document.Pages)
                {
                    Console.WriteLine(" " + page.GetType().Name);
                }
            }

            // Wait for user

            Console.ReadKey();
        }
    }

    /// <summary>
    /// The 'Product' abstract class
    /// </summary>

    abstract class Page
    {
    }

    /// <summary>
    /// A 'ConcreteProduct' class
    /// </summary>

    class SkillsPage : Page
    {
    }

    /// <summary>
    /// A 'ConcreteProduct' class
    /// </summary>

    class EducationPage : Page
    {
    }

    /// <summary>
    /// A 'ConcreteProduct' class
    /// </summary>

    class ExperiencePage : Page
    {
    }

    /// <summary>
    /// A 'ConcreteProduct' class
    /// </summary>

    class IntroductionPage : Page
    {
    }

    /// <summary>
    /// A 'ConcreteProduct' class
    /// </summary>

    class ResultsPage : Page
    {
    }

    /// <summary>
    /// A 'ConcreteProduct' class
    /// </summary>

    class ConclusionPage : Page
    {
    }

    /// <summary>
    /// A 'ConcreteProduct' class
    /// </summary>

    class SummaryPage : Page
    {
    }

    /// <summary>
    /// A 'ConcreteProduct' class
    /// </summary>

    class BibliographyPage : Page
    {
    }

    /// <summary>
    /// The 'Creator' abstract class
    /// </summary>

    abstract class Document
    {
        private List<Page> _pages = new List<Page>();

        // Constructor calls abstract Factory method

        public Document()
        {
            this.CreatePages();
        }

        public List<Page> Pages
        {
            get { return _pages; }
        }

        // Factory Method

        public abstract void CreatePages();
    }

    /// <summary>
    /// A 'ConcreteCreator' class
    /// </summary>

    class Resume : Document
    {
        // Factory Method implementation

        public override void CreatePages()
        {
            Pages.Add(new SkillsPage());
            Pages.Add(new EducationPage());
            Pages.Add(new ExperiencePage());
        }
    }

    /// <summary>
    /// A 'ConcreteCreator' class
    /// </summary>

    class Report : Document
    {
        // Factory Method implementation

        public override void CreatePages()
        {
            Pages.Add(new IntroductionPage());
            Pages.Add(new ResultsPage());
            Pages.Add(new ConclusionPage());
            Pages.Add(new SummaryPage());
            Pages.Add(new BibliographyPage());
        }
    }
}
```
**Output**

```cpp
Resume -------
 SkillsPage
 EducationPage
 ExperiencePage

Report -------
 IntroductionPage
 ResultsPage
 ConclusionPage
 SummaryPage
 BibliographyPage
```

