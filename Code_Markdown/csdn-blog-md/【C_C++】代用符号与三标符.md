@[toc]

# 1. 代用符号
C和C++中令人惊奇的事情多了去了。比如这些代用符号：
```cpp
and		or		not		xor
and_eq  or_eq	not_eq  xor_eq
compl
bitand	<%	<:	%:
bitor	%>	:>	%:%:
```
这些符号能和原有的操作符完美替换，以适应更加受限的历史字符集，其中：
- `and` 表示逻辑与操作，等价于 `&&` 操作符，`and_eq` 等价于 `&=` 按位与赋值操作符；
	```cpp
	int n, m, t;
	cin >> n >> m >> t;
	if (n == m and m == t) cout << "Yes\n"; //and或者&& 
	else cout << "No\n";
		
	n and_eq m;
	cout << n;
	```
- `or` 表示逻辑或操作，等价于 `||` 操作符，`or_eq` 等价于 `|=` 按位或赋值操作符；
	```cpp
	int n, m, t;
	cin >> n >> m >> t;
	if (n == m or m == t) cout << "Yes\n"; //or或者||
	else cout << "No\n";
		
	n or_eq m;
	cout << n;
	```
- `not` 表示逻辑非操作，等价于 `!` 操作符，`not_eq` 等价于 `!=` 不等操作符；
	```cpp
	int n, m;
	cin >> n >> m;
	vector<int> t(2, 0);
	if (not(t.empty()) cout << n;
	
	if (n not_eq m) cout << "Yes\n"; //and或者&& 
	else cout << "No\n";
	```
- `xor` 表示按位异或操作，等价于 `^` 操作符，`xor_eq` 表示 `^=` 按位异或赋值操作符；
	```cpp
	int a, b;
	cin >> a >> b;
	cout << (a xor b) << ' ';
	
	//相当于a^=b^=a^b; 即a^=b;b^=a;a^=b; 交换a,b
	a xor_eq b xor_eq a xor_eq b;  
	cout << a << ' ' << b;
	```
- `bitand` 等价于 `&` 操作符，`bitor` 等价于 `|` 操作符，`compl` 等价于 `~` 操作符；
	```cpp
	int k;
	cin >> k;
	cout << (int)(compl k);
	```
- `<%` 和 `%>`  分别等价于 `{` 和 `}` ，`<:` 和 `:>` 分别等价于 `[` 和 `]` ，`%:, %:%:` 分别等价于 `#, ##` ；
	```cpp
	%:include <bits/stdc++.h>
	using namespace std;
	%:define maxn 100
	%:define F(i, a, b) for (register int i = a, i%:%:_end = b; i <= i%:%:_end; ++i)
	//除了用#来预处理外,#name表示将name变为"name",即变为字符串; ##则起连接作用,i##_end等同于i_end
	int arr<:maxn:>;
	int main() <%
		cin >> arr<:1:>;
		cout << arr<:1:>;
		return 0;
	%> 
	```

要注意的有以下几点：
- 没有相等运算符的代用符号 `eq` ，因为 `=` 字符在所有受支持的字符集中均存在；
- C语言中，要使用这些代用符号，需要包含文件 `<iso646.h>` ，其实囊括了这些代用符号的宏定义；C++中提供了语言内建的支持，因此 `<ciso646>` 中不定义任何内容，如下图所示：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210703164522751.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
- 当词法分析器遇到字符序列 `<::` 且后序字符非 `:` 也不是 `>` 时，将 `<` 作为预处理记号本身，而非代用符号 `<:` ——因此，对于 `std::vector<::std::string>` 不会被错误处理为 `std::vector[:std::string>` ，从 `C++11` 起。

下面是综合练习，编译一下就知道了，此时输入 `1314520` 会输出 `249036820` ：
```cpp
%:include <bits/stdc++.h>
%:define ll long long
%:define maxn 100
using namespace std;
int d<:maxn:>;
int main()
<%
	ll a, b, ans = 0;
	scanf("%lld", &a);
	for (int i = 0; i < 32; ++i) <%
		d<:i:> = (char)(a % 2 + '0'); //求出int型整数二进制形式
		a /= 2;
	%>
	for (int i = 0; i < 16; ++i) <% //交换前16位比特和后16位比特
		d<:i:> xor_eq d<:i + 16:> xor_eq d<:i:> xor_eq d<:i + 16:>; //交换x[i]和x[i+16]
	%>
	for (int i = 0, b = 1; i < 32; ++i, b *= 2) //恢复为int整型数
		ans += (int)(d<:i:> - '0') * b;
	printf("%lld", ans);
%> 
```

---
# 2. 三标符
三标符倒是没什么用处，而且在C++17时被移除了……看一下就可以了：
```cpp
??<  ->  {
??>  ->  }
??(  ->  [
??)  ->  ]
??=  ->  #
??/  ->  \
??'  ->  ^
??!  ->  |
??-  ->  ~
```
