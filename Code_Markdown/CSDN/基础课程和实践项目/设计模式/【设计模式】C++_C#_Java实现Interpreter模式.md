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
# C++/C#/Java Interpreter
Given a language, the `Interpreter` design pattern **defines a representation** for its grammar **along with an interpreter** that uses the representation to interpret sentences in the language.

Frequency of use：low(Level 1)

## UML class diagram
A visualization of the classes and objects participating in this pattern.
![在这里插入图片描述](https://img-blog.csdnimg.cn/8b92f114cc4246588f0a84c5070c30e9.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_14,color_FFFFFF,t_70,g_se,x_16)
### Participants
The classes and objects participating in this pattern include: 
- `AbstractExpression`（`Expression`）：declares an interface for executing an operation
- `TerminalExpression`  (`ThousandExpression, HundredExpression, TenExpression, OneExpression`)
   - implements an Interpret operation associated with terminal symbols in the grammar.
   -  **an instance** is required for every terminal symbol in the sentence.
- `NonterminalExpression`  (`not used`)
   -  one such class is required for every rule `R ::= R1R2...Rn` in the grammar
   - **maintains instance variables** of type `AbstractExpression` for each of the symbols `R1` through `Rn` .
  - implements an Interpret operation for nonterminal symbols in the grammar. Interpret typically calls itself recursively on the variables representing `R1` through `Rn` .

- `Context` (`Context`)：contains information that is global to the interpreter
- `Client` (`InterpreterApp`)
    - **builds (or is given) an abstract syntax tree** representing a particular sentence in **the language that the grammar defines**. The abstract syntax tree is assembled from instances of the `NonterminalExpression` and `TerminalExpression` classes
    - invokes the Interpret operation


---
# Structural code in C++/C#/Java
This structural code demonstrates the Interpreter patterns, which **using a defined grammer**, **provides the interpreter** that processes parsed statements. 
```csharp
using System;
using System.Collections.Generic;

namespace Interpreter.Structural
{
    /// <summary>
    /// Interpreter Design Pattern
    /// </summary>

    public class Program
    {
        public static void Main(string[] args)
        {
            Context context = new Context();

            // Usually a tree 

            List<AbstractExpression> list = new List<AbstractExpression>();

            // Populate 'abstract syntax tree' 

            list.Add(new TerminalExpression());
            list.Add(new NonterminalExpression());
            list.Add(new TerminalExpression());
            list.Add(new TerminalExpression());

            // Interpret

            foreach (AbstractExpression exp in list)
            {
                exp.Interpret(context);
            }

            // Wait for user

            Console.ReadKey();
        }
    }

    /// <summary>
    /// The 'Context' class
    /// </summary>

    public class Context
    {
    }

    /// <summary>
    /// The 'AbstractExpression' abstract class
    /// </summary>

    public abstract class AbstractExpression
    {
        public abstract void Interpret(Context context);
    }

    /// <summary>
    /// The 'TerminalExpression' class
    /// </summary>

    public class TerminalExpression : AbstractExpression
    {
        public override void Interpret(Context context)
        {
            Console.WriteLine("Called Terminal.Interpret()");
        }
    }

    /// <summary>
    /// The 'NonterminalExpression' class
    /// </summary>

    public class NonterminalExpression : AbstractExpression
    {
        public override void Interpret(Context context)
        {
            Console.WriteLine("Called Nonterminal.Interpret()");
        }
    }
}
```
**Output**

```csharp
Called Terminal.Interpret()
Called Nonterminal.Interpret()
Called Terminal.Interpret()
Called Terminal.Interpret()
```
---
# Real-World code in C++/C#/Java
This real-world code demonstrates the Interpreter pattern which is used to **convert a Roman numeral to a decimal**. 
```csharp
This is a placeholder code. To be replaced.using System;
using System.Collections.Generic;

namespace Interpreter.RealWorld
{
    /// <summary>
    /// Interpreter Design Pattern
    /// </summary>

    public class Program
    {
        public static void Main(string[] args)
        {
            string roman = "MCMXXVIII";
            Context context = new Context(roman);

            // Build the 'parse tree'

            List<Expression> tree = new List<Expression>();
            tree.Add(new ThousandExpression());
            tree.Add(new HundredExpression());
            tree.Add(new TenExpression());
            tree.Add(new OneExpression());

            // Interpret

            foreach (Expression exp in tree)
            {
                exp.Interpret(context);
            }

            Console.WriteLine("{0} = {1}",
                roman, context.Output);

            // Wait for user

            Console.ReadKey();
        }
    }

    /// <summary>
    /// The 'Context' class
    /// </summary>

    public class Context
    {
        string input;
        int output;

        // Constructor

        public Context(string input)
        {
            this.input = input;
        }

        public string Input
        {
            get { return input; }
            set { input = value; }
        }

        public int Output
        {
            get { return output; }
            set { output = value; }
        }
    }

    /// <summary>
    /// The 'AbstractExpression' class
    /// </summary>

    public abstract class Expression
    {
        public void Interpret(Context context)
        {
            if (context.Input.Length == 0)
                return;

            if (context.Input.StartsWith(Nine()))
            {
                context.Output += (9 * Multiplier());
                context.Input = context.Input.Substring(2);
            }
            else if (context.Input.StartsWith(Four()))
            {
                context.Output += (4 * Multiplier());
                context.Input = context.Input.Substring(2);
            }
            else if (context.Input.StartsWith(Five()))
            {
                context.Output += (5 * Multiplier());
                context.Input = context.Input.Substring(1);
            }

            while (context.Input.StartsWith(One()))
            {
                context.Output += (1 * Multiplier());
                context.Input = context.Input.Substring(1);
            }
        }

        public abstract string One();
        public abstract string Four();
        public abstract string Five();
        public abstract string Nine();
        public abstract int Multiplier();
    }

    /// <summary>
    /// A 'TerminalExpression' class
    /// <remarks>
    /// Thousand checks for the Roman Numeral M 
    /// </remarks>
    /// </summary>

    public class ThousandExpression : Expression
    {
        public override string One() { return "M"; }
        public override string Four() { return " "; }
        public override string Five() { return " "; }
        public override string Nine() { return " "; }
        public override int Multiplier() { return 1000; }
    }

    /// <summary>
    /// A 'TerminalExpression' class
    /// <remarks>
    /// Hundred checks C, CD, D or CM
    /// </remarks>
    /// </summary>

    public class HundredExpression : Expression
    {
        public override string One() { return "C"; }
        public override string Four() { return "CD"; }
        public override string Five() { return "D"; }
        public override string Nine() { return "CM"; }
        public override int Multiplier() { return 100; }
    }

    /// <summary>
    /// A 'TerminalExpression' class
    /// <remarks>
    /// Ten checks for X, XL, L and XC
    /// </remarks>
    /// </summary>

    public class TenExpression : Expression
    {
        public override string One() { return "X"; }
        public override string Four() { return "XL"; }
        public override string Five() { return "L"; }
        public override string Nine() { return "XC"; }
        public override int Multiplier() { return 10; }
    }

    /// <summary>
    /// A 'TerminalExpression' class
    /// <remarks>
    /// One checks for I, II, III, IV, V, VI, VI, VII, VIII, IX
    /// </remarks>
    /// </summary>

    public class OneExpression : Expression
    {
        public override string One() { return "I"; }
        public override string Four() { return "IV"; }
        public override string Five() { return "V"; }
        public override string Nine() { return "IX"; }
        public override int Multiplier() { return 1; }
    }
}
```
**Output**
```csharp
MCMXXVIII = 1928
```

