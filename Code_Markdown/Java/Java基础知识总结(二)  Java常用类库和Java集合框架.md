## Java常用类库
@[toc]
### 一、StringBuilder和StringBuffer
StringBuffer是可变字符串对象，可以将其他各种类型的数据追加，插入到字符序列中，线程安全。
StringBuilder和StringBuilder很相似，但是它不是线程安全的，因此性能高一些，**通常需要创建一个内容可变的String对象时，优先使用StringBuilder类**。
一旦得到最终的字符串对象，应该使用StringBuilder提供的`toString()`方法转换为String。
```java
/* String StringBuilder StringBuffer使用
 * String: 不可变字符序列
 * StringBuffer: 可变字符序列，一旦得到了最终的字符串，可以
 * 通过toString方法转换成String类的对象，线程安全
 * StringBuilder: 可变字符序列，线程不安全，性能更高 
 * */
public class StringBuilderTest {
	public static void main(String[] args) {
		StringBuilder s = new StringBuilder();
		
		s.append("java");
		System.out.println(s);
		
		s.insert(0, "Hello ");
		System.out.println(s);
		
		s.append(1.8);
		System.out.println(s);
		
		s.replace(2, 3, "Best");
		System.out.println(s);
		
		s.delete(2, 3);
		s.reverse();
		System.out.println(s);
		
		String stringS = s.toString();
		System.out.println(stringS.toUpperCase()); // String 大写形式
	}
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191208002222967.png)
```java
public class StringBuilderTest {
	public static void main(String[] args) {
		StringBuilder s = new StringBuilder();
		s.append(33);
		s.append(new String("天"));
		s.append("学会");
		s.insert(s.length(), "java");
		s.append(", 走上人生巅峰!");
		s.insert(s.length(), ", 奥利给!");
		System.out.println(s);
		s.replace(2, 3, "懵逼");
		s.delete(s.length() - 6, s.length());
		System.out.println(s);
	}
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191208001901509.png)
### 二、Math类
```java
// Math类里面的全部方法和常量都是静态的
public class MathTest {
	static char sex = 'F';
	static String desc;
	static { // 类加载时就应该对静态变量初始化
		if (sex == 'F') desc = "female";
		else desc = "male";
	}
	public void testClass() {
		System.out.println("This is MathTest.");
	}
	public static void main(String[] args) {
		System.out.println(Math.E + " " + Math.PI);
		System.out.println(Math.abs(-23.32));
		System.out.println(Math.log(Math.E));
		System.out.println(Math.max(234, 323.3));
		System.out.println(Math.min(234, 323.3));
		System.out.println(Math.random() * 100);
		System.out.println(Math.sqrt(9));
	}
}
```
一个类中可以存在静态变量和成员变量， 成员变量用构造方法初始化，**静态变量在实例化的时候已经存在，那么在类加载时进行静态变量初始化，方法是使用静态块**。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191208002345772.png)
### 三、基本数据类型包装类
基本数据类型在栈中分配内存，栈从上往下增长，包装类是引用类型，在堆中分配内存，堆从下往上增长。

内存布局最低部分的地址是保留区，其上是代码段，再上是静态数据段(static数据)，再上是动态数据段(堆)，再上是栈。
```java
// 基本数据的包装类测试
// 包装类提供了基本数据类型转换为包装类对象的方法，提供了多种方法进行数据类型之间的转换
// 基本类型的值使用栈内存，包装类使用堆内存

public class PackageClassTest { 
	public static void main(String[] args) { 
		Integer int1 = new Integer(12), int2 = new Integer("12");
		Integer int3 = Integer.valueOf(12), int4 = Integer.parseInt("12");
		System.out.println(int2 + " " + int4);
		
		int Int1 = int1.intValue();
		String strint = int1.toString();
		System.out.println(int1 + " " + strint);
		
		Double double1 = new Double(12), double2 = new Double("12");
		Double double3 = Double.valueOf(12), double4 = Double.parseDouble("12");
		System.out.println(double2 + " " + double4);
		
		double Double1 = double1.doubleValue();
		String strdouble = double1.toString();
		System.out.println(double1 + " " + strdouble);
	}
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191208002641751.png)
### 四、Class类
Class是一个final类，是Java中类对象的描述类，**通过一个类的Class对象，获取该对象的属性和方法，并且动态地对该对象及其对象的属性和方法进行操作(创建对象，调用方法等)，称为类的反射**。
```java
import java.lang.reflect.Method;
// Class类是对类结构的描述, 每一个Java类都有一个Class对象, 它是具体类的描述, 包括具体
// 类的类名, 属性, 方法, 构造方法等
// Object中有getClass()方法, 所以每个类都可以通过调用实例对象的getClass()方法得到该类的Class对象
// 也可以通过类属性.class获取. 
public class ClassTest { 
	public static void main(String[] args) throws InstantiationException, IllegalAccessException { 
		try {
		    // 使用"类全限定名"动态加载类, 类名可以在程序编译期确定
			Class mathClz = Class.forName("MathTest"); 
			// 也可以这样获取MathTest的Class对象
			// Class mathClz = MathTest.class; // 加载一个类，类名在程序编译时确定
			// Class mathClz = new MathTest().getClass(); // 从实例对象中获取该对象所在类的Class对象
			Method[] ms = mathClz.getMethods();
			for (Method method : ms) {
				System.out.println(method.getName());
			} 
			// 通过一个类的Class对象可以实例化该类 
			// newInstance方法默认调用类的无参构造函数，没有则抛出异常
			MathTest m = (MathTest) mathClz.newInstance();
			// 等价于 m = MathTest.class.newInstance(); 
			m.testClass(); // 获取上面的MathTest的类的实例对象，并运行testClass方法
		} catch (ClassNotFoundException e) {
			e.printStackTrace();
		} 
	}
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191208003250548.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
### 五、日期和时间类
JDK中的日期类Date，包装了毫秒值，从1970.1.1 00:00:00 GMT开始到现在经过的毫秒值。

Calendar是一个抽象类，提供了日历字段间的转换和操作日历字段的方法。**因为是抽象类，所以不可以实例化，为了在无法实例化的情况下使用Calendar类**，**提供了静态方法**`getInstance()`等，可以获得相应的Calendar子类的对象，然后赋给父类Calendar，用父类使用子类的方法，是经典的多态应用。

java.text中提供了大量格式化有关的类，目的是把一个对象用不同的格式表示，满足不同环境对格式的要求。对日历进行格式化，需要用到DateFormat类，同样，它是一个抽象类，使用相应的getxxxxInstance方法以得到其子类的对象。
```java
import java.util.Date;
import java.util.Calendar;
import java.util.Locale;
import java.text.DateFormat;
// 日期类的使用和日期格式化
public class DateTimeTest {
	public static void main(String[] args) {
		Date date = new Date(); // 根据当前系统时间构造Date对象
		System.out.println(date);
		Date date2 = new Date(date.getTime()); // 根据给定的long时间毫秒值构造Date对象
		System.out.println(date2);
		date.setTime((10L * 365 + 2) * 24 * 60 * 60 * 1000);
		System.out.println(date); 
		
		Calendar cale = Calendar.getInstance(); 
		cale.set(2019, 9, 11);  
		System.out.println(cale.getTime());
		
		cale.set(Calendar.DAY_OF_WEEK, 2); // Calendar中对星期几的计算从星期天开始, 0-6
		cale.set(Calendar.MONTH, 3); // Calendar中对月份的计算从0开始, 0-11
		cale.set(Calendar.DAY_OF_MONTH, 28); 
		cale.set(Calendar.YEAR, 1960);
		Date date3 = cale.getTime();
		System.out.println(date3);
		
		System.out.println("--------------------------------------");
		
		Date today = new Date();
		DateFormat df = DateFormat.getDateInstance(DateFormat.FULL, Locale.CHINA);
		Locale[] locals = new Locale[] {Locale.CHINA, Locale.US, Locale.JAPAN, Locale.UK, Locale.FRANCE, Locale.CANADA};
		for (int i = 0; i < locals.length; i++) {
			DateFormat df1 = DateFormat.getDateInstance(DateFormat.SHORT, locals[i]);
			DateFormat df2 = DateFormat.getDateInstance(DateFormat.MEDIUM, locals[i]);
			DateFormat df3 = DateFormat.getDateInstance(DateFormat.LONG, locals[i]);
			DateFormat df4 = DateFormat.getDateInstance(DateFormat.FULL, locals[i]);
			System.out.println(locals[i].getDisplayCountry() + "的日期格式");
			System.out.println("\tShort格式：" + df1.format(today));
			System.out.println("\tMedium格式：" + df2.format(today));
			System.out.println("\tLong格式：" + df3.format(today));
			System.out.println("\tFull格式：" + df4.format(today) );
		}
	}
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191208004652270.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)![在这里插入图片描述](https://img-blog.csdnimg.cn/20191208005225705.png)SimpleDateFormat对象的parse方法，声明可能抛出一个ParseException，它实际是一个`extends Exception`的**非运行时异常**，必须try...catch捕捉或者继续向调用者抛出。
```java
import java.text.SimpleDateFormat;
import java.util.Date;
import java.text.ParseException;
// SimpleDateFormat是DateFormat的子类，主要使用format方法，通过模式字符串进行格式化
// 解析日期的时候需要处理ParseException
public class SimpleDateFormatTest {
	public static void main(String[] args) {
		Date today = new Date();
		SimpleDateFormat format1 = new SimpleDateFormat("yyyy-MM-dd");
		SimpleDateFormat format2 = new SimpleDateFormat("yyy年MM月dd HH:mm:ss");
		SimpleDateFormat format3 = new SimpleDateFormat("yyyy");
		SimpleDateFormat format4 = new SimpleDateFormat("HH:mm:ss");
		
		System.out.println(format1.format(today));
		System.out.println(format2.format(today));
		System.out.println(format3.format(today));
		System.out.println(format4.format(today));
		
		String birthday = "1999-02-11";
		SimpleDateFormat format = new SimpleDateFormat("yyyy-MM-dd");
		try {
			Date bir = format.parse(birthday);
			System.out.println(bir);
		} catch (ParseException e) {
			e.printStackTrace();
		}
	}
}
```

### 六、数字格式化
对数字的格式化，也是非常常用的。更复杂的数值格式化，使用java.text.DecimalFormat。
```java
import java.text.NumberFormat;
import java.text.DecimalFormat;
import java.util.Locale;
// NumberFormat是所有数值格式的抽象基类，提供格式化和数值解析的接口
// DecimalFormat通过模式字符串对数字进行格式化
public class NumberFormatTest {
	public static void main(String[] args) {
		double myNum1 = 230456789;
		double myNum2 = 0.23;
		NumberFormat nf1 = NumberFormat.getInstance(Locale.CHINA);
		NumberFormat nf2 = NumberFormat.getCurrencyInstance(Locale.CHINA);
		NumberFormat nf3 = NumberFormat.getCurrencyInstance(Locale.US);
		NumberFormat nf4 = NumberFormat.getPercentInstance();
		
		System.out.println(nf1.format(myNum1));
		System.out.println(nf2.format(myNum1));
		System.out.println(nf3.format(myNum1));
		System.out.println(nf4.format(myNum2));
		
		int num1 = 1234567;
		double num2 = 0.1234567;
		DecimalFormat df1 = new DecimalFormat("#,###"); // #代表一个位置数字
		DecimalFormat df2 = new DecimalFormat("#.00");  // 0代表一个数字位置, 不存在时用0补全
		DecimalFormat df3 = new DecimalFormat("00.#");  // #小数部分表示只显示一位小数, 进行四舍五入 
		DecimalFormat df4 = new DecimalFormat("0.##E0"); // E科学记数法
		DecimalFormat df5 = new DecimalFormat("0.##%"); // %百分数表示数字
		
		System.out.println(df1.format(num1));
		System.out.println(df2.format(num2));
		System.out.println(df3.format(num2));
		System.out.println(df4.format(num1));
		System.out.println(df5.format(num2));
	}
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191208005001919.png)



