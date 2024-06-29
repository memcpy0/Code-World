重载是在一个类里面定义多个，方法名字相同，参数数量、类型或顺序不同的方法，返回类型可同可不同。**每个重载的方法（或者构造函数）都必须有一个独一无二的参数类型列表**。

> 为什么重载？
> 最常用的构造器的重载。方便使用。方法重载是一个类的多态性表现

### 1. 白板实现
```java
public class Overloading {
	public int test() {
		System.out.println("test");
	}

	public void test(int a) {
		System.out.println("test" + a);
	}

	
    //以下两个参数类型顺序不同
    public String test(int a,String s){
        System.out.println("test3");
        return "returntest3";
    }   
 
    public String test(String s,int a){
        System.out.println("test4");
        return "returntest4";
    }   
 
    public static void main(String[] args){
        Overloading o = new Overloading();
        System.out.println(o.test());
        o.test(1);
        System.out.println(o.test(1,"test3"));
        System.out.println(o.test("test4",1));
    }
}
```
### 2. 重载规则
-   被重载的方法必须改变参数列表(参数个数或类型不一样)；
-   被重载的方法可以改变返回类型；
-   被重载的方法可以改变访问修饰符；
-   被重载的方法**可以声明新的或更广的检查异常**；
-   方法能够在同一个类中或者在一个子类中被重载。
-   无法以返回值类型作为重载函数的区分标准。

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304101544939.png)
