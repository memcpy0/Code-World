快要出C++20了，我还在学C++11的一些特性……这里讲一下C++11的 `raw string literals` 原始字符串字面量。

什么是 `raw string literals` 呢？就是一个字符串，其中转义字符 `\n \t, \"` 等不被处理。如果是正常的字符串，其中 `\n` 就是一个换行字符，而在 `raw string literals` 中 `\n` 是两个字符 `\` 和 `n` 。

C++中 `raw string literals` 以 `R"(` 开始，以 `)"` 结束。下面是一个示例：
```cpp
#include <iostream>
#include <string>
using namespace std;

int main() {
	string normal_str = "First line.\nSecond line.\nEnd of message.\n";
	string raw_str    = R"(First lien.\nSecond line.\nEnd of message.\n)";
	cout << normal_str << endl;
	cout << raw_str << endl;
	return 0;
}
```
运行如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200804115005942.png)
`normal_str` 在编译期被处理，所以可以看到是3行字符串和一个空行。而 `raw_str` 中的转义字符不会被处理，所以看到的就是一行字符串，其中的转义字符原原本本的显示出来。

DevC++和Codeblocks都可以支持C++11，VS2010就比较老了，它不支持C++11的大部分特性，包括 `raw string literals` 。


`raw string literals` 的应用之一是：简化正则表达式的书写。正则中有许多元字符，如 `\, ., +` 等，如果我们想要使用这些元字符的本来含义，而不是正则中的含义，就需要转义。

比如加号 `+` ，我们需要转义为 `\+` ，写到正则中是 `\\+` 。但是用了 `raw string literals` 就可以写成 `\+` 了。其实就是Python的 `r""` 字符串……

```cpp
#include <iostream>
#include <string>
#include <regex> 
using namespace std;

int main() { 
	//regex integer("(\\+|-)?[[:digit:]]+");
	regex raw_pattern(R"((\+|-)?[[:digit:]]+)");
	regex integer(raw_pattern);
	cout << "输入一个数:" << endl;
	string input;
	cin >> input;
	if (regex_match(input, integer)) cout << "int" << endl;
	else cout << "invalid input" << endl; 
	return 0;
}
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/20200804121606180.png)




