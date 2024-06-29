### Java重写、白板实现
重写Overriding是子类对「父类允许访问的方法」的实现过程重新编写，在子类存在方法与父类的方法的名字相同、参数的个数与类型一样,返回值也一样的方法，即**外壳不变核心变化**。

> 为什么重写？
**子类可以根据需要，定义特定于自己的行为**，即根据需要实现父类的方法。方法重写是**子类与父类的一种多态性表现**。

实现如下：
```java
class Animal {
	public void move() {
		System.out.println("动物可以移动");
	}
}

class Dog extends Animal {
	public void move() {
		System.out.println("狗可以跑和走");
	}
}

public class TestDog {
	public static void main(String args[]) {
		Animal a = new Animal(); // Animal对象
		Animal b = new Dog(); // Dog对象
		a.move(); // 执行Animal类的方法
		b.move(); // 执行Dog类的方法
	}
}
```
重写后，用父类引用的子类对象，调用重写方法执行的是子类的方法。
> 由于在编译阶段，只是检查引用类型，之所以能编译成功是因为Animal类存在move方法。在运行时，[Java虚拟机](https://so.csdn.net/so/search?q=Java%E8%99%9A%E6%8B%9F%E6%9C%BA&spm=1001.2101.3001.7020)(JVM)指定对象的类型并且运行该对象的方法。

思考以下例子：
```java
class Animal{
   public void move(){
      System.out.println("动物可以移动");
   }
}
 
class Dog extends Animal{
   public void move(){
      System.out.println("狗可以跑和走");
   }
   public void bark(){
      System.out.println("狗可以吠叫");
   }
}
 
public class TestDog{
   public static void main(String args[]){
      Animal a = new Animal(); // Animal 对象
      Animal b = new Dog(); // Dog 对象
 
      a.move();// 执行 Animal 类的方法
      b.move();//执行 Dog 类的方法
      b.bark();
   }
}
```
运行结果：
```java
TestDog.java:30: cannot find symbol
symbol  : method bark()
location: class Animal
                b.bark();
                 ^
```
该程序将抛出一个编译错误，因为b的引用类型Animal没有bark方法。

### 重写说明  
- 参数列表必须完全与被重写方法的相同。
- 返回类型与被重写方法的返回类型可以不相同，但是必须是父类返回值的派生类（java5 及更早版本返回类型要一样，java7 及更高版本可以不同）。
- 访问权限不能比父类中被重写的方法的访问权限更低。例如：如果父类的一个方法被声明为 public，那么在子类中重写该方法就不能声明为 protected。
- 如果不能继承一个方法，则不能重写这个方法。
- 父类的成员方法只能被它的子类重写。
	- 声明为 final 的方法不能被重写。
	- 声明为 static 的方法不能被重写，但是能够被再次声明。
	- 子类和父类在同一个包中，那么子类可以重写父类所有方法，除了声明为 private 和 final 的方法。
	- 子类和父类不在同一个包中，那么子类只能够重写父类的声明为 public 和 protected 的非 final 方法。
- 重写的方法能够抛出任何非强制异常，无论被重写的方法是否抛出异常。但是，重写的方法不能抛出新的检查型异常，或者比被重写方法声明的更广泛的检查型异常，反之则可以。
- 构造方法不能被重写。

# Super关键字调用父类被重写方法
当在子类中调用父类的被重写方法时，需要使用 `super` 关键字：
```cpp

class Animal{
   public void move(){
      System.out.println("动物可以移动");
   }
}
 
class Dog extends Animal{
   public void move(){
      super.move(); // 应用super类的方法
      System.out.println("狗可以跑和走");
   }
}
 
public class TestDog{
   public static void main(String args[]){
 
      Animal b = new Dog(); // Dog 对象
      b.move(); //执行 Dog类的方法
 
   }
}
运行结果：

动物可以移动
狗可以跑和走 
```