@[toc]

# 7-1 简单题 (5 分)

这次真的没骗你 —— 这道超级简单的题目没有任何输入。

你只需要在一行中输出事实：`This is a simple problem.` 就可以了。

**输入样例：** 
无

**输出样例：** 
```cpp
This is a simple problem.
```


代码如下：
```cpp
#include <bits/stdc++.h>
int main() {
    printf("This is a simple problem.");
    return 0;
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210527214913444.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)


# 7-2 A乘以B (5 分)

看我没骗你吧 —— 这是一道你可以在10秒内完成的题：给定两个绝对值不超过100的整数A和B，输出A乘以B的值。

**输入格式：**
输入在第一行给出两个整数A和B（−100≤A,B≤100），数字间以空格分隔。

**输出格式：**
在一行中输出A乘以B的值。

**输入样例：** 
```cpp
-8 13
```

**输出样例：**

```cpp
-104
```
代码如下：
```cpp
#include <bits/stdc++.h>
int main() {
    int a, b;
    scanf("%d%d", &a, &b);
    printf("%d\n", a * b);
    return 0;
}
```


![在这里插入图片描述](https://img-blog.csdnimg.cn/20210527214857296.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

# 7-3 心理阴影面积 (5 分)

![xlyy.JPG](https://img-blog.csdnimg.cn/20210527215236688.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)


这是一幅心理阴影面积图。我们都以为自己可以匀速前进（图中蓝色直线），而拖延症晚期的我们往往执行的是最后时刻的疯狂赶工（图中的红色折线）。由红、蓝线围出的面积，就是我们在做作业时的心理阴影面积。

现给出红色拐点的坐标 (x,y)，要求你算出这个心理阴影面积。

**输入格式：**
输入在一行中给出 2 个不超过 100 的正整数 x 和 y，并且保证有 x>y。这里假设横、纵坐标的最大值（即截止日和最终完成度）都是 100。

**输出格式：** 
在一行中输出心理阴影面积。

友情提醒：三角形的面积 = 底边长 x 高 / 2；矩形面积 = 底边长 x 高。嫑想得太复杂，这是一道 5 分考减法的题……

**输入样例：**

```cpp
90 10
```

**输出样例：**

```cpp
4000
```
代码如下：
```cpp
#include <bits/stdc++.h>
int main() {
    int x, y;
    scanf("%d%d", &x, &y);
    int tri = 100 * 100 / 2, rect = x * y / 2, trapezoid = (y + 100) * (100 - x) / 2;
    printf("%d\n", tri - rect - trapezoid);
    return 0;
}
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/20210527215221882.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

# 7-4 宇宙无敌大招呼 (5 分)

据说所有程序员学习的第一个程序都是在屏幕上输出一句“Hello World”，跟这个世界打个招呼。作为天梯赛中的程序员，你写的程序得高级一点，要能跟任意指定的星球打招呼。

**输入格式：**
输入在第一行给出一个星球的名字S，是一个由不超过7个英文字母组成的单词，以回车结束。

**输出格式：**
在一行中输出Hello S，跟输入的S星球打个招呼。

**输入样例：**

```cpp
Mars
```

**输出样例：**

```cpp
Hello Mars
```

代码如下：
```cpp
#include <bits/stdc++.h>
char s[8];
int main() {
    scanf("%s", s);
    printf("Hello %s", s);
    return 0;
}
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/20210527214835543.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)


# 7-5 新胖子公式 (10 分)

根据钱江晚报官方微博的报导，最新的肥胖计算方法为：体重(kg) / 身高(m) 的平方。如果超过 25，你就是胖子。于是本题就请你编写程序自动判断一个人到底算不算胖子。

**输入格式：** 
输入在一行中给出两个正数，依次为一个人的体重（以 kg 为单位）和身高（以 m 为单位），其间以空格分隔。其中体重不超过 1000 kg，身高不超过 3.0 m。

**输出格式：** 
首先输出将该人的体重和身高代入肥胖公式的计算结果，保留小数点后 1 位。如果这个数值大于 25，就在第二行输出 PANG，否则输出 Hai Xing。

**输入样例 1：**

```cpp
100.1 1.74
```

**输出样例 1：**

```cpp
33.1
PANG
```

**输入样例 2：**

```cpp
65 1.70
```

**输出样例 2：**

```cpp
22.5
Hai Xing
```

代码如下：
```cpp
#include <bits/stdc++.h>
using namespace std;

int main() {
    double w, h;
    scanf("%lf%lf", &w, &h);
    double t = w / (h * h);
    printf("%.1lf\n", t);
    printf(t > 25.0 ? "PANG" : "Hai Xing");
    return 0;
}
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/20210527214813426.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

