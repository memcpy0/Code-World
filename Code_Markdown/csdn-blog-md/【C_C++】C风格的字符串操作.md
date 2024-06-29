@[toc]

---
# 1. `string.h` 头文件重要函数
## (1) `strlen()` 
`strlen(cstr)` 可以得到字符数组中第一个 `\0` 前的字符个数，示例如下：
```cpp
#include <stdio.h>
#include <string.h>
int main() {
	char str[10];
	gets(str);
	int len = strlen(str);
	printf("%d\n", len);
	return 0;
}
```
输入字符串 `amenda` ，输出 `6` 。

## (2) `strcmp()`
`strcmp(cstr1, cstr2)` 返回两个字符串大小按照字典序的比较结果：
- 如果 `cstr1 < cstr2` ，则返回 `-1` 或者其他负整数；
- 如果 `cstr1 == cstr2` ，则返回 `0` ；
- 如果 `cstr1 > cstr2` ，则返回 `1` 或者其他正整数
 
 类似的有 `stricmp(cstr1, cstr2)` ，比较两个字符串的大小，但不区分字母的大小写。示例如下：
```cpp
#include <stdio.h>
#include <string.h>
int main() {
	char s1[20], s2[20];
	gets(s1), gets(s2);
	int cmp1 = strcmp(s1, s2), cmp2 = stricmp(s1, s2);
	printf("%s %s %s\n", s1, cmp1 < 0 ? "<" : (!cmp1 ? "==" : ">"), s2);
	printf("%s %s %s\n", s1, cmp2 < 0 ? "<" : (!cmp2 ? "==" : ">"), s2);
	return 0;
}
```
输入字符串 `mihoyo sucks` 和 `mIhOyO SUCKS` ，输出如下：
```cpp
mihoyo sucks > mIhOyO SUCKS
mihoyo sucks == mIhOyO SUCKS
```
## (3) `strcpy()`
`strcpy(dest_cstr, src_cstr)` 可以将字符串 `src_cstr` 复制到字符串 `dest_cstr` ，**这里的复制包括了 `\0`** 。示例如下：
```cpp
#include <stdio.h>
#include <string.h>
int main() {
	char s1[20], s2[20];
	gets(s1), gets(s2); 
	strcpy(s1, s2);
	puts(s1); 
	return 0;
}
```
输入 `ineffective` 和 `Code World` ，输出 `Code World` 。

## (4) `strcat()`
`strcat(dest_cstr, src_cstr)` 将字符串 `src_cstr` 接到字符串 `dest_cstr` 的后面，这里的拼接包括 `src_cstr` 末尾的 `\0` 。示例如下：
```cpp
#include <stdio.h>
#include <string.h>
int main() {
	char s1[50], s2[50];
	gets(s1), gets(s2); 
	strcat(s1, s2);
	puts(s1); 
	return 0;
}
```
输入字符串 `hello ` 和 `world` ，输出 `helloworld` 。

---
# 2. `sscanf` 和 `sprintf` 、对比 `<sstream>`
`sscanf` 和 `sprintf` 是处理C风格字符串的重要利器，其作用和 `<sstream>` 头文件中定义的三个类 `istringstream, ostringstream, stringstream` 类似，主要用来进行数据类型转换。

后者的优势在于，`<sstream>` 使用了 `string` 对象来代替字符数组（`sscanf, sprintf` 方式），避免了缓冲区溢出的危险；能够自动推导传入参数和目标对象的类型，不存在错误使用格式化符的问题。即 `<sstream>` 更加安全、自动和直接。缺陷在于，流输入输出对象的操作速度较慢，效率较低。


`sscanf = string + scanf, sprintf = string + printf` ，顾名思义，是从字符数组中读取数据、向字符数组写入数据（会自动添加 `'\0'` ）的函数。回想一下 `scanf, printf` ，如果从屏幕上输入和输出整型变量，写法如下：
```cpp
scanf("%d", &n);
printf("%d", n);
```
上面的写法其实可以换成如下形式，其中的 `screen` 表示屏幕：
```cpp
scanf(screen, "%d", &n);
printf(screen, "%d", n);
```
`sscanf, sprintf` 乃至 `fscanf, fprintf` (`fscanf = file scanf, fprintf = file printf` ，是从文件中读取数据、向文件写入数据的函数）与上面的格式都相同，只不过分别将 `screen` 换成了字符数组、文件指针：
```cpp
sscanf(str, "%d", &n);    //将字符数组中的内容以"%d"的格式读入到n中
sprintf(str, "%d", n);    //将n以"%d"的格式写入到字符数组中
fscanf(File*, "%d", &n);  //将文件中的内容以"%d"的格式读入到n中
fprintf(File*, "%d", n);  //将n以"%d"的格式写入到文件中
```
下面给出具体代码示例：
```cpp
#include <stdio.h>
int main() {
	int n;
	double a, b;
	char input[200] = "123 0.123 2.34";
	sscanf(input, "%d%lf%lf", &n, &a, &b);
	printf("%d %lf %lf\n", n, a, b);
	
	char output[200];
	sprintf(output, "%lf %lf %d\n", a, b, n);
	printf("%s\n", output);
	return 0;
}
```
运行结果如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210512000911871.png)
不过上述代码也只是简单应用，**我们完全可以像 `scanf, printf` 一样进行复杂的格式化输入输出**。如下面的代码使用 `sscanf` 将字符串 `input` 中的内容按照 `"%s,%d:%lf"` 的格式读入到字符数组 `str` 、整型变量 `a` 、浮点型变量 `b` 中：
```cpp
#include <stdio.h>
int main() {
	char input[200] = "hellosprintfsscanf 2021:3.1415926"; 
	int a;
	double b;
	char str[200];
	sscanf(input, "%s %d:%lf", str, &a, &b); //输入字符串时要使用空白符隔开
	printf("str = %s, a = %d, b = %.3lf\n", str, a, b);
	return 0;
}
```
运行结果如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210512002045224.png)

下面的代码使用 `sprintf` 将 `a, b, str` 以 `"%s,%d:%.3lf"` 的格式写入到字符串 `output` 中：
```cpp
#include <stdio.h>
int main() {
	int a = 2021;
	double b = 3.1415926;
	char str[200] = "hellosprintfsscanf"; 
	char output[200];
	sprintf(output, "%s,%d:%.3lf", str, a, b);
	printf("output = %s\n", output);
	return 0;
}
```
运行结果如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210512002508349.png)
最后需要说明的是，`sscanf` 和 `scanf` 都**支持正则表达式**，如果配合正则来处理字符串，将无往而不利。日后如有时间，将在本文末尾补充说明。
