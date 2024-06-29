## Java基础知识总结
@[toc]
### 一、 Java初识
1. Java源代码为.java，用`javac`编译后产生字节码文件.class，然后可以使用JDK的`java`命令运行。字节码文件也可以反编译回Java文件。
2. JVM负责运行这些编译后的class文件，不同的计算机平台拥有不同的JVM，这样就实现了Java的跨平台特性。
3. Java程序打包，可以用JDK中的`jar`对.class文件进行打包，在命令台输入jar了解其完整参数。
 ![在这里插入图片描述](https://img-blog.csdnimg.cn/20191202200101194.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)![在这里插入图片描述](https://img-blog.csdnimg.cn/20191202200149152.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)![在这里插入图片描述](https://img-blog.csdnimg.cn/20191202200421236.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
4. 示例如下：
	 ```java
	public class HelloJava {
	       public static void main(String[] args) {
	              System.out.println("Hello Java!");
	       }
	}
	```
	`D:\Java_Projects>javac -g -verbose HelloJava.java`
![在这里插入图片描述](https://img-blog.csdnimg.cn/201912022007191.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)	
	`D:\Java_Projects>java HelloJava`
	![在这里插入图片描述](https://img-blog.csdnimg.cn/20191202200833296.png)
	`D:\Java_Projects>jar cvf HelloJava.jar HelloJava.class`
	![在这里插入图片描述](https://img-blog.csdnimg.cn/20191202201009797.png)
	![在这里插入图片描述](https://img-blog.csdnimg.cn/20191202201058435.png)
### 二、Java语言基础
1. Java的标识符和C语言的规定差不多，不过是新增加了一个‘$'字符罢了。
2. Java的数据类型分为基础数据类型和引用数据类型，基础数据类型只有`boolean, byte, char, short, int, long, float, double`这八种，其余的**包括数组在内**都是引用数据类型，这里的引用和C艹 里面的差不多。
3. Java各种基本数据类型的范围和C语言中相应的有符号数的范围差不多。但是也有不同。Java中**没有将其他类型使用成布尔型的写法**，`boolean`就是单纯的`true`和`false`两种值，不可以和其他类型转换。**Java没有无符号整数**，`byte`是一个字节；`char`**占用两个字节**，和C++的宽字符差不多，可以保存单个字母或者汉字；`short`2个字节；`int`4个字节，范围-2^31^~2^31^-1；`long`占8个字节；`float`4个字节；`double`8个字节。都是有符号数。
4. Java变量，定义时分配内存空间，根据变量的类型不同，基础类型分配栈空间，引用类型分配堆空间。
5. Java字面常量和类型前后缀：Java字面常量分为整型、浮点型、布尔型、字符型(单引号)、字符串(双引号)字面量和`null`。可以用`0`为前缀表示八进制数，`0x`作为前缀表示十六进制数。整数字面量默认为int型，可以使用`L`后缀将其指定为long类型；浮点字面量一般默认为double，可以使用`F`指定为float。
6. 算术运算和算术运算符：+, -, *, /, %, ++, --(自增自减有前后缀版本，**只允许操作整型变量**)；关系运算和运算符：>, >=, <, <=；逻辑运算符：&&, ||, !(**逻辑与和逻辑或都进行短路求值**)；位运算：&, |, ^, ~, <<, >>, >>>；赋值运算符：=，和其他复合赋值运算符。
7. 类型间可以进行转换，包括自动转换和强制转换，`没有C艹的const_cast, static_cast, dynamic_cast, reinterpret_cast这些麻烦的东西`。自动转换byte->char->short->int(float)->long->double，注意boolean不在其中。字符串和其他类型的数据用加号进行连接，其他类型的数据自动转换为字符串进行连接。
8. `java.util.Scanner`提供了获取控制台输入信息的功能。`Scanner scanner = new Scanner(System.in);`
    | 方法名 | 返回类型 | 
    |:-|:-|
    | next | 字符串|
    | nextByte | byte|
    |nextShort| short|
    | nextInt | int| 
    |nextLong | long|
    |nextFloat | float |
    | nextDouble | double|
  
 ### 三、Java语言程序结构
1. 顺序、分支语句和C没有差别。switch条件变量只能为byte, char, short, int或者枚举型，不能使用其他类型。if-else可以写成?:三元运算符，**注意?前的表达式必须返回boolean型，不能像C一样只写个变量**。
2. 循环结构和C/C++一样。

### 四、数组
1. 数组是引用类型。声明时使用`type[] arrayName`，可读性更好，而不是使用C/C++风格的数组写法。**这里的数组不能指定长度**，因为声明数组仅仅定义了一个引用变量，并没有指向有效的内存空间，因此不能使用。
2. 数组必须进行初始化才能使用。声明后静态初始化：`arrayName = new type[]{element1, element2...};`，如果**声明的同时进行静态初始化**，可以写成：`type[] arrayName = new type[]{element1, element2...}`，也可以简写成：`type[] arrayName = {element1, element2...}`，注意：**静态初始化不需要指定数组长度**！其长度由{}中的元素个数决定，其内容由{}中的元素值决定。
3. 数组动态初始化：`arrayName = new type[length];`，**需要指定数组长度**~，**数组内容则由数组元素类型决定，执行值初始化**。如byte, short, int, long则数组元素为0；float, double为0.0；char型为`'\u0000'`；boolean则为false；**引用类型(包括类、接口、数组)，数组元素值为null**。
4. 数组内存空间重分配。可以再次进行内存申请，变更内存空间。
5. 数组使用：可以通过[]访问数组；由于是引用类型，通过[]改变值会真正影响到数组中元素的值；数组检查索引越界，运行时抛出`java.lang.ArrayIndexOutOfBoundsException`异常。
6. 常用的属性就是`array.length`，得到数组的长度；数组的方法如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191202210233643.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
7. 多维数组：Java一样没有多维数组，只有数组的数组。**二维数组实际上是元素为一维数组引用的数组，其中的每个一维数组不一定有同样的长度。**
8. 二维数组：声明：`type[][] arrayName;`，同样**不能指定数组长度**；初始化：`arrayName = new type[length][];`。初始化后，二维数组中的元素大概是`{null, null,....}`，每个元素都是一个一维数组的引用。
9. 二维数组可以先声明再静态初始化，也可以一起写，和上面一样，**静态初始化不需要指定数组长度**，因为`{{e1, e2...}, {f1, f2, ...}, {}...}`决定了二维数组的长度和内容；**动态初始化则需要指定高维的长度**。

### 五、字符串
1. Java.lang包中的String类，是自定义类型，字符串变量实际是一个对象。String代表了不可变的字符序列。还有StringBuilder和StringBuffer两个可变字符串类。
2. String的定义和赋初识值：`String s = "value"; String s2 = new String("value");`，第一种更常用，建议使用。
3. 字符串的连接操作用+；**相等比较用equals()方法，不能直接使用==运算符**，它比较对象的引用地址是否相等，即只有同一个对象才等于自己。而equals方法继承自Object，Object中的equals也是比较内存地址，但是String中的equals进行了重写，改成了比较字符串内容。如果是基本数据类型，就使用相等运算符就好了。
4. 字符串的常用操作：
     | 方法名 | 返回类型 | 说明 |
    |:-|:-|:-| 
    | charAt(int) | char | 获取字符串中相应位置的字符|
    | **compareTo(String)** | int | 判断两个字符串的大写 |
    | indexOf(String) | int | 判断传入的字符串在原字符串中**第一次出现**的位置，没有出现返回-1 |
    | **length()** | int | 字符串长度 |
    | lastIndexOf(String) | int| 判断传入的字符串在原字符串中**最后一次出现**的位置，没有出现返回-1 |
    | startsWith(String) | boolean | 判断原字符串是否以传入字符串开头 |
    | endsWith(String) | boolean | 判断原字符串是否以传入字符串结尾 |
    | toLowerCase() | String | 字符串全小写 |
    | toUpperCase() | String | 字符串全大写 |
    | **substring(int)** | String | 取子串，从传入参数位置开始到末尾 |
    | **substring(int, int)** | String | 取子串，从传入参数1位置到传入参数2位置 |
    | trim() | String | 去除字符串两端的空格 |
    | **split(String)** | String[] | 将原字符串按传入参数分割为字符串数组 |
    
5. 随机数类：`java.util.Random`，使用nextInt/nextLong...等方法生成相应的随机数。
6. 字符串类与基本类型的转换：字符串转换为基本类型，需要**使用基本类型的包装类的相应方法**，如`parseBoolean(String), parseByte(String), parseShort(String), parseInt(String), parseLong(String), parseFloat(String),  parseDouble(String)`，转换为char用字符串的`charAt()`方法。；同样，也可以将基本类型转换为字符串，用字符串连接+，或者字符串内置的`String.valueOf`方法，或者包装类的方法，如`toHexString(int), toBinaryString(int)`等。
7. 字符串的格式化输出：（在`String.format`方法里面）
     | 转换符| 说明 |  
    |:-|:-| 
     |%%| 百分比|
    | %n | 换行 |  
    |%c | 字符类型 |
    | %s | 字符串 |
    | %b |布尔类型 |
    | %o | 整数类型(八进制) |
    |%d | 整数类型(十进制) |
    | %x | 整数类型(十六进制) |
    | %f | 浮点类型 |
    | %a |十六进制浮点类型 |
    | %e | 指数类型 |
    | %g | 通用指数类型(f和e类型较短的) |
    | %h | 散列码 |
    |%tx | 日期时间类型，x代表不同的日期和时间转换符) |
     ```java
	 // 字符串格式化操作示例
	public class StringFormatTest {
		public static void main(String[] args) {
			String str = null;
			String name = "zhangsan";
			str = String.format("Hi, %s, 251 warning!%n", name);
			System.out.println(str);
			str = String.format("%d + %d = %d", 1, 2, 1 + 2);
			System.out.println(str);
			str = String.format("%d%%", 40);
			System.out.println(str);
			str = String.format("100的十六进制数是：%x", 100);
			System.out.println(str);
			str = String.format("100的八进制数是：%o", 100);
			System.out.println(str);
		}
	}
	```
8. 字符串的正则表达式：String提供了matches(reg)方法验证给定的字符串是否和正则表达式匹配。通过String对象调用matches方法。`java.util.Regex`提供了更完善的支持。

### 六、Java中的方法
1. Java天生就是面向对象，没有函数，只有方法。方法名的命名规范和变量名的命名一致，**采用小写字母开头的驼峰式写法**，库函数基本都是这种写法。方法命名采用动词。
2. 方法之间是独立的，**方法内部不能嵌套其他方法**。方法只能返回一个值 ，我们可以用对象包括多个结果。 

### 七、类和对象
1. 类名采用首字母大写的驼峰式写法，通常是名词，为了良好的设计风格，不应出现“_"和"$"这样的特殊字符。一个公共类就是一个文件，文件名必须与公共类名字相同。
2. 类中存在方法和成员变量。成员变量定义时可以有初始值，没有的话会进行值初始化，作用域是整个类。方法中的是局部变量，没有初始值，只在方法中起作用。 当局部变量和成员变量重合时，优先使用局部变量；**可以使用关键字this来指向成员变量**，`this.attribute， this.method(args)`，this就代表当前类的当前对象，是一个引用，在类中使用以操作本类的属性和方法。
3. 通过类可以定义对象，都是引用，放在栈内存中，指向堆内存中实际的对象。不像C艹对象实际存在，还要定义对象的拷贝、赋值、移动和销毁，传递引用还要自己写，**Java直接让所有对象都是引用**。
4. `ClassName objectName = new ClassName()`。使用对象的属性`objectName.attribute`，使用对象的方法`objectName.method(args)`。
5. 类、方法、成员变量修饰符之一：static，修饰成员变量，成为静态成员，它不依赖于类的某一实例，使用类名直接调用，`ClassName.staticAttr`，不像C艹还要使用作用域运算符::。一个类的不同对象间，普通成员变量会不同，静态成员一定相同。普通成员变量(实例变量)生命周期是随着它的对象的产生到对象回收，静态变量(类变量)则从类加载的时候初始化，直到程序结束。

    static修饰方法成为静态方法，同样不依赖于任何对象，`ClassName.method(args)`。普通成员方法则必须使用类对象来调用。main方法是静态的，因此JVM首先运行main方法时没有也不会创建main方法所在类的实例对象，因此，**main方法中不能直接访问该类中的非静态成员，不然运行会出错**。必须创建该类的实例对象并借此访问非静态成员。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191203212825354.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191203212931507.png)
6. 类、方法、变量修饰符之一：final，final类表示**最终的类，无法继承**，断子绝孙(笑)，如String，System，Scanner类都是；final方法，**最终的方法，无法在子类中重写overwrite**；final修饰的变量(就是常量)，其值不能改变，既然一个类的诸多对象都使用这个无法更改的final常量，那么就没有必要出现在每个对象中，应该**直接设置其为`static final`**，共享数据节省空间，这是约定的写法，另外**常量命名使用全大写，单词间用分隔符_分割**。
7. Java中有构造方法，允许程序员控制对象的定义和初始化。默认的初始化会将所有基本数据类型的值设置为0/false，所有引用类型设置为null。如果类中没有任何构造方法，系统会为类提供一个无参构造方法，执行默认初始化。如果要改变的话就要自己定义构造函数，一旦自己定义了就不再提供无参构造方法。
8. 方法重载：可以通过方法参数的不同(**方法参数数量不同；类型不同；同样数量参数中，相同参数类型在参数列表中的顺序不同**)进行重载override，和返回值类型没有关系，和参数名称没有关系。构造方法可以重载，以适应灵活的对象初始化需求。
9. Java中的package：为了避免类名冲突，Java使用不同的包管理类文件，把不同类组合在一起，易于查找和使用，对类、类成员变量、静态变量、成员方法、静态方法也有保护作用。在类文件的开头用`package`定义包名。常用包：`java.lang(语言核心类); java.util(常用工具类); java.text(格式化相关); java.io(输入输出相关); java.net(网络操作); java.awt, java.swing(Java图形界面程序); java.sql(数据库操作)`。使用其他包中的类用`import`，如`import java.io.FileInputStream`。Java的lang包默认导入。
10. 面向对象封装：把对象的属性和方法组合在一起，统一提供对外访问权限，把对象的使用者和设计者分开。设计者用类修饰符控制使用者对类的访问权限，`public`：公共的，任何其他类都可以访问；`protected`：保护的，允许**相同包中的任何类**和**非相同包的子类**访问；`(default)`：缺省，没有修饰符，表示允许**相同包中的任何类**访问，不允许其他包的任何类(包括子类)访问；`private`：私有的，只允许在**本类内部**访问，子类和其他任何类都不可以访问。public最开放，private最封闭，protected和default意在控制包一级的访问权限和子类的访问权限。一般属性都使用private，用get/set方法进行访问，只有get则为只读属性，只有set则为只写属性。
11. 面向对象继承：继承使用`extends`，**子类继承父类的所有属性和方法**，比如private属性也是，但是子类中不能访问它；**例外的是父类的构造方法子类没有继承**。子类中可以访问父类的public、protected的属性和方法，private属性和方法绝对不能访问，**如果子类和父类不在同一个包中，则(default)属性和方法也不能被访问**。
    
    当然，继承绝非照搬，可以额外增加新的方法，但有时也会重写overwrite父类的方法，但子类重写了父类的有关方法时，就无法访问父类的方法了，但是可以用`super`来访问，**super表示直接父类对象的默认引用**，`super(args)`调用父类的构造方法进行初始化，`super.method(args)`调用父类中的方法。**super和this一样，无法在static方法中使用**，它是属于对象的，在没有实例化对象的情况下也就不存在父对象，也就不能使用super。如果**父类方法具有private访问权限**，那么该方法对子类隐藏，**子类无法访问也就无法重写该方法，只算是重新定义了一个新方法**。
    
    子类对象实例化过程：
    1. 实例化子类对象，会先申请一片内存空间，先为子类中的属性设置初始值；
    2. 将参数传递给相应的构造函数；
    3. 如果**构造方法体的第一行有this()或者super方法调用**，则流程跳转到第2步。如果没有，则此时会**默认调用父类的无参构造函数**，流程继续跳转到第2步...**一直追溯到Object方法**，初始化Object对象，然后一路返回。
    4. 运行当前构造方法体中的方法。
    5. 最终创建出当前子类的对象。
  
      注意：**由此，this()/super()必须是构造函数体的第一行代码，并且两者不能同时出现在同一个构造函数中**。
      子类对象实例化之前，必须调用父类的构造方法，用super关键字可以显式指定调用父类的构造方法，没有指定就默认调用父类的无参构造方法(此时父类没有无参构造方法就会编译出错，因此**建议显式定义一个无参构造方法**)。

 	`java.lang.Object`是所有类默认的父类，如果一个类没有显示extends其他类，那么就会默认继承Object类。因此，所有类都是Object的子类，都具有Object的属性和方法，常用方法(需要被重写)： 
     | 方法名 | 返回类型 | 说明 |
    |:-|:-|:-| 
    | clone() | Object | 创建并返回此对象的一个副本 |
    | equals(Object obj) | boolean | 判断是否是同一个对象 |
    | finalize() | void | 定义该方法后，当垃圾回收器确定不存在对该对象的更多引用时自动调用该方法 |
    | hashCode() | int | 返回该对象的哈希值 |
    | toString() | String | 返回该对象的字符串表示 |
    | wait | void | 多线程中让其等待 |
    | notify()/notifyAll() | void | 唤醒其他线程 |
	   ![在这里插入图片描述](https://img-blog.csdnimg.cn/20191204142008600.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
12. 面向对象的多态：多态就是**把子类的实例对象赋值给父类的引用，使用父类的引用调用方法时，如果子类重写了父类的方法，会调用子类的方法，不然就调用父类的方法**。如，`human.eat()`，human可以调用的是`chinese`的eat()，也可以是`american`的eat()。一般来说父类是有意义的，因此父类是实际类，但是抽象类更进一步深化了面向对象思想，接口又在抽象类上面进一步提升(**只要有可能，不要从具体类继承，使用抽象类作为父类，用接口指示必须实现的方法**)。
      
      抽象类：用`abstract`修饰的类，其中不用实现的方法用`abstract`加以声明，称为抽象方法(`java的抽象方法不能是static、final、private`)。含有抽象方法的类必须用abstrac声明为抽象类。**抽象类中只提供一个类的部分实现，因此不能实例化**；子类继承了抽象类后，如果没有实现相应的抽象方法，则子类也是抽象类，必须用`abstract`修饰。
      ![在这里插入图片描述](https://img-blog.csdnimg.cn/2019120416040852.png)   
      接口：接口是一个只有**static final数据成员**和**public abstract抽象方法声明**的**public abstract类**，因此数据成员不能修改，方法不做具体实现。用`interface`关键字声明接口。类和接口并列，类可以被类继承，接口只能被类用`implements`实现。**一个类不能继承多个类，但是可以实现多个接口**。接口是Java对多继承的实现方法。实现接口的类和接口间是is-a的关系。接口中的方法必须全部实现，不然继承该接口的类是抽象类，必须用`abstract`修饰。
	  ![在这里插入图片描述](https://img-blog.csdnimg.cn/20191204160630273.png)
	  接口和抽象类的关系：
	  - 相同点：接口属于抽象类的特例，接口中的抽象方法和抽象类的抽象方法声明的要求一致(public abstract)；**继承接口或抽象类的类如果没有实现所有的抽象方法，就必须显式声明为抽象类**；抽象类和接口都不会有对象，但是它们都可以作"父类"，指向子类对象的引用，实现多态。
	  - 区别：一个类可以实现多个接口，但只能继承一个抽象类；**抽象类中可以有自己的数据成员、非abstract的成员方法和abstract方法，接口只能有static final数据成员和public abstract方法**；抽象类中可以有非抽象方法实现，但是接口中不能有方法实现(因为都是抽象方法)。一般在继承时优先考虑接口，然后才是抽象类。
		 
13. Java中的枚举：一个季节类，只有春夏秋冬四个实例对象。我们可以通过定义一个Season类，**构造方法设为`private`让外部程序不能实例化Season类**，在Season**类内部定义4个`public static final`Season对象**`SPRING, SUMMER, AUTUMN, WINTER`，然后在外部使用时，用Season.SPRING来表示Season的一个对象；我们也能用枚举`java.lang.Enum`类型定义枚举类(所有枚举类默认继承java.lang.Enum)，枚举类中可以有自己的构造方法，但**必须是private的**，枚举类中还需要**显式列出**它的**所有实例常量**，枚举值用逗号分割。
    ```java 
	public enum SeasonEnum {
		SPRING("春天", "鸟语花香"), SUMMER("夏天", "烈日炎炎"), 
		AUTUMN("秋天", "秋高气爽"), WINTER("冬天", "白雪皑皑");
		private String name; // 季节名称
		private String desc; // 季节描述
		private SeasonEnum(String name, String desc) {
			this.name = name;
			this.desc = desc;
		}
		public String getName() {
			return name;
		}
		public String getDesc() {
			return desc;
		}
	} 
	```
	```java
	public class SeasonEnumTest {
		public static void main(String[] args) {
			// 枚举对象常用方法
			// 返回此枚举实例的名称
			System.out.println(SeasonEnum.SPRING.name());
			// 返回当前枚举在定义在的索引值，从0开始
			System.out.println(SeasonEnum.WINTER.ordinal());
			// 判断枚举对象的定义顺序，当前对象在枚举值参数之前返回负值
			System.out.println(SeasonEnum.SUMMER.compareTo(SeasonEnum.AUTUMN));
			// 枚举类自定义方法
			System.out.println(SeasonEnum.SPRING.getName());
			System.out.println(SeasonEnum.SUMMER.getName());
			System.out.println(SeasonEnum.AUTUMN.getDesc());
			System.out.println(SeasonEnum.WINTER.getDesc()); 
		}
	} 
	```
	![在这里插入图片描述](https://img-blog.csdnimg.cn/20191203223255665.png)
### 八、异常处理
1. 异常类都是`Throwable`的子类，不同的子类描述不同的异常，*一一对应*。JDK中定义了一个异常继承系统和一些基本异常类型，我们也可以通过继承`Throwable`的子类`Exception`甚至是`Exception`的子类来创建自己的异常。
2. 异常系统：Throwable继承Object，子类为Error和Exception；Error和它的子类的实例识别为**致命的、无法修复的、不应该由用户程序捕获的错误**；Exception和它的子类的实例是可控制的异常，可以捕获并修复：其中，RuntimeException和RuntimeException的子类是运行时异常，可以**通过程序员努力加以避免，因此不要求必须处理**，其他的异常类(Exception和非RuntimeExcepton的子类)都是**非运行时异常，必须用try...catch...finally进行捕获并处理**，否则连编译都无法通过。常用的一些异常类型包括：`IOException, FileNotFoundException`等非运行时异常，`NullPointerException(尝试访问null对象的成员时发生的空指针异常), ArrayIndexOutOfBoundsException(数组越界异常), IllegalArgumentException(方法接受到非法参数), ArithmeticExceptoin`等运行时异常。
3. 有了这些异常类，异常对象从何而来？一是Java运行时**环境自动抛出**，不管是否愿意捕获和处理，二是程序员**自己抛出的异常**，可以是Java语言中已经定义了的，也可以是自己定义的，用`throw`抛出异常。
4. `try`和`catch`，包围可能出现异常的程序段，一旦出现异常，会生成一个异常对象，然后**从上到下**在catch中寻找有没有能够匹配的异常类型，匹配上了就运行catch块中的内容。每个异常对象有一个`getMessage()`方法获取异常的描述信息。如果同时处理多个异常，**父类异常要放在后面**。
5. `finally`：用于**完成一部分无论发不发生异常都必须完成的操作**，如关闭文件，关闭数据库连接等。finally块在try块或者catch块完成后运行，**如果try或者catch中遇到return时，会先完成finally块的内容，然后再返回**。
6. 对于一些异常不立即进行处理，可以使用`throws`在定义方法的参数列表后面声明该方法可能抛出的异常类型。方法的调用者：**如果是运行时异常，不必理会声明/使用try...catch处理/throws继续延迟处理**，都可以编译通过；如果是非运行时异常，**必须使用try...catch进行处理/使用throws延迟处理**，不然无法通过编译。因此，在后面我们会遇到调用一些对象的方法时，必须要求添加try...catch语句或者throws的情况，一般来说IDE会给出相应的提示，Eclipse就是这样的。	   ![在这里插入图片描述](https://img-blog.csdnimg.cn/20191204135818300.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
8. 除了系统创建并抛出，程序员还能`throw ExceptionObject`，手动抛出异常对象，可以随即使用try...catch处理，还可以throws声明让其抛给方法的调用者。
    	
9. 自定义异常：用户自定义异常往往会继承Exception，但是这样定义的就是非运行时异常，**必须使用try...catch进行处理/使用throws延迟处理**，然而用户自定义异常一般可控，因此建议继承RuntimeExcepion。RuntimeException父类会提供无参构造方法和传递字符串参数的构造方法，**在子异常类的构造函数中可以使用super()和super(message)调用**(毕竟没有继承构造方法)。
