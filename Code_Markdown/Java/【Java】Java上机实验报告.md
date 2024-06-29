@[toc]

# 1. 命令行日历绘制程序
编写基于命令行形式的日历绘制程序，要求程序能根据用户输入完成某年日历的绘制、或特定日期的星期输出。
```cpp
> java cal 4 2025
April 2025
Su Mo Tu We Th Fr Sa
       1  2  3  4  5
 6  7  8  9 10 11 12
13 14 15 16 17 18 19
20 21 22 23 24 25 26
27 28 29 30
> java cal 4 25 2025
Friday
```


---
# 2. 设计学生、课程、教材类
Create classes named `Book, Course, Student` to perform simple course-selecting system (CSS). Student may select one or more Course, and one Course also has some members of Book. 注：执行方式自己设计，体现上述思路即可。如：
```cpp
> java CSS 19030500001 Java
  19030500001
  courses:Java
  books:Thinking in Java, Java 8
  
> java CSS 19030500001 Java Python
  19030500001
  courses:Java, Python
  books:Thinking in Java, Java 8;Python Programming
```

---
# 3. 命令行整数求和程序
编写基于命令行形式的 `SumOfArgs` 程序，将命令行输入的所有整数相加，输出和。注：输入中非整数的部分直接跳过，不进行求和，不论输入是什么，都不输出错误提示。 运行效果如下所示：（提示: `Integer.parseInt(String s)` ）




---
掌握面向对象的类、继承、多态概念，并能运用Java语言完成基于面向对象的指定功能的程序制作。
#  4. 自然数和整数类
编写一个程序，要求如下：
1. 设计并实现自然数（`MyNaturalNumbers`）及整数（`MyInteger`）类。
2. 自然数及整数类至少能表示及处理**20位自然数或整数数据**。
3. 自然数及整数类能表示及处理**任意位数的自然数或整数数据**。（选作。第五章内容）
4. 自然数及整数类的设计应体现体系结构（即应设计实现完整的**继承关系**）。
5. 自然数及整数类应提供**数据赋值、数据输出、加法计算、减法计算功能**。
6. 自然数及整数类应提供**乘法运算、除法运算**（选作）。
7. 自然数及整数类的赋值应能正确接收带千分符号的格式、及不带千分符号的格式。
8. 自然数及整数类的赋值，对于无效数据应向用户提示信息（选作，第五章及第六章内容）。
9. 自然数及整数类应通过**重写 `equals` 方法**实现对两个数据的大小比较。
10. 自然数及整数类应通过**重写 `toString` 方法**实现对数据的格式化。
11. 自然数类应定义 `toMyInteger` 方法，该方法实现创建一个同值的整数类实例。
12. 整数类应可通过自然数对象实例创建等值的整数对象实例。
13. 自然数及整数类中应**定义成员变量 `length` ，表示当前数据的长度**。
14. 自然数及整数类中应定义静态成员变量 `MAX_VALUE` 、`MIN_VALUE` ，表示数据的最大及最小取值。（选作）
15.  构建测试类，该类实现与用户的交互，接收用户键入的数据，并完成类功能的测试。


---
# 5. 命令行图形绘制程序
编写基于命令行形式的图形绘制程序，实现对多个图元的创建、检索、批量绘制功能。步骤：
1. 创建所有图元的父类：Shape，该类中定义图形的基本属性及方法；
2. 通过继承和重写方法实现全部图元类：矩形(Rectangle)、圆(Circle)、三角形(Triangle)等；
3. 可通过多态实现对任意图形的绘制；
4. 定义静态方法，该方法可以对传入的对象实例进行判断，并输出该对象实例的类型；

创建测试类，完成用户交互（向用户提示信息并接受用户的操作请求），实现程序功能，如下：
1. 程序实现图形创建（模拟创建，如：用户输入1，表示创建一个矩形；输入2，表示创建一个圆；输入3，表示创建一个三角形），并在创建时对该图形命名，最后在内存中保存这些创建的图形（最多20个）。
2. 程序提供针对名称的检索功能，即，根据用户提供的名称在保存的图形中查找图形并输出该图形的类型及创建序号。
3. 程序提供对图形的绘制功能（模拟绘制，如：绘制序号为1，名称为教学楼的矩形图形，可以输出“1 矩形教学楼”），即，根据用户输入的创建序号，顺序输出该序号之前，包括该序号的全部图形。


---
# 6. 设计 `BankSystem` 类
Design `BankSystem` class. You need to design more than one class. For example: You need to design a class `BankAccount` to model users' bank accounts. Probably different bank accounts (`CashAccount`, `CreditAccount`, ...). The account should keep a user's name and balance, accurate to the nearest cent... The user should be able to make deposits and withdrawals on his/her account, as well as changing the account's name at any time. Also, the system needs to be able to find out how many `BankAccount`s have been created in total. For each account, only the last 6 `Transaction`s should be able to store in ascending order and be printed.


---
目标是熟悉Java的集合类与泛型，了解Java的泛型特点；初步掌握泛型编程方法。

# 7. 命令行记事本程序
编写基于命令行形式的记事本程序，实现对记录的添加、删除、检索、展示功能。要求如下：
1. 创建记事本类，通过集合类实现不定数量记录的存储；能够获得已经存储的记录数量；能够追加记录；能够展示已经存储的全部记录或其中任何一条记录；能够删除已经存储的全部记录或其中任何一条记录。
2. 创建测试类，该类实现与用户的交互，向用户提示操作信息，并接受用户的操作请求（如，1添加、2删除、3检索和4展示）。


---
# 8. 命令行个人信息管理程序
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
- `PIMTodo` : Todo items must be PIMEntites defined in a class named `PIMTodo` . Each todo item must have a priority (a string), a date and a string that contains the actual text of the todo item.
- `PIMNote` : Note items must be PIMEntites defined in a class named `PIMNote` . Each note item must have a priority (a string), and a string that contains the actual text of the note.
- `PIMAppointment` : Appointment items must be PIMEntites defined in a class named `PIMAppointment` . Each appointment must have a priority (a string), a date and a description (a string).
- `PIMContact` : Contact items must be PIMEntites defined in a class named `PIMContact` . Each contact item must have a priority (a string), and strings for each of the following: first name, last name, email address.

	There is one additional requirement on the implementation of the 4 item classes listed above, the 2 classes that involve a date must **share** an interface that you define. You must formally create this interface and have both `PIMAppointment` and `PIMTodo` implement this interface.

- `PIMManager` : You must also create a class named `PIMManager` that includes a `main` and provides some way of creating and managing items (from the terminal). You must support the following commands (functionality):
	- `List` : print a list of all PIM items
	- `Create` : add a new item
	- `Save` : save the entire list of items（本次不用做）
	- `Load` : read a list of items from a storage（本次不用做）

	When creating a new item it is expected that the user must response to a sequence of prompts to enter the appropriate information (and even to indicate what kind of item is being created). **Do this any way you want** , just make sure that your system provides enough information (instructions) so that we can use your systems!

There is **no required format** for the user interface, anything that allows users to create, list, save and load is fine. Here is what it might look like (user input shown in red):
```java
java PIMManager

Welcome to PIM.
---Enter a command (suported commands are List Create Save Load Quit)---
List
There are 0 items.
---Enter a command (suported commands are List Create Save Load Quit)---
Create
Enter an item type ( todo, note, contact or appointment )
todo
Enter date for todo item: 
04/26/2020
Enter todo text:
Submit java homework.
Enter todo priority:
urgent
---Enter a command (suported commands are List Create Save Load Quit)---
List
There are 1 items.
Item 1: TODO urgent 04/26/2020 Submit Java homework.
---Enter a command (suported commands are List Create Save Load Quit)---
Save
Items have been saved.
---Enter a command (suported commands are List Create Save Load Quit)---
Quit
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/5dd836b81bc3457faecd3c8e4bdf1043.png)

**接口方案：`PIMEntity`** ——Reimpletement `PIMEntity` as interface, add change the rest of all code to use it.

输入提示：
```java
java.util.Scanner
Scanner in = new Scanner(System.in); //创建Scanner对象
String command = in.nextLine();//获取输入的一行字符串
```
**Storage options (seperate projects)**
1. **array version**: a simple array with size 100. （注释：数组版本，可以约定待办事项不超过100项，不需要实现Delete）
   Note that there is not a required Delete command. Feel free to use any data structure you want to hold a list of items, you are allowed to use a simple array with size 100 (you are not required to support lists of more than 100 items). Handling array bound exception is required.
2. **collection version: PIMCollection class**（注释：自定义PIMCollection集合类，实现Collection接口，扩充下面给出的几个方法）

    This option involves the creation of a custom collection object that can be used to manage PIMEntities (from array version). You must create a class named PIMCollection that implements the interface Collection. In addition to being a Collection, your class must implement the following methods:
  	
	- getNotes：public Collection getNotes();
getNotes() returns a Collection that holds all the PIMNote items currently in the PIMCollection. If there are no PIMNote items in the PIMCollection, this method should return an empty Collection. Note that it is not specified exactly what implementation of a Collection is returned (that's up to you), just that whatever is returned implements the Collection interface. 
	- getTodos：public Collection getTodos();
 getNotes() returns a Collection that holds all the PIMTodo items currently in the PIMCollection. If there are no PIMTodo items in the PIMCollection, this method should return an empty Collection. Note that it is not specified exactly what implementation of a Collection is returned (that's up to you), just that whatever is returned implements the Collection interface.
	- getAppointments public Collection getAppointments();
 getAppointments() returns a Collection that holds all the PIMAppointment items currently in the PIMCollection. If there are no PIMAppointment items in the PIMCollection, this method should return an empty Collection. Note that it is not specified exactly what implementation of a Collection is returned (that's up to you), just that whatever is returned implements the Collection interface.
	- getContacts public Collection getContact();
  getContacts() returns a Collection that holds all the PIMContact items currently in the PIMCollection. If there are no PIMContact items in the PIMCollection, this method should return an empty Collection. Note that it is not specified exactly what implementation of a Collection is returned (that's up to you), just that whatever is returned implements the Collection interface.
	- getItemsForDate  public Collection getItemsForDate(Date d);
 getItemsForDate returns a Collection that holds all the PIMEntities in the PIMCollection that have a date that matches the date d. If there are no items that match the date, this method should return an empty Collection. Note that it is not specified exactly what implementation of a Collection is returned (that's up to you), just that whatever is returned implements the Collection interface.

Note: It is expected that all PIMEntities in the returned collection implement the interface you created during array version. (There should only be PIMTodo and PIMAppointment items, since notes and contacts are not associated with dates).
    注意：所选择的日期工具对比较的支持是否达到你想要的效果，如果达不到，阅读API文档。如用java.util.Date日期有年月日，如果相比较日期，注意Date.compareTo方法是否会比较getTime()的值Returns the number of milliseconds since January 1, 1970, 00:00:00 GMT represented by this Date object；如用java.util.Calendar，是否找到自己合适的方法；如用java.time.*包下各种日期时间工具，注意阅读匹配你自己安装JDK版本的文档（Java SE 1.8+）。


**Testing PIMCollection**

Test your `PIMCollection` program using your other classes (everything from previous array version). Although your PIMCollection class should not depend on any of your other classes (except for possibly the name of the "dateable" interface you created), we will use your classes when testing a PIMCollection, so make sure you include them in your submission. Feel free to make any changes you want to your previous array version classes for this collection version.

注释：PIMCollection并不依赖前面数组版本的实现，前面的实现用来测试PIMCollection。

**IMPORTANT NOTE**

Your PIMCollection class must be a Collection! All methods defined of any Collection object must be supported. Although this sounds difficult, all you need to do is to extend an existing Collection class (like an ArrayList or HashSet). Use whatever makes sense to you as your base class, but it isn't necessary (or wise) to implement the entire Collection interface from scratch (just inherit it!).
 
 注释：继承Collection框架中符合条件的某个具体类，然后进行特定扩展即可。


---
# 9. 对 `BankSystem` 程序添加异常处理
对第4章实验内容3添加异常处理，要求如下：
(1) Add the CheckingAccount class of BankSystem , Designing and Using Classes to throw an IllegalArgumentException in any of the following circumstances:
-  when the account is constructed with a negative balance,
- when a negative amount is deposited, or when the account is overdrawn (when the amount withdrawn exceeds the current balance).

(2) An IllegalArgumentException is an unchecked exception that is thrown to indicate that a method has been passed an illegal or inappropriate argument. 
Instructions:
-  Add (modify if you've already finished) the CheckingAccount class to handle errors and write a test program as indicated above.



