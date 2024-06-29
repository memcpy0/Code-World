[程序设计实习MOOC / 程序设计与算法（三）测验题汇总(2019夏季) ](http://cxsjsxmooc.openjudge.cn/2019t3summerall/)

@[toc]
### 001:简单一点的swap
在函数内部真正交换A类型，要定义A类型的引用。
```c
#include <iostream>
using namespace std;
class A
{
	public:
	int x;
	int getX() { return x; }	
};
void swap(A &a, A &b)
{
	int  tmp = a.x;
	a.x = b.x;
	b.x = tmp;
}
int main()
{
	A a,b;
	a.x = 3;
	b.x = 5;
	swap(a,b);
	cout << a.getX() << "," << b.getX();
	return 0;
}
```
### 002:难一点的swap
在函数内部真正交换指针，要定义整型指针的引用。
```c
#include <iostream>
using namespace std;

void swap(int *&a, int *&b)
{
	int * tmp = a;
	a = b;
	b = tmp;
}
int main()
{
	int a = 3,b = 5;
	int * pa = & a;
	int * pb = & b;
	swap(pa,pb);
	cout << *pa << "," << * pb;
	return 0;
}
```
### 003:好怪异的返回值
返回引用的函数，可以作为表达式的左值。
```c
#include <iostream>
using namespace std;

int &getElement(int * a, int i)
{
	return a[i];
}
int main()
{
	int a[] = {1,2,3};
	getElement(a,1) = 10;
	cout << a[1] ;
	return 0;
}
```
### 004:神秘的数组初始化
a数组是存储的类型是整型指针，因此a[2]是取下标为2的指针，*a[2]是向该指针指向的位置写入元素。为了在数组中得到一个整型指针，我们必须动态申请。
```c
#include <iostream>
using namespace std;

int main()
{
	int * a[] = {
NULL, NULL, new int, new int[6]
};
	
	*a[2] = 123;
	a[3][5] = 456;
	if(! a[0] ) {
		cout << * a[2] << "," << a[3][5];
	}
	return 0;
}
```
### 005:编程填空：学生信息处理程序
005:编程填空：学生信息处理程序
描述

    实现一个学生信息处理程序，计算一个学生的四年平均成绩。

    要求实现一个代表学生的类，并且类中所有成员变量都是【私有的】。

    补充下列程序中的 Student 类以实现上述功能。

    #include <iostream>
    #include <string>
    #include <cstdio>
    #include <cstring>
    #include <sstream>
    #include <cstdlib>
    using namespace std;

    class Student {

// 在此处补充你的代码

    };

    int main() {
    	Student student;        // 定义类的对象
    	student.input();        // 输入数据
    	student.calculate();    // 计算平均成绩
    	student.output();       // 输出数据
    }

输入
    输入数据为一行，包括：
    姓名,年龄,学号,第一学年平均成绩,第二学年平均成绩,第三学年平均成绩,第四学年平均成绩。
    其中姓名为由字母和空格组成的字符串（输入保证姓名不超过20个字符，并且空格不会出现在字符串两端），年龄、学号和学年平均成绩均为非负整数。信息之间用逗号隔开。
输出
    输出一行数据，包括：
    姓名,年龄,学号,四年平均成绩。
    信息之间用逗号隔开。
样例输入

    Tom Hanks,18,7817,80,80,90,70

样例输出

    Tom Hanks,18,7817,80

提示
    必须用类实现，其中所有成员变量都是私有的。
    输出结果中，四年平均成绩不一定为整数。 
