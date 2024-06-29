@[toc]

在[Windows10下VS Code配置Graphviz和DOT语言环境]()这篇文章中，我们配置了Graphviz的语言学习环境。现在开始学习DOT这个纯文本图像描述语言的常用语法。

---
# 1. 无向图 
无向图的格式如下：
```cpp
graph 名字 {
    无向图的内容
}
```
无向图使用 `a--b` 的形式来描述边，Graphviz会自动调整每个节点的位置。简单示例如下：
```cpp
graph example {
    a--b;
    a--c;
    b--c;
}
```
预览效果如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/8d65519c78e348ea911c0e7f24b69497.png)
如果边数太多，可以使用 `a -- {b; c; d; e}` 的方式来描述多条边，比如：
```cpp
graph example {
    a -- {b; c; d; e; f; g; h};
}
```
预览效果如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/e4264c0251f1454588097022426ba097.png)
如果还需要不同的形状和颜色，可以参考以下示例：
```clike
graph example {
    a -- b[color = red; label = "岛风"];
    a -- c[label = "恶毒"];
    b -- {d; e}[label = "z23"];
    b[shape = box];
}
```
预览效果如下： 
![在这里插入图片描述](https://img-blog.csdnimg.cn/3810a93f12174d99b363153d2f44d849.png)

即边和点的后面，可以使用 `[color=xxx]` 表示颜色，使用 `[label="xxx"]` 表示边权，`[shape=xxx]` 表示形状。常用的形状（点）有：`box, oval, circle, point, egg, triangle, square, record` 等，更多细节可以参考Graphviz的[官方文档](http://www.graphviz.org/documentation/)。

如果需要同时设置所有的边或点，可以使用 `edge[xxx]` 和 `node[xxx]` 。一个简单示例如下：
```cpp
graph example {
    edge[color = red];
    node[shape = box];

    a -- b;
    b -- c;
}
```
预览效果如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/d36e83d0b3d34b029844c31ec48f2977.png)


---
# 2. 有向图

有向图的格式和无向图基本一样，只是边使用 `a -> b` 来描述：
```cpp
digraph 名字（随意）{
    图
}
```
简单示例如下：
```clike
digraph example {
    node[shape = circle];
    root[shape = box];
    root -> {b; e};
    b -> {c; d};
}
```
预览效果如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2504fccfde5744358baac71804f4ff52.png)

---
# 3. 进阶
## 3.1 指定边射出的方向
使用具体示例来说明：
```cpp
digraph example{
    a0:w -> b0; //其中w意思是west，支持w,e,n,s,sw,se,nw,ne
    a1:e -> b1;
    a2:n -> b2;
    a3:s -> b3;
    a4:nw -> b4;
    a5:ne -> b5;
    a6:sw -> b6;
    a7:se -> b7;
}
```
预览效果如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/02ca50734ac447d2a94db82436688693.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
## 3.2 修改图片的朝向
把上面的例子做些修改，添加 `rankdir = LR;` ，将图片横置：
```clike
digraph example {
    rankdir= LR;
    a0:w -> b0; //其中w意思是west，支持w,e,n,s,sw,se,nw,ne
    a1:e -> b1;
    a2:n -> b2;
    a3:s -> b3;
    a4:nw -> b4;
    a5:ne -> b5;
    a6:sw -> b6;
    a7:se -> b7;
}
```
预览效果如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/b29518fee5084d77ba2c4de52f6a3cd3.png)

## 3.3 绘制类似结构体的图形
下面以一个链表示例来解释：
```clike
digraph example {
    rankdir = LR;
    node[shape = record]; //record形状
    a[label = "{A|}"]; //每个'|'都是一列
    b[label = "{B|}"];
    c[label = "{C|}"];
    d[label = "{D|NULL}"];
    a -> b:w;
    b -> c:w;
    c -> d:w;
}
```
预览效果如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/af9885406db8483080eacc19950546ce.png)
再通过几个示例，来学习 `record` ：
```clike
digraph example {
    a[shape = record; label = "A | B | C | D"];
}
```
预览效果如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/a8b1d6cbb28e435792dc23dc79329ed3.png)

```cpp
digraph example {
    rankdir = LR;
    a[shape = record; label = "A | B | C | D"];
}
```
预览效果如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/5ea41df353c94c0b8ddf0a8c4b613312.png)



---
# 4. 其他
Graphviz还有很多功能，比如更换不同的排版引擎等，以后有时间再写一下。学习Graphviz的同时，还可以学一下PlantUML——基于Graphviz和Java，用于快速绘制UML图的特化工具，其环境配置见[这篇文章](https://memcpy0.blog.csdn.net/article/details/120401758)。
 

