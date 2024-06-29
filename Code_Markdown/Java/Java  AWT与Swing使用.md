@[toc]
### 一. 概述
AWT，抽象窗口工具包，是用于GUI编程的Java包，基于本地化的风格，是Java的早期选择，但是组件种类有限，只是基本的GUI设计工具。Swing是对AWT的改进，它**不仅包括AWT中具有的所有组件，还有更加丰富的组件**，足以实现GUI设计的一切功能。

Frame类是一个框架窗口类。
```java
import java.awt.Frame;
import java.awt.Rectangle;

public class AwtTest {
    public static void main(String[] args) {
    	Frame frame = new Frame();
    	frame.setTitle("第一个窗口程序");
    	// 窗口位置 x=150, y=200, 宽640, 高480
    	Rectangle r = new Rectangle(150, 200, 640, 480);
    	frame.setBounds(r);
    	frame.setVisible(true); 
    }
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191118202752204.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
Java.awt为各种组件提供了对应的Java类，它们都是java.awt.Component的直接和间接子类。这些类可以分为基本组件和容器，容器容纳其他组件。所有的容器都是java.awt.Container类的直接或间接子类。而Container又是Component的子类。

### 二、容器
容器类Container是Component的子类，是一种特殊的组件，功能是包括其他组件。特点：
- 有一定的范围，一般容器是矩形的；
- 有一定的位置；
- 有一个背景，填充整个容器；
- 可以包含其他组件，或者被包含；
- 可以按照一定规则排列它包含的其他组件；

 

