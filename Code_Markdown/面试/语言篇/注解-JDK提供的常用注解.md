面试高频指数：⭐️⭐️⭐️

要想知道注解怎么用，一般来说，点开源代码看看就清楚了。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304091014119.png)

@Override 标记重写
@Override 来自 java.lang 包，源代码如下：
```java
@Target(ElementType.METHOD)
@Retention(RetentionPolicy.SOURCE)
public @interface Override {
}
```
（@Target 与 @Retention 是修饰注解的注解，也叫元注解，元注解在第 3 节介绍。 ）

注解 Override 被另外两个注解修饰，它们是修饰注解的注解，通常叫它们「元注解」，「元注解」我们下一部分讲。

我们看到了两个词 METHOD 和 SOURCE。再结合一些资料可以知道： METHOD 表示 @Override 注解用于修饰方法，并且 SOURCE 表示 @Override 只保留在源代码阶段，编译以后，在字节码文件里是没有 @Override 注解的。

读者可以使用 IDEA 提供的反编译工具，或者 Java 的反编译命令 javap 把 .class 文件打开看看。

@Deprecated 标记过时
被 @Deprecated 标记的「目标」，表示「过时」「不推荐使用」的意思。java.util.Date 里面就有大量被标注了 @Deprecated 的方法。



IDEA 工具会把这些被 @Deprecated 标注的方法画上横线，告诉我们这些方法不推荐使用。如果我们用到了这个方法，这个时候应该去看看源代码，一般都会有注释（解释被弃用的原因）和推荐使用的新的方法（或类等其它目标）。

点开源码看一下：

```java
@Documented
@Retention(RetentionPolicy.RUNTIME)
@Target(value={CONSTRUCTOR, FIELD, LOCAL_VARIABLE, METHOD, PACKAGE, MODULE, PARAMETER, TYPE})
public @interface Deprecated {
    
    String since() default "";

    boolean forRemoval() default false;
}
```
我们可以知道 Deprecated 可以修饰在构造函数 CONSTRUCTOR 、属性 FIELD、局部变量 LOCAL_VARIABLE、方法 METHOD、包 PACKAGE、模块 MODULE（JDK9 推出的新特性）, PARAMETER（参数）、 TYPE （类）；
@Retention(RetentionPolicy.RUNTIME) 表示注解 @Deprecated 保留到程序运行的时候；
注解的属性就是注解携带的额外信息，实现个性化配置。注解里面可以定义一些附加信息，就是这里的 since() 和 forRemoval() （它们是 JDK9 的时候出现的）。注意：虽然它们带上了括号，但实际上应该把 since() 看成属性 since ，而不是方法。同理 forRemoval() 表示 Deprecated 可以携带一个属性 forRemoval。这两个属性后面都有一个 default ， default 后面跟上的是默认值。这里读者朋友们应该能够猜出来它们如何使用，我们就不再赘述了。
@SuppressWarnings 抑制编译警告
这个注解使用得很少。就像有的人有强迫症，看到自己的微信图标上面的消息数量，想把它点掉一样。除了让自己看得顺眼以外，没有实质上的作用。

@FunctionalInterface 声明函数式接口
注解 @FunctionalInterface 标注接口上，表示这是一个「函数式接口」（「函数式接口」以后我们再深入了解）。枚举类 ElementType 里没有单独表示接口的值（对象）。

JDK8 开始出现的注解，源代码：


@Documented
@Retention(RetentionPolicy.RUNTIME)
@Target(ElementType.TYPE)
public @interface FunctionalInterface {}
标注了@FunctionalInterface 的接口只能包含一个未实现的方法，该接口可以有多个默认方法，也可以有多个静态方法。

参考资料
https://docs.oracle.com/javase/tutorial/java/annotations/index.html

作者：LeetCode
链接：https://leetcode.cn/leetbook/read/java-interview-breakthrough/7u0czv/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。