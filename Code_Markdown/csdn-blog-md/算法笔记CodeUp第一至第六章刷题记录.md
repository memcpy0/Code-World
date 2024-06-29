
@[toc]

---
## 《算法笔记》2.2小节——C/C++快速入门->顺序结构
http://codeup.cn/contest.php?cid=100000566
### 1.例题1-1-1 按要求输出信息(1) 

```c
#include <stdio.h>

int main()
{
    printf("%s", "This is my first c program!");
    return 0;
}
```
### 2.例题1-1-2 按要求输出信息(2)
```c
#include <stdio.h>

int main() 
{
    char *asterisk = "********************";
    char *s = "Very Good!";
    printf("%s\n%s\n%s", asterisk, s, asterisk);
    return 0;
}
```
### 3.例题1-2-1 求两个整数之和(1)
```c
int main()
{
    /* 设置3个变量a, b, sum，其中a, b用来存放两个整数，sum用来存放a, b两个数的和，
    通过赋值（即采用赋值运算符"="）的方式将a初始化为123，b初始化为456，并把两个
    变量相加的结果赋值给sum。*/
    int a, b, sum;
    a = 123, b = 456;
    sum = a + b;
    printf("sum=%d", sum);
    return 0;
}
```
### 4.例题1-2-2 求两整数数之和(2)
```c
#include <stdio.h>

int main()
{
    int a, b;
    scanf("%d%d", &a, &b);
    printf("%d", a+b);
    return 0;
}
```
### 5.例题3-5 求一元二次方程的根
```c
#include <stdio.h>
#include <math.h>
int main()
{ 
    /*
    求一元二次方程ax2+bx+c=0的根，三个系数a, b, c由键盘输入，且a不能为0，且保证b2-4ac>0。
    程序中所涉及的变量均为double类型。
    输入 以空格分隔的一元二次方程的三个系数，双精度double类型
    输出 分行输出两个根如下（注意末尾的换行）：
        r1=第一个根
        r2=第二个根
    结果输出时，宽度占7位，其中小数部分2位。*/
    double a, b, c;
    scanf("%lf%lf%lf", &a, &b, &c);
    double r1, r2;
    // 求根公式 x=[-b±√(b2-4ac)]/2a 
    r1 = (double)((-b + sqrt(pow(b, 2.0) - 4*a*c)) / (2 * a));
    r2 = (double)((-b - sqrt(pow(b, 2.0) - 4*a*c)) / (2 * a));
    printf("r1=%7.2f\nr2=%7.2f", r1, r2);
}
```
### 6.例题3-9 字符输入输出
```c
#include <stdio.h>
int main()
{
    /* 从键盘输入三个字符BOY，然后把他们输出到屏幕上
    输入 BOY三个字符，中间无分隔符
    输出 BOY，注意末尾的换行 */
    char s[10];
    scanf("%s", s);
    printf("%s", s);
    return 0;
}
```
## 《算法笔记》2.3小节——C/C++快速入门->选择结构
http://codeup.cn/contest.php?cid=100000567
### 1.例题4-1 一元二次方程求根
```c
#include <stdio.h>
#include <math.h>
int main()
{ 
    /* 不保证b2-4ac>0, 如果方程无实根，输出一行如下信息（注意末尾的换行）
    No real roots! */
    double a, b, c, cond, r1, r2;
    scanf("%lf%lf%lf", &a, &b, &c);
    cond = pow(b, 2.0) - 4*a*c;
    if (cond >= 0) {
        r1 = (double)(-b + sqrt(cond));
        r2 = (double)(-b - sqrt(cond));
        printf("r1=%7.2f\nr2=%7.2f", r1, r2);
    } else {
        printf("No real roots!\n")
    }
    return 0;
} 
```
###  2.例题4-2 比较交换实数值
```c
#include <stdio.h>
int main()
{
    /* 
    输入 用空格分隔的两个实数。
    输出 从小到大输出这两个实数，中间以空格来分隔，
    小数在前，大数在后。小数点后保留2位小数。末尾输出换行符。 */
    double a, b;
    scanf("%lf%lf", &a, &b);
    if (a > b) {
        printf("%.2f %.2f", b, a);
    } else {
        printf("%.2f %.2f", a, b);
    }
    return 0;
}
```
### 3.例题4-3 比较交换3个实数值，并按序输出
```c
#include <stdio.h>
void swap(double *a, double *b) {
    double temp;
    temp = *a;
    *a = *b;
    *b = temp;
}
int main()
{
    /*
    输入 输入以空格分隔的三个实数
    输出 按照从小到大的顺序输出这三个实数，中间以空格分隔，最小值在前，
    最大值在后。小数点后保留2位小数。末尾输出换行。
    */
    double a, b, c;
    scanf("%lf%lf%lf", &a, &b, &c);
    if (a > b) swap(&a, &b);
    if (b > c) swap(&b, &c);
    if (a > b) swap(&a, &b);
    printf("%.2f %.2f %.2f\n", a, b, c);
    return 0;
}
```
### 4.习题4-4 三个整数求最大值
```c
#include <stdio.h>

int main()
{
    /* 输入 以空格分割的三个整数。
    输出 三个数中的最大值，末尾换行。*/
    int a, b, c;
    scanf("%d%d%d", &a, &b, &c);
    if (a >= b && a >= c) printf("%d\n", a);
    else if (b >= a && b >= c) printf("%d\n", b);
    else if (c >= a && c >= b) printf("%d\n", c);
    return 0;
}
```
### 5.习题4-10-1 奖金计算
```c
#include <stdio.h>

int main()
{
    double income, bonus;
    scanf("%lf", &income);
    /* 某企业发放的奖金根据利润提成。利润I低于或等于100000时，奖金可提10%；
    利润（100000<I<=200000）时，低于100000元的部分仍按10%提成，高于100000元的部分提成比例为7.5%；
    200000<I<=400000时，低于200000元的部分仍按上述方法提成（下同），高于200000元的部分按5%提成；
    400000<I<=600000元时，高于400000元的部分按3%提成；
    600000<I<=1000000时，高于600000元的部分按1.5%提成；
    I>1000000元时，超过1000000元的部分按1%提成。
    输入 企业利润，小数，双精度double类型
    输出 应发奖金数，保留2位小数，末尾换行。*/
    double b1, b2, b3, b4, b5;
    b1 = (double)(100000 * 10) / 100;
    b2 = b1 + (double)(100000 * 7.5) / 100;
    b3 = b2 + (double)(200000 * 5) / 100;
    b4 = b3 + (double)(200000 * 3) / 100;
    b5 = b4 + (double)(400000 * 1.5) / 100;
    if (income <= 100000) {
        bonus = (double)(income * 10) / 100;
    } else if (100000 < income && income <= 200000) {
        bonus = b1 + ((income - 100000) * 7.5) / 100;
    } else if (200000 < income && income <= 400000) {
        bonus = b2 + ((income - 200000) * 5.0) / 100;
    } else if (400000 < income && income <= 600000) {
        bonus = b3 + ((income - 400000) * 3.0) / 100;
    } else if (600000 < income && income <= 1000000) {
        bonus = b4 + ((income - 600000) * 1.5) / 100;
    } else {
        bonus = b5 + ((income - 1000000) * 1.0) / 100;
    }
    printf("%.2f\n", bonus);
    return 0;
}
```
## 《算法笔记》2.4小节——C/C++快速入门->循环结构
http://codeup.cn/contest.php?cid=100000567
### 问题 A: 例题5-1-1 连续自然数求和
```c
#include <stdio.h>
int main() 
{
    int i=1, sum=0;
    while (i <= 100) {
        sum += i;
        i++;
    }
    printf("%d\n", sum);
    return 0;
}
/*---------------------------------------------------*/
#include <stdio.h>
int main() 
{
    printf("%d\n", 100 * (100 + 1) / 2);
    return 0;
}
```
### 问题 B: 例题5-1-2 连续自然数求和
```c
#include <stdio.h>
int main() 
{
    int i = 0, sum = 0;
    do {
        sum += i;
        i++;
    } while (i <= 100);
    printf("%d\n", sum);
    return 0;
}
```
### 问题 C: 例题5-1-3 连续自然数求和
```c
#include <cstdio>
int main() 
{
    int sum = 0;
    for (int i = 0; i < 100; i++, sum+=i);
    printf("%d\n", sum);
    return 0;
}
```
### 问题 D: 例题5-1-4 连续自然数求和
```c
#include <cstdio>
int main() 
{
    int n;
    scanf("%d", &n);
    printf("%d\n", n * (n + 1) / 2);
    return 0;
}
```
### 问题 E: 例题5-1-5 连续自然数求和
```c
#include <cstdio>
int main() {
    /* 求1+2+3+...和的程序，要求得到使和数大于1000的最小正整数N。*/
    int i, sum = 0;
    for (i = 1; sum <= 1000; i++, sum += i);
    printf("%d\n", i);
    return 0;
}

```
### 问题 F: 例题5-6 矩阵输出
```c
#include <cstdio>
int main() {
    for (int i=1; i <= 4; i++) {
    	for (int j=1; j <= 5; j++) {
    		printf("%3d", i*j);
	        if (i * j % 5 == 0) 
	            printf("\n");	
		} 
    }
    return 0;
}
```
### 问题 G: 例题5-7 求圆周率pi的近似值
```c
#include <cstdio>
/* 用如下公式 pi/4 = 1-1/3+1/5-1/7....求圆周率PI的近似值，直到发现某一项的绝对值
 小于10-6为止（该项不累加）。
 如果需要计算绝对值，可以使用C语言数学库提供的函数fabs，如求x的绝对值，则为fabs(x).
 输出 PI=圆周率的近似值 输出的结果总宽度占10位，其中小数部分为8位。末尾输出换行。 */
int main() {
    double pi = 0.0;
    for (double term = 1, i = 1, j = 3; fabs(term) >= 1e-6; j+=2) {    
		pi += term;
		i = -i;
        term = i / j;
    }
    printf("PI=%10.8f\n", pi * 4);
    return 0;
} 
```
### 问题 H: 例题5-8 Fibonacci数列
```c
#include <cstdio>
/*输入 一个不超过50的正整数
  输出 Fibonacci数列的第n个数，末尾输出换行。*/
int fibo(int n) {
    if (n == 1 || n == 2) return 1;
    else return fibo(n-1) + fibo(n-2);
}
int main() {
    int N;
    scanf("%d", &N);
    printf("%d\n", fibo(N));
    return 0;
}
```
###  问题 I: 习题5-10 分数序列求和
```c
#include <cstdio>
/* 有一个分数序列:2/1 , 3/2 , 5/3 , 8/5 , 13/8 , 21/13 ...
求出这个数列的前20项之和. 输出 小数点后保留6位小数，末尾输出换行。*/
double top(int n) {
    if (n == 1) return 2;
    else if (n == 2) return 3;
    else return top(n-1) + top(n-2);
}
double bottom(int n) {
    if (n == 1) return 1;
    else if (n == 2) return 2;
    else return bottom(n-1) + bottom(n-2);
}
int main() {
    double sum = 0.0;
    for (int i = 1; i <= 20; i++) 
        sum += (double)(top(i) / bottom(i));
    printf("%.6f\n", sum);
    return 0;
}
```
## 《算法笔记》2.5小节——C/C++快速入门->数组
### 问题 A: 习题6-4 有序插入
```c
#include <cstdio>
/* 第一行输入以空格分隔的9个整数数，要求按从小到大的顺序输入。
第二行输入一个整数, 将此整数插入到前有序的9个数中，使得最终的10个数
依然是从小到大有序的。
输出 从小到大输出这10个数，每个数一行。*/
int main()
{
    int a[10], temp;
    for (int i = 0; i < 9; i++) 
        scanf("%d", &a[i]);
    scanf("%d", &temp);
    /* 类似插入排序 */
    int i;
    for (i = 9; a[i-1] > temp && i >= 0; i--)
        a[i] = a[i-1];
    a[i] = temp;
    for (int i=0; i < 10; i++) 
        printf("%d\n", a[i]);
    return 0;
}
```
### 问题 B: 习题6-5 数组元素逆置
```c
#include <cstdio>
/*  将一个长度为10的整型数组中的值按逆序重新存放。
输入 从键盘上输入以空格分隔的10个整数。
输出 按相反的顺序输出这10个数，每个数占一行。*/
int main()
{
    int a[10];
    for (int i=0; i<10; i++) {
        scanf("%d", &a[i]);
    }
    for (int i=0; i<5; i++) {
        int temp = a[9-i];
        a[9-i] = a[i];
        a[i] = temp;
    }
    for (int i=0; i<10; i++) {
        printf("%d\n", a[i]);
    }
    return 0;
}
```
### ★ 问题 C: 习题6-6 杨辉三角
```c
#include <cstdio>
#include <cstring>
/* 
输入 输入只包含一个正整数n，表示将要输出的杨辉三角的层数。
输出 对应于该输入，请输出1-相应层数的杨辉三角，每一层的整数之间用一个空格隔开, 最多输出10层 */
int main()
{
    int n, a[10] = {1, 1};
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) {
        /* 打印第1、2层 */
        if (i == 1) printf("%d\n", a[0]);
        else if (i == 2) printf("%d %d\n", a[0], a[1]);
        else { /* 打印其他层 */
            int temp[10]; 
            temp[0] = a[0];
            int j; 
            /* 从a的现在一层推导出下一层, 存入临时数组 */
            for (j = 1; j <= i-2; j++) {
                temp[j] = a[j-1] + a[j];
            }
            temp[j] = 1;
            /* 打印这一层 */
            for (int k=0; k < j; k++) {
                printf("%d ", temp[k]);
            }
            printf("%d\n", temp[j]);
            /* 更新数组a */
            for (int m=0; m < i; m++) {
    	        a[m] = temp[m];
      		}
    	}
	} 
	return 0;	
}
```
### 问题 D: 习题6-12 解密
```c
#include <cstdio>
#include <cstring>
/* 第一个字母变成第26个字母，第i个字母变成第（26-i+1)个字母，非字母字符不变。
   要求根据密码译回原文，并输出。
   输入 输入一行密文; 输出 解密后的原文，单独占一行。*/
int main()
{
    char s[100], t[100];
    gets(s);
    for (int i=0; i < strlen(s); i++) {
        if (s[i] >= 65 && s[i] <= 90) {
            s[i] = 155 - s[i];  //'A'65 Z'90'
        } else if (s[i] >= 97 && s[i] <= 122) {
            s[i] = 219 - s[i]; // 'a'97 'z'122
        } 
    }
    puts(s);
    return 0;
}
```
### 问题 E: 习题6-13 字符串比较
这个题目似乎只是对等长字符串进行比较，如果不等长的话......需要修改一下。
```c
#include <cstdio>
#include <cstring>
/* 比较两个字符串s1和s2的大小，如果s1>s2，则输出一个正数; 
若s1=s2，则输出0；若s1<s2，则输出一个负数。
要求：不用strcpy函数；两个字符串用gets函数读入。
输入 输入2行字符串
输出 一个整数，表示这两个字符串 比较的差值，单独占一行。*/
int main()
{
    char str1[100], str2[100];
    gets(str1);
    gets(str2);
    int res, lt = strlen(str1) < strlen(str2) ? strlen(str1) : strlen(str2);
    for (int i = 0; i < lt; i++) {
        res = str1[i] - str2[i];
        if (res) break;
    }
    printf("%d\n", res);
    return 0;
}
```
### 问题 F: 例题6-1 逆序输出数组元素
```c
#include <cstdio>
/* 输入 10个整数，以空格分隔
   输出 将输入的10个整数逆序输出，每个数占一行。 */
int main()
{
    int a[10];
    for (int i = 0; i < 10; i++) 
        scanf("%d", a + i);
    for (int i = 9; i >= 1; i--) 
        printf("%d\n", a[i]);
    printf("%d", a[0]);
    return 0;
}
```
### 问题 G: 例题6-2 数组求解Fibonacci数列问题
**这是最好的求解Fibonacci数列的方法。**
```c
#include <cstdio>
/* 输入 无
   输出 Fibonacci数列的前20个数，每个数占一行。 */
int main()
{
    int fib[20] = {1, 1};
    for (int i = 2; i < 20; i++) 
        fib[i] = fib[i-1] + fib[i-2];
    for (int i = 0; i < 20; i++) 
        printf("%d\n", fib[i]);
    return 0;
}
```
### 问题 H: 例题6-3 冒泡排序
```c
#include <cstdio>
int main()
{
    int a[10];
    for (int i = 0; i < 10; i++) 
        scanf("%d", a + i);
    /* 标记+冒泡排序 */
    for (int i = 9; i >= 0; i--) {
        int flag = 1;
        for (int j = 0; j < i; j++) {
            if (a[j] > a[j+1]) {
                int temp = a[j];
                a[j] = a[j+1];
                a[j+1] = temp;
                flag = 0;  
            }
        }
        if (flag) break;
    }
    for (int i = 0; i < 9; i++) 
        printf("%d\n", a[i]);
    printf("%d", a[9]);
    return 0;
}
```
### 问题 I: 例题6-4 矩阵转置
```c
#include <cstdio>
/* 输入 2行数据，每行3个整数，以空格分隔。
输出 行列互换后的矩阵，3行，每行2个数据，以空格分隔。*/
int main()
{
    int a[2][3], aT[3][2];
    for (int i = 0; i < 2; i++) 
        for (int j = 0; j < 3; j++)
            scanf("%d", *(a + i) + j);
    /* 转置操作 */
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 3; j++) {
            aT[j][i] = a[i][j];
        }
    }
    for (int i = 0; i < 2; i++) 
        printf("%d %d\n", aT[i][0], aT[i][1]);
    printf("%d %d", aT[2][0], aT[2][1]);
    return 0;
}
```
### 问题 J: 例题6-9 字符串求最大值
```c
#include <cstdio>
#include <cstring>
/* 输入 输入3行，每行均为一个字符串。
   输出 一行，输入三个字符串中最大者。*/
int main() 
{
    char s1[100], s2[100], s3[100], *Max;
    gets(s1);
    gets(s2);
    gets(s3);
    Max = s1;
    if (strcmp(Max, s2) < 0) Max = s2;
    if (strcmp(Max, s3) < 0) Max = s3;
    printf("%s", Max);
    return 0;
}
```
## 《算法笔记》2.6小节——C/C++快速入门->函数
### 问题 A: 习题7-5 字符串逆序存放
我这里使用的是原地修改， 懒得返回值了。
```c
#include <cstdio>
#include <cstring>
/* 输入 一行字符串。
   输出 输入字符串反序存放后的字符串。单独占一行。*/
void strReverse(char s[]) {
    int len = strlen(s);
    for (int i = 0; i < len / 2; i++) {
        char temp = s[i];
        s[i] = s[len-1-i];
        s[len-1-i] = temp; 
    }
}

int main()
{
    char s[100];
    gets(s);
    strReverse(s);
    printf("%s\n", s);
    return 0;
}
```
### 问题 B: 习题7-7 复制字符串中的元音字母
```c
/* 输入 一个字符串（一行字符）。
   输出 该字符串所有元音字母构成的字符串。行尾换行。*/
void vowels(char s1[], char s2[]) {
    int len = strlen(s1), j = 0;
    for (int i = 0; i < len; i++) {
        if (s1[i] == 'a' || s1[i] == 'e' || s1[i] == 'i' || s1[i] == 'o' || s1[i] == 'u') {
            s2[j] = s1[i];
            j++;
        }
    }
    s2[j] = '\0';
}
int main()
{
    char s1[100], s2[100];
    gets(s1);
    vowels(s1, s2);
    printf("%s\n", s2);
    return 0;
}
```
## 《算法笔记》2.7小节——C/C++快速入门->指针
### 问题 A: C语言10.1
```c
#include <cstdio>
/* 输入 两个用空格隔开的整数a和b。
输出 按先大后小的顺序输出a和b，用空格隔开。请注意行尾输出换行。*/
int main()
{
    int a, b;
    scanf("%d%d", &a, &b);
    if (a > b) printf("%d %d\n", a, b);
    else printf("%d %d\n", b, a);
    return 0;
}
```
### 问题 B: C语言10.2
```c
#include <cstdio>
/*输入 三个用空格隔开的整数a、b和c。
  输出 按先大后小的顺序输出a、b和c，用空格隔开。请注意行尾输出换行。*/
int main()
{
    int a[3];
    scanf("%d%d%d", &a[0], &a[1], &a[2]);
    for (int i=0; i < 2; i++) {
    	for (int *p=a; p < a + 2; p++) {
	        if (*p < *(p+1)) {
	            int temp = *p;
	            *p = *(p+1);
	            *(p+1) = temp; 
	        }
   	    }
	}
    printf("%d %d %d\n", *a, *(a+1), *(a+2));
    return 0;
}
```
### 问题 C: C语言10.10
```c
#include <cstdio>
/* 给定字符串定义char *a = “I love China!”，
读入整数n，输出在进行了a = a + n这个赋值操作以后字符指针a对应的字符串。
输入 一个整数n，保证0<=n<13.
输出 输出进行了题目描述中赋值操作之后a对应的字符串. 请注意行尾输出换行。*/
int main()
{
    char *a = "I love China!";
    int n;
    scanf("%d", &n);
    puts(a + n);
    return 0;
}

```
### 问题 D: C语言10.15
```c
#include <cstdio>
#include <cstring>
/* 输入3个字符串，按从小到大的顺序输出。要求使用指针的方法进行处理。
输入 3行，每行一个用字符串。保证每个字符串的长度不超过20。
输出 按从小到大的顺序输出这3个字符串，每个字符串一行. */
void swap(char s[], char a[]) {
    char t[30];
    strcpy(t, s);
    strcpy(s, a);
    strcpy(a, t);
}
int main()
{
    char a[30], b[30], c[30];
    gets(a);
    gets(b);
    gets(c);
    if (strcmp(a, b) > 0) 
        swap(a, b);
    if (strcmp(a, c) > 0) 
        swap(a, c);
    if (strcmp(b, c) > 0) 
        swap(b, c);
    puts(a);
    puts(b);
    puts(c);
    return 0;
}
```
### 问题 E: C语言10.16
```c
#include <cstdio>
/*输入10个整数，将其中最小的数与第一个数对换，把最大的数与最后一个数对换。要求
用3个函数实现，分别为输入10个数、进行处理、输出10个数。要求使用指针的方法进行处理。
输入 用空格隔开的10个整数。
输出 输出进行题目描述操作之后的10个整数，每个整数之后输出一个空格。*/
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}
void Input(int t[], int len) {
    for (int i = 0; i < len; i++)
        scanf("%d", t + i);
}
void Deal(int t[], int len) {
    int min , max, minK, maxK;
    min = max = t[0];
    for (int i = 1; i < len; i++) {
        if (t[i] > max) {
            max = t[i];
            maxK = i;
        }
        if (t[i] < min) {
            min = t[i];
            minK = i;
        }
    }
    swap(t, t + minK);
    swap(t + 9, t + maxK);
}
void Print(int t[], int len) {
    for (int i = 0; i < len-1; i++) 
        printf("%d ", t[i]);
    printf("%d", t[9]);
}
int main()
{
    int a[11];
    Input(a, 10);
    Deal(a, 10);
    Print(a, 10);
    return 0;
}
```
## 《算法笔记》2.8小节——C/C++快速入门->指针
### 问题 A: C语言11.1
```c
#include <cstdio>
#include <cstring>
/* 输入第一行有一个整数n，表示以下有n张选票信息将会输入。保证n不大于100。
以后的n行中，每一行包含一个人名，为选票的得票人。保证每一个人名都是Li，
Zhang和Fun中的某一个。
输出
有三行，分别为Li，Zhang和Fun每人的得票数。格式为首先输出人名，其后输出一个冒号，
最后输出候选人的得票数。注意行尾输出换行。*/
struct person {
    char name[20];
    int count;
} leader[3] = {"Li", 0, "Zhang", 0, "Fun", 0};
int main()
{
    int N;
    scanf("%d", &N);
    for (int i = 0; i <= N; i++) {
        char s[10];
        gets(s);
        if (!strcmp(s, leader[0].name)) leader[0].count++;
        else if (!strcmp(s, leader[1].name)) leader[1].count++;
        else if (!strcmp(s, leader[2].name)) leader[2].count++;
    }
    
    for (int i = 0; i < 3; i++) 
        printf("%s:%d\n", leader[i].name, leader[i].count);
    return 0;
}
```
### 问题 B: C语言11.2
```c
#include <cstdio>
#include <cstring>
/*  输入 第一行有一个整数n，表示以下有n个学生的信息将会输入。保证n不大于20。
以后的n行中，每一行包含对应学生的学号、名字、性别和年龄，用空格隔开。保证每一个人名
都不包含空格且长度不超过15，性别用M和F两个字符来表示。
    输出 有n行，每行输出一个学生的学号、名字、性别和年龄，用空格隔开。*/
struct student {
    int num;
    char name[20];
    char sex;
    int age;
};
int main()
{
    int n;
    scanf("%d", &n);
    int i;
    struct student stus[n], *ptrToStus[n];
    for (i = 0; i < n; i++) {
        scanf("%d %s %c %d", &stus[i].num, stus[i].name, &stus[i].sex, &stus[i].age);
        ptrToStus[i] = &stus[i];
    }
    for (i = 0; i < n; i++) {
        printf("%d %s %c %d\n", ptrToStus[i]->num, ptrToStus[i]->name, ptrToStus[i]->sex, ptrToStus[i]->age);
    }
    return 0;
}
```
### 问题 C: C语言11.4
好久没有用共用体了，而且就隔了两天，这么简单的东西都会写错，汗颜。
```c
#include <cstdio>
struct Job{
    int num;
    char name[10];
    char sex;
    char job;
    union {
        int Class;
        char position[10];
    } category;
};
int main()
{
    int n;
    scanf("%d", &n);
    Job info[n];
    for (int i = 0; i < n; i++) {
        scanf("%d %s %c %c", &info[i].num, info[i].name, &info[i].sex, &info[i].job);
        if (info[i].job == 's') scanf("%d", &info[i].category.Class);
        else if (info[i].job == 't') scanf("%s", info[i].category.position);
    }
    for (int i = 0; i < n; i++) {
        printf("%d %s %c %c ", info[i].num, info[i].name, info[i].sex, info[i].job);
        if (info[i].job == 's') printf("%d\n", info[i].category.Class);
        else if (info[i].job == 't') printf("%s\n", info[i].category.position);
    }
    return 0;
}

```
### 问题 D: C语言11.7
```c
#include <cstdio>
typedef struct student {
    int num;
    char name[25];
    int score1;
    int score2;
    int score3;
} student;
void input(student stds[]) {
    for (int i = 0; i < 5; i++) 
        scanf("%d %s %d %d %d", &stds[i].num, stds[i].name, &stds[i].score1, &stds[i].score2, &stds[i].score3);
}
void print(student *stds) {
    for (int i = 0; i < 5; i++) 
        printf("%d %s %d %d %d\n", stds[i].num, stds[i].name, stds[i].score1, stds[i].score2, stds[i].score3);
}

int main()
{
    struct student stds[5];
    input(stds);
    print(stds);
    return 0;
}
```
### 问题 E: C语言11.8
```c
#include <cstdio>
typedef struct student {
    int num;
    char name[25];
    int score1;
    int score2;
    int score3;
} student;

void input(student stds[]) {
    for (int i = 0; i < 10; i++) 
        scanf("%d %s %d %d %d", &stds[i].num, stds[i].name, &stds[i].score1, &stds[i].score2, &stds[i].score3);
}
void print(student *stds) {
    double sum1 = 0, sum2 = 0, sum3 = 0, max_aver = (stds[0].score1 + stds[0].score2 + stds[0].score3) / 3.0, N = 10.0;
    int k = 0;
    for (int i = 0; i < 10; i++) {
        sum1 += stds[i].score1;
        sum2 += stds[i].score2;
        sum3 += stds[i].score3;
        double now_max = (stds[i].score1 + stds[i].score2 + stds[i].score3) / 3.0;
        if ( now_max > max_aver) {
            k = i;
            max_aver = now_max;
        }
    }
    printf("%.2f %.2f %.2f \n", sum1 / N, sum2 / N, sum3 / N);
    printf("%d %s %d %d %d\n", stds[k].num, stds[k].name, stds[k].score1, stds[k].score2, stds[k].score3);
}

int main()
{
    struct student stds[10];
    input(stds);
    print(stds);
    return 0;
}
```
## ☆《算法笔记》2.10小节——C/C++快速入门->黑盒测试
**多点测试**中有如下结构用于反复执行程序的核心部分：

- `while-EOF类型`：如`while (scanf("%d", &a) != EOF)`；`while (scanf("%s", s) != EOF)`；`while (gets(str) != NULL)`等用于输入字符串和数值。如题1。
- `while-EOF-break`类型：输入数据满足某个条件时退出，如`while (scanf("%d %d", &a, &b) != EOF, a | b)`。如题3。
- `while (T--)`类型，给出测试数据的组数。如题2。
### 1.A+B 输入输出练习I
```c
#include <cstdio>
int main()
{
    int a, b;
    while (scanf("%d %d", &a, &b) == 2) 
    // while (scanf("%d %d", &a, &b) != EOF)
        printf("%d\n", a + b);
    return 0;
}
```
### 2.A+B 输入输出练习II
```c
/* 输入 第一行是一个整数N，表示后面会有N行a和b，通过空格隔开。
输出 对于输入的每对a和b，你需要在相应的行输出a、b的和。 */
#include <cstdio>
int main()
{
    int a, b, T;
    scanf("%d", &T);
    while (T--) {
        scanf("%d %d", &a, &b);
        printf("%d\n", a + b);
    }
    return 0;
}
```
### 3.A+B 输入输出练习III
```c
/* 输入 输入中每行是一对a和b。其中会有一对是0和0标志着输入结束，且这一对不要计算。
输出 对于输入的每对a和b，你需要在相应的行输出a、b的和。*/
#include <cstdio>
int main()
{
    int a, b;
    while (scanf("%d%d", &a, &b), a || b) 
        printf("%d\n", a + b);
    return 0;
}
```
### 4.A+B 输入输出练习IV
```c
/*输入 每行的第一个数N，表示本行后面有N个数。
如果N=0时，表示输入结束，且这一行不要计算。*/
#include <cstdio>
int r[100];
int main()
{
    int sum = 0, N, a, i = 0;
    scanf("%d", &N);
    while (N) {
        while (N--) {
            scanf("%d", &a);
            sum += a;
        }
        r[i++] = sum;
        scanf("%d", &N);
        sum = 0;
    }
    for (int j = 0; j < i; j++)  printf("%d\n", r[j]);
    return 0;
}
```
### 5.A+B 输入输出练习V
```c
/* 输入的第一行是一个正数N，表示后面有N行。每一行的第一个数是M，表示本行后面还有M个数。*/

#include <cstdio>
int r[100];
int main()
{
    int N, M, i = 0;
    scanf("%d", &N);
    while (N--) {
        scanf("%d", &M);
        int sum = 0, a;
        while (M--) {
            scanf("%d", &a);
            sum += a;
        }
        r[i++] = sum;
        sum = 0;
    }
    for (int j = 0; j < i; j++) printf("%d\n", r[j]);
    return 0;
}
```
### 6.A+B 输入输出练习VI
```c
#include <cstdio>
int r[100];
int main()
{
    int sum = 0, N, a, i = 0;
    while (scanf("%d", &N) != EOF) {
        while (N--) {
            scanf("%d", &a);
            sum += a;
        }
        r[i++] = sum;
        sum = 0;
    }
    for (int j = 0; j < i; j++)  printf("%d\n", r[j]);
    return 0;
}
```
### 7.A+B 输入输出练习VII
```c
#include <cstdio>
int main()
{
    int a, b;
    while (scanf("%d%d", &a, &b) != EOF) 
        printf("%d\n\n", a + b);
    return 0;
}
```
### 8.A+B 输入输出练习VIII
```c
#include <cstdio>
int r[100];
int main() {
    int N, M, i = 0;
    scanf("%d", &N);
    while (N--) {
        int sum = 0, a;
        scanf("%d", &M);
        while (M--) {
            scanf("%d", &a);
            sum += a;
        }
        r[i++] = sum;
        sum = 0;
    }
    for (int j = 0; j < i; j++) printf("%d\n\n", r[j]);
    return 0;
}
```
---
## 《算法笔记》3.1小节——入门模拟->简单模拟
简单模拟**这类题目不涉及算法，完全只是根据题目描述来进行代码的编写**。
### 问题 A: 剩下的树 
```c
#include <cstdio>
#include <cstring>
/*有一个长度为整数L(1<=L<=10000)的马路，想象成数轴上长度为L的一个线段，起点是坐标原点，
在每个整数坐标点有一棵树，即在0,1,2，...，L共L+1个位置上有L+1棵树。
  现在要移走一些树，移走的树的区间用一对数字表示，如 100 200表示移走从100到200之间（包括端点）所有的树。
  可能有M(1<=M<=100)个区间，区间之间可能有重叠。现在要求移走所有区间的树之后剩下的树的个数。
输入 
  两个整数L(1<=L<=10000)和M(1<=M<=100)。接下来有M组整数，每组有一对数字。输入0 0表示结束。
输出
  可能有多组输入数据，对于每组输入数据，输出一个数，表示移走所有区间的树之后剩下的树的个数。*/
 
int main()
{
    int r[200], j = 0;
    memset(r, 0, sizeof(r));
    int L, M;
    int left, right;
    while (scanf("%d%d", &L, &M), L) {
        int a[L+1];
        memset(a, 0, sizeof(a));
        while (M--) {
            scanf("%d%d", &left, &right);
            for (int i = left; i <= right; i++) {
                a[i] = 1;
            }
        }
        for (int i = 0; i <= L; i++) 
            if (!a[i]) r[j]++;
        j++;
    }
    for (int i = 0; i < j; i++) {
        printf("%d\n", r[i]);
    }
    return 0;
}
```
### 问题 B: A+B
```c
/*给定两个整数A和B，其表示形式是：从个位开始，每三位数用逗号","隔开。
输入 输入包含多组数据数据，每组数据占一行，由两个整数A和B组成（-10^9 < A,B < 10^9）。
输出 请计算A+B的结果，并以正常形式输出，每组数据占一行。*/
#include <cstdio>
#include <cstring>
long long to_int(char s[]) {
    int len = strlen(s);
    long long r = 0;
    int positive = 1;
    for (int i = 0; i < len; i++) {   //','就直接跳过
        if (s[i] <= '9' && s[i] >= '0') {
            r = r * 10 + (s[i] - '0');
        } else if (s[i] == '-') 
            positive = 0;
    }
    if (!positive) r = -r; 
    return r;
}

int main()
{
    char s[50], r[50];
    while (scanf("%s %s", s, r) != EOF) {
        long long snum, rnum;
        snum = to_int(s);
        rnum = to_int(r);
        printf("%lld\n", snum + rnum);
    }
    return 0;
}
```
### 问题 C: 特殊乘法
```c
/*写个算法，对2个小于1000000000的输入，求结果。特殊乘法举例：123 * 45 = 1*4 +1*5 +2*4 +2*5 +3*4+3*5
输入 两个小于1000000000的数
输出 输入可能有多组数据，对于每一组数据，输出两个数按照题目要求的方法进行运算后得到的结果。*/
#include <cstdio>
int int_to_array(int n, int num[]) {
    int i = 0;
    while (n) {
        num[i++] = n % 10;
        n /= 10;
    }
    return i;
}
int specialMulti(int num1[], int num2[], int len1, int len2) {
    int sum = 0;
    for (int i = 0; i < len1; i++) {
        for (int j = 0; j < len2; j++) {
            sum += (num1[i] * num2[j]);
        }
    }
    return sum;
}
int main()
{
    int M, N;
    int numM[15], numN[15], lenM, lenN;
    while (scanf("%d%d", &M, &N) != EOF) {
        lenM = int_to_array(M, numM);
        lenN = int_to_array(N, numN);
        printf("%d\n", specialMulti(numM, numN, lenM, lenN));
    }
    return 0;
}
```
### 问题 D: 比较奇偶数个数
```c
/* 输入 输入有多组数据。每组输入n，然后输入n个整数（1<=n<=1000）。
输出 如果偶数比奇数多，输出NO，否则输出YES。*/
#include <cstdio>
int main()
{
    int N, a, odd, even;
    odd = even = 0;
    while (scanf("%d", &N) != EOF) {
        while (N--) {
            scanf("%d", &a);
            if (a % 2) odd++;
            else even++;
        }
        if (even > odd) printf("NO\n");
        else printf("YES\n");
    }
    return 0;
}
```
### ★ 问题 E: Shortest Distance (20)
- 题目描述
The task is really simple: given N exits on a highway which forms a simple cycle, 
you are supposed to tell the shortest distance between any pair of exits.
- 输入
Each input file contains one test case. For each case, the first line contains an 
integer N (in [3, 105]), followed by N integer distances D1 D2 ... DN, where Di 
is the distance between the i-th and the (i+1)-st exits, and DN is between the 
N-th and the 1st exits. All the numbers in a line are separated by a space. 
The second line gives a positive integer M (<=104), with M lines follow, each 
contains a pair of exit numbers, provided that the exits are numbered from 1 to 
N. It is guaranteed that the total round trip distance is no more than 107.
- 输出
For each test case, print your results in M lines, each contains the shortest 
distance between the corresponding given pair of exits.
		
- 样例输入

		5 1 2 4 14 9
		3
		1 3
		2 5
		4 1
- 样例输出

		3
		10
		7
```c
#include <cstdio>

int RightDistance(int a[], int left, int right) {
    // [left, right)
    int sum = 0;
    for (int i = left - 1; i < right - 1; i++) // 逻辑序号映射到物理序号
        sum += a[i];
    return sum;
}

int LeftDistance(int a[], int N, int rightDist) {
    int sum = 0;
    for (int i = 0; i < N; i++)
        sum += a[i];
    return sum - rightDist;  // 环的性质
}

int main()
{
    int N, M;
    while (scanf("%d", &N) != EOF) {
        int a[N];
        for (int i = 0; i < N; i++) {
            scanf("%d", &a[i]);
        }
        scanf("%d", &M);
        int left, right;
        while (M--) {
            scanf("%d%d", &left, &right);
            int r1, r2;
            if (left > right) r1 = RightDistance(a, right, left); // 改变方向
            else r1 = RightDistance(a, left, right);
            r2 = LeftDistance(a, N, r1);
            printf("%d\n", r1 < r2 ? r1 : r2);
        }
    }
    return 0;
}
```
### 问题 F: A+B和C (15)
这题用int会溢出，干脆使用long long。除非这样也不行，就用大整数结构。
```c
/*题目描述
给定区间[-2^31, 2^31]内的3个整数A、B和C，请判断A+B是否大于C。
输入
输入第1行给出正整数T(<=10)，是测试用例的个数。随后给出T组测试用例，每组占一行，
顺序给出A、B和C。整数间以空格分隔。
输出
对每组测试用例，在一行中输出“Case #X: true”如果A+B>C，否则输出“Case #X: false”，其中X是
测试用例的编号（从1开始）。
*/
#include <cstdio>
 
int main()
{
    int T;
    scanf("%d", &T);
    for (int i = 1; i <= T; i++) {
        long long a, b, c;
        scanf("%lld%lld%lld", &a, &b, &c);
        if (a + b > c) printf("Case #%d: true\n", i);
        else printf("Case #%d: false\n", i);
    }
    return 0;
}
```
###  问题 G: 数字分类 (20)
给定一系列正整数，请按要求对数字进行分类，并输出以下5个数字：
- A1 = 能被5整除的数字中所有偶数的和；
- A2 = 将被5除后余1的数字按给出顺序进行交错求和，即计算n1-n2+n3-n4...；
- A3 = 被5除后余2的数字的个数；
- A4 = 被5除后余3的数字的平均数，精确到小数点后1位；
- A5 = 被5除后余4的数字中最大数字。

输入
每个输入包含1个测试用例。每个测试用例先给出一个不超过1000的正整数N，随后给出N个
不超过1000的待分类的正整数。数字间以空格分隔。
输出
对给定的N个正整数，按题目要求计算A1~A5并在一行中顺序输出。数字间以空格分隔，但行末不得有多余空格。**若其中某一类数字不存在，则在相应位置输出“N”**。

这题本来简单，就是要求输出N的判断麻烦，可能会栽在这里。
```c
#include <cstdio>

int main()
{
    int N, a;
    int a1, a2, a3, a5, n;
    double a4;
    int b1, b2, b5;
    while (scanf("%d", &N) != EOF) {	
        a1 = a2 = a3 = a4 = a5 = n = b1 = b2 = b5 = 0;
        int flag = 1;
        for (int i = 0; i < N; i++) {
            scanf("%d", &a);
            
            switch (a % 5) {
                case 0:
                    if (a % 2 == 0) {
                       a1 += a;
					   b1++;	
					}
                    break;
                case 1:
                    if (flag) {
                        a2 += a;
                        b2++;
                        flag = 0;
                    } else {
                        a2 += -a;
                        b2++;
                        flag = 1;
                    } 
                    break;
                case 2:
                    a3++;
                    break;
                case 3:
                    a4 += a;
                    n++;
                    break;
                case 4:
                    if (a > a5) 
                    	a5 = a;
                   	b5++;
                    break;
            } 
        }
        if (b1) printf("%d ", a1);
        else printf("N ");
        if (b2) printf("%d ", a2);
        else printf("N ");
        if (a3) printf("%d ", a3);
        else printf("N ");
        if (n) printf("%.1f ", (double)a4 / n);
        else printf("N ");
        if (b5) printf("%d\n", a5);
        else printf("N\n");
    }
    return 0;
}
```
### 问题 H: 部分A+B (15)
正整数A的“DA（为1位整数）部分”定义为由A中所有DA组成的新整数PA。例如：给定A = 3862767，DA = 6，则A的“6部分”PA是66，因为A中有2个6。
**现给定A、DA、B、DB，请编写程序计算PA + PB。**
- 输入
输入在一行中依次给出A、DA、B、DB，中间以空格分隔，其中0 < A, B < 10^10^。
- 输出
在一行中输出PA + PB的值。
```c
#include <cstdio>
#include <cstring>
/* 给出字符如'6'出现n次的数字, n=1时为6 */
int charToNum(char d, int n) { 
    int numD = d - '0', res = 0;
    for (int i = 0; i < n; i++) {
        res = (res * 10 + numD);
    }
    return res;
}

int main()
{
    char a[25], b[25];
    char da, db;
    while (scanf("%s %c %s %c", a, &da, b, &db) != EOF) {
        int len1 = strlen(a), len2 = strlen(b), n1 = 0, n2 = 0;
        for (int i = 0; i < len1; i++)
            if (a[i] == da) n1++;
        for (int i = 0; i < len2; i++)
            if (b[i] == db) n2++;
        printf("%d\n", charToNum(da, n1) + charToNum(db, n2));
    }    
    return 0;
}
```
### 问题 I: 锤子剪刀布 (20)
大家应该都会玩“锤子剪刀布”的游戏：两人同时给出手势。**现给出两人的交锋记录，请统计
双方的胜、平、负次数，并且给出双方分别出什么手势的胜算最大。**
- 输入
输入第1行给出正整数N（<=105），即双方交锋的次数。随后N行，每行给出一次交锋的信息
即甲、乙双方同时给出的的手势。C代表“锤子”、J代表“剪刀”、B代表“布”，第1个字母代表
甲方，第2个代表乙方，中间有1个空格。
- 输出
输出第1、2行分别给出甲、乙的胜、平、负次数，数字间以1个空格分隔。第3行给出两个字母
，分别代表甲、乙获胜次数最多的手势，中间有1个空格。如果解不唯一，则输出按字母序最小的解。

`本题我发现对于scanf("%c")的用法容易出错，此时会将空格和换行符一并输入，因此需要用getchar()吸收。`

```c
#include <cstdio>
#include <cstring>
char c[] = {'B', 'C', 'J'};
/* 返回获胜次数最多的手势，如果解不唯一，则返回按字母序最小的手势字符 */
char checkWinWay(int m[])
{
	int k = 0;
	for (int i = 1; i < 3; i++) 
		if (m[i] > m[k]) k = i;
	return c[k];
}
int main()
{
    int N, winJ, winY, par, J[3], Y[3];
    winJ = winY = par = 0;
    memset(J, 0, sizeof(J)); // B C J
    memset(Y, 0, sizeof(Y));
    char a, b;
    scanf("%d", &N);
    getchar();        // 吸收换行 
    
    for (int i = 0; i < N; i++) {
        scanf("%c", &a);
        getchar();    // 吸收空格 
        scanf("%c", &b);
        getchar();    // 吸收换行 
        switch(a) {
            case 'C':
                switch(b) {
                    case 'C': par++; break;
                    case 'J': winJ++; J[1]++; break;
                    case 'B': winY++; Y[0]++; break;
                }
                break;
            case 'J':
                switch(b) {
                    case 'C': winY++; Y[1]++; break;
                    case 'J': par++; break;
                    case 'B': winJ++; J[2]++; break;
                }
                break;
            case 'B':
                switch(b) {
                    case 'C': winJ++; J[0]++; break;
                    case 'J': winY++; Y[2]++; break;
                    case 'B': par++; break;
                }
                break;
        }
    }
    printf("%d %d %d\n%d %d %d\n", winJ, par, N-winJ-par, winY, par, N-winY-par);
    printf("%c %c\n", checkWinWay(J), checkWinWay(Y)); 
    return 0;
}
```

## 《算法笔记》3.2小节——入门模拟->查找元素
http://codeup.cn/contest.php?cid=100000576
### 问题 A: 统计同成绩学生人数
这一题需要用输入的人数N作为循环判断条件。
```c
/*读入N名学生的成绩，将获得某一给定分数的学生人数输出。
输入 每个测试用例的格式为
第1行：N
第2行：N名学生的成绩，相邻两数字用一个空格间隔。
第3行：给定分数
当读到N=0时输入结束。其中N不超过1000，成绩分数为（包含）0到100之间的一个整数。

输出
对每个测试用例，将获得给定分数的学生人数输出。
*/
#include <cstdio>
int main()
{
    int N;
    while (scanf("%d", &N), N) {
        int a[N];
        for (int i = 0; i < N; i++)
            scanf("%d", &a[i]);
        int num, cnt = 0;
        scanf("%d", &num);
        for (int i = 0; i < N; i++) 
            if (a[i] == num) cnt++;
        printf("%d\n", cnt);
    }
    return 0;
}
```
### 问题 B: 找x
```c
/*输入一个数n，然后输入n个数值各不相同，再输入一个值x，输出这个值在这个数组中的下标（从0开始，若不在数组中则输出-1）。
输入 测试数据有多组，输入n(1<=n<=200)，接着输入n个数，然后输入x。
输出 对于每组输入,请输出结果。
*/
#include <cstdio>
int main()
{
    int N;
    while (scanf("%d", &N) != EOF) {
        int a[N];
        for (int i = 0; i < N; i++) 
            scanf("%d", &a[i]);
        int num, index;
        scanf("%d", &num);
        int flag = 1;
        for (int i = 0; i < N; i++) 
            if (a[i] == num) {
                flag = 0;
                printf("%d\n", i);
                break;
            }
        if (flag) printf("%d\n", -1);
    }
    return 0;
}
```
### 问题 C: 查找学生信息
CodeUp是多点测试的，竟然忘记了这个情况！同时，在对字符串申请空间时，尽量多申请一点。
```c
#include <cstdio>
#include <cstring>
typedef struct student {
    char index[5];
    char name[100];
    char sex[10];
    int age;
} student;
student stu[1010];

int main() 
{
    int N;
    while (scanf("%d", &N) != EOF) {
    	for (int i = 0; i < N; i++) 
            scanf("%s %s %s %d", stu[i].index, stu[i].name, stu[i].sex, &stu[i].age);
		int M;
	    scanf("%d", &M); 
	    while (M--) {
	    	char in[10];
	        scanf("%s", in);
	        int i;
	        for (i = 0; i < N; i++) {
	        	if (strcmp(stu[i].index, in) == 0) {
	            	printf("%s %s %s %d\n", stu[i].index, stu[i].name, stu[i].sex, stu[i].age);
	            	break;
				}  
			}     
	        if (i == N) printf("No Answer!\n");
	    }
	}
    return 0;
}
```
### 问题 D: 查找
```c
/*输入数组长度 n 
输入数组      a[1...n] 
输入查找个数m 
输入查找数字b[1...m] 
输出 YES or NO  查找有则YES 否则NO 。
输入 输入有多组数据。
每组输入n，然后输入n个整数，再输入m，然后再输入m个整数（1<=m<=n<=100）。
输出
如果在n个数组中输出YES否则输出NO。
*/
#include <cstdio>

int main()
{
    int n;
    while (scanf("%d", &n) != EOF) {
        int a[150];
        for (int i = 0; i < n; i++) 
            scanf("%d", &a[i]);
        int m;
        scanf("%d", &m);
        while (m--) {
            int num, i;
            scanf("%d", &num);
            for (i = 0; i < n; i++) 
                if (a[i] == num) {
                	printf("YES\n"); 
                	break;
				}
            if (i == n) printf("NO\n");
        }
    }
    return 0;
}
```
### 问题 E: 学生查询
```c
/*
输入n个学生的信息，每行包括学号、姓名、性别和年龄，每一个属性使用空格分开。
最后再输入一学号，将该学号对应的学生信息输出。
输入
测试数据有多组，第一行为样例数m。对于每个样例，第一行为学生人数n(n不超过20)，
加下来n行每行4个整数分别表示学号、姓名、性别和年龄，最后一行表示查询的学号。

输出
输出m行，每行表示查询的学生信息，格式参见样例。
*/
#include <cstdio>
typedef struct student {
    int index;
    char name[100];
    char sex[10];
    int age;
} student;
student stu[50];

int main() 
{
    int m;
    scanf("%d", &m);
    while (m--) {
        int n;
        scanf("%d", &n);
    	for (int i = 0; i < n; i++)  
            scanf("%d %s %s %d", &stu[i].index, stu[i].name, stu[i].sex, &stu[i].age);
        int in;
	    scanf("%d", &in);
        int i;
        for (i = 0; i < n; i++) {
        	if (stu[i].index == in) {
            	printf("%d %s %s %d\n", stu[i].index, stu[i].name, stu[i].sex, stu[i].age);
            	break;
			}  
		}     
	}
    return 0;
}
```
## 《算法笔记》3.3小节——入门模拟->图形输出
### 问题 A: 输出梯形
输入一个高度h，输出一个高为h，上底边为h的梯形。

- 输入 一个整数h(1<=h<=1000)。
- 输出 h所对应的梯形。 

样例输入

	5
样例输出
```
        *****
      *******
    *********
  ***********
*************
```
这个题目也是多点测试的，别看它题目说是“一个”。
```c
#include <cstdio>
int main()
{
    int h;
    while (scanf("%d", &h) != EOF) {
        int UpEdge = h, DownEdge = 3 * h - 2;
        for (int i = UpEdge; i <= DownEdge; i += 2) {
            int spaceNum = DownEdge - i;
            for (int j = 1; j <= spaceNum; j++)
                printf(" ");
            for (int k = 1; k <= i; k++)
                printf("*");
            printf("\n");
        }
    }
    return 0;
}
```
### ★ 问题 B: Hello World for U
Given any string of N (>=5) characters, you are asked to form the characters into the shape of U.

That is, the characters must be printed in the original order, starting top-down from the left vertical 
line with n1 characters, then left to right along the bottom line with n2 characters, and finally bottom-up 
along the vertical line with n3 characters. And more, we would like U to be as squared as possible -- that is, 
it must be satisfied that `n1 = n3 = max { k| k <= n2 for all 3 <= n2 <= N } with n1 + n2 + n3 - 2 = N`.


- 输入
Each input file contains one test case. Each case contains one string with no less than 5 and no more than 80 characters in a line. The string contains no white space.
- 输出
For each test case, print the input string in the shape of U as specified in the description.
- 样例输入
	```
	helloworld!
	```
- 样例输出
	```
	h   !
	e   d
	l   l
	lowor
	```

这一题**需要解决的问题是将一个字符串写成U字形**。拿到这一题的第一映像是U字的写法，先是写第一排第一个字符，然后写第二排第一个字符……然后是最后一排，然后是倒数第二排……但在C语言中如果我们要这
样写U字形的字符串就需要在数组中操作了。如果是**直接输出的话，那只能自上至下一行一行输出**。首先是第一行，写出第一个字符和最后一个字符，第二行写出第二个字符和倒数第二个字符……最后是最后一行。需要注意的是除了**最后一行输出所有字符，前面每一行只输出两个字符。中间还有空格来隔开每行的两个字符**。

思路有了，看看具体的要求。字符串的长度是N，n1，n3代表两边每列字符的数目。n2代表最后一行的字符数。题目中给了一个算式：
`n1 = n3 = max { k| k <= n2 for all 3 <= n2 <= N } with n1 + n2 + n3 - 2 = N.`
仔细研究这个算式，这里的k是不大于n2的，也就是说**n1和n3是不大于n2且满足n1+n2+n3=N+2的最大值**。`类似于将字符串长度增加2再对折成三段`。那么自然有`n1=n3=(N+2)/3，n2=N+2-(n1+n3)`。
也就是说设side为两边的字符数（包括最后一行的两端），则`side=n1=n3=(N+2)/3`。设mid为最后一行除去两端的两个字符后剩下的字符数，`mid=N-side*2`（总长度减去两边的字符数）。同时**mid也是我们输出除最后一行外前面所有行需要空出的空格数**。

最后，如何**在第i行输出第一个字符和最后一个字符**呢？那自然是`str[i]和str[len-1-i]`（len为字符串的长度，也就是N）。具体细节详见代码。

于是问题完美解决，步骤如下：
1）计算字符串长度len；
2）计算两边的字符数side=(len+2)/3；
3）计算最后一行中间的字符数（前面每行中间的空格数）
4）输出每行相应的字符。
```c
#include <cstdio>
#include <cstring>
int main()
{
    char s[100];
    int side, mid, len;
    while (scanf("%s", s) != EOF) {
        len = strlen(s);
        side = (len + 2) / 3;
        mid = len - side * 2;
        for (int i = 0; i < side; i++) {
            if (i < side - 1) { //输出除最后一行的前面所有行
                printf("%c", s[i]);
                for (int j = 0; j < mid; j++)
                    printf(" ");
                printf("%c\n", s[len - 1 - i]);
            } else if (i == side - 1) { //输出最后一行
                for (int j = 0; j < mid + 2; j++) 
                    printf("%c", s[i++]);
                printf("\n");
            }
        }
    }
    return 0;
}
```
### 问题 C: 等腰梯形
请输入高度h，输入一个高为h，上底边长为h 的等腰梯形。
- 输入 输入第一行表示样例数m，接下来m行每行一个整数h，h不超过10。
- 输出 对应于m个case输出要求的等腰梯形。
样例输入
```
1
4
```
样例输出
```
   ****
  ******
 ********
**********
```
这题与上上题的打印梯形有很大联系，只是变化了一下。
```c
#include <cstdio>
int main()
{
    int m, h;
    scanf("%d", &m);
    while (m--) {
        scanf("%d", &h);
        int UpEdge = h, DownEdge = 3 * h - 2;
        for (int i = UpEdge; i <= DownEdge; i += 2) {
            //打印空格
            int spaceNum = (DownEdge - i) / 2;
            for (int j = 0; j < spaceNum; j++)
                printf(" ");
            //打印等腰梯形主体
            for (int k = 0; k < i; k++)
                printf("%c", '*');
            printf("\n");
        }
    }
    return 0;
}
```
### 问题 D: 沙漏图形 tri2str [1*+]
问题：输入n，输出正倒n层星号三角形。首行顶格，星号间有一空格，效果见样例
输入样例：
```
3
```
输出样例：
```
* * *
 * * 
  *
 * * 
* * *
```
数据规模 1<= n <=50
```c
#include <cstdio>

int main()
{
    int n;
    while (scanf("%d", &n) != EOF) {
        //打印上半部分 
        int UpEdge = n + n - 1; //第一层的字符数(包括空格和星号)
        for (int i = UpEdge; i >= n; i--) { //i表示每层所拥有的字符数
            int spaceNum = UpEdge - i;
            for (int j = 0; j < spaceNum; j++)
                printf(" ");
            int charNum = i - n + 1;
            for (int j = 0; j < charNum - 1; j++) 
                printf("* ");
            printf("*\n");
        }
        //打印下半部分 
        for (int i = n + 1; i <= UpEdge; i++) {
            int spaceNum = UpEdge - i;
            for (int j = 0; j < spaceNum; j++)
                printf(" ");
            int charNum = i - n + 1;
            for (int j = 0; j < charNum - 1; j++) 
                printf("* ");
            printf("*\n");
        }
    }
    return 0;
}
```

## 《算法笔记》3.4小节——入门模拟->日期处理
http://codeup.cn/contest.php?cid=100000578
### ★ 问题 A: 日期差值
题目描述：有两个日期，求两个日期之间的天数，如果两个日期是连续的我们规定他们之间的天数为两天。
- 输入：有多组数据，每组数据有两行，分别表示两个日期，形式为YYYYMMDD。
- 输出：每组数据输出一行，即日期差值。
```c
#include <cstdio>
int month[13][2] = { //平年闰年的每个月天数(月/日), 第一行平年, 第二行闰年
    {0, 0}, {31, 31}, {28, 29}, {31, 31}, {30, 30}, {31, 31}, {30, 30}, {31, 31},
    {31, 31}, {30, 30}, {31, 31}, {30, 30}, {31, 31} // {0, 0}没有意义
};
bool isLeap(int year) { //判断是否是闰年
    return (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);
}

int main()
{
    int time1, y1, m1, d1;
    int time2, y2, m2, d2;
    while (scanf("%d%d", &time1, &time2) != EOF) { //scanf(%d)会忽视空格和换行 
        if (time1 > time2) { //如果time1晚于time2则交换, 使第一个日期早于第二个日期
            int temp = time1;
            time1 = time2;
            time2 = temp;
        }
        y1 = time1 / 10000, m1 = time1 % 10000 / 100, d1 = time1 % 100;
        y2 = time2 / 10000, m2 = time2 % 10000 / 100, d2 = time2 % 100;
        int ans = 1; //记录日期差值, 因为连续两个日期天数为两天, 所以初值为1
        /* 第一个日期没有达到第二个日期时循环 */
        while (y1 < y2 || m1 < m2 || d1 < d2) {
            d1++;
            if (d1 == month[m1][isLeap(y1)] + 1) { //满当月天数
                d1 = 1;  //日期变为下个月的1号
                m1++;    //月份+1
            }
            if (m1 == 13) { //月份满12个月
                m1 = 1;  //月份变为下一年的1月
                y1++;    //年+1
            }
            ans++; //累计 
        }
        printf("%d\n", ans);
    }
    return 0;
}
```
### ★★ 问题 B: Day of Week
We now use the Gregorian style of dating in Russia. The leap years are years with number divisible 
by 4 but not divisible by 100, or divisible by 400.

For example, years 2004, 2180 and 2400 are leap. Years 2004, 2181 and 2300 are not leap.
Your task is to write a program which will compute the day of week corresponding 
to a given date in the nearest past or in the future using today’s agreement about dating.
- 输入
There is one single line contains the day number d, month name M and year number y(1000≤y≤3000). 
The month name is the corresponding English name starting from the capital letter.
- 输出
Output a single line with the English name of the day of week corresponding to the date, 
starting from the capital letter. All other letters must be in lower case.
- 样例输入
	```
	21 December 2012
	5 January 2013
	```
- 样例输出
	```
	Friday
	Saturday
	```
这么写只有50%？！不知道问题出在哪里……**不过很多日期问题的实质就是在花式数天数**。
```c
#include <cstdio>
#include <cstring>
struct Date{
	int y, m, d;
};

char MonthName[13][12]= {"None", "January", "February", "March", "April", "May", 
   "June", "July", "August", "September", "October", "November", "December"};

char Weekday[8][12] = {"None", "Monday", "Tuesday", "Wednesday", "Thursday",
"Friday", "Saturday", "Sunday"};

int mapMonth(char s[]) {
    for (int i = 1; i < 13; i++) {
    	if (strcmp(s, MonthName[i]) == 0) 
           return i;
	}
    return 0;
}
int month[13][2] = { //平年闰年的每个月天数(月/日), 第一行平年, 第二行闰年
    {0, 0}, {31, 31}, {28, 29}, {31, 31}, {30, 30}, {31, 31}, {30, 30}, {31, 31},
    {31, 31}, {30, 30}, {31, 31}, {30, 30}, {31, 31} // {0, 0}没有意义
};
bool isLeap(int year) { //判断是否是闰年
    return (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);
}
bool MoreEqu(struct Date day1, struct Date day2) { //如果第一个日期晚(大)于第二个日期, 返回true
    if (day1.y != day2.y) return day1.y >= day2.y;
    else if (day1.m != day2.m) return day1.m >= day2.m;
    else return day1.d >= day2.d;
}

int main() {
    struct Date MyDay, now = {2019, 7, 29};
    char mEnglish[14];
    while (scanf("%d%s%d", &MyDay.d, mEnglish, &MyDay.y) != EOF) {
        MyDay.m = mapMonth(mEnglish);
        int flag = 0;
        struct Date tmp = now;
        if (MoreEqu(MyDay, now)) {
            struct Date t = MyDay;
            MyDay = tmp;
            tmp = t;
            flag = 1;
        }
        int cnt = 0;
        while (MyDay.y < tmp.y || MyDay.m < tmp.m || MyDay.d < tmp.d) {
        	MyDay.d++;
        	cnt++;
        	if (MyDay.d == month[MyDay.m][isLeap(MyDay.y)] + 1) {
        		MyDay.m++;
        		MyDay.d = 1;
			}
			if (MyDay.m == 13) {
				MyDay.y++;
				MyDay.m = 1;
			}
		}
	 	int offset = cnt % 7; // x
	 	if (flag) printf("%s\n", Weekday[1 + offset]); // x
	 	else printf("%s\n", Weekday[1 - offset + 7]); // x
    }
    return 0;
}
```
我知道错误了，小于20190729的每隔7天的日期(星期一)无法成功输出。把上面的最后标记的几句改成下面这样就可以了：
```c
if (!flag) cnt = - cnt;
int offset = ((cnt % 7) + 7) % 7;
printf("%s\n", Weekday[1 + offset]);
```
或者打个补丁：
```c
int offset = cnt % 7; //22 July 2019
if (flag) printf("%s\n", Weekday[1 + offset]); 
else {
	if (offset) printf("%s\n", Weekday[8 - offset]); 
	else printf("%s\n", Weekday[1]);
}
```
换种数法，**计算从公元到输入日期的天数**好了，然后与标的日期天数相减。数的时候**先数年再数月再数日**，加快速度。这样节省了一点代码。
```c
#include <cstdio>
#include <cstring>
struct Date {
	int y, m, d;
};
char MonthName[13][12]= {"None", "January", "February", "March", "April", "May", 
   "June", "July", "August", "September", "October", "November", "December"};
char Weekday[8][12] = {"None", "Monday", "Tuesday", "Wednesday", "Thursday",
"Friday", "Saturday", "Sunday"};
int month[13][2] = { //平年闰年的每个月天数(月/日), 第一行平年, 第二行闰年
    {0, 0}, {31, 31}, {28, 29}, {31, 31}, {30, 30}, {31, 31}, {30, 30}, {31, 31},
    {31, 31}, {30, 30}, {31, 31}, {30, 30}, {31, 31} // {0, 0}没有意义
};

int mapMonth(char s[]) {
    for (int i = 1; i < 13; i++) {
    	if (strcmp(s, MonthName[i]) == 0) 
           return i;
	}
    return 0;
}

bool isLeap(int year) { //判断是否是闰年
    return (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);
}

int sumDays(struct Date tmp) {
	int day = 0;
	for (int i = 0; i < tmp.y; i++) {
		day += (isLeap(i) ? 366 : 365);
	}
    for (int i = 1; i < tmp.m; i++) {
    	day += month[i][isLeap(tmp.y)];
	}
	for (int i = 1; i < tmp.d; i++) {
		day++;
	}
	return day; // 737634
}

int main() {
    struct Date MyDay; // now = {2019, 7, 29}; 距离公元元年737634天 
    char mEnglish[14];
    while (scanf("%d%s%d", &MyDay.d, mEnglish, &MyDay.y) != EOF) {
        MyDay.m = mapMonth(mEnglish);
        int dayCount = sumDays(MyDay) - 737634; //得到日期差值
	 	int offset = ((dayCount % 7) + 7) % 7;  //对负数和超出的正数修正 
	 	printf("%s\n", Weekday[1 + offset]); //1-7 20190729星期一
    }
    return 0;
}
```
### ★ 问题 C: 打印日期
题目描述：给出年分m和一年中的第n天，算出第n天是几月几号。
- 输入：输入包括两个整数y(1<=y<=3000)，n(1<=n<=366)。
- 输出：可能有多组测试数据，对于每组数据，按 yyyy-mm-dd的格式将输入中对应的日期打印出来。
- 样例输入
	```
	2013 60
	2012 300
	2011 350
	2000 211
	```
- 样例输出
	```
	2013-03-01
	2012-10-26
	2011-12-16
	2000-07-29
	```
将天数翻译成对应年的第几天，这个问题跟`A.日期差值`是基本一样的，只是这题不是比较两个日期的差别，而是用一个累加器，每过一天+1，与输入的一年中第几天比较，不断循环。
```c
#include <cstdio>
int month[13][2] = { //平年闰年的每个月天数(月/日), 第一行平年, 第二行闰年
    {0, 0}, {31, 31}, {28, 29}, {31, 31}, {30, 30}, {31, 31}, {30, 30}, {31, 31},
    {31, 31}, {30, 30}, {31, 31}, {30, 30}, {31, 31} // {0, 0}没有意义
};
bool isLeap(int year) { //判断是否是闰年
    return (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);
}

int main()
{
    int year, day;
    while (scanf("%d%d", &year, &day) != EOF) {
        int y = year, m = 1, d = 1, cnt = 1; //cnt计数, 表示第几天

        while (cnt < day) {
            d++;
            if (d == month[m][isLeap(y)] + 1) {
                m++;
                d = 1;
            }
            cnt++; 
        }
        printf("%04d-%02d-%02d\n", y, m, d);
    }
    return 0;
}
```
### 问题 D: 日期类
题目描述：编写一个日期类，要求按xxxx-xx-xx 的格式输出日期，实现加一天的操作。
- 输入：输入第一行表示测试用例的个数m，接下来m行每行有3个用空格隔开的整数，分别表示年月日。
测试数据不会有闰年。
- 输出：输出m行。按xxxx-xx-xx的格式输出，表示输入日期的后一天的日期。
- 样例输入
	```
	2
	1999 10 20
	2001 1 31
	```
- 样例输出
	```
	1999-10-21
	2001-02-01
	```
```c
#include <cstdio>
int month[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

int main()
{
    int M;
    scanf("%d", &M);
    int y, m, d;
    for (int i = 0; i < M; i++) {
        scanf("%d%d%d", &y, &m, &d);
        d++;
        if (d == month[m] + 1) {
            m++;
            d = 1;
        }
        printf("%04d-%02d-%02d\n", y, m, d);
    }
    return 0;
}
```
### ★ 问题 E: 日期累加
题目描述：设计一个程序能计算一个日期加上若干天后是什么日期。
- 输入：输入第一行表示样例个数m，接下来m行每行四个整数分别表示年月日和累加的天数。
- 输出：输出m行，每行按yyyy-mm-dd的个数输出。
- 样例输入
	```
	1
	2008 2 3 100
	```
- 样例输出
	```
	2008-05-13
	```
与`A.日期差值`很多一样的代码，或者说**日期类的题目，平年闰年、大月小月的分辨就是基础**了。
这一题是上一题`D.日期类`的深入，上一题只要求+1天，无闰年，而这一题就不一样了。但是大体上还是一样的思路。
```c
#include <cstdio>
int month[13][2] = { //平年闰年的每个月天数(月/日), 第一行平年, 第二行闰年
    {0, 0}, {31, 31}, {28, 29}, {31, 31}, {30, 30}, {31, 31}, {30, 30}, {31, 31},
    {31, 31}, {30, 30}, {31, 31}, {30, 30}, {31, 31} // {0, 0}没有意义
};
bool isLeap(int year) { //判断是否是闰年
    return (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);
}

int main() {
    int M;
    scanf("%d", &M);
    int y, m, d, days;
    while (M--) {
        scanf("%d %d %d %d", &y, &m, &d, &days);
        int cnt = 0; //表示已增加的天数, 与days比较做循环
        while (cnt < days) {
            d++;
            if (d == month[m][isLeap(y)] + 1) {
                m++;
                d = 1;
            }
            if (m == 13) {
                y++;
                m = 1;
            }
            cnt++;
        }
        printf("%04d-%02d-%02d\n", y, m, d);
    }
    return 0;
} 
```
### 问题F: 公元时间
得到某一日期距离公元元年的天数。
- 法一：
```c
struct Date {
	int y, m, d;
};
int sumDays(struct Date tmp) {
	int reDay = 0;
	struct Date Gen = {0, 1, 1};
    while (Gen.y < tmp.y || Gen.m < tmp.m || Gen.d < tmp.d) {
    	Gen.d++;
    	reDay++;
    	if (Gen.d == month[Gen.m][isLeap(Gen.y)] + 1) {
    		Gen.m++;
    		Gen.d = 1;
		}
		if (Gen.m == 13) {
			Gen.y++;
			Gen.m = 1;
		}
	}
	return reDay; 
}
```
- 法二：
```c
int sumDays(struct Date tmp) {
	int day = 0;
	for (int i = 0; i < tmp.y; i++) { //先数年
		day += (isLeap(i) ? 366 : 365);
	}
    for (int i = 1; i < tmp.m; i++) { //再数月
    	day += month[i][isLeap(tmp.y)];
	}
	for (int i = 1; i < tmp.d; i++) { //再数日
		day++;
	}
	return day; 
}

```
##  《算法笔记》3.5小节——入门模拟->进制转换9
http://codeup.cn/contest.php?cid=100000579
进制转换的基础主要是`10进制转换为R进制和R进制转换为10进制`，掌握这两个代码片就可以了。P进制转换为Q进制是这两个过程的统合，通过十进制作为中转站。
### 问题 A: 又一版 A+B
```c
/* 
输入两个不超过整型定义的非负10进制整数A和B(<=231-1)，输出A+B的m (1 < m <10)进制数。
输入：测试输入包含若干测试用例。每个测试用例占一行，给出m和A，B的值。当m为0时输入结束。
输出：每个测试用例的输出占一行，输出A+B的m进制数。
*/
#include <cstdio>

int main() {
    long long a, b, sum;
    int m, nums[50];
    while (scanf("%d", &m), m) {
        scanf("%lld%lld", &a, &b);
        sum = a + b;
        int i = 0;
        /* 10进制转换为R进制的代码片 */
        do {
            nums[i++] = sum % m;
            sum /= m;
        } while (sum != 0);
        //倒着打印
        for (int j = i - 1; j >= 0; j--) {
            printf("%d", nums[j]);
            if (j == 0) printf("\n");
        }
    }
    return 0;
}
```
### ★ 问题 B: 数制转换
题目描述：求**任意两个不同进制非负整数的转换**（2进制～16进制），**所给整数在long所能表达的范围之内**。不同进制的表示符号为（0，1，...，9，a，b，...，f）或者（0，1，...，9，A，B，...，F）。
- 输入
输入只有一行，包含三个整数a，n，b。a表示其后的n是a进制整数，b表示欲将a进制整数n转
换成b进制整数。a，b是十进制整数，2 =< a，b <= 16。
- 输出
可能有多组测试数据，对于每组数据，输出包含一行，该行有一个整数为转换后的b进制数。
**输出时字母符号全部用大写表示**，即（0，1，...，9，A，B，...，F）。
- 样例输入
	```
	4 123 10
	```
- 样例输出
	```
	27
	```
这一题的关键在于要**用字符串存储和表示不同进制的数**，因为输入的数为a进制，可能是16进制，会使用a-f或A-F的字符，而且输出的数也可能有字母。可以说，这一题就是比较通用的**2-16进制非负整数间的转换程序**。
```c
#include <cstdio>
#include <cstring>

int CharToOct(int a, char n[]) {  //按进制a将n字符串(可表示2-16进制)转换为10进制数
    int sum = 0, product = 1;
    for (int i = strlen(n) - 1; i >= 0; i--) {
        if (n[i] <= '9') sum += (n[i] - '0') * product;
        else if (n[i] <= 'F') sum += (n[i] - 'A' + 10) * product; //大写字母符号
        else if (n[i] <= 'f') sum += (n[i] - 'a' + 10) * product; //小写字母符号
        product *= a;
    }
    return sum;
}

void OctToChar(int temp, int b, char r[]) { //将10进制数按b进制转换成r字符串
    int i = 0;
    do {
        int k = temp % b;
        if (k <= 9) r[i++] = '0' + k; //十进制符号
        else r[i++] = 'A' + k - 10;   //用大写字母表示大于9的数字
        temp /= b;
    } while (temp != 0);
    r[i] = '\0';  //必须添加结束符, 不然strlen无法正确判别长度 
}

int main() {
    int a, b; // 2-16
    char n[100];
    while (scanf("%d%s%d", &a, n, &b) != EOF) {
        int temp = CharToOct(a, n);
        if (b == 10) {
        	printf("%d\n", temp);
        	continue;
		}
		char r[100];
        OctToChar(temp, b, r);
        
        for (int j = strlen(r) - 1; j >= 0; j--) 
            printf("%c", r[j]);
        printf("\n");
    }
    return 0;
}
```
### ★★ 问题 C: 进制转换(也算得上是大整数的题目)
题目描述：将一个长度最多为30位数字的十进制非负整数转换为二进制数输出。
- 输入： 多组数据，每行为一个长度不超过30位的十进制非负整数。
- 输出： 每行输出对应的二进制数。
- 样例输入
	```
	985
	211
	1126
	```
- 样例输出
	```
	1111011001
	11010011
	10001100110
	```
**使用字符数组存储的时候，是逆位存储的，即整数低位存储在字符数组的高位，整数高位存储在字符数组的低位**。虽然理顺了逻辑也很不错。
```c
#include <cstdio>

int main() {
    char s[32]; //将十进制字符串转换为倒排的二进制字符串, 需模拟多次数字取余和除法
    while (gets(s)) {
        char nums[100] = {};
        int numsSize = 0, sum = 1; //全十进制字符串
        while (sum) { //当十进制字符还未除完时继续循环
            sum = 0; //每一次十进制字符串除以2都恢复0
            for (int i = 0; s[i]; i++) {
                int digit = s[i] - '0';
                int x = digit / 2; 
                sum += x;
                if (s[i + 1]) {
                    s[i + 1] += (digit % 2 * 10);
                } else {
                    nums[numsSize++] = digit % 2 + '0'; //从低位向高位存入取余的字符
                }
                s[i] = x + '0'; //一位字符的变化
            }
        }
        for (int k = numsSize - 1; k >= 0; k--) {
            printf("%c", nums[k]);
        }
        printf("\n");
    }
    return 0;
}
```
### 问题 D: 八进制
```c
#include <cstdio>

int main() {
    int N, nums[50];
    while (scanf("%d", &N) != EOF) {
        int i = 0;
        do {
            nums[i++] = N % 8;
            N /= 8;
        } while (N != 0);
        for (int j = i - 1; j >= 0; j--) 
            printf("%d", nums[j]);
        printf("\n");
    }
    return 0;
}
```
##  《算法笔记》3.6小节——入门模拟->字符串处理
### 问题 A: 字符串连接
```c
#include <cstdio>

int main() {
    char s1[200], s2[100], r[210];
    while (scanf("%s%s", s1, s2) != EOF) {
        int i;
        for (i = 0; s1[i]; i++) {
            r[i] = s1[i];
        }
        for (int j = 0; s2[j]; j++) {
            r[i++] = s2[j];
        }
        r[i] = '\0';
        puts(r);
    }
    return 0;
}
```
### 问题 B: 首字母大写
- 题目描述：对一个字符串中的所有单词，如果单词的首字母不是大写字母，则把单词的首字母变成大写字母。在字符串中，**单词之间通过空白符分隔，空白符包括：空格(' ')、制表符('\t')、回车符('\r')、换行符('\n')**。
- 输入：一行待处理的字符串（长度小于100）。
- 输出：可能有多组测试数据，对于每组数据，输出一行转换后的字符串。
- 样例输入
	```
	if so, you already have a google account. you can sign in on the right.
	```
- 样例输出
	```
	If So, You Already Have A Google Account. You Can Sign In On The Right.
	```
这个题其实很简单，`PAT B1009说反话`与之类似，都可以用两种方法实现，**一者是直接对字符串进行整体处理**，是一种简单的办法，但是可能会写错。像这题就是将**第一个字母和每个空格符后面的字母变成大写**，我的思路是这样，但是写的时候搞错判断了(主要是if条件太长)，要**对这些字母本身是否已经是大写**进行判断，不然就会出错。
```c
#include <cstdio>

int main() {
    char s[120];
    while (gets(s) != NULL) {
        for (int i = 0; s[i]; i++) {
            if (
			   ((!i) || (s[i-1] == ' ' || s[i-1] == '\t' || s[i-1] == '\r' || s[i-1] == '\n'))
			    && s[i] >= 'a' && s[i] <= 'z'
	        ) 
                s[i] -= ('a' - 'A'); // s[i] - 32
        }
        puts(s);
    }
    return 0;
}
```
第二种方法就是**分割字符串成为一个个单词**，这时省去对第一个字母进行特判，然后判断每个单词的首字母本身是否已经是大写，不是则变为大写，最后输出。这样麻烦一点。
而且CodeUp的判题机也有问题，有时候加个大括号就可以正确了……
```c
#include <cstdio>

int main() {
    char s[120];
    while (gets(s) != NULL) {
    	char words[100][100] = {};
        int r = 0, c = 0;
        //分割字符串成多个单词
        for (int i = 0; s[i]; i++) {
            if (s[i] != ' ' && s[i] != '\t' && s[i] != '\r' && s[i] != '\n') 
                words[r][c++] = s[i];
            else {
                words[r++][c] = '\0';
                c = 0;
            }
        }
        //单词首字母大写
        for (int j = 0; j <= r; j++) 
            if (words[j][0] >= 'a') 
            	words[j][0] -= 32;
        //输出单词
        for (int j = 0; j <= r; j++) {
   	        printf ("%s", words[j]);  
   	    	if (j < r)   printf (" ");  
   	    	else 		 printf ("\n");
 	  	}  
    }
    return 0;
}
```
### ★★ 问题 C: 字符串的查找删除
题目描述：给定一个短字符串（不含空格），再给定若干字符串，在这些字符串中删除所含有的短字符串。
- 输入：输入只有1组数据。输入一个短字符串（不含空格），再输入若干字符串直到文件结束为止。
- 输出：**删除输入的短字符串**(**不区分大小写**)**并去掉空格**，输出。
- 样例输入
	```
	in
	#include 
	int main()
	{
	
	printf(" Hi ");
	}
	```
- 样例输出
	```
	#clude
	tma()
	{
	
	prtf("Hi");
	}
	```
- 提示：注：将字符串中的In、IN、iN、in删除。

这个题目还有点难度，必须一口气输入所有字符串进行处理。我的代码如下：
判断逻辑：遍历字符串，某字符与短字符第一个字符相同；再判断下一位字符是否相同，直至完全相同，完全相同则跳过。
```c
#include <cstdio>
char del[1000], temp[1001][1001], ans[1001][1001];

int main() {
    int index = 0;
    gets(del);
    for (; gets(temp[index]) != NULL; index++);  //读入数据
    /* 将要删除的字符转为小写 */
    for (int i = 0; del[i]; i++) {
        if (del[i] >= 'A' && del[i] <= 'Z') {
            del[i] += 32; 
        }
    }
    for (int i = 0; i < index; i++) {
        int k;
        /* 用另一二维数组存储原数据，然后将原数据转化为小写字母 */
        for (k = 0; temp[i][k]; k++) {
            ans[i][k] = temp[i][k];
            if (temp[i][k] >= 'A' && temp[i][k] <= 'Z') {
                temp[i][k] += 32;
            }
        }
        ans[i][k] = '\0'; //结束一句的复制
        /* 进行短字符判断，如果不是短字符或空格就输出 */
        for (int j = 0, len = 0; temp[i][j]; j++, len = 0) { //用len加以试探 
            while (temp[i][j + len] == del[len]) { //判断是否与短字符第一个字符相同, 相同则继续循环
                len++;
                if (!del[len]) { //这一段与短字符完全相同
                    j += len;    //指向i行j列的指针跳过这一段
                    len = 0;     //接着循环比较, 不同则进入下面的if语句打印字符
                }
            }
            if (temp[i][j] != ' ')  printf("%c", ans[i][j]);
        }
        printf("\n");  //输出一句后的换行
    }
    return 0;
}
```
这一段的逻辑也可以这么写，两者等价：
```c
 for (int j = 0, len = 0; temp[i][j]; ) {		
		if (temp[i][j + len] == del[len]) {			
			len++;								
			if (!del[len])  {	//若完全相同则跳过 
				j += len;       //接着就可以开始下一个字符的判断
				len = 0;
			}
		} else {  // 不同则进入下面的if语句打印字符
			if (temp[i][j] != ' ') printf ("%c", ans[i][j]); //输出原字符 
			j++;  //打印一个字符后j+1
			len = 0;
		}
}
printf ("\n");
```
### 问题 D: 单词置换
本题要求将s中所有单词a替换成b之后的字符串，如果真正替换很麻烦，所以这里先将字符串分割成单词组，然后对要替换的单词a，输出替换后的单词b。
```c
#include <cstdio>
#include <cstring>
int main() {
    char s[120];
    while (gets(s)) {
        char source[110] = {}, dest[110] = {}, words[110][50] = {};
        gets(source);
        gets(dest);
        int r = 0, c = 0; 
        for (int i = 0; s[i]; i++) {
            if (s[i] != ' ') {
                words[r][c++] = s[i];
            } else {
                words[r++][c] = '\0';
                c = 0;
            }
        }
        for (int j = 0; j <= r; j++) {
            if (strcmp(words[j], source) == 0) {
                printf("%s", dest);
            } else {
                printf("%s", words[j]);
            }
            if (j < r) printf(" ");
            else printf("\n");
        }
    }
    return 0;
}
```
### 问题 E: 字符串去特定字符
这道题总是卡在50%上面。而且用`scanf("%s\n%c", s, &c) != EOF`在我的电脑上可以正确运行，在网上就不可以了。所以有时还是用gets和getchar()。
这里，我发现有时候不一定要使用strlen函数，直接用结束符判断就很好，**这也是结束符的本意**。
```c
#include <cstdio>

int main() {
    char s[1000], c; 
    
    while (gets(s) != NULL) {
        c = getchar();
        for (int i = 0; s[i]; i++) 
            if (s[i] != c) printf("%c", s[i]);
        printf("\n");
        getchar();
    }
    return 0;
}
```
### 问题 F: 数组逆置
```c
#include <cstdio>
#include <cstring>

int main() {
    char s[210];
    while (gets(s) != NULL) { //可能有空格
        for (int i = strlen(s) - 1; i >= 0; i--) 
            printf("%c", s[i]);
        printf("\n");
    }
    return 0;
}
```
### 问题 G: 比较字符串
这里我没有直接用字符串函数strlen，不过我觉得strlen可能就是这么写的。
```c
#include <cstdio>

int main() {
    char s[100], r[100];
    int m;
    scanf("%d", &m);
    while (m--) {
        scanf("%s%s", s, r);
        int slen = 0, rlen = 0;
        for (slen = 0; s[slen]; slen++);
        for (rlen = 0; r[rlen]; rlen++);
        if (slen == rlen) {
            printf("%s is equal long to %s\n", s, r);
        } else if (slen > rlen) {
            printf("%s is longer than %s\n", s, r);
        } else {
            printf("%s is shorter than %s\n", s, r);
        }
    }
}
```
### ★ 问题 H: 编排字符串
题目描述：请输入字符串，最多输入4 个字符串，要求后输入的字符串排在前面，例如：
```
输入：EricZ
输出：1=EricZ
输入：David
输出：1=David 2=EricZ
输入：Peter
输出：1=Peter 2=David 3=EricZ
输入：Alan
输出：1=Alan 2=Peter 3=David 4=EricZ
输入：Jane
输出：1=Jane 2=Alan 3=Peter 4=David
```
- 输入：第一行为字符串个数m，接下来m行每行一个字符床，m不超过100，每个字符床长度不超过20。
- 输出：输出m行，每行按照样例格式输出，注意用一个空格隔开。
- 样例输入
	```
	5
	EricZ
	David
	Peter
	Alan
	Jane
	```
- 样例输出
	```
	1=EricZ
	1=David 2=EricZ
	1=Peter 2=David 3=EricZ
	1=Alan 2=Peter 3=David 4=EricZ
	1=Jane 2=Alan 3=Peter 4=David
	```
这题麻烦在于没怎么做过这种类型，相当于实现了**一个有大小限制的字符串容器**，新进入的字符串会把旧的字符串挤下来，甚至挤出去。
```c
#include <cstdio>
#include <cstring> 
int main() {
    int m;
    scanf("%d", &m);
    char s[5][35]; //使用1-4行 
    for (int i = 1; i <= m; i++) {
        int j, w = i >= 1 && i <= 4 ? i : 4; //获得字符串数目
        for (j = w; j > 1; j--) { //将旧的字符串移到下一个位置
            strcpy(s[j], s[j - 1]);
  		}
        scanf("%s", s[1]);  //永远在第一个位置存入新字符串
        for (int k = 1; k <= w; k++) { //按格式打印全部已存入的字符串
            if (k < w) printf("%d=%s ", k, s[k]); 
			else  printf("%d=%s\n", k, s[k]);
        }
    }
   return 0;
}
```
### 问题 I: 【字符串】回文串
**一行字符串，长度不超过255**。个人觉得，我这种判断回文串的方法最简单最好写。
```c
#include <cstdio>
#include <cstring>

int main() {
    char s[260];
    while (gets(s) != NULL) {
        int left = 0, right = strlen(s) - 1, flag = 1;
        for (int i = left, j = right; i < j; i++, j--) { //双指针滑动
            if (s[i] != s[j]) {
                printf("NO\n");
                flag = 0;
                break;
            }
        }
        if (flag) printf("YES\n");
    }
    return 0;
}
```
---
##  《算法笔记》4.1小节——算法初步->排序
### 问题 A: 排序
 对输入的n个数进行从小到大的排序并输出。
先自己实现一个插入排序的程序。
```c
#include <cstdio>

int main() {
    int n, a[120];
    while (scanf("%d", &n) != EOF) {
        for (int i = 0; i < n; i++) {
            scanf("%d", &a[i]);
        }
        //插入排序
        for (int i = 1; i < n; i++) {
            int temp = a[i], j;
            for (j = i; j > 0 && temp < a[j - 1]; j--) 
                a[j] = a[j - 1];
            a[j] = temp;
        }
        //输出
        for (int i = 0; i < n; i++) {
            printf("%d", a[i]);
            if (i < n - 1) printf(" ");
            else printf("\n");
        }
    }
}
```
再使用C++的库函数sort()直接排序。其使用格式`sort(要排序的数组, 数组元素末项的下一项, 排序函数)`。
```c
#include <cstdio>
#include <algorithm>
using namespace std;

int main() {
    int n, a[120];
    while (scanf("%d", &n) != EOF) {
        for (int i = 0; i < n; i++) {
            scanf("%d", &a[i]);
        }
        //排序
        sort(a, a + n);
        //输出
        for (int i = 0; i < n; i++) {
            printf("%d", a[i]);
            if (i < n - 1) printf(" ");
            else printf("\n");
        }
    }
}
```
### 问题 B: 特殊排序
```c
#include <cstdio>
#include <algorithm>
using namespace std;

int main() {
    int n, a[1010];
    while (scanf("%d", &n) != EOF) {
        for (int i = 0; i < n; i++) {
        	scanf("%d", &a[i]);
		} 
        if (n == 1) { //只有1个元素
            printf("%d\n", a[0]);
            printf("-1\n");
            continue;
        }
        sort(a, a + n);
        printf("%d\n", a[n - 1]);
        for (int i = 0; i < n - 1; i++) {
            printf("%d", a[i]);
            if (i < n - 2) printf(" ");
            else printf("\n");
        }
    }
    return 0;
}
```
### 问题 C: EXCEL排序 
**题目描述**
Excel可以对一组纪录按任意指定列排序。现请你编写程序实现类似功能。
对每个测试用例，首先输出1行“Case i:”，其中 i 是测试用例的编号（从1开始）。随后在 N 行中输出按要求排序后的结果，即：当 C=1 时，按学号递增排序；当 C=2时，按姓名的非递减字典序排序；当 C=3 时，**按成绩的非递减排序。当若干学生具有相同姓名或者相同成绩时，则按他们的学号递增排序**。
- 输入
测试输入包含若干测试用例。每个测试用例的第1行包含两个整数 N (N<=100000) 和 C，
其中 N 是纪录的条数，C 是指定排序的列号。以下有N行，每行包含一条学生纪录。每条学
生纪录由学号（6位数字，同组测试中没有重复的学号）、姓名（不超过8位且不包含空格
的字符串）、成绩（闭区间[0, 100]内的整数）组成，每个项目间用1个空格隔开。**当读到 N=0 时，全部输入结束，相应的结果不要输出**。
- 输出
对每个测试用例，首先输出1行“Case i:”，其中 i 是测试用例的编号（从1开始）。随后在 
N 行中输出按要求排序后的结果，即：当 C=1 时，按学号递增排序；当 C=2时，按姓名
的非递减字典序排序；当 C=3 时，按成绩的非递减排序。**当若干学生具有相同姓名或者相同成绩时，则按他们的学号递增排序**。
- 样例输入
	```
	4 1
	000001 Zhao 75
	000004 Qian 88
	000003 Li 64
	000002 Sun 90
	4 2
	000005 Zhao 95
	000011 Zhao 75
	000007 Qian 68
	000006 Sun 85
	4 3
	000002 Qian 88
	000015 Li 95
	000012 Zhao 70
	000009 Sun 95
	0 3
	```
- 样例输出
	```
	Case 1:
	000001 Zhao 75
	000002 Sun 90
	000003 Li 64
	000004 Qian 88
	Case 2:
	000007 Qian 68
	000006 Sun 85
	000005 Zhao 95
	000011 Zhao 75
	Case 3:
	000012 Zhao 70
	000002 Qian 88
	000009 Sun 95
	000015 Li 95
	```
这道问题的题目描述中，2和3的排序都要二级排序。
```c
#include <cstdio>
#include <cstring> 
#include <algorithm>
using namespace std;
struct student {
	int id; // 6
	char name[10];
	int score;
} excelBook[100050];

bool cmp1(struct student a, struct student b) {
	return a.id < b.id;  //按学号递增排序
}

bool cmp2(struct student a, struct student b) { //按姓名的非递减字典序排序
	if (strcmp(a.name, b.name)) return strcmp(a.name, b.name) < 0;  
	else return a.id < b.id;
}

bool cmp3(struct student a, struct student b) {
	if (a.score != b.score) return a.score < b.score;  //按成绩的递增排序
	else return a.id < b.id; 
}

int main() {
	int N, C, caseCount = 0;
	while (scanf("%d%d", &N, &C), N) {
		for (int i = 0; i < N; i++) {
			scanf("%d%s%d", &excelBook[i].id, excelBook[i].name, &excelBook[i].score);
		}
		switch (C) {
			case 1:  //当 C=1 时，按学号递增排序
		        sort(excelBook, excelBook + N, cmp1); break;
			case 2:  //当 C=2时，按姓名的非递减字典序排序
				sort(excelBook, excelBook + N, cmp2); break; 
			case 3:  //当 C=3 时，按成绩的非递减排序, 当若干学生具有相同姓名或者相同成绩时，
			//则按他们的学号递增排序
				sort(excelBook, excelBook + N, cmp3); break;
		}
		//打印输出 
		printf("Case %d:\n", ++caseCount);
		for (int i = 0; i < N; i++) {
			printf("%06d %s %d\n", excelBook[i].id, excelBook[i].name, excelBook[i].score);
		}
	}
	return 0;
}
```
### 问题 D: 字符串内排序
**注意输入的字符串中可能有空格**。因此要使用gets输入字符串，而且按字符从小到大排序，空格排在最前。
```c
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

int main() {
    char s[210];
    while (gets(s)) {
        int len = strlen(s);
        sort(s, s + len);
        puts(s);
    }
    return 0;
}
```
### 问题 E: Problem B
求矩阵每一行、每一列、主次对角线和，并排序。**即使只有一个元素，也要输出4个和**。
```c
#include <cstdio>
#include <algorithm>
using namespace std;
bool cmp(int a, int b) {
    return a > b;
}
int matrix[12][12];
int r[30];

int main() {
    int m;
    while (scanf("%d", &m) != EOF) {
        int len = 0;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < m; j++) {
                scanf("%d", &matrix[i][j]);
            }
        }
        //每一行与列求和
        for (int i = 0; i < m; i++) {
            int Rsum = 0, Csum = 0;
            for (int j = 0; j < m; j++) {
                Rsum += matrix[i][j];
                Csum += matrix[j][i];
            }
            r[len++] = Rsum;
            r[len++] = Csum;
        }
        //主次对角线求和
        int LDsum = 0, RDsum = 0;
        for (int i = 0; i < m; i++) {
            LDsum += matrix[i][i];
            RDsum += matrix[i][m - i - 1];
        }
        r[len++] = LDsum;
        r[len++] = RDsum;
        sort(r, r + len, cmp);
        for (int i = 0; i < len - 1; i++) {
            printf("%d ", r[i]);   
        }
        printf("%d\n", r[len - 1]);
    }
    return 0;
}
```
### 问题 F: 小白鼠排队 
```c
#include <cstdio>
#include <algorithm>
using namespace std;
struct rat {
    int weight;
    char color[15];
} mice[120];
bool cmp(struct rat a, struct rat b) {
	return a.weight > b.weight; //按重量从大到小, 重量各不相同	
}

int main() {
    int N;
    while (scanf("%d", &N) != EOF) {
        for (int i = 0; i < N; i++) {
            scanf("%d%s", &mice[i].weight, mice[i].color);
        }
        sort(mice, mice + N, cmp); 
        for (int i = 0; i < N; i++) {
        	puts(mice[i].color);  //输出小白鼠的颜色
		}
    }
}
```
### 问题 G: 中位数
```c
#include <cstdio>
#include <algorithm>
using namespace std;
int a[10010];
int main() {
	int N;
	while (scanf("%d", &N), N) {
		for (int i = 0; i < N; i++) {
			scanf("%d", &a[i]);
		}
		sort(a, a + N);
		int mid = (0 + N - 1) / 2;
		if (N % 2) {
			printf("%d\n", a[mid]);
		} else {
			//求最中间两个数的平均数，向下取整即可
			printf("%d\n", (a[mid] + a[mid + 1]) / 2);
		}
	}
	return 0;
}
```
### 问题 H: 整数奇偶排序 
```c
#include <iostream>
#include <algorithm>
using namespace std;
bool cmp(int a, int b) {
	return a > b;
}
int main() {
	int a[10], odd[10], even[10];
	while(cin>>a[0]>>a[1]>>a[2]>>a[3]>>a[4]
	      >>a[5]>>a[6]>>a[7]>>a[8]>>a[9]) {
      	int len1 = 0, len2 = 0;
	    for (int i = 0; i < 10; i++) {
	    	a[i] % 2 ? odd[len1++] = a[i] : even[len2++] = a[i];
		}
		sort(odd, odd + len1, cmp);
		sort(even, even + len2);
		//输出其中的奇数,并按从大到小排列; 输出其中的偶数,并按从小到大排列
		for (int i = 0; i < len1; i++) a[i] = odd[i];
		for (int j = len1; j < 10; j++) a[j] = even[j - len1];
		for (int k = 0; k < 10; k++) {
			cout << a[k];
			if (k < 9) cout << " ";
			else cout << endl;
		}
    }
    return 0;
}
```
### 问题 I: 排名
这题比较简单，计算好**合格的分数**后按题目要求排序即可。
```c
#include <cstdio>
#include <cstring>
#include <algorithm>
struct student {
	char id[25];
	int m, TotalScores;
} stu[1050];
using namespace std;
bool cmp(struct student a, struct student b) {
	if (a.TotalScores != b.TotalScores) return a.TotalScores > b.TotalScores;
	else return strcmp(a.id, b.id) < 0;
}
int main() {
	int N, M, G;
	while (scanf("%d", &N), N) {
		scanf("%d%d", &M, &G);  //记录考题数M、分数线G
		int exerScores[M + 1], cases = 0; //通过人数 
		for (int i = 1; i <= M; i++) 
			scanf("%d", &exerScores[i]);  //记录每道题的分数
		for (int i = 0; i < N; i++) {
			scanf("%s%d", stu[i].id, &stu[i].m); //记录考号和做出题数 
			int sum = 0, t;
			for (int j = 0; j < stu[i].m; j++) {  
				scanf("%d", &t);
				sum += exerScores[t];  //求出考生总分 
			}
			if (sum >= G) {
			   cases++;     //超出分数线 
			   stu[i].TotalScores = sum;  //记录合格的分数 
            }
		}
		sort(stu, stu + cases, cmp);
		printf("%d\n", cases);
		for (int i = 0; i < cases; i++) {
			printf("%s %d\n", stu[i].id, stu[i].TotalScores);
		}
	}
	return 0;
}
```
##  《算法笔记》4.2小节——算法初步->哈希
http://codeup.cn/contest.php?cid=100000582
在哈希这一块常用的问题包括：判断<=10^5^个正整数中某m个正整数**是否出现过、出现了多少次**——声明`bool/int hashTable[maxn] = {false}/{0}`。其做法本质是**直接将输入的整数作为数组的下标**来对这个数的性质进行统计，即`hash(key)=key`直接定址，是最常见也最实用的散列应用。**复杂一点还有二次映射**。把这两个思想掌握了就差不多了。
### 问题 A: 谁是你的潜在朋友
本题中也是使用了这种做法，直接将输入的数作为数组的下标。分别有两个数组，记录记录1-N每个人喜欢看的书号的数组person，记录1-M每本书的喜欢人数的数组note。如果要*通过某个人的编号查询到有多少人和这个人喜欢同一本书*的写法是：`note[person[i]]`。实质是在人与书号间形成了一种单射。接下来就简单了。
```c
#include <cstdio>

int main() {
	int N, M;
	while (scanf("%d%d", &N, &M) != EOF) {
		int person[N + 1], note[M + 1] = {0}, tmp;
		for (int i = 1; i <= N; i++) {
			scanf("%d", &tmp);
			person[i] = tmp; //记录1-N每个人喜欢看的书号 
			note[tmp]++; //记录每本书的喜欢人数 
		}		
		for (int i = 1; i <= N; i++) {
			if (note[person[i]] > 1) printf("%d\n", note[person[i]] - 1);
			else printf("BeiJu\n");
		} 
	}
	return 0;
}
```
### ★★★ 问题 B: 分组统计
题目描述：先输入一组数，然后输入其分组，按照分组统计出现次数并输出，参见样例。
- 输入：输入第一行表示样例数m，对于每个样例，第一行为数的个数n，接下来两行分别有n个数，第一行有n个数，第二行的n个数分别对应上一行每个数的分组，**n不超过100**。
- 输出：输出m行，**格式参见样例，按从小到大排**。
- 样例输入
	```
	1
	7
	3 2 3 8 8 2 3
	1 2 3 2 1 3 1
	```
- 样例输出
	```
	1={2=0,3=2,8=1}
	2={2=1,3=0,8=1}
	3={2=1,3=1,8=0
	```
解析：这一道题目可以说是集整数哈希思想于大成，值得仔细分析。
- 首先，题目中说输入的数据量不超过100，类别同样不超过100，但是没有说数据的具体范围，可能会在`整数作为数组的下标`超过数组下标范围，理论上来说，对于这种题目，**以空间换时间，要求数据应该不超过10^6^**。因此，统计输入的数据和组别是否已经出现，可以开hashTable[100100]这样大。
- 去重操作。在**有了标识数据是否出现的哈希表的情况**下，可以很简单的做到。
- 答案矩阵是一个二次映射的二维数组，可以直接通过组别和数据查到相应组别的数据出现的次数。
- 出现50%错误：**如果二维数组开的太小，很容易溢出**，所以记录第一行数据中最大的值，将其+10作为二维数组中的列数即可解决。

思路：
1. 输入第一行数据，存入data[]中，同时使用hashTable1[]去重，将去重后的数存入nums[]中，并排序。
2. 输入第二行组别，存入group[]中，同时使用hashTable2[]函数去重，将去重后的数存入g[]中，并排序。
3. 同时，将group[]和data[]对应映射到ans[][]中，ans[i][j]中i为分组，j为数。`a[i][j]为第i组的j数出现的次数`。
4. 遍历输出。
```c
#include <cstdio>
#include <algorithm>
using namespace std;

int main() {
	int m;
	scanf("%d", &m);
	while (m--) {
		int n, maxCol = 0, data[110], group[110]; //分别记录输入的数据和分组
		scanf("%d", &n);
		
		//nums记录输入的数据去重后的数据  
		int nums[120], len1 = 0;
		//使用哈希表对data进行存在标识, 以便去重 
		bool hashTable1[100100] = {false}; 
		for (int i = 0; i < n; i++) { 
			scanf("%d", &data[i]);      //边录入边去重 
			if (!hashTable1[data[i]]) { //如果这个数据尚未被记录 
	    		nums[len1++] = data[i]; 
	            hashTable1[data[i]] = true; 
			}
			//得到最大的数, 方便答案直接映射而不溢出 
			if (maxCol < data[i]) maxCol = data[i];  
		} 
		sort(nums, nums + len1); //数据从小到大存放在nums中, 无重复  
		
		//g记录输入的组别去重后的数据  
		int g[120], len2 = 0; 
		//使用哈希表对group进行存在标识, 以便去重
		bool hashTable2[100100] = {false};
		/*二维答案表，元素ans[g[i]][nums[j]]表示g[i]组中对应的nums[j]出现的次数 
		ans[i][j], i为分组, j为数, a[i][j]为第i组的j数出现的次数 */
		int ans[n + 10][maxCol + 10] = {0};  
		for (int i = 0; i < n; i++) {
			scanf("%d", &group[i]);
			ans[group[i]][data[i]]++; 
			if (!hashTable2[group[i]]) { //如果这个组别尚未被记录 
			   g[len2++] = group[i];  
			   hashTable2[group[i]] = true;
            } 
		}
		sort(g, g + len2); //组别从小到大存放在g中, 无重复 
	
		//输出结果 
		for (int i = 0; i < len2; i++) {
			printf("%d={", g[i]);
			for (int j = 0; j < len1; j++) {
				printf("%d=%d", nums[j], ans[g[i]][nums[j]]);
				if (j < len1 - 1) printf(",");
				else printf("}\n");
			}
		} 
	}
	return 0;
}
```
### 问题 C: Be Unique (20)
这道题的难点在于找出第一个独特的数，其实**只要按照输入的数组顺序，在记录次数的哈希表中查找只出现一次的数**，发现了一个就结束查找并输出，直到最后也没找到就是没有。
```c
#include <cstdio>
const int maxn = 10500;

int main() {
	int N; 
	while (scanf("%d", &N) != EOF) {
		int hashTable[maxn] = {0}, tmp[N];
		for (int i = 0; i < N; i++) {
			scanf("%d", &tmp[i]);
			hashTable[tmp[i]]++;
		}
		int win = 0;
		for (int i = 0; i < N; i++) {
			if (hashTable[tmp[i]] == 1) {
				printf("%d\n", tmp[i]);
				win = 1;
				break;
			}
		}
		if (!win) printf("None\n");
	}
}
```
### 问题 D: String Subtraction (20)
`all the characters are visible ASCII codes and white space`。输入的字符包括字母数字和各种符号这些可见ascll字符，还有空格，因此需要能容纳整个ascll码的哈希表。**这题虽然是字符串，但实际上还是整数哈希**。
```c
#include <cstdio>
int main() {
	char s1[10100], s2[10100];
	while (gets(s1) != NULL) {
		bool hashTable[130] = {false}; 
		gets(s2);
		for (int i = 0; s2[i]; i++) {
			hashTable[s2[i]] = true;  //将字符相应ascii码位置的哈希表设为true
		}
		for (int i = 0; s1[i]; i++) {
			if (!hashTable[s1[i]]) //如果字符没有出现在s2中
			   printf("%c", s1[i]);
		}
		printf("\n");
	}
}
```
## 《算法笔记》4.3小节——算法初步->递归
http://codeup.cn/contest.php?cid=100000583
**很多数据结构本身也是递归定义的**。
### 问题 A: 吃糖果 
**题目描述**
名名的妈妈从外地出差回来，带了一盒好吃又精美的巧克力给名名（盒内共有 N 块巧克力，20 > N >0）。
妈妈告诉名名每天可以吃一块或者两块巧克力。
如果N=1，则名名第1天就吃掉它，共有1种方案；
如果N=2，则名名可以第1天吃1块，第2天吃1块，也可以第1天吃2块，共有2种方案；
如果N=3，则名名第1天可以吃1块，剩2块，也可以第1天吃2块剩1块，所以名名共有2+1=3种方案；
如果N=4，则名名可以第1天吃1块，剩3块，也可以第1天吃2块，剩2块，共有3+2=5种方案。
假设名名每天都吃巧克力，问名名共有多少种不同的吃完巧克力的方案。
- 输入：输入只有1行，即整数N。 
- 输出：可能有多组测试数据，对于每组数据，输出只有1行，即名名吃巧克力的方案数。 
- 样例输入
	```
	1
	2
	4
	```
- 样例输出
	```
	1
	2
	5
	```
每次吃的时候，名名可以吃一块巧克力，或者吃两块巧克力，也就是说，每次吃的时候，有两种吃法。
- 第一种吃法：第一次吃了一块巧克力，那么还剩下N-1块还没吃，**剩下的N-1块的吃法是f(n-1)种**。相信递归函数可以递归求解这个子问题。
- 第二种跳法：第一次吃了两块块巧克力，那么还剩下N-2块还没吃，**剩下的N-2块的吃法有f(n-2)种**。同样相信递归函数可以递归求解这个子问题。
- 所以，N块巧克力的全部吃法就是这两种吃法之和了，即 f(n) = f(n-1) + f(n-2)。

这个题目可以有多种解法，不过大体是分治的思路(循环实现)。将一个`大的问题F(N)`分成多个`小的性质相同的问题F(N-1)和F(N-2)`，运用递归或非递归求解这些子问题，然后合并起来。动态规划的实际代码如下，就是变形的斐波拉契数列。
```c
#include <cstdio>
int a[30] = {0, 1, 2};

int main() {
    int N;
    for (int i = 3; i <= 20; i++) {
        a[i] = a[i - 1] + a[i - 2];
    }
    while (scanf("%d", &N) != EOF) {
        printf("%d\n", a[N]);
    }
    return 0;
}
```
### 问题 B: 数列
题目描述：编写一个求斐波那契数列的递归函数，输入n 值，使用该递归函数，输出如下图形（参见样例）。
- 输入：输入第一行为样例数m，接下来有m行每行一个整数n，n不超过10。
- 输出：对应每个样例输出要求的图形(参见样例格式)。
- 样例输入
```
1
6
```
- 样例输出
```
          0
        0 1 1
      0 1 1 2 3
    0 1 1 2 3 5 8
  0 1 1 2 3 5 8 13 21
0 1 1 2 3 5 8 13 21 34 55
```
出于对递归求解斐波那契数列(这绝对不是分治！)的厌恶，我还是选择了循环。
```c
#include <cstdio>

int main() {
    int fib[25] = {0, 1, 1}; //f(0) = 0
    for (int i = 3; i <= 20; i++)
        fib[i] = fib[i - 1] + fib[i - 2];
    int m, n;
    scanf("%d", &m);
    while (m--) {
        scanf("%d", &n);
        for (int i = 1; i <= n; i++) { //打印n层
            int spaceNum = (n - i) * 2; //打印空格
            for (int l = 0; l < spaceNum; l++) {
                printf(" ");
            }
            for (int j = 0; j < 2 * i - 1; j++) {
                printf("%d", fib[j]);
                if (j < 2 * i - 2) printf(" ");
                else printf("\n");
            }
        }
    }
    return 0;
}
```
### ★★ 问题 C: 神奇的口袋
**题目描述**
有一个神奇的口袋，**总的容积是40**，用这个口袋可以变出一些物品，这些物品的总体积必须是40。John现在有n个想要得到的物品，每个物品的体积分别是a1，a2……an。John可以从这些物品中选择一些，如果选出的物体的总体积是40，那么利用这个神奇的口袋，John就可以得到这些物品。现在的问题是，John有多少种不同的选择物品的方式。
- 输入：输入的第一行是正整数n (1 <= n <= 20)，表示不同的物品的数目。接下来的n行，每行有一个1到40之间的正整数，分别给出a1，a2……an的值。
- 输出：输出不同的选择物品的方式的数目。
- 样例输入
	```
	2
	12
	28
	3
	21
	10
	5
	```
- 样例输出
	```
	1
	0
	```
在我刷LeetCode的**数组-回溯算法中的子集问题**与这个问题十分相似，参照一下，都是对一棵树的深度遍历。**递归边界是40**，**递归式**(**缩小问题范围**)**是通过移动数组指针的方式进行的**。https://blog.csdn.net/myRealization/article/details/97446896
```c
#include <cstdio>
const int maxn = 25, Capacity = 40;
int cnt;  //合法方案的数目 

/* 相当于求出stuffs[]集合的真子集, 并判断总重量是否合适 */ 
void MagicalDFS(int weight, int stuffs[], int pos) {
	if (weight == Capacity) { //进入这里的就是合法方案 
		cnt++;  //方案+1 
		return;
	} 
	for (int i = pos; stuffs[i] != 0; i++) {
		//printf("weight = %d + %d\n", weight, stuffs[i]); 
		weight += stuffs[i];
		if (weight <= Capacity) {
		   MagicalDFS(weight, stuffs, i + 1); //深入遍历下一层 
		}
		weight -= stuffs[i]; //这一条路径遍历到底部, 从下一层返回到本层的状态 
	}
}

int main() {
	int n;
	while (scanf("%d", &n) != EOF) {
		int a[maxn] = {0}; 
		for (int i = 0; i < n; i++) {
			scanf("%d", &a[i]);
		}
		cnt = 0;
		MagicalDFS(0, a, 0);
		printf("%d\n", cnt);
	}
}
```
### ★★ 问题 D: 八皇后
**题目描述**：会下国际象棋的人都很清楚：皇后可以在横、竖、斜线上不限步数地吃掉其他棋子。如何将8个皇后放在棋盘上（有8 * 8个方格），使它们谁也不能被吃掉！这就是著名的八皇后问题。
对于某个满足要求的8皇后的摆放方法，定义一个皇后串a与之对应，即a=b1b2...b8，其中bi为相应摆法中第i行皇后所处的列数。已经知道8皇后问题一共有92组解（即92个不同的皇后串）。
给出一个数b，要求输出第b个串。串的比较是这样的：皇后串x置于皇后串y之前，当且仅当将x视为整数时比y小。
- 输入：第1行是测试数据的组数n，后面跟着n行输入。每组测试数据占1行，包括一个正整数b(1 <= b <= 92)
- 输出：输出有n行，每行输出对应一个输入。输出应是一个正整数，是对应于b的皇后串。 
- 样例输入
	```
	3
	6
	4
	25
	```
- 样例输出
	```
	25713864
	17582463
	36824175
	```
在我刷LeetCode的**回溯算法中的N皇后问题**与这个问题同出一源，参照一下，思路基本一样。https://blog.csdn.net/myRealization/article/details/97446896
```c
#include <cstdio>
#include <algorithm>
using namespace std;
//n为皇后的个数, 哈希表用来记录整数x是否已经在col中, col为不同行皇后所在列的一个排列 
int n, hashTable[10] = {false}, col[10] = {0}, QueenStringList[100], p = 1; //皇后串的结果表 

void QueenDFS(int CurRow) {
	if (CurRow == n + 1) {
		for (int i = 1; i <= 8; i++) 
			QueenStringList[p] = QueenStringList[p] * 10 + col[i];
		p++;
		return;
	}
	for (int x = 1; x <= 8; x++) {   //第x列 
		if (hashTable[x] == false) { //第x列还没有皇后 
			bool flag = true;        //flag为true表示和之前的皇后不会冲突 
			for (int pre = 1; pre < CurRow; pre++) {
			 	if (abs(CurRow - pre) == abs(x - col[pre])) {
			 	    flag = false;    //同之前的皇后在一条对角线上冲突 
					break;	
		        }
		    }
		    if (flag) { //如果可以摆放在这一列 
		    	hashTable[x] = true; //这一列已经被占用 
		    	col[CurRow] = x;     //令当前行的列号为x 
				QueenDFS(CurRow + 1); //递归处理第curRow+1行的皇后该摆放在哪一列
				hashTable[x] = false; //递归完毕, 无论是否这次递归抵达边界, 都还原其为未占用  
			}	
		}
	}
}
int main() {
    int m;
    scanf("%d", &m);
    n = 8; //启动为8皇后 
    QueenDFS(1);
    while (m--) {
        int b;
        scanf("%d", &b);
        printf("%d\n", QueenStringList[b]);
    }
    return 0;
}
```

## 《算法笔记》4.4小节——算法初步->贪心
http://codeup.cn/contest.php?cid=100000584
### ★★ 问题 A: 看电视
**题目描述**
暑假到了，小明终于可以开心的看电视了。但是小明喜欢的节目太多了，他希望尽量多的看到完整的节目。
现在他把他喜欢的电视节目的转播时间表给你，你能帮他合理安排吗？
- 输入
输入包含多组测试数据。每组输入的第一行是一个整数n（n<=100），表示小明喜欢的节目的总数。
接下来n行，每行输入两个整数si和ei（1<=i<=n），表示第i个节目的开始和结束时间，为了简化问题，每个时间都用一个正整数表示。
当n=0时，输入结束。
- 输出
对于每组输入，输出能完整看到的电视节目的个数。
- 样例输入
	```
	12
	1 3
	3 4
	0 7
	3 8
	15 19
	15 20
	10 15
	8 18
	6 12
	5 10
	4 14
	2 9
	0
	```
- 样例输出
	```
	5
	```
很有意思的区间贪心问题，这里**总是先选择左端点最大的区间**，**总是先选择右端点最小的区间也是可行的**。

与之类似的是区间选点问题，`给定N个闭区间[x, y]，求最少需要确定多少个点才能保证每个闭区间中都至少存在一个点`。例如对闭区间`[1, 4]、[2, 6]、[5, 7]`来说需要两个点如3、5，才能确保每个闭区间内都至少有一个点。代码写法也是类似的。
```c
#include <cstdio>
#include <algorithm>
using namespace std;
struct Inteval {
	int left, right; //左右区间端点表示 
};
bool cmp(struct Inteval a, struct Inteval b) {
	if (a.left != b.left) return a.left > b.left; // 按区间左端点大小从大到小排序 
	else return a.right < b.right;  //左端点相同, 按右端点大小从小到大排序 
}

int main() {
	int N;
	while (scanf("%d", &N), N) {
		struct Inteval a[N]; 
		for (int i = 0; i < N; i++) {
			scanf("%d%d", &a[i].left, &a[i].right);
		}
		sort(a, a + N, cmp); //把区间排序 
		//ans为不相交区间个数, lastLeft为上一个被选中区间的左端点 
		int ans = 1, lastLeft = a[0].left;  
		for (int i = 1; i < N; i++) {
			if (a[i].right <= lastLeft) { //如果该区间右端点在lastLeft左边 
				ans++;  //不相交区间个数加1 
				lastLeft = a[i].left; //以a[i]作为新选中的区间 
			}
		}
		printf("%d\n", ans);
	} 
	return 0;
}

```
### 问题 B: 出租车费
```c
#include <cstdio>
#include <algorithm>
using namespace std;

int main() {
	int N;
	while (scanf("%d", &N), N) { //一个正整数n表示整个行程的公里数
		if (N <= 4) {  //不足4公里肯定10元 
			printf("%d\n", 10);
			continue;
		} else if (N <= 8) { //4-8公里第二段直接计算 
			printf("%d\n", 10 + (N - 4) * 2); //每公里最小单价
			continue; 
		} else { 
            //超过8公里优先走8公里的第二段与第一段的混合 
			int part = N / 8, last = N % 8;
			if (last == 0) printf("%d\n", part * 18);
			else if (last <= 4) { //超过8公里最后的部分在4以内走2.4元那段
				printf("%.1f\n", part * 18 + last * 2.4);
			} else { //在4-8的话还走第一二段
				printf("%d\n", part * 18 + 10 + (last - 4) * 2);
			}
		}
		
	}
	return 0;
}
```
### ★★★ 问题 C: To Fill or Not to Fill 
提示：该题目所要解决的问题是：给定若干加油站信息，问能否驾驶汽车行驶一定的距离。**如果能够行驶完全程，则计算最小花费**。若不能行驶完全程，则**最远能够行驶多长距离**。

拿到这一题，**首先判断汽车是否能够行驶到终点**。什么情况下汽车无法行驶到终点呢？`两种情况：起点根本就没有加油站，汽车无法启动；或者中途两个加油站之间的距离大于加满油后汽车能够行驶的最大距离`。前者汽车行驶的最大距离为0.00，而后者最大距离为当前加油站的距离加上在这个加油站加满油后能够行驶的最大距离。在这里，需要**将加油站按到杭州的距离从小到大排序**。

接下来**在能够行驶到终点的情况下**计算最小花费。我们首先从路程来考虑，如果在路上，我们能够使用最便宜的汽油，当然就在那个加油站加油了。所以**从起点开始遍历每个加油站**。假设遍历到了第i个加油站，我们现在来**判断在加油站i应该加多少油**。
- 在第i个加油站的作用范围内寻找有没有更为便宜的加油站，如果有，则**下次使用这个加油站的油**j:
	* 如果可以直接到达j，就不加油；
	* 不然应该在站点i加一定的油行驶到那个加油站j，即touch=nodes[j].dis。
+ 如果**没有找到更为便宜的加油站则可以在第i个加油站加满油**，然后行驶到下一个加油站再进行判断。

单点测试，有难度，而且我不太明白 。
```c
#include <cstdio>
#include <algorithm>
using namespace std;
typedef struct gasStation {
	double p, d, s; //p: 价格，d：据出发点距离，s：离上个站点的间距
} gs;
bool cmp(gs a, gs b) {
	return a.d < b.d; 
}

int main() {
	double C, D, Davg; 
	int n;  //从杭州到目的地的加油站数目 
	while (scanf("%lf%lf%lf%d", &C, &D, &Davg, &n) != EOF) {
		gs gstat[n + 1];
		double maxl = C * Davg; //满油后最大行驶距离 
		for (int i = 0; i < n; i++) {
			scanf("%lf%lf", &gstat[i].p, &gstat[i].d);
		}
		gstat[n].p = 0; //目的地 
		gstat[n].d = D;
		sort(gstat, gstat + n + 1, cmp);
		for (int i = 1; i <= n; i++) {
			gstat[i].s = gstat[i].d - gstat[i - 1].d;
		}
		int flag = 1;
		/* 如果第一个加油站不在杭州, 没法加油 */
		if (gstat[0].d > 0) {
			flag = 0;
			printf("The maximum travel distance = 0.00\n");
		} else {
			for (int i = 1; i <= n; i++) {
				if (gstat[i].s > maxl) { //有站点不可达 
					flag = 0;
					printf("The maximum travel distance = %.2f\n", gstat[i - 1].d + maxl);
					break;
				}
			}
		}
		double cost = 0, nowTank = 0;
		int nowSta = 0, signal = 1; 
		if (flag) { 
			for (int i = 0; i < n; i++) {
				if (i != nowSta) continue;
				for (int j = i + 1; j <= n && (gstat[j].d - gstat[i].d) <= maxl; j++) {
					if (gstat[j].p < gstat[nowSta].p) {
		        	   if (nowTank < (gstat[j].d - gstat[nowSta].d) / Davg) {
		        	       cost += 	gstat[nowSta].p * ((gstat[j].d - gstat[nowSta].d) / Davg - nowTank);
		        	       nowTank = 0;
					   } else nowTank -= (gstat[j].d - gstat[nowSta].d) / Davg;
					   nowSta = j;
					   signal = 1;
					   break;
					} else signal = 0;
				}
		        if (!signal) {
		        	cost += gstat[i].p * (C - nowTank);
		        	nowTank = C - (gstat[i + 1].s / Davg);
		        	nowSta++;
				}
			} 
			printf("%.2f\n", cost);
		}
	}
	return 0;
}
```
### 问题 D: Repair the Wall 
先用更长的砖头修墙。
```c
#include <cstdio>
#include <algorithm>
using namespace std;
bool cmp(int a, int b) {return a > b;}

int main() {
	int L, N;
	while (scanf("%d%d", &L, &N) != EOF) {
		int a[N + 1], sum = 0;
		for (int i = 0; i < N; i++) {
			scanf("%d", &a[i]);
			sum += a[i];
		}
		if (sum < L) printf("impossible\n");
		else {
			sort(a, a + N, cmp); //砖按长度从长到短排序 
			int cnt = 0;
			for (int i = 0; i < N; i++) {
				if (L > a[i]) {
					L -= a[i];
					cnt++;
				} else {
					L = 0;
					cnt++;
					break;
				}
			}
			printf("%d\n", cnt);
		}
	}	
	return 0;
}
```
### 问题 E: FatMouse's Trade 
这道题简单，**能够多换的房间**就多换。
```c
#include <cstdio>
#include <algorithm>
using namespace std;
struct room {
	double J, F; //J: javabeans F: cat food 
	double rate; // J/F
};
bool cmp(struct room a, struct room b) {
	return a.rate > b.rate;
}
int main() {
	int m, n;
	while (scanf("%d%d", &m, &n), m != -1 && n != -1) {
		struct room house[n + 1];
		for (int i = 0; i < n; i++) {
			scanf("%lf%lf", &house[i].J, &house[i].F);
			house[i].rate = house[i].J / house[i].F;
		}
		sort(house, house + n, cmp);
		double javabeans = 0.0; 
		/* 从猫食物换取Javabeans的兑换比率最高的换起 */
		for (int i = 0; i < n; i++) {
			/* 如果老鼠的猫食物够多, 可以将这个房间中的java豆全部换完 */
			if (m >= house[i].F) {
				javabeans += house[i].J;
				m -= house[i].F; 
			} else { /* 如果猫食物不够多, 能换多少就换多少 */
				javabeans += house[i].rate * m;
				m = 0; break;
			}
		}
		printf("%.3f\n", javabeans);
	}
	return 0;
}
```
### 问题 F: 迷瘴
`溶质体积/溶液体积=浓度`。从浓度小的溶液开始配。
```c
#include <cstdio>
#include <algorithm>
using namespace std;
bool cmp(int a, int b) {return a > b;}

int main() {
	int C, N;
	while (scanf("%d", &C) != EOF) {
		while (C--) {
		    int n, V, W;
		    scanf("%d%d%d", &n, &V, &W);
			double p[n + 1];
			for (int i = 0; i < n; i++) scanf("%lf", &p[i]);
			sort(p, p + n);
			double solP = 0.0; int solV = 0;
			
			for (int i = 0; i < n; i++) {
				if ((p[i] * V + solP * solV) / (V + solV) <= W) {
					solP = (p[i] * V + solP * solV) / (V + solV);
					solV += V;
				} else break;
			}
			if (!solV) printf("0 0.00\n");
			else printf("%d %.2lf\n", solV, solP / 100);  
		} 	
	}
	return 0;
}
```
### ★ 问题 G: 找零钱
**题目描述**：小智去超市买东西，买了不超过一百块的东西。收银员想尽量用少的纸币来找钱。
纸币面额分为50 20 10 5 1 五种。请在知道要找多少钱n给小明的情况下，输出纸币数量最少的方案。 1<=n<=99。
- 输入：有多组数据  1<=n<=99;
- 输出：对于每种数量不为0的纸币，输出他们的面值*数量，再加起来输出
- 样例输入
	```
	25
	32
	```
- 样例输出
	```
	20*1+5*1
	20*1+10*1+1*2
	```
```c
#include <cstdio>

int main() {
    int n;
    while (scanf("%d", &n) != EOF) {
        int hashTable[51] = {0}, money[5] = {50, 20, 10, 5, 1}, cnt = 0;
        for (int i = 0; i < 5 && n; i++) {
            while (n >= money[i]) { //优先减少最大额度的纸币
                n -= money[i];
                hashTable[money[i]]++; //数该额度的纸币的张数
                cnt++;
            }
        }
        for (int i = 0; i < 5; i++) {
            if (hashTable[money[i]]) {
                printf("%d*%d", money[i], hashTable[money[i]]);
                cnt -= hashTable[money[i]];
				if (cnt > 0) printf("+");
            	else printf("\n");
            } 
        }
    }
    return 0;
}
```
## 《算法笔记》4.5小节——算法初步->二分
http://codeup.cn/contest.php?cid=100000585
### 问题 A: 找x
```c
#include <cstdio>
int findx(int a[], int n, int x) {
	for (int i = 0; i < n; i++) 
        if (a[i] == x) return i;
    return -1;
}

int main() {
    int n;
    while (scanf("%d", &n) != EOF) {
        int a[n];
        for (int i = 0; i < n; i++)
            scanf("%d", &a[i]);
        int x;
        scanf("%d", &x);
        printf("%d\n", findx(a, n, x));
    }
    return 0;
}
```
### 问题 B: 打印极值点下标
必须得说这个和二分没有关系。
```c
#include <cstdio>

int main() {
    int n;
    
    while (scanf("%d", &n) != EOF) {
 	    while (n--) {
	        int k;
	        scanf("%d", &k);
	        int a[k], index[k], cnt = 0;
	        for (int i = 0; i < k; i++) {
	            scanf("%d", &a[i]);
	        }
	        if (a[0] != a[1]) index[cnt++] = 0;
	        for (int i = 1; i < k - 1; i++) {
	            if (
	                (a[i] > a[i - 1] && a[i] > a[i + 1]) ||
	                (a[i] < a[i - 1] && a[i] < a[i + 1])
	            ) index[cnt++] = i;
	        }
	        if (a[k - 1] != a[k - 2]) index[cnt++] = k - 1;
	        
	        for (int i = 0; i < cnt; i++) {
	            printf("%d", index[i]);
	            if (i < cnt - 1) printf(" ");
	            else printf("\n");
	        }
	    }	
	}
    return 0;
}
```
### 问题 C: 查找
```c
#include <cstdio>
int main() {
    int n;
    while (scanf("%d", &n) != EOF) {
        int a[n];
        for (int i = 0; i < n; i++) {
            scanf("%d", &a[i]);
        }
        int m;
        scanf("%d", &m);
        int t;
        while (m--) {
            scanf("%d", &t);
            int i;
            for (i = 0; i < n; i++) {
                if (a[i] == t) {
                    printf("YES\n");
                    break;
                }
            }
            if (i == n) printf("NO\n");
        }
    }
    return 0;
}
```
###  问题D: 习题5-15 二分法求方程的根
题目不可用?!
## 《算法笔记》4.6小节——算法初步->two pointers
http://codeup.cn/contest.php?cid=100000566
双指针的经典问题可以看我的LeetCode刷题记录中的283题-移动零。https://blog.csdn.net/myRealization/article/details/97446896
### ★★★ 问题 A: 二路归并排序(mergesort)递归法 [2*+]
书上的二路归并排序写得并不好，**主要是每一次都申请一个临时数组太浪费了**。而且这个题目测试用例有问题，我的代码线下可以正确运行，线上都是运行错误0……
啊，找到原因了，这个题目没有给出输出格式，且只有一个测试用例，把全代码的main函数改成下面的**结构和输出方式**就可以了。因此，程序正确的话显示为运行错误80。
```c
int main() {
    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; i++) scanf("%d", &a[i]);
    mergeSort(a, n);
    for (int i = 0; i < n; i++) printf("%d\n", a[i]);
    return 0;
}
```
```c
#include <cstdio>
#include <cstring>
#include <cstdlib>
/* 将数组A的[L1, R1]与[L2, R2]区间归并为有序区间到tmpArray, 再拷贝回A */
void merge(int A[], int tmpArray[], int L1, int R1, int L2, int R2) {
    int i = L1, j = L2, index = 0;
    while (i <= R1 && j <= R2) {
        if (A[i] <= A[j]) tmpArray[index++] = A[i++];
        else tmpArray[index++] = A[j++];
    }
    while (i <= R1) tmpArray[index++] = A[i++]; //将[L1, R1]剩余元素加入序列
    while (j <= R2) tmpArray[index++] = A[j++]; //将[L2, R2]剩余元素加入序列
    for (i = 0; i < index; i++) A[L1 + i] = tmpArray[i]; //将合并后的序列赋值回A
}

void MSort(int A[], int tmpArray[], int left, int right) {
    if (left < right) {
        int mid = (left + right) / 2;
        MSort(A, tmpArray, left, mid);
        MSort(A, tmpArray, mid + 1, right);
        merge(A, tmpArray, left, mid, mid + 1, right);
    }
}
void mergeSort(int A[], int N) {
    int *tmpArray = (int *)malloc(N * sizeof(int));
    MSort(A, tmpArray, 0, N - 1);
    free(tmpArray);
}

int a[100100];
int main() {
    int n;
    while (scanf("%d", &n) != EOF) {
    	memset(a, 0, sizeof(a));
	    for (int i = 0; i < n; i++) scanf("%d", &a[i]);
	    mergeSort(a, n);
	    for (int i = 0; i < n; i++) {
	        if (i > 0) printf(" ");
	        printf("%d", a[i]);
	    }  
	    printf("\n");
	}
    return 0;
}
```
### 问题 B: 基础排序III：归并排序
**题目描述**：归并排序是一个时间复杂度为O(nlogn)的算法，对于大量数据远远优于冒泡排序与插入排序。这是一道排序练习题，数据量较大，请使用归并排序完成。
- 输入：第一行一个数字n，代表输入的组数
其后每组第一行输入一个数字m，代表待排序数字的个数
其后m行每行一个数据，大小在1～100000之间，互不相等，最多有10万个数据。
- 输出：升序输出排好序的数据，每行一个数字

又写了一遍递归归并排序，这才是上一题的题目吧。 

```c
#include <cstdio>
#include <cstring>
#include <cstdlib>
void merge(int a[], int tmpArray[], int L1, int R1, int L2, int R2) {
    int i = L1, j = L2, index = 0;
    while (i <= R1 && j <= R2) {
        if (a[i] <= a[j]) tmpArray[index++] = a[i++];
        else tmpArray[index++] = a[j++];
    }
    while (i <= R1) tmpArray[index++] = a[i++];
    while (j <= R2) tmpArray[index++] = a[j++];
    for (int i = 0; i < index; i++) a[L1 + i] = tmpArray[i];
}

void msort(int a[], int tmpArray[], int left, int right) {
    int i = left, j = right, mid;
    if (left < right) {
        mid = (left + right) / 2;
        msort(a, tmpArray, left, mid);
        msort(a, tmpArray, mid + 1, right);
        merge(a, tmpArray, left, mid, mid + 1, right);
    }
}
void mergeSort(int a[], int n) {
    int* tmpArray = (int *)malloc(sizeof(int) * n);
    msort(a, tmpArray, 0, n - 1);
    free(tmpArray);
}
int a[100010];
int main() {
    int n, m;
    scanf("%d", &n);
    while (n--) {
    	memset(a, 0, sizeof(a));
        scanf("%d", &m);
        for (int i = 0; i < m; i++) 
			scanf("%d", &a[i]);
        mergeSort(a, m);
        for (int i = 0; i < m; i++) 
			printf("%d\n", a[i]);
    }
    return 0;
}
```
### 	问题 C: 快速排序 qsort [2*]
```c
#include <cstdio>

int partition(int a[], int left, int right) {
    int temp = a[left];    //将a[left]存进临时变量
    while (left < right) { //只要两者不相遇
        while (left < right && a[right] > temp) right--; //持续左移
        a[left] = a[right]; //将a[right]移至a[left]
        while (left < right && a[left] <= temp) left++;
        a[right] = a[left]; //将a[left]移至a[right]
    }
    a[left] = temp; //将temp移至left与right相遇的地方
    return left;    //返回枢纽元的位置即相遇的下标(分割点)
}
void qsort(int a[], int left, int right) {
    if (left < right) { //当前区间的长度超过1
        //将[left, right]按照a[left]一分为2
        int pos = partition(a, left, right); 
        qsort(a, left, pos - 1);  //对左区间递归
        qsort(a, pos + 1, right); //对右区间递归
    }   
}
void quickSort(int a[], int n) {
    qsort(a, 0, n - 1);
}

int a[6000];
int main() {
    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; i++) scanf("%d", &a[i]);
    quickSort(a, n);
    for (int i = 0; i < n; i++) printf("%d\n", a[i]);
    return 0;
}
```
### 	★★★ 问题 D: 二分递归快排（Qsort） [2*]
这两个题目都是写二分递归快排的，所以问题C我写的是始终采取第一个元素作为枢纽元的二分递归快排，
而这一题我写的是随机选择快排。 
```c
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>
using namespace std;

//选择随机主元, 对区间[left, right]进行划分
int randomPartition(int a[], int left, int right) {
    //生成[left, right]区间的随机数pos
    int pos = (int)(round(1.0 * rand() / RAND_MAX * (right - left) + left));
    swap(a[pos], a[left]); //交换a[pos]和a[left]
    int temp = a[left];
    while (left < right) {
        while (left < right && a[right] > temp) right--;
        a[left] = a[right];
        while (left < right && a[left] <= temp) left++;
        a[right] = a[left];
    }
    a[left] = temp;
    return left;
}
void qsort(int a[], int left, int right) {
    if (left < right) {
        int pos = randomPartition(a, left, right);
        qsort(a, left, pos - 1);
        qsort(a, pos + 1, right);
    }
}
void quickSort(int a[], int n) {
    qsort(a, 0, n - 1);
}

int a[100100];
int main() {
    srand((unsigned)time(NULL));
    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; i++) scanf("%d", &a[i]);
    quickSort(a, n);
    for (int i = 0; i < n; i++) printf("%d\n", a[i]);
    return 0;
}
```
## 《算法笔记》4.7小节——算法初步->其他高效技巧与算法
http://codeup.cn/contest.php?cid=100000587
### ★★ 问题 A: 求第k大数
- 第一种方法：将数组从大到小排序，取出第K大的元素，即取a[k-1]。使用快排可以达到O(nlogn)的时间复杂度。
```c
#include <cstdio>
#include <algorithm>
using namespace std;
bool cmp(int a, int b) {
    return a > b; //从大到小排序
}

int main() {
    int m, n;
    scanf("%d%d", &m, &n);
    int a[m];
    for (int i = 0; i < m; i++) 
        scanf("%d", &a[i]);
    sort(a, a + m, cmp);
    printf("%d", a[n-1]);
    return 0;
}
```
实际上这道题是**考验写随机选择算法**的，代码如下，我的版本为randSelect2，加强版的类似递归二分查找。书上的讲解讲错了，**代码实际上求的是第k小的数，第k大的数就是第(n-k+1)小的数**，输入参数改一下就行了。
```c
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>
using namespace std;

//选择随机主元, 对区间[left, right]进行划分
int randPartition(int a[], int left, int right) {
    //生成[left, right]区间的随机数pos
    int pos = (int)(round(1.0 * rand() / RAND_MAX * (right - left) + left));
    swap(a[pos], a[left]); //交换a[pos]和a[left]
    int temp = a[left];
    while (left < right) {
        while (left < right && a[right] > temp) right--;
        a[left] = a[right];
        while (left < right && a[left] <= temp) left++;
        a[right] = a[left];
    }
    a[left] = temp;
    return left;
}

int randSelect(int a[], int left, int right, int K) {
	if (left == right) return a[left];
    int pos = randPartition(a, left, right); //划分后主元位置为pos
	int M = pos - left + 1; //a[p]是a[left, right]中的第M小
	
	if (K == M) return a[pos]; //找到第K小的数 
	if (K < M) return randSelect(a, left, pos - 1, K); //向左边找第K小的数 
	else return randSelect(a, pos + 1, right, K - M); //向右边找第K-M小 
}
int randSelect2(int a[], int left, int right, int K) {
	while (left <= right) {
		int pos = randPartition(a, left, right); //划分后主元位置为pos
		int M = pos - left + 1; //a[p]是a[left, right]中的第M小		
		if (K == M) return a[pos]; //找到第K小的数 
		if (K < M) randSelect(a, left, pos - 1, K); //向左边找第K小的数 
		else randSelect(a, pos + 1, right, K - M); //向右边找第K-M小 
	}
}
int a[1000100];
int main() {
    srand((unsigned)time(NULL));
    int m, n;
    scanf("%d%d", &m, &n);
    for (int i = 0; i < m; i++) scanf("%d", &a[i]);
    int ans = randSelect2(a, 0, n - 1, n);
    printf("%d\n",  m - ans + 1);
    return 0;
}
```
---
## 《算法笔记》5.1小节——数学问题->简单数学
http://codeup.cn/contest.php?cid=100000588

### 问题 A: 守形数
这个题目检查低位，其实就是**将一个十进制数除了最高位外的其他位转换为另一个十进制数**(即它的低位部分)。
```c
#include <cstdio>
int main() {
    int N;
    while (scanf("%d", &N) != EOF) {
        int dN = N * N, low = 0, product = 1, flag = 0;
        while (dN / 10) { //dN还有最后一位(高位)时退出
            low += (dN % 10) * product;  // 逐渐检查低位
            if (low == N) {
                printf("Yes!\n");
                flag = 1;
                break;
            }
            product *= 10;
            dN /= 10;
        }
        if (!flag) printf("No!\n");
    }
}
```
### 问题 B: 反序数
一开始我还怀疑有没有这种数，不过确实有一个1089，只要**检查**1002(甚至更大一点)**到1111为止的数**就可以了，之后的数字*9会超出四位数。知道答案的话也可以直接打印1089(?)。
```c
#include <cstdio>

int main() {
    for (int i = 1002; i <= 1111; i++) {
        int reverseNum = i / 1000 + i % 1000 / 100 * 10 + i % 100 / 10 * 100 + i % 10 * 1000;
        if (i * 9 == reverseNum) printf("%d\n", i);
    }
    return 0;
}
```
### 问题 C: 百鸡问题
朴素的暴力解法。不过我之前不小心写出来这样的判断条件`if (i + j + k == 100 && 5 * i + 3 * j + (double)(1 / 3) * k <= n)`，导致最小鸡的价格为0(?)……(double)类型转换使用要细心。
```c
#include <cstdio>

int main() {
    double n;
    while (scanf("%lf", &n) != EOF) {
        //暴力解法
        int big = n / 5, small = n / 3, little = 3 * n; //整数只 
        for (int x = 0; x <= big; x++) {
            for (int y = 0; y <= small; y++) {
                for (int z = 0; z <= little; z++) {
                	double sum = 5 * x + 3 * y + (double)z / 3; //浮点数总价 
                    if (x + y + z == 100 && sum <= n) {
                        printf("x=%d,y=%d,z=%d\n", x, y, z);
                    }
                }
            }
        }
    }
    return 0;
}
```
### 问题 D: abc
```c
#include <cstdio>
int main() {
    for (int a = 1; a <= 9; a++) {
        for (int b = 1; b <= 9; b++) {
            for (int c = 0; c <= 9; c++) {
                if (a * 100 + b * 10 + c + b * 100 + c * 10 + c == 532) {
                    printf("%d %d %d\n", a, b, c);
                }
            }
        }
    }
    return 0;
}
```
### 问题 E: 众数
这个题目容易粗心，我提交错了两次。
```c
#include <cstdio>
int main() {
    int a[20];
    while (scanf("%d", &a[0]) != EOF) {
    	int hashTable[12] = {0}; //1-10 要放在里面 
    	hashTable[a[0]]++; // 容易忘记统计第一个输入的数字 
        for (int i = 1; i < 20; i++) {
            scanf("%d", &a[i]);
            hashTable[a[i]]++;
        }
        int m = 0;
        for (int i = 1; i <= 10; i++) { //只有某个数出现次数更大时更新m
            if (hashTable[i] > hashTable[m]) m = i;
        }
        printf("%d\n", m);
    }
    return 0;
}
```
### 问题 F: 计算两个矩阵的乘积
这个题目，其实也简单，看看矩阵乘法的定义就行了。不过在结果矩阵上，因为有多组输入数据，因此必须在while内部申请，并且每一次申请要顺便清零，不然就会`答案错误50`。
```c
#include <cstdio>
int main() {
    int x, i, j, k;
    while (scanf("%d", &x) != EOF) {
    	int a[2][3], b[3][2], c[2][2] = {0};
        for (i = 0; i < 2; i++) {
        	for (j = 0; j < 3; j++) {
        		if (!i && !j) a[0][0] = x;
        		else scanf("%d", &a[i][j]);
			}
        }
        for (i = 0; i < 3; i++) {
            for (j = 0; j < 2; j++) {
                scanf("%d", &b[i][j]);
            }
        }
        for (i = 0; i < 2; i++) {
            for (j = 0; j < 2; j++) {
                for (k = 0; k < 3; k++) {
                	c[i][j] += a[i][k] * b[k][j];
				} 
            }
        }
        for (i = 0; i < 2; i++) {
        	for (j = 0; j < 2; j++) {
        		printf("%d", c[i][j]);
        		if (j < 2) printf(" ");
			}
			printf("\n");
		}
    }
    return 0;
}
```
### 问题 G: 加法等式
???和问题D一样的题目？
```c
#include <cstdio>
int main() {
    for (int a = 1; a <= 9; a++) {
        for (int b = 1; b <= 9; b++) {
            for (int c = 0; c <= 9; c++) {
                if (a * 100 + b * 10 + c + b * 100 + c * 10 + c == 532) {
                    printf("%d %d %d\n", a, b, c);
                }
            }
        }
    }
}
```
### 问题 H: 整数和
等差数列求和。
```c
#include <cstdio>
int main() {
    int m;
    scanf("%d", &m);
    while (m--) {
        int N;
        scanf("%d", &N);
        int ans = N >= 0 ? 3 * (N * N + N) / 2 : - 3 * (N * N - N) / 2;
        printf("%d\n", ans);
    }
    return 0;
}
```
### 问题 I: 反序相等
又一个同样的题目……
```c
#include <cstdio>

int main() {
    printf("1089\n");
    return 0;
}
```
### 问题 J: 多项式的值
可以像我这么算多项式求和，也可以使用秦九韶的方法。
```c
#include <cstdio>
int f(int max, int coef[], int x) {
    int product = 1, sum = 0;
    for (int i = 0; i <= max; i++) {
        sum += coef[i] * product;
        product *= x;
    }
    return sum;
}
int f1(int max, int coef[], int x) { //多项式系数由低阶向高阶存储
	int sum = 0;
	for (int i = max; i >= 0; i--) {
		sum = sum * x + coef[i]; //秦九韶算法
	}
	return sum;
}
int main() {
    int m;
    scanf("%d", &m);
    while (m--) {
        int n;
        scanf("%d", &n);
        int coef[n + 1];
        for (int i = 0; i <= n; i++) {
            scanf("%d", &coef[i]);
        }
        int x; 
        scanf("%d", &x);
        printf("%d\n", f1(n, coef, x));
    }
    return 0;
}
```
### 问题 K: 迭代求立方根
 立方根的逼近迭代方程是`y(n+1) = y(n)*2/3 + x/(3*y(n)*y(n))`，其中y0=x。求给定的x经过n次迭代后立方根的值。
- 输入：输入有多组数据。每组一行，输入x n。
- 输出：迭代n次后的立方根，double精度,保留小数点后面六位。
- 样例输入
	```
	4654684 1
	65461 23
	```
- 样例输出
	```
	3103122.666667
	40.302088
	```
```c
#include <cstdio>
int main() {
    double x;
    int n;
    while (scanf("%lf%d", &x, &n) != EOF) {
        double rootX = x;
        for (int i = 0; i < n; i++) {
            rootX = rootX * 2 / 3 + x / (3 * rootX * rootX);
        }
        printf("%.6f\n", rootX);
    }
    return 0;
}
```
### 问题 L: 与7无关的数
```c
#include <cstdio>
int main() {
    int n;
    while (scanf("%d", &n) != EOF) {
        int sum = 0;
        for (int i = 1; i <= n; i++) {
            if (i % 7 != 0 && i % 10 != 7 && i / 10 != 7) {
                sum += i * i;
            }
        }
        printf("%d\n", sum);
    }
    return 0;
}
```
### 问题 M: 鸡兔同笼
这个问题可以描述成任给一个整数 N，`如果N是奇数，输出0 0`(鸡兔没有奇数只脚)，否则`如果N是4的倍数，输出N / 4 N / 2`(最少全是兔子，最多全是鸡)，`如果N 不是4 的倍数，输出N/4+1 N/2`(最少是N/4只兔子+1只鸡，最多全是鸡)。这是一个一般的计算题，只要实现相应的判断和输出代码就可以了。
题目中说明了输入整数在一个比较小的范围内，所以只需要考虑整数运算。 
```c
#include <cstdio>

int main() {
    int n;
    scanf("%d", &n);
    while (n--) {
        int N;
        scanf("%d", &N);
        if (N & 1) printf("0 0\n");
        else {
            if (N % 4) {
                printf("%d %d\n", N / 4 + 1, N / 2);
            } else {
                printf("%d %d\n", N / 4, N / 2);
            }
        }
    }
    return 0;
}
```
## 《算法笔记》5.2小节——数学问题->最大公约数与最小公倍数
http://codeup.cn/contest.php?cid=100000589
### 问题 A: Least Common Multiple
**题目描述**
`The least common multiple (LCM) of a set of positive integers 
is the smallest positive integer which is divisible by all the 
numbers in the set.`For example, the LCM of 5, 7 and 15 is 105.
- 输入
Input will consist of multiple problem instances. The first line of the input will contain a single integer indicating the number of problem instances. Each instance will consist of a single line of the form m n1 n2 n3 ... nm where m is the number of integers in the set and n1 ... nm are the integers. All integers will be positive and lie within the range of a 32-bit integer.
- 输出
For each problem instance, output a single line containing the corresponding LCM. All results will lie in the range of a 32-bit integer.
- 样例输入
	```
	2
	2 3 5
	3 4 6 12
	```
- 样例输出
	```
	15
	12
	```
两个正整数a，b的最小公倍数等于`a / gcd(a，b) * b`。
```c
#include <cstdio>
int gcd(int a, int b) {
    return !b ? a : gcd(b, a % b);
}
int lcm(int a, int b) {
    return a / gcd(a, b) * b;
}

int main() {
    int n;
    scanf("%d", &n);
    while (n--) {
        int m;
        scanf("%d", &m);
        int a, b;
        scanf("%d", &a);
        for (int i = 1; i < m; i++) {
            scanf("%d", &b);
            a = lcm(a, b);
        }
        printf("%d\n", a);
    }
    return 0;
}
```
### 问题 1818: 最大公约数
输入两个正整数，求其最大公约数。
- 输入：测试数据有多组，每组输入两个正整数。
- 输出：对于每组输入,请输出其最大公约数。
- 样例输入
	```
	49 14
	```
- 样例输出
	```
	7
	```
```c
#include <cstdio>
int gcd(int a, int b) {
    return !b ? a : gcd(b, a % b);
}
int main() {
    int a, b;
    while (scanf("%d%d", &a, &b) != EOF) {
        printf("%d\n", gcd(a, b));
    }
}
```
## 《算法笔记》5.3小节——数学问题->分数的四则运算
http://codeup.cn/contest.php?cid=100000566
### 问题 A: 分数矩阵
我们定义如下矩阵：
```
1/1 1/2 1/3
1/2 1/1 1/2
1/3 1/2 1/1
```
矩阵对角线上的元素始终是1/1，对角线两边分数的分母逐个递增。请求出这个矩阵的总和。
- 输入：输入包含多组测试数据。每行给定整数N（N<50000），表示矩阵为N*N。当N=0时，输入结束。
- 输出：输出答案，结果保留2位小数。
- 样例输入
	```
	1
	2
	3
	4
	0
	```
- 样例输出
	```
	1.00
	3.00
	5.67
	8.83
	```
这个题目只用分开看就好做了，1的数量为N，1/2的数量为2*(N-1)，以此类推。求和即可。
```c
#include <cstdio>
int main() {
    int N;
    while (scanf("%d", &N), N) {
        double sum = 0.0;
        for (int i = 1; i <= N; i++) {
            sum += (1.0 / i) * (N + 1 - i) * 2;
        }
        printf("%.2f\n", sum - N); //前面多加了N*1
    }
    return 0;
}
```
## 《算法笔记》5.4小节——数学问题->素数
### 问题 A: 素数
这道题用一般求素数的方法也能做，毕竟n未超过10^5^级别，`O(n*sqrt(n))`的复杂度也可以。
```c
#include <cstdio>
bool isPrime(int x) {
    for (int i = 2; i * i <= x; i++) {
        if (x % i == 0) return false;
    }
    return true;
}

int main() {
    int n; //n最大为4次方级别
    while (scanf("%d", &n) != EOF) {
        int Prime[n + 10] = {0}, pNum = 0;
        for (int i = 2; i < n; i++) {
            if(isPrime(i) && i % 10 == 1) { //如果i是素数且个位为1
                Prime[pNum++] = i;  //记录i
            }
        }
        if (pNum) {
            for (int i = 0; i < pNum; i++) {
                printf("%d", Prime[i]);
                if (i < pNum - 1) printf(" ");
                else printf("\n");
            }
        } else printf("-1\n");
    }
}
```
### 问题 B: Prime Number
将maxn由10e6改成1000000就通过了，不然就是运算错误0……什么垃圾判题机，浪费我时间好吧。**或许因为科学记数法是表示浮点数，不能作为数组大小**。
```c
#include <cstdio>
const int maxn = 1000000;
int Prime[maxn], pNum = 0;
bool p[maxn] = {0}; //是素数就为false
void findPrime() {
    for (int i = 2; i < maxn; i++) {
        if (!p[i]) {
            Prime[pNum++] = i;
            for (int j = i + i; j < maxn; j += i) {
                p[j] = true;
            }
        }
    }
}


int main() {
    int k;
    findPrime();
    while (scanf("%d", &k) != EOF) {
        printf("%d\n", Prime[k - 1]);
    }
    return 0;
}
```
### ★ 问题 C: Goldbach's Conjecture(哥德巴赫猜想)
这个问题是找出对于一个给定的[4，2^15^]内的偶数even，存在多少组不同的质数对(p1, p2)满足`p1+p2=even`。
**必备的起始点是素数表，采用埃氏筛法**，O(n log logn)的复杂度。然后，**双指针在素数表上滑动**，计算对数即可。
```c
#include <cstdio>
const int maxn = 100000;
int prime[maxn], pNum = 0;
bool p[maxn] = {0};

void findPrime() {
    for (int i = 2; i < maxn; i++) {
        if (p[i] == false) {
            prime[pNum++] = i;
            for (int j = i + i; j < maxn; j += i) {
                p[j] = true;
            }
        }
    }
}
int main() {
    int even;
    findPrime();
    while (scanf("%d", &even), even) {
        int cnt = 0;
        for (int i = 0, j = pNum - 1; i <= j;) {
            if (prime[i] + prime[j] == even) {
            	cnt++;
            	i++;
            	j--;
			} else if (prime[i] + prime[j] > even) {
				j--;
			} else {
				i++;
			}
        }
        printf("%d\n", cnt);
    }
    return 0;
}
```
## 《算法笔记》5.5小节——数学问题->质因子分解
### ★ 问题 A: 完数
**题目描述**
求1-n内的完数，所谓的**完数是这样的数，它的所有因子相加等于它自身**，比如6有3个因子1,2,3,1+2+3=6，那么6是完数。即完数是等于其所有因子相加和的数。
- 输入：测试数据有多组，输入n，n数据范围不大。
- 输出：对于每组输入,请输出1-n内所有的完数。如有案例输出有多个数字，用空格隔开，输出最后不要有多余的空格。
- 样例输入
	```
	6
	```
- 样例输出
	```
	6
	```
这个题目，不要受到书上面求质因子分解的影响，n的因子集合定义为[1, n)范围内能整除n的所有数，这里的因子包括1，这些因子相乘不等于n。28`其因子集合为{1, 2, 4, 7, 14}`，`质因子分解为28=2^2 * 7`，两者不同。
因此，本题其实很简单，只需**采用枚举[1, a)的所有因子并相加的方法**判断a是不是完数，并对1-n范围内的所有数判断一遍就可以了。
另外，我们注意到，**因子的集合，除去1后是关于sqrt(n)对称的**，如`2*14=28，4*7=28`。另一个例子36的`因子集合{1, 2, 3, 4, 6, 9, 12, 18}`，也一样如此。

本题因为题目数据范围较小，不用这么优化。像`问题 D: 约数的个数`与本题非常相似，约数集合不仅包括1，也包括n本身，完全对称，如果运行超时，就可以只算<=sqrt(n)的一半集合。
```c
#include <cstdio>

bool checkFullNum(int n) {
	int sum = 0;
	for (int i = 1; i < n; i++) {
		if (n % i == 0) { //i是n的因子  
            sum += i;
		}
	}
	if (sum == n) return true; //是完数返回true 
	else return false;
}

int main() {
    int n;
    while (scanf("%d", &n) != EOF) {
    	int ans[1000], k = 0;
    	for (int i = 1; i <= n; i++) { //求出1到n的所有完数
    		if (checkFullNum(i) == true) 
			   ans[k++] = i; 
        } 
        for (int i = 0; i < k; i++) {
        	printf("%d", ans[i]);
        	if (i < k - 1) printf(" ");
        	else printf("\n");
		}
	}
}
```
### 问题 B: 完数
一样的题目！
### ★(质因子分解) 问题 C: 质因数的个数
**质因子分解的代码片和方法**都在以下的代码中了，也没必要多说什么。
```c
#include <cstdio>
#include <cmath>
struct factor {
	int x, cnt; //x为质数因子, cnt为x个数 
};

const int maxn = 100020; 
int Prime[maxn], pNum = 0;
bool p[maxn] = {false}; 

void Find_Prime() { 
	for (int i = 2; i < maxn; i++) {
		if (p[i] == false) {
			Prime[pNum++] = i;
			for (int j = i + i; j < maxn; j += i) {
				p[j] = true;
			}
		}
	}	
}

int main() {
	int n;
	Find_Prime();
	while (scanf("%d", &n) != EOF) {
		struct factor fac[10];
		int sqr = (int)sqrt(1.0 * n);
		int num = 0; //num为n的不同质因子的个数 ;
		//枚举素数表中根号n以内的质因子 
		for (int i = 0; i < pNum && Prime[i] <= sqr; i++) {
			if (n % Prime[i] == 0) {   //如果Prime[i]是n的质因子 
			    fac[num].x = Prime[i]; //记录该质因子 
			    fac[num].cnt = 0;
			    while (n % Prime[i] == 0) { //计算出该质因子的个数 
		   	        fac[num].cnt++;
		   	        n /= Prime[i];
			    } 
			    num++; //不同质因子个数+1   
			} 
			//如果n为1, 说明n的质因子全部<=sqrt(n), 及时退出
            if (n == 1) break; 
		}
		if (n != 1) { //如果无法被根号n以内的质因子除尽 
			fac[num].x = n;  //那么一定有一个大于根号n的质因子 
			fac[num++].cnt = 1; //即此时的n 
		}
		int sum = 0; //计算全部质因子的总个数, 对于相同的质因数需要重复计算
		for (int i = 0; i < num; i++) 
			sum += fac[i].cnt;
		printf("%d\n", sum); 
	}
	return 0;
}
```
### 问题 D: 约数的个数
约数集合不仅包括1，也包括n本身，完全对称。36的`约数集合{1, 2, 3, 4, 6, 9, 12, 18, 36}`，以6为对称。由此可以**只算到sqrt(n)**，优化**计算一个数约数**的时间到O(√n)，总体时间复杂度为O(n√n)。
```c
#include <cstdio>
#include <cmath>
int main() {
    int n;
    while (scanf("%d", &n), n) {
        for (int i = 0; i < n; i++) {
            int t, cnt = 0;
            scanf("%d", &t);
            int sqr = (int)sqrt(1.0 * t);
            for (int i = 1; i <= sqr; i++) {
                if (t % i == 0) cnt += 2; //如1和t本身, 一次性算2个
                if (i * i == t) cnt--; //如36, 作为约数集合的对称轴6只算一次
            }
            printf("%d\n", cnt);
        }
    } 
    return 0;
}

```
### 问题 E: 完数与盈数
这一道题目和问题A、问题D都有一定的联系，可以一起思考。
```c
#include <cstdio>
int check(int n) {
    int sum = 0;
    for (int i = 1; i < n; i++) {
        sum += (n % i == 0) ? i : 0;
    }
    if (sum == n) return 0;
    else if (sum > n) return 1;
    else return 2;
}

int main() {
    int E[100], G[100], pE = 0, pG = 0;
    for (int i = 2; i <= 60; i++) {
        switch(check(i)) {
            case 0: E[pE++] = i; break;
            case 1: G[pG++] = i; break;
            case 2: break;
        }
    }
    printf("E: "); //打印完数
    for (int i = 0; i < pE; i++) {
        printf("%d", E[i]);
        if (i < pE - 1) printf(" ");
        else printf("\n");
    }
    printf("G: "); //打印盈数
    for (int i = 0; i < pG; i++) {
        printf("%d", G[i]);
        if (i < pG - 1) printf(" ");
        else printf("\n");
    }
    return 0;
}
```
## 《算法笔记》5.6小节——数学问题->大整数运算
http://codeup.cn/contest.php?cid=100000593
### ★ 问题 A: a+b
**题目描述**：实现一个加法器，使其能够输出a+b的值。
- 输入：输入包括两个数a和b，其中**a和b的位数不超过1000位**。
- 输出：可能有多组测试数据，对于每组数据，输出a+b的值。
- 样例输入
	```
	6 8
	2000000000 30000000000000000000
	```
- 样例输出
	```
	14
	30000000002000000000
	```
要实现1千位的加法，非得要**构建大整数结构**不可，值得一提的是有的人使用字符数组，但是我认为，字符数组除了**在高精度与低精度的乘法和除法上面还有优势**(毕竟不用自己构建结构)，在加法减法上面都没有优势，需要面对逆位存储低位不对齐带来的问题。
```c
#include <cstdio>
#include <cstring>

struct bign {
	int d[1100], len;
	bign() {
		memset(d, 0, sizeof(d));
		len = 0;
	} 
};
bign change(char str[]) {
	/* 将逆位存储的字符数组转变为顺位存储的bign */
	bign a;
	a.len = strlen(str); //bign的长度就是字符串的长度 
	for (int i = 0; str[i]; i++)
		a.d[i] = str[a.len - i - 1] - '0'; //逆着赋值 
	return a;
}

int compare(bign a, bign b) {
	/* 比较两个bign变量, 先比较长度, 再从高位到低位进行比较 */ 
	if (a.len > b.len) return 1;  //a大 
	else if (a.len < b.len) return -1; //a小
	else {
		for (int i = a.len - 1; i >= 0; i--) {
			if (a.d[i] > b.d[i]) return 1; //a大 
			else if (a.d[i] < b.d[i]) return -1; //a小 
		}
	} 
	return 0; //两数相等 
} 

bign add(bign a, bign b) {
	bign c;
	int carry = 0;  //carry是进位 
	for (int i = 0; i < a.len || i < b.len; i++) {//以较长的为界限  
        int t = a.d[i] + b.d[i] + carry;
        c.d[c.len++] = t % 10;
        carry = t / 10;
	}
	if (carry) c.d[c.len++] = carry;
	return c;
}

bign print(bign a) {
	for (int i = a.len - 1; i >= 0; i--) {
		printf("%d", a.d[i]);
	}	
}

int main() {
    char s1[1100], s2[1100];
    while (scanf("%s%s", s1, s2) != EOF) {
        bign a = change(s1);
        bign b = change(s2);
        print(add(a, b));
        printf("\n");
    }
    return 0;
}
```
### ★ 问题 B: N的阶乘
**题目描述**：输入一个正整数N，输出N的阶乘。
- 输入：正整数N(0<=N<=1000)
- 输出：输入可能包括多组数据，对于每一组输入数据，输出N的阶乘
- 样例输入
	```
	0
	4
	7
	```
- 样例输出
	```
	1
	24
	5040
	```
这一题我对0进行特判，对小于等于10的N使用fac函数计算，大于10的N使用大整数乘法，毕竟N的最大值可以是1000，已经远远超出long long范围了。
```c
#include <cstring>
#include <cstdio> 
int fac(int n) {
    int ans = 1;
    for (int i = 1; i <= n; i++) {
        ans *= i;
    }
    return ans;
}

struct bign {
    int d[10000], len;
    bign() {
        memset(d, 0, sizeof(d));
        len = 0;
    }
};

bign multi(bign a, int b) { //高精度与低精度的乘法 
    bign c;
    int carry = 0;
    for (int i = 0; i < a.len; i++) {
        int t = a.d[i] * b + carry;
        c.d[c.len++] = t % 10; //个位作为该位结果 
        carry = t / 10;  //高位部分作为新的进位 
    }
    while (carry != 0) { //乘法的进位可能不止一位, 用while 
        c.d[c.len++] = carry % 10;
        carry /= 10;
    }
    return c;
}

void print(bign a) {
    for (int i = a.len - 1; i >= 0; i--) {
        printf("%d", a.d[i]);
    }    
}

int main() {
    int N;
    while (scanf("%d", &N) != EOF) {
        if (N == 0) printf("1\n");
        else if (N <= 10) printf("%d\n", fac(N));
        else {
            bign a, temp;
            a.len = 1;
            a.d[0] = 1;
            for (int i = 2; i <= N; i++) {
                a = multi(a, i);
            }
            print(a);
            printf("\n");
        }
    }
    return 0;
}
```
### ★★★ 问题 C: 浮点数加法 
**题目描述**：求2个浮点数相加的和。题目中输入输出中出现浮点数都有如下的形式：
`P1P2...Pi.Q1Q2...Qj`，对于整数部分，P1P2...Pi是一个非负整数；对于小数部分，Qj不等于0。
- 输入：对于每组案例，第1行是测试数据的组数n，每组测试数据占2行，分别是两个加数。每组测试数据之间有一个空行，每行数据不超过100个字符。
- 输出：每组案例是n行，每组测试数据有一行输出是相应的和。输出保证一定是一个小数部分不为0的浮点数。
- 样例输入
	 ```
	2
	3.756
	90.564
	
	4543.5435
	43.25
	```
- 样例输出
	```
	94.32
	4586.7935
	```
```c

```
### ★★★ 问题 D: 进制转换
**题目描述**：将M进制的数X转换为N进制的数输出。
- 输入：输入的第一行包括两个整数：M和N(2<=M,N<=36)。
下面的一行输入一个数X，X是M进制的数，现在要求你将M进制的数X转换成N进制的数输出。
- 输出：输出X的N进制表示的数。
- 样例输入
	```
	10 2
	11
	```
- 样例输出
	```
	1011
	```
- 提示：注意输入时如有字母，则字母为大写，输出时如有字母，则字母为小写。
**类似于10进制转换为2进制，采用辗转相除法**，此题也是同样的思路。
1. 直接采用字符数组或者string类型，如同3.5问题C: 进制转换的改版一样，只不过这里是M进制到N进制的版本(2 <= M, N <= 36);
```c
#include <iostream>
#include <cstdio>
#include <string>
#include <algorithm>
using namespace std;

string conversion(int m, string s, int n) {
	string b;
    int len = s.size(); 
    for (int i = 0; i < len; ) { //当十进制字符还未除完时继续循环
    	int rest = 0, digit, x; //余数 
        for (int j = i; j < len; j++) {
        	if (s[j] >= '0' && s[j] <= '9') digit = s[j] - '0';
        	else if (s[j] >= 'A' && s[j] <= 'Z') digit = s[j] - 'A' + 10;
			int temp = (rest * m + digit), x = temp / n;
			if (x >= 0 && x <= 9) s[j] = x + '0'; //一位字符的变化
			else if (x >= 10 && x <= 36) s[j] = x - 10 + 'A';
            rest = temp % n; //这一位的余数=(上一位余数乘权+这一位数)%取余 
        }
        if (rest >= 0 && rest <= 9) b += char(rest + '0'); //从低位向高位存入取余的字符
        else if (rest >= 10 && rest <= 36) b += char(rest + 'a' - 10);
        while (s[i] == '0') i++;//跳过这一次产生的高位的0
        //12345 / 2 变成06172 那么下次跳过了数字为0的下标, 从6开始 ,但是下标总是以n结束
    }
    return b; 
}
int main()
{
	int M, N; string a,b,c;
	while (scanf("%d%d", &M, &N) != EOF) {
		cin >> a;
		b = conversion(M, a, N);
		reverse(b.begin(), b.end());
		cout << b << endl;
	} 
	return 0;
}
```
2. 或者转换成大整数再做，采用辗转相除法。 
```c
#include<stdio.h>
#include<string.h>
struct bign{
    int d[1010];
    int len;
    bign(){
        len = 0;
        memset(d, 0, sizeof(d));
    }
};
bign change(char a[])
{
    bign t;
    for(int i=strlen(a)-1; i>=0; i--)
    {
        if(a[i]>='0'&&a[i]<='9')
            t.d[t.len++] = a[i]-'0';
        else
            t.d[t.len++] = (a[i]-'A'+10);
    }
    return t;
}
int main()
{
    int m, n;
    while(scanf("%d%d",&m,&n)!=EOF)
    {
        getchar();//消去换行
        char x[10001];
        scanf("%s",x);
        bign a;
        a = change(x);
        char ans[1001];
        int len = strlen(x),index=0;
        while(a.len>0)//a.len==0即辗转相除到了最后
        {
            int carry = 0;
            for(int i=a.len-1; i>=0; i--)
            {
                int temp = a.d[i] + carry*m;
                carry = temp % n;
                a.d[i] = temp/n;
            }
            while(a.d[a.len-1]==0)
                a.len--;
            if(carry>=0&&carry<=9)
                ans[index++] = carry+'0';
            else
                ans[index++] = carry-10+'a';
        }
        for(int i=index-1; i>=0; i--)
            printf("%c", ans[i]);
        printf("\n");
    }
    return 0;
}

```

### 问题 E: 大整数排序
对N个长度最长可达到1000的数进行排序。
- 输入：输入第一行为一个整数N，(1<=N<=100)。接下来的N行每行有一个数，**数的长度范围为1<=len<=1000**。每个数都是一个正数，并且保证不包含前缀零。
- 输出：可能有多组测试数据，对于每组数据，将给出的N个数从小到大进行排序，输出排序后的结果，每个数占一行。
- 样例输入
	```
	4
	123
	1234
	12345
	2345
	```
- 样例输出
	```
	123
	1234
	2345
	12345
	```
这一题我对scanf("%d")的使用方法更了解了，它调用后会先跳过换行、空格等符号，然后输入一个整数就结束了，**下面的输入从整数之后开始，如果整数后面有换行符，而且我们使用gets的话，第一个gets把这个换行符吸收掉就会停止**，导致结果错误。 
```c
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

struct bign {
    int d[2000], len;
    bign() {
        memset(d, 0, sizeof(d));
        len = 0;
    }
};

bign change(char str[]) {
	/* 将逆位存储的字符数组转变为顺位存储的bign */
	bign a;
	a.len = strlen(str); //bign的长度就是字符串的长度 
	for (int i = 0; str[i]; i++)
		a.d[i] = str[a.len - i - 1] - '0'; //逆着赋值 
	return a;
}

void print(bign a) {
    for (int i = a.len - 1; i >= 0; i--) {
        printf("%d", a.d[i]);
    }    
}

bool cmp(bign a, bign b) {
    if (a.len != b.len) return a.len < b.len;  //从小到大排序 
    else {
        for (int i = a.len - 1; i >= 0; i--) {
            if (a.d[i] != b.d[i]) return a.d[i] < b.d[i]; //从小到大排序 
        }
    }
    return false;
}

int main() {
    int n;
    while (scanf("%d", &n) != EOF) {
    	getchar();  //吸收n后面的换行符 
        bign total[n + 10];
        char s[1100];
        for (int i = 0; i < n; i++) {
            gets(s);
            total[i] = change(s);
        }
        sort(total, total + n, cmp);
        for (int i = 0; i < n; i++) {
            print(total[i]);
            printf("\n");
        }
    }
    return 0;
}
```
### ★★★ 问题 F: 10进制 VS 2进制
**题目描述**：对于一个十进制数A，将A转换为二进制数，**然后按位逆序排列**，再转换为十进制数B，我们称B为A的二进制逆序数。
例如对于十进制数173，它的二进制形式为10101101，逆序排列得到10110101，其十进制数为181，181即为173的二进制逆序数。
- 输入：一个1000位(即10^999)以内的十进制数。
- 输出：输入的十进制数的二进制逆序数。
- 样例输入
	```
	985
	```
- 样例输出
	```
	623
	```
1. 按最普通的进制转换方法(`10->2，除2取余倒排；2->10，按位乘权相加`)，大整数。
```c
#include <cstdio>
#include <cstring>
struct bign {
	int d[5000], len;
	bign() {
		memset(d, 0, sizeof(d));
		len = 0;
	}
};

void print(bign a) {
	for (int i = a.len - 1; i >= 0; i--) {
		printf("%d", a.d[i]);
	}
	printf("\n");
}
bign change(char str[]) {
	bign t;
	t.len = strlen(str);
	for (int i = 0; i < t.len; i++) {
		t.d[i] = str[t.len - i - 1] - '0';
	} 	
	return t;
}

bign divideInt(bign a, int b, int &r) { //r为余数 
	bign t;
	t.len = a.len;
	for (int i = a.len - 1; i >= 0; i--) {
		r = r * 10 + a.d[i];
		if (r < b) t.d[i] = 0;
		else {
			t.d[i] = r / b;
			r %= b;
		} 
	} //去除高位的0 
	while (t.len >= 1 && t.d[t.len - 1] == 0) t.len--;
	return t; 
}
bign multiInt(bign a, int b) {
	bign t;
	int carry = 0;
	for (int i = 0; i < a.len; i++) {
		int temp = a.d[i] * b + carry;
		t.d[t.len++] = temp % 10;
		carry = temp / 10;
	}
	while (carry) {
		t.d[t.len++] = carry % 10;
		carry /= 10;
	}
	return t;
}
bign addInt(bign a, int b) {
	bign t; int carry;
	t.d[t.len] = a.d[0] + b;
	carry = t.d[t.len] / 10;
	t.d[t.len++] %= 10;
	for (int i = 1; i < a.len; i++) {
		t.d[t.len] = a.d[i] + carry;
		carry = t.d[t.len] / 10;
		t.d[t.len++] %= 10;
	}
	while (carry) {
		t.d[t.len++] = carry % 10;
		carry /= 10;
	}
	return t;
}
int t[10000], len = 0;
bign ArrToBign(int b) { //将b进制数组s(逆位存储)转换成10进制大整数 
	bign sum;
	for (int i = 0; i < len; i++) {	
		sum = addInt(multiInt(sum, 2), t[i]);
	} 
	return sum;
}
void BignToArr(bign a, int b, int s[]) { //将10进制大整数数转换成b进制数组s
	int r = 0;
	while (a.len > 0) { 
		a = divideInt(a, b, r);
		t[len++] = r;
		r = 0; //重新清零 
	}
}
int main() {
	char A[1015];
	while (scanf("%s", A) != EOF) {
		memset(t, 0, sizeof(t));
		bign a = change(A);
		BignToArr(a, 2, t);
		bign ans = ArrToBign(2);
		print(ans);
	}
	return 0;
}
```
2. 辗转相除法，这是3.5节问题C: 进制转换的**终极版本**。 
```c
#include <iostream>
#include <cstdio>
#include <string>
#include <algorithm>
using namespace std;

string conversion(int m, string s, int n) {
	string b;
    int len = s.size(); 
    for (int i = 0; i < len; ) { //当十进制字符还未除完时继续循环
    	int rest = 0; //余数 
        for (int j = i; j < len; j++) {
            int digit = s[j] - '0';
			int temp = (rest * m + digit) % n;//这一位的余数=(上一位余数乘权+这一位数)%取余 
            s[j] = (rest * m + digit) / n + '0'; //一位字符的变化
            rest = temp;
        }
        b += char(rest + '0'); //从低位向高位存入取余的字符
        while (s[i] == '0') i++;//跳过这一次产生的高位的0
        //12345 / 2 变成06172 那么下次跳过了数字为0的下标, 从6开始 ,但是下标总是以n结束
    }
    return b; 
}
int main()
{
    string a,b,c;
    while (cin >> a){
        b = conversion(10, a, 2);
        a = conversion(2, b, 10);
        reverse(a.begin(), a.end());
        cout << a << endl;
    }
}
```
## 《算法笔记》5.7小节——数学问题->扩展欧几里德算法
http://codeup.cn/contest.php?cid=100000594
### 问题 A: 同余方程-NOIP2012TGD2T1
```c
```

## 《算法笔记》5.8小节——数学问题->组合数
http://codeup.cn/contest.php?cid=100000595
### 问题 A: 计算组合数
要求将计算阶乘运算的函数写为fact(n)，**函数返回值的类型为float**。输出返回值的时候要用`%f`，如果用了`%d`，就只会输出0了。
```c
#include <cstdio>
float fact(int n) {
    float ans = 1.0;
    for (int i = 1; i <= n; i++) {
        ans *= i;
    }
    return ans;
}

int main() {
    int m, n;
    while (scanf("%d%d", &m, &n) != EOF) { //计算组合数的公式
        printf("%.0f\n", fact(m) / (fact(n) * fact(m - n)));
    }
    return 0;
}
```
### 问题 B: 求组合数
**题目描述**：组合数的计算虽说**简单但也不乏有些陷阱**，这主要是因为语言中的数据类型在表示范围上是有限的。更何况还有中间结果溢出的现象，所以千万要小心。
- 输入：求组合数的数据都是成对（M与N）出现的，每对整数M和N满足0＜m, n≤20，以EOF结束。
- 输出：输出该组合数。每个组合数换行。
- 样例输入
	```
	5 2
	18 13
	```
- 样例输出
	```
	10
	8568
	```
20的阶乘，已经大大超过了int的范围，但是还没有超出long long的范围，直接写一个阶乘函数的long long版就可以了。
```c
#include <cstdio>
typedef long long ll;
ll fact(int n) {
    ll ans = 1;
    for (int i = 1; i <= n; i++) {
        ans *= i;
    }
    return ans;
}

int main() {
    int m, n;
    while (scanf("%d%d", &m, &n) != EOF) {
        printf("%lld\n",  fact(m) / (fact(n) * fact(m - n)));
    }
    return 0;
}
```
---
## 《算法笔记》6.1小节——C++标准模板库(STL)介绍->vector的常见用法详解
### ★ 问题 A: Course List for Student (25)
运行错误86？？？本地可以测试过，但是CodeUp错误。我这里想的是**课程序号作为二维数组的行映射，选课的学生名映射到二维数组的列**，每一行是个记录某学生是否选择该门课程的哈希表，该表可以称作课程的学生名单(Student List for Course)。
```c
#include <cstdio>
#include <cstring>
int hashFunc(char s[]) { //3大写字母+1数字 
	int id = 0;
	for (int i = 0; i < 3; i++) {
		id = id * 26 + (s[i] - 'A');
	} 
	id = id * 10 + (s[3] - '0');
	return id;
} 

int main() {
	int N, K, cid, stuNum, id;
	char tempStr[6];
	scanf("%d%d", &N, &K);	
	bool cList[K + 1][26 * 26 * 26 * 10 + 1] = {false};
	for (int i = 0; i < K; i++) {			
		scanf("%d%d", &cid, &stuNum);
		while (stuNum--) {		
			scanf("%s", tempStr);
			id = hashFunc(tempStr);
			cList[cid][id] = true;
		} 
	}
	while (N--) {
		scanf("%s", tempStr);
		id = hashFunc(tempStr);
		int cour[K] = {0}, cnt = 0;
		for (int cid = 1; cid <= K; cid++) {
			if (cList[cid][id]) {
				cour[cnt++] = cid;
			}
		}
		printf("%s %d", tempStr, cnt);
		for (int t = 0; t < cnt; t++) {
			printf(" %d", cour[t]);
		}
		printf("\n");
	}
	return 0;
}
```
使用vector等重写后可以通过，如下，**这实际上是Course List for Student**，是学生的课程表(即题目希望我们要输出的结果)，每一行代表学生选的课程，是一个变长数组，可以使用排序和size()函数直接输出答案。
```c
#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>
using namespace std;
//只有一个测试用例
int hashFunc(char s[]) { //用hash存学生名, 3大写字母+1数字 
	int id = 0;
	for (int i = 0; i < 3; i++) {
		id = id * 26 + (s[i] - 'A');
	} 
	id = id * 10 + (s[3] - '0');
	return id;
} 

vector<int> stuOfCours[175761]; //即学生编号数26*26*26*10+1

int main() {
	int N, K, cid, stuNum, id; //课程ID 课程学生人数 学生ID 
	char tempStr[6];
	scanf("%d%d", &N, &K);
	while (K--) {			
		scanf("%d%d", &cid, &stuNum);
		while (stuNum--) {		
			scanf("%s", tempStr);
			id = hashFunc(tempStr);
			stuOfCours[id].push_back(cid); //一个学生的课程表向量 
		} 
	}
	while (N--) {
		scanf("%s", tempStr);
		id = hashFunc(tempStr);
		sort(stuOfCours[id].begin(), stuOfCours[id].end());
		printf("%s %d", tempStr, stuOfCours[id].size());
		for (vector<int>::iterator it = stuOfCours[id].begin(); it != stuOfCours[id].end(); it++) {
			printf(" %d", *it);
		}
		printf("\n");
	}
	return 0;
}
```
### 问题 B: Student List for Course (25)
从学生的课程表转换输出课程的学生名单，现在我已经轻车熟路了。使用vector、string的代码如下，确实简单了很多：
```c
#include <cstdio>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

vector<string> studentList[2510];
int main() {
	int N, K, n, cid;
	scanf("%d%d", &N, &K);
	while (N--) {
		string temp;
		cin >> temp >> n;
		while (n--) {
			scanf("%d", &cid);
			studentList[cid].push_back(temp);
		}
	}
	for (int i = 1; i <= K; i++) {
		printf("%d %d\n", i, studentList[i].size());
		sort(studentList[i].begin(), studentList[i].end());
		for (int j = 0; j < studentList[i].size(); j++) {
			cout << studentList[i][j] << endl; //string要用cout打印 
		}
	}
	return 0;
}

```
## 《算法笔记》6.2小节——C++标准模板库(STL)介绍->set的常见用法详解
### 问题 A: Set Similarity (25)
题目说的是，先计算**两个集合去重后的交集Nc**，然后计算**两个集合并集并去重Nt**，两者的元素个数相除。我下面这么写时间超限83，估计是**计算两个集合交集元素个数时超时**了，自己**手动遍历两个集合**就是O(n^2^)。而且本题只有一个测试用例。
```c
#include <cstdio>
#include <set>
using namespace std;

int main() {
	int N, n, temp, M, set1, set2;
	double ans;
	scanf("%d", &N);
	set<int> AllSet[N + 2];
	for (int t = 1; t <= N; t++) {
        scanf("%d", &n);
		for (int i = 1; i <= n; i++){
			scanf("%d", &temp);
            AllSet[t].insert(temp);
		}	
	}
	set<int>::iterator it1, it2;
	scanf("%d", &M);
	while (M--) {
		int nc = 0, nt = 0;
		scanf("%d%d", &set1, &set2);
		for (it1 = AllSet[set1].begin(); it1 != AllSet[set1].end(); it1++) {
			for (it2 = AllSet[set2].begin(); it2 != AllSet[set2].end(); it2++) { //1
				if (*it1 == *it2) nc++;   //2
			} //3
		}
		nt = AllSet[set1].size() + AllSet[set2].size() - nc;
		ans = nc * 100.0 / nt;
		printf("%.1f%\n", ans);
	}
	return 0;
} 
```
将这一标记了的部分改成下面一句以后，就通过了。set的find操作为O(logn)的复杂度，**计算两个集合交集元素个数**的速度可以提升到O(nlogn)。
```c
if (AllSet[set2].find(*it1) != AllSet[set2].end()) nc++;
```

## 《算法笔记》6.3小节——C++标准模板库(STL)介绍->string的常见用法详解
### ★★★ 问题 A: 字符串处理(科学计数法处理)
```c
```


## 《算法笔记》6.4小节——C++标准模板库(STL)介绍->map的常见用法详解
### 问题 A: Speech Patterns (25)
```c
```
## 《算法笔记》6.5小节——C++标准模板库(STL)介绍->queue的常见用法详解
### 问题 A: C语言-数字交换
`输入10个整数，将其中最小的数与第一个数对换，把最大的数与最后一个数对换`。虽然我没有看出这个题和队列的使用有半毛钱关系，但是我还是大体按照题目要求做了，写三个函数，最后一个数后输出空格。交换函数我使用了**algorithm提供的swap函数**，应该是传递了引用进去，才能修改外部的元素。

这道题略坑的地方在于，**必须先找到最小值交换，再找到最大值交换**。不能同时找到最小值最大值的下标，再进行交换。如果最大值出现在0位，最小值出现在x位，将其中最小的数与第一个数对换，就破坏了最大值的下标了。

```c
#include <cstdio>
#include <algorithm>
using namespace std;

void input(int num[]) {
    for (int i = 0; i < 10; i++) {
        scanf("%d", &num[i]);
    }
}

void process(int num[]) {
    int max, min;
    max = min = 0;
    for (int i = 0; i < 10; i++) {
        if (num[i] < num[min]) min = i;
    }
    swap(num[0], num[min]);
    for (int i = 0; i < 10; i++) {
        if (num[i] > num[max]) max = i;
    }
    swap(num[9], num[max]);
}

void print(int num[]) {
    for (int i = 0; i < 10; i++) {
        printf("%d ", num[i]);
    }
}
int main() {
    int num[10];
    input(num);
    process(num);
    print(num);
    return 0;
}
```
## 《算法笔记》6.6小节——C++标准模板库(STL)介绍->priority_queue的常见用法详解
### 问题 A: 

```c
```

## 《算法笔记》6.7小节——C++标准模板库(STL)介绍->stack的常见用法详解
### ★★★ 问题 A: 简单计算器 
```c
#include <cstdio>
#include <iostream>
#include <string>
#include <map>
#include <queue>
#include <stack>
using namespace std;
typedef struct node {
	bool flag; //true操作数, false操作符
	double num;
    char op;
} node;

string str; //中缀表达式字符串 
map<char, int> op; //操作符与优先级的映射 
stack<node> oper; //操作数或操作符栈 
queue<node> expr; //后缀表达式队列

void Transform() {
	double num;
	node temp;
	for (int i = 0; i < str.size(); ) {
		if (str[i] >= '0' && str[i] <= '9') {
			temp.flag = true; //标记为操作数
			temp.num = str[i++] - '0'; //记录这个操作数的第一个数位
			while (i < str.length() && str[i] >= '0' && str[i] <= '9') {
				temp.num = temp.num * 10 + (str[i] - '0');
				i++;
			} 
			expr.push(temp); //将遇见的操作数推入后缀表达式队列 
		} else { //如果是操作符 
			temp.flag = false; //标记为操作符
			/*只要操作符栈栈顶操作符的优先级高于等于这个操作符
			就弹出栈顶操作符到后缀表达式队列中, 直到栈顶操作符优先级低于这个操作符为止 */ 
			while (!oper.empty() && op[oper.top().op] >= op[str[i]]) {
				expr.push(oper.top());
				oper.pop();
			} 
			temp.op = str[i];
			oper.push(temp); //把该操作符压入操作符栈中 
			i++; 
		} 
	}
	/* 当操作符栈中还有操作符, 全部弹出到后缀表达式中 */
	while (!oper.empty()) { 
		expr.push(oper.top());
		oper.pop();
	} 
} 
 
double Calc() { //计算后缀表达式 
	double t1, t2;
	node cur, temp;
	while (!expr.empty()) { //只要后缀表达式非空 
		cur = expr.front(); //cur记录队首元素 
		expr.pop();
		if (cur.flag == true) oper.push(cur); //如果是操作数直接压入操作数栈中
		else { //如果是操作符 
			t2 = oper.top().num;  //弹出第二操作数
			oper.pop();
			t1 = oper.top().num; //弹出第一操作数
			oper.pop();
			temp.flag = true;   //临时记录操作数
			if (cur.op == '+') temp.num = t1 + t2;
			else if (cur.op == '-') temp.num = t1 - t2;
			else if (cur.op == '*') temp.num = t1 * t2;
			else temp.num = t1 / t2;  //除法
			oper.push(temp);  //把该操作数重新压入栈中 
		} 
	}
	return oper.top().num; //栈顶元素的值就是后缀表达式的值 
}
 
int main() { 
	op['+'] = op['-'] = 0;
	op['*'] = op['/'] = 1;
	while (getline(cin, str), str != "0") { //string类可以直接比较
        for (string::iterator it = str.begin(); it != str.end(); it++) {
        	if (*it == ' ') str.erase(it); //删去所有空格 
		} 
		while (!oper.empty()) oper.pop(); //初始化栈 
		Transform(); //将中缀表达式转换为后缀表达式 
		printf("%.2f\n", Calc()); //计算后缀表达式 
	}
	return 0;
}
```
### ★ 问题 B: Problem E
```c
#include <cstdio>
#include <stack>
#include <map>
using namespace std;

int main() {
	int n;
	scanf("%d", &n);
	getchar(); //吸收整数输入后的换行符 
	map<char, char> mp;
	mp[']'] = '[';
	mp[')'] = '(';
	mp['}'] = '{';
	char str[10000];
	stack<char> s;
	while (n--) {
		while (!s.empty()) s.pop(); //清空循环外面定义的栈 
		gets(str);
		int flag = 1; //符号是平衡的 
		for (int i = 0; str[i]; i++) {
			if (str[i] == '[' || str[i] == '(' || str[i] == '{') s.push(str[i]);
			else if (str[i] == ']' || str[i] == ')' || str[i] == '}') {
				if (s.empty() || s.top() != mp[str[i]]) {
					flag = 0; break;
				} else s.pop(); 
			}
		}
		if (!s.empty() || !flag) printf("no\n");
		else printf("yes\n");
	}
	return 0;
} 
```

## 《算法笔记》6.8小节——C++标准模板库(STL)介绍->pair的常见用法详解
### 问题 A: 重心在哪里
在平面直角坐标系中，**重心是中线的交点，其坐标是顶点坐标的算术平均**，即其重心坐标为：`(x1+x2+x3)/3，(y1+y2+y3)/3`。 
```c
#include <cstdio>
#include <map>
using namespace std;

int main() {
    int N;
    pair<double, double> t; //代表一个临时点
    while (scanf("%d", &N), N) {
    	while (N--) {
    		pair<double, double> cord;
	        for (int i = 0; i < 3; i++) {
	            scanf("%lf%lf", &t.first, &t.second);
	            cord.first += t.first;
				cord.second += t.second; 
	        } //顶点坐标的算术平均
	        printf("%.1f %.1f\n", cord.first / 3.0, cord.second / 3.0);	
		}
    }
    return 0;
}
```
## 《算法笔记》6.9小节——C++标准模板库(STL)介绍->algorithm头文件下常用函数介绍
这些题目目的在于熟悉algorithm头文件提供的函数，如`max、min、swap、reverse、next_permutation、fill、sort`。
### 问题 A: 求最大最小数
```c
#include <cstdio>
#include <algorithm>
using namespace std;

int main() {
    int N;
    while (scanf("%d", &N) != EOF) {
        int t, maxNum = -1100000, minNum = 1100000;  //输入的数的绝对值不大于10^6
        for (int i = 0; i < N; i++) {
            scanf("%d", &t);
            maxNum = max(maxNum, t);
            minNum = min(minNum, t);
        }
        printf("%d %d\n", maxNum, minNum);
    }
}
```
### 问题 B: 全排列
用STL中的next_permutation会非常简洁。不过如果自己写的话就有点麻烦。
```c
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

int main() {
    char s[10];
    while (scanf("%s", s) != EOF) {
        int len = strlen(s);
        do {
            printf("%s\n", s);
        } while (next_permutation(s, s + len));
        printf("\n");
    }
    return 0;
}
```
### 问题 C: 数组逆置
```c
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;
int main() {
    char s[250];
    while (gets(s) != NULL) {
        int len = strlen(s);
        reverse(s, s + len);
        printf("%s\n", s);
    }
    return 0;
}
```
---
# 其他题目
### 打印菱形
```
										    *
										   * *
										  * * * 
										 * * * *
										* * * * *
										 * * * *
										  * * *
										   * *
										    *
```
```c
#include <cstdio>
int main()
{
	for (int i = 1; i <= 5; i++) {
		for (int j = i; j < 5+5+5; j++) { // 5+5用于平移图形
			printf(" ");
		}
		for (int k = 1; k <= i; k++) {
			if (k == i) printf("*\n");
			else printf("* ");
		}
	}
	for (int i = 1; i <= 4; i++) {
		for (int j = 1; j <= i+5+5; j++) { // 5+5用于平移图形
			printf(" ");
		}
		for (int k = i; k < 5; k++) {
			if (k == 4) printf("*\n");
			else printf("* ");
		}
	}
	return 0;
 } 
```

