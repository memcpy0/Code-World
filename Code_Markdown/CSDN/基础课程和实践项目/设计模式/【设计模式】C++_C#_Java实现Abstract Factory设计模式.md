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
# Abstract Factory
The `Abstract Factory` design pattern **provides an interface for creating families of related or dependent objects without specifying their concrete classes**.

Frequency of use: high(level 5)

# UML class diagram
A visualization of the classes and objects participating in this pattern.
![在这里插入图片描述](https://img-blog.csdnimg.cn/083d86d246074cbaa07fc9246a871d91.png)

# Participants
 The classes and objects participating in this pattern include: 
 - `AbstractFactory` (`ContinentFactory`)
 	- declares an interface for operations that **create `AbstractProduct` objects**.
 - `ConcreteFactory` (`AfricaFactory, AmericaFactory`)
    -  implements the operations to **create `ConcreteProduct` objects**.
 - `AbstractProduct` (`Herbivore, Carnivore`)
	- declares an interface for a type of `Product` object.
 - `Product` (`Wildebeest, Lion, Bison, Wolf`)
 	- defines a `Product` object to be created by the corresponding `ConcreteFactory` .
 	-  implements the `AbstractProduct` interface 
 - `Client` (`AnimalWorld`)
 	-  uses interfaces declared by `AbstractFactory` and `AbstractProduct` classes 
 

# Structural code in C++/C#/Java
This structural code demonstrates the `Abstract Factory` pattern **creating parallel hierarchies of objects**. Object creation has been abstracted and **there is no need for hard-coded class names** in the client code. 
```csharp
using System;

namespace DoFactory.GangOfFour.Abstract.Structural
{
    /// <summary>
    /// MainApp startup class for Structural
    /// Abstract Factory Design Pattern.
    /// </summary>

    class MainApp
    {
        /// <summary>
        /// Entry point into console application.
        /// </summary>

        public static void Main()
        {
            // Abstract factory #1

            AbstractFactory factory1 = new ConcreteFactory1();
            Client client1 = new Client(factory1);
            client1.Run();

            // Abstract factory #2

            AbstractFactory factory2 = new ConcreteFactory2();
            Client client2 = new Client(factory2);
            client2.Run();

            // Wait for user input

            Console.ReadKey();
        }
    }

    /// <summary>
    /// The 'AbstractFactory' abstract class
    /// </summary>

    abstract class AbstractFactory
    {
        public abstract AbstractProductA CreateProductA();
        public abstract AbstractProductB CreateProductB();
    }


    /// <summary>
    /// The 'ConcreteFactory1' class
    /// </summary>

    class ConcreteFactory1 : AbstractFactory
    {
        public override AbstractProductA CreateProductA()
        {
            return new ProductA1();
        }
        public override AbstractProductB CreateProductB()
        {
            return new ProductB1();
        }
    }

    /// <summary>
    /// The 'ConcreteFactory2' class
    /// </summary>

    class ConcreteFactory2 : AbstractFactory
    {
        public override AbstractProductA CreateProductA()
        {
            return new ProductA2();
        }
        public override AbstractProductB CreateProductB()
        {
            return new ProductB2();
        }
    }

    /// <summary>
    /// The 'AbstractProductA' abstract class
    /// </summary>

    abstract class AbstractProductA
    {
    }

    /// <summary>
    /// The 'AbstractProductB' abstract class
    /// </summary>

    abstract class AbstractProductB
    {
        public abstract void Interact(AbstractProductA a);
    }


    /// <summary>
    /// The 'ProductA1' class
    /// </summary>

    class ProductA1 : AbstractProductA
    {
    }

    /// <summary>
    /// The 'ProductB1' class
    /// </summary>

    class ProductB1 : AbstractProductB
    {
        public override void Interact(AbstractProductA a)
        {
            Console.WriteLine(this.GetType().Name +
              " interacts with " + a.GetType().Name);
        }
    }

    /// <summary>
    /// The 'ProductA2' class
    /// </summary>

    class ProductA2 : AbstractProductA
    {
    }

    /// <summary>
    /// The 'ProductB2' class
    /// </summary>

    class ProductB2 : AbstractProductB
    {
        public override void Interact(AbstractProductA a)
        {
            Console.WriteLine(this.GetType().Name +
              " interacts with " + a.GetType().Name);
        }
    }

    /// <summary>
    /// The 'Client' class. Interaction environment for the products.
    /// </summary>

    class Client
    {
        private AbstractProductA _abstractProductA;
        private AbstractProductB _abstractProductB;

        // Constructor

        public Client(AbstractFactory factory)
        {
            _abstractProductB = factory.CreateProductB();
            _abstractProductA = factory.CreateProductA();
        }

        public void Run()
        {
            _abstractProductB.Interact(_abstractProductA);
        }
    }
}
```
**Output**

```cpp
ProductB1 interacts with ProductA1
ProductB2 interacts with ProductA2 
```
# Real-world code in C++/C#/Java
This real-world code demonstrates **the creation of different animal worlds** for a computer game **using different factories**. Although the animals created by the `Continent` factories are different, the interactions among the animals remain the same.
```csharp
using System;

namespace DoFactory.GangOfFour.Abstract.RealWorld
{
    /// <summary>
    /// MainApp startup class for Real-World
    /// Abstract Factory Design Pattern.
    /// </summary>

    class MainApp
    {
        /// <summary>
        /// Entry point into console application.
        /// </summary>

        public static void Main()
        {
            // Create and run the African animal world

            ContinentFactory africa = new AfricaFactory();
            AnimalWorld world = new AnimalWorld(africa);
            world.RunFoodChain();

            // Create and run the American animal world

            ContinentFactory america = new AmericaFactory();
            world = new AnimalWorld(america);
            world.RunFoodChain();

            // Wait for user input

            Console.ReadKey();
        }
    }


    /// <summary>
    /// The 'AbstractFactory' abstract class
    /// </summary>

    abstract class ContinentFactory
    {
        public abstract Herbivore CreateHerbivore();
        public abstract Carnivore CreateCarnivore();
    }

    /// <summary>
    /// The 'ConcreteFactory1' class
    /// </summary>

    class AfricaFactory : ContinentFactory
    {
        public override Herbivore CreateHerbivore()
        {
            return new Wildebeest();
        }
        public override Carnivore CreateCarnivore()
        {
            return new Lion();
        }
    }

    /// <summary>
    /// The 'ConcreteFactory2' class
    /// </summary>

    class AmericaFactory : ContinentFactory
    {
        public override Herbivore CreateHerbivore()
        {
            return new Bison();
        }
        public override Carnivore CreateCarnivore()
        {
            return new Wolf();
        }
    }

    /// <summary>
    /// The 'AbstractProductA' abstract class
    /// </summary>

    abstract class Herbivore
    {
    }

    /// <summary>
    /// The 'AbstractProductB' abstract class
    /// </summary>

    abstract class Carnivore
    {
        public abstract void Eat(Herbivore h);
    }

    /// <summary>
    /// The 'ProductA1' class
    /// </summary>

    class Wildebeest : Herbivore
    {
    }

    /// <summary>
    /// The 'ProductB1' class
    /// </summary>

    class Lion : Carnivore
    {
        public override void Eat(Herbivore h)
        {
            // Eat Wildebeest

            Console.WriteLine(this.GetType().Name +
              " eats " + h.GetType().Name);
        }
    }

    /// <summary>
    /// The 'ProductA2' class
    /// </summary>

    class Bison : Herbivore
    {
    }

    /// <summary>
    /// The 'ProductB2' class
    /// </summary>

    class Wolf : Carnivore
    {
        public override void Eat(Herbivore h)
        {
            // Eat Bison

            Console.WriteLine(this.GetType().Name +
              " eats " + h.GetType().Name);
        }
    }

    /// <summary>
    /// The 'Client' class 
    /// </summary>

    class AnimalWorld
    {
        private Herbivore _herbivore;
        private Carnivore _carnivore;

        // Constructor

        public AnimalWorld(ContinentFactory factory)
        {
            _carnivore = factory.CreateCarnivore();
            _herbivore = factory.CreateHerbivore();
        }

        public void RunFoodChain()
        {
            _carnivore.Eat(_herbivore);
        }
    }
}
```
**Output**

```cpp
Lion eats Wildebeest
Wolf eats Bison 
```

