@[toc]
总时间限制: 1000ms
内存限制: 65536kB

**描述**
    The objective of the program you are going to produce is to evaluate boolean expressions as the one shown next:
    Expression: ( V | V ) & F & ( F | V )

 where V is for True, and F is for False. The expressions may include the following operators: ! for not , & for and, | for or , the use of parenthesis for operations grouping is also allowed.

   To perform the evaluation of an expression, it will be considered the priority of the operators, the not having the highest, and the or the lowest. The program must yield V or F , as the result for each expression in the input file.

**输入**
    The expressions are of a variable length, although will never exceed 100 symbols. Symbols may be separated by any number of spaces or no spaces at all, therefore, the total length of an expression, as a number of characters, is unknown.

   The number of expressions in the input file is variable and will never be greater than 20. Each expression is presented in a new line, as shown below.

**输出**
    For each test expression, print "Expression " followed by its sequence number, ": ", and the resulting value of the corresponding test expression. Separate the output for consecutive test expressions with a new line.

   Use the same format as that shown in the sample output shown below.

**样例输入**

    ( V | V ) & F & ( F| V)
    !V | V & V & !F & (F | V ) & (!F | F | !V & V)
    (F&F|V|!V&!F&!(F|F&V))

**样例输出**

    Expression 1: F
    Expression 2: V
    Expression 3: V

题意：给出一个含有$(,\ ),\ V,\ F,\ !,\ \&,\ |$等符号的表达式，其中$V$代表真，$F$代表假，求出整个式子的值。
## 20200325 更改
发现下面的程序有问题，虽然通过了：
$31$ 行的那个 $||$ 应该改为 $\&\&$。
```cpp
...
//如果不为空并且栈顶非( 且栈顶的元素优先级更高 就弹出 
					while (!sign.empty() && (sign.top() != '(' && prior[sign.top()] > prior[*it])) {
...
```
不然下面的原程序会出错，对于 $(V\ |\ ! F\ \&\ F)\ |\ F$会得到 $F$ 的结果，实际上应该为 $V$。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200325133421211.png)
修改后得到：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200325133559623.png)
##  中缀转后缀，后缀求值
思路：很简单，就是实现起来有点麻烦。

```cpp
#include <iostream>
#include <map>
#include <stack>
#include <string>
using namespace std;
string line; 
int caseNo = 0;
map<char, int> prior = {{'!', 3}, {'&', 2}, {'|', 1}};

string getPostExpr() {
	string res;
	stack<char> sign; //符号栈 
	for (auto it = line.begin(); it != line.end(); ++it) { 
		switch (*it) {
			case 'V' : 
			case 'F' : res += *it; break;
			case '(' : sign.push(*it); break;
			case ')' : 
				while (sign.top() != '(') {
					res += sign.top();
					sign.pop();
				}
				sign.pop(); //把(弹出 
				break;
			case '!': 
			case '&':
			case '|': //栈空或者队首为(或者符号优先级>=队首符号优先级 
				if (sign.empty() || sign.top() == '(' || prior[sign.top()] <= prior[*it]) 
					sign.push(*it);
				else {
					while (!sign.empty() && (sign.top() != '(' || prior[sign.top()] > prior[*it])) {
						res += sign.top();
						sign.pop();
					}
					sign.push(*it);
				}
				break;
		}
	}
	while (!sign.empty()) res += sign.top(), sign.pop(); 
	return res;
}

bool eval() {
	string newLine = getPostExpr();	
	stack<char> val; 
	char t1, t2;
	for (auto it = newLine.begin(); it != newLine.end(); ++it) {
		switch (*it) {
			case 'V':
			case 'F': val.push(*it); break;
			case '!':
				t1 = val.top(); val.pop();
				val.push(t1 == 'F' ? 'V' : 'F');
				break;
			case '&':
				t1 = val.top(); val.pop();
				t2 = val.top(); val.pop();
				if (t1 == 'F' || t2 == 'F') val.push('F');
				else val.push('V');
				break;
			case '|':
				t1 = val.top(); val.pop();
				t2 = val.top(); val.pop();
				if (t1 == 'V' || t2 == 'V') val.push('V');
				else val.push('F');
				break;
		}
	}
	if (val.top() == 'V') return true;
	return false;
} 

int main() {
	while (getline(cin, line)) {
		for (auto it = line.begin(); it != line.end(); ++it)
			if (*it == ' ') 
				line.erase(it); 
		if (eval()) printf("Expression %d: V\n", ++caseNo);
		else printf("Expression %d: F\n", ++caseNo);
	}
	return 0;
}
```


