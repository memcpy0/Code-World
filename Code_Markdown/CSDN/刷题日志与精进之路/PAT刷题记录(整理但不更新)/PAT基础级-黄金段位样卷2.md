@[toc]

# 7-1 PTA使我精神焕发
以上是湖北经济学院同学的大作。本题就请你用汉语拼音输出这句话。

**输入格式：** 
本题没有输入。

**输出格式：** 
在一行中按照样例输出，以惊叹号结尾。

**输入样例：**

无

**输出样例：**
```cpp
PTA shi3 wo3 jing1 shen2 huan4 fa1 !
```
代码如下：
```cpp
#include <iostream>
using namespace std;

int main() {
    cout << "PTA shi3 wo3 jing1 shen2 huan4 fa1 !";
    return 0;
}
```
# 7-2 宇宙无敌大招呼
据说所有程序员学习的第一个程序都是在屏幕上输出一句“Hello World”，跟这个世界打个招呼。作为天梯赛中的程序员，你写的程序得高级一点，要能跟任意指定的星球打招呼。

**输入格式：** 
输入在第一行给出一个星球的名字 `S` ，是一个由不超过7个英文字母组成的单词，以回车结束。

**输出格式：**
在一行中输出 `Hello S` ，跟输入的 `S` 星球打个招呼。

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
#include <iostream>
using namespace std;

int main() {
    string s;
    cin >> s;
    cout << "Hello " << s;
    return 0;
}
```
# 7-3 谁是赢家
某电视台的娱乐节目有个表演评审环节，每次安排两位艺人表演，他们的胜负由观众投票和 3 名评委投票两部分共同决定。规则为：如果一位艺人的观众票数高，且得到至少 1 名评委的认可，该艺人就胜出；或艺人的观众票数低，但得到全部评委的认可，也可以胜出。节目保证投票的观众人数为奇数，所以不存在平票的情况。本题就请你用程序判断谁是赢家。

**输入格式：** 
输入第一行给出 2 个不超过 1000 的正整数 Pa 和 Pb，分别是艺人 a 和艺人 b 得到的观众票数。题目保证这两个数字不相等。随后第二行给出 3 名评委的投票结果。数字 0 代表投票给 a，数字 1 代表投票给 b，其间以一个空格分隔。

**输出格式：**
按以下格式输出赢家：

```cpp
The winner is x: P1 + P2
```

其中 `x` 是代表赢家的字母，`P1` 是赢家得到的观众票数，`P2` 是赢家得到的评委票数。

**输入样例：**

```cpp
327 129
1 0 1
```

输出样例：

```cpp
The winner is a: 327 + 1
```

代码如下：

```cpp
#include <iostream>
using namespace std;

int main() {
    int pa, pb, x, y, z;
    cin >> pa >> pb >> x >> y >> z;
    int a = 0, b = 0;
    x ? ++b : ++a;
    y ? ++b : ++a;
    z ? ++b : ++a;
    cout << "The winner is ";
    if (pa > pb && a >= 1 || a == 3) cout << "a: " << pa << " + " << a;
    else cout << "b: " << pb << " + " << b;
    return 0;
}
```
# 7-4 大笨钟
微博上有个自称“大笨钟V”的家伙，每天敲钟催促码农们爱惜身体早点睡觉。不过由于笨钟自己作息也不是很规律，所以敲钟并不定时。一般敲钟的点数是根据敲钟时间而定的，如果正好在某个整点敲，那么“当”数就等于那个整点数；如果过了整点，就敲下一个整点数。另外，虽然一天有24小时，钟却是只在后半天敲1~12下。例如在23:00敲钟，就是“当当当当当当当当当当当”，而到了23:01就会是“当当当当当当当当当当当当”。在午夜00:00到中午12:00期间（端点时间包括在内），笨钟是不敲的。

下面就请你写个程序，根据当前时间替大笨钟敲钟。

**输入格式：** 
输入第一行按照 `hh:mm` 的格式给出当前时间。其中 `hh` 是小时，在00到23之间；`mm` 是分钟，在00到59之间。

**输出格式：**
根据当前时间替大笨钟敲钟，即在一行中输出相应数量个 `Dang` 。如果不是敲钟期，则输出：

```cpp
Only hh:mm.  Too early to Dang.
```

其中 `hh:mm` 是输入的时间。

**输入样例1：**

```cpp
19:05
```

**输出样例1：**

```cpp
DangDangDangDangDangDangDangDang
```

**输入样例2：**

```cpp
07:05
```

**输出样例2：**

```cpp
Only 07:05.  Too early to Dang.
```

  
代码如下：
```cpp
#include <iostream> 
#include <cstdio>
using namespace std;

int main() {
	int h, m;
	scanf("%d:%d", &h, &m);
	if (h < 12 || (h == 12 && m == 0))
		printf("Only %02d:%02d.  Too early to Dang.\n", h, m);
	else {
		for (int i = 13; i <= h; ++i)
			cout << "Dang";
		if (m != 0) cout << "Dang\n";
	}
	return 0;
}
```
# 7-5 幸运彩票

彩票的号码有 6 位数字，若一张彩票的前 3 位上的数之和等于后 3 位上的数之和，则称这张彩票是幸运的。本题就请你判断给定的彩票是不是幸运的。
输入格式：

输入在第一行中给出一个正整数 N（$≤ 100$）。随后 N 行，每行给出一张彩票的 6 位数字。
输出格式：

对每张彩票，如果它是幸运的，就在一行中输出 `You are lucky!` ；否则输出 `Wish you good luck.` 。

**输入样例：**
```cpp
2
233008
123456
```

**输出样例：** 
```cpp
You are lucky!
Wish you good luck.
```
代码如下：
```cpp
#include <iostream>
using namespace std;
bool isLucky(int n) {
    int f3 = 0, s3 = 0;
    s3 = n % 10 + n / 10 % 10 + n / 100 % 10;
    f3 = n / 1000 % 10 + n / 10000 % 10 + n / 100000;
    if (s3 == f3) return true;
    return false;
}
int main() {
    int n, a;
    cin >> n;
    while (n--) {
        cin >> a;
        if (isLucky(a)) cout << "You are lucky!\n";
        else cout << "Wish you good luck.\n";
    }
}
```
# 7-6 福到了
“福”字倒着贴，寓意“福到”。不论到底算不算民俗，本题且请你编写程序，把各种汉字倒过来输出。这里要处理的每个汉字是由一个 N × N 的网格组成的，网格中的元素或者为字符 `@` 或者为空格。而倒过来的汉字所用的字符由裁判指定。

**输入格式：**
输入在第一行中给出倒过来的汉字所用的字符、以及网格的规模 N （不超过100的正整数），其间以 1 个空格分隔；随后 N 行，每行给出 N 个字符，或者为 `@` 或者为空格。

**输出格式：**
输出倒置的网格，如样例所示。但是，如果这个字正过来倒过去是一样的，就先输出 `bu yong dao le` ，然后再用输入指定的字符将其输出。

**输入样例 1：**

```cpp
$ 9
 @  @@@@@
@@@  @@@ 
 @   @ @ 
@@@  @@@ 
@@@ @@@@@
@@@ @ @ @
@@@ @@@@@
 @  @ @ @
 @  @@@@@
```

**输出样例 1：**

```cpp
$$$$$  $ 
$ $ $  $ 
$$$$$ $$$
$ $ $ $$$
$$$$$ $$$
 $$$  $$$
 $ $   $ 
 $$$  $$$
$$$$$  $ 
```

**输入样例 2：**

```cpp
& 3
@@@
 @ 
@@@
```

**输出样例 2：**

```cpp
bu yong dao le
&&&
 & 
&&&
```

本题是模拟题，代码如下：
```cpp
#include <iostream>
#include <string>
#include <algorithm> 
using namespace std;

int main() {
	char c;
	int n;
	scanf("%c %d", &c, &n); 
	string s[n], revs[n]; 
	getchar();
	for (int i = 0; i < n; ++i) {
		getline(cin, s[i]); 
		int t = n - 1 - i;
		revs[t] = s[i];
		reverse(revs[t].begin(), revs[t].end());
	}
	bool flag = true;
	for (int i = 0; i < n; ++i) {
		if (s[i] != revs[i]) {
			flag = false;
			break;
		}
	}
	if (flag) cout << "bu yong dao le\n";
	for (int i = 0; i < n; ++i) 
		for (int j = 0; j < revs[i].size(); ++j) 
			if (revs[i][j] == '@')
				revs[i][j] = c;
	for (int i = 0; i < n; ++i) 
		cout << revs[i] << endl;
	return 0; 
}
```

