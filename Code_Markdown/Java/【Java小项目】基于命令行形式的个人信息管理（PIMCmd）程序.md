@[toc]

（必做）编写基于命令行形式的个人信息管理（PIMCmd）程序，要求如下：

This assignment involves the creation of simple `Personal Information Management` (个人信息管理 ) system that can deal with 4 kinds of items: todo items, notes, appointments and contacts. Each of these kinds of items is described in more detail below. The assignment requires that you create a class for each item type, and that each class extends an abstract base class provided for you. In addition to creating the four classes, you need to create a manager class that supports some simple text-based commands for creating and managing items.

Each of your 4 item type classes will be derived from the following abstract class:
```java
public abstract class PIMEntity {
    String Priority; // every kind of item has a priority

    // default constructor sets priority to "normal"
    PIMEntity() {
        Priority = "normal";
    }

    // priority can be established via this constructor.
    PIMEntity(String priority) {
        Priority =  priority;
    }

    // accessor method for getting the priority string
    public String getPriority() {
        return Priority;
    }
    // method that changes the priority string
    public void setPriority(String p) {
        Priority = p;
    }

    // Each PIMEntity needs to be able to set all state information
    // (fields) from a single text string.
    abstract public void fromString(String s);

    // This is actually already defined by the super class
    // Object, but redefined here as abstract to make sure
    // that derived classes actually implement it
    abstract public String toString();
}
```
注释：PIM可以处理4种类别事项：待办事项，备忘，约会和联系人，PIMEntity是公共抽象父类，创建PIMManager进行测试，(有给定名称的要按给定的名称)。

![](https://img-blog.csdnimg.cn/4ce3dbd174a04b2692e871c645fd90f4.png)

![](https://img-blog.csdnimg.cn/00632a7b37b040d491297918723af6df.png)

![](https://img-blog.csdnimg.cn/7e8c5da6cb0b41bf9b47cc824a53d30c.png)

![](https://img-blog.csdnimg.cn/94d9234a6e0e405190d44cbcc228bd30.png)
（注释：数组版本，可以约定待办事项不超过100项，不需要实现Delete）

（注释：自定义PIMCollection集合类，实现Collection接口，扩充下面给出的几个方法）
![](https://img-blog.csdnimg.cn/f4dce56211964c7aad5f5819b9871b77.png)

![](https://img-blog.csdnimg.cn/15668bff26f242b4b3bafcf0c4a6c800.png)

注意：所选择的日期工具对比较的支持是否达到你想要的效果，如果达不到，阅读API文档。如用java.util.Date日期有年月日，如果相比较日期，注意Date.compareTo方法是否会比较getTime()的值——Returns the number of milliseconds since January 1, 1970, 00:00:00 GMT represented by this Date object；如用java.util.Calendar，是否找到自己合适的方法；如用java.time.*包下各种日期时间工具，注意阅读匹配你自己安装JDK版本的文档（Java SE 1.8+）。

![](https://img-blog.csdnimg.cn/10e3dd512c9149819f1cf58cb4283bb6.png)
注释：PIMCollection并不依赖前面数组版本的实现，前面的实现用来测试PIMCollection。

![](https://img-blog.csdnimg.cn/e640373d086844f5a740e80934595f17.png)
注释：继承Collection框架中符合条件的某个具体类，然后进行特定扩展即可。

---
# 一、实验目的
1. 熟悉Java的集合类与泛型，了解Java的泛型特点
2. 初步掌握泛型编程方法


# 二、实验过程
1. 实验环境
操作系统：Windows 11
集成开发环境：Eclipse IDE for Enterprise Java and Web Developers (includes Incubating components) 2022-03 (4.23.0)
2. 题目分析
本实验要求编写一个个人信息管理程序，并且基于命令行形式实现四种功能：List（打印所有的PIM对象）、Create（添加一个新的PIM对象）、Save（保存整个PIM对象列表）、Load（从存储中读取整个PIM对象列表），后面两种功能可选。
由于时间比较紧张，本次实验我选择了用抽象类PIMEntity+array version实现个人信息管理程序，源代码放在abstract_class_ver包中。
3. 抽象类PIMEntity+数组版本代码简介
PIMEntity抽象类由实验题目给出，这里不做介绍。继承了PIMEntity的有四个类：PIMToDo、PIMNote、PIMAppointment、PIMContact，分别代表待办事项、备忘录、邀约、联系人。它们的代码比较简单：
![](https://img-blog.csdnimg.cn/716e02887ea2471cb073f0f76347f129.png)
![](https://img-blog.csdnimg.cn/5a5d8a3616074912a84d8720ac06e47c.png)
![](https://img-blog.csdnimg.cn/e28bd106575140e3bc5e03c3295b2f77.png)
![](https://img-blog.csdnimg.cn/37195cbff1164676b0f97aea57bda6c5.png)

	之所以fromString抽象方法的实现都为空方法体，是因为从题意来看，用户的输入不会是一个单独的文本字符串，而且这里我没有正式实现Save和Load功能（比如将个人信息PIM对象转换为字符串存入文件中，或从文件中读出），所以就没有真正实现fromString方法。此外，PIMTodo和PIMAppointment两个类包含日期，需要共享我定义的接口SharedDate：
	
	![](https://img-blog.csdnimg.cn/9ad6025b91b04e1abe8d26fc36ca758f.png)

	PIMManager类虽然代码比较多，但实现起来相对简单，这里只展示部分截图：
	
	![](https://img-blog.csdnimg.cn/d4d00c3478e44b6493ff5673d55dd87d.png)

# 三、实验结果分析
完成程序编写后，在Eclipse中点击Run，执行结果如下：
![](https://img-blog.csdnimg.cn/a1a872d17cb54ebd8fd59b2d992d91af.png)

可见，实验结果正确。

# 四、实验小结
本次实验由于时间有限，没有实现接口版本的PIMEntity和继承某一Collection类（如ArrayList）的PIMCollection集合类，但是也充分体会到了Java面向对象开发对于实际问题建模的巨大帮助，较为熟练地掌握了Java的集合类（特别是ArrayList）。


---
完整的代码和报告见本人的GitHub：[https://github.com/memcpy0/Mini-Java-Projects/tree/main/PIMCmd](https://github.com/memcpy0/Mini-Java-Projects/tree/main/PIMCmd)
