
已知2021年11月14日为星期天，输入之前的任意一个年份的时间，求解是星期几。

说明：输入是三个整数，第一个表示年份，第二个表示月份，第三个表示日期，比如2020年11月5日为2020 11 5，之间以空格隔开；输出是对应英文单词，是星期4，为Thursday，首字母大写，其余小写。提示，星期一到星期天对应英文是Monday， Tuesday， Wendnesday， Thursday，Friday， Saturday和Sunday。

**输入示例1:** 

```cpp
2020 11 5
```

**输出示例１：**

```cpp
Thursday
```

**输入示例2:** 

```cpp
2021 6 2
```

**输出示例2：**

```cpp
Wendnesday
```

---
### 解法 模拟
这道题比较麻烦，我的解法是：先计算输入日期是该年的第几天，记为 `days` ，再计算该年1月1日到2021年11月14日（星期天）有多少天，记为 `totdays` 。二者之差就是输入日期到2021年11月14日的天数 `diffDays` ，这时就容易计算出答案了。
```cpp
#include <bits/stdc++.h>
using namespace std;
int y, mm, d;
int m[2][13] = {
	{0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
	{0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
};
string weekdays[7] = {"Monday", "Tuesday", "Wendnesday", "Thursday", "Friday", "Saturday", "Sunday"};
bool isLeap(int y) {
	return (y % 400 == 0) || (y % 4 == 0 && y % 100 != 0);	
}
int totdays = 0;
int main() {
	cin >> y >> mm >> d;
	int days = d;
	int f = isLeap(y) ? 1 : 0;
	for (int i = 1; i < mm; ++i) 
		days += m[f][i];
	mm = 1, d = 1;
	while (true) {
		if (y < 2021) {
			totdays += isLeap(y++) ? 366 : 365;
		} else if (mm < 11) {
			totdays += m[isLeap(y)][mm++];
		} else if (d < 14) {
			totdays += 14;
			break;	
		}
	}
	int diffDays = totdays - days;
	diffDays %= 7;
	cout << weekdays[7 - diffDays - 1];
	return 0;
}
```
