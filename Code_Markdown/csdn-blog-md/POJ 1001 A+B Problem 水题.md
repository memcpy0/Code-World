 <b><font face="Helvetica" color="blue" size="5">Description</font></b>
Calculate `a+b`.

 <b><font face="Helvetica" color="blue" size="5">Input</font></b>
Two integer `a,b (0<=a,b<=10)`

 <b><font face="Helvetica" color="blue" size="5">Output</font></b>
Output `a+b`.

 <b><font face="Helvetica" color="blue" size="5">Sample Input</font></b>

```cpp
1 2
```

 <b><font face="Helvetica" color="blue" size="5">Sample Output</font></b>

```cpp
3
```

题目：A+B。

### 思路一 直接加
直接用加法。
```cpp
#include <iostream>
using namespace std;

int main() {
    int a, b;
    cin >> a >> b;
    cout << (a + b);
    return 0;
}
```
### 思路二 位操作
不用 `+` 的加法。
```cpp
#include <iostream>
using namespace std;

int add(int x, int y) {
	while (y != 0) {
		int carry = x & y;
		x = x ^ y;
		y = carry << 1;
	}
	return x;
}

int main() {
    int a, b;
    cin >> a >> b;
    cout << add(a, b);
    return 0;
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/2020061420384997.png)
