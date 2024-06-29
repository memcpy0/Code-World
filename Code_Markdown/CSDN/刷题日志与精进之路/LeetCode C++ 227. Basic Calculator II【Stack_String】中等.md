 
<p>Given a string <code>s</code> which represents an expression, <em>evaluate this expression and return its value</em>.&nbsp;</p>

<p>The integer division should truncate toward zero.</p>

<p><strong>Example 1:</strong></p>

```css
Input: s = "3+2*2"
Output: 7
```

</pre><p><strong>Example 2:</strong></p>

```css
Input: s = " 3/2 "
Output: 1
```

</pre><p><strong>Example 3:</strong></p>

```css
Input: s = " 3+5 / 2 "
Output: 5
```

<p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= s.length &lt;= 3 * 10<sup>5</sup></code></li>
	<li><code>s</code> consists of integers and operators <code>('+', '-', '*', '/')</code> separated by some number of spaces.</li>
	<li><code>s</code> represents <strong>a valid expression</strong>.</li>
	<li>All the integers in the expression are non-negative integers in the range <code>[0, 2<sup>31</sup> - 1]</code>.</li>
	<li>The answer is <strong>guaranteed</strong> to fit in a <strong>32-bit integer</strong>.</li>
</ul>


题意：实现一个基本的计算器来计算一个简单的字符串表达式的值。字符串表达式**仅包含非负整数**，`+`，`-` ，`*`，`/` 四种运算符和空格。**整数除法仅保留整数部分**。


---
### 解法1 逆波兰表达式+符号栈+数值栈
先去除字符串表达式中的空格，再分割成 `token` 序列。最后使用符号栈和数值栈，将 `token` 序列边处理为逆波兰形式，边求值。具体代码如下：
```cpp
class Solution {
private:
    vector<string> tokens;
    unordered_map<char, int> rec = {{'+', 0}, {'-', 0}, {'*', 1}, {'/', 1}};
    function<int(int, char, int)> eval = [](int a, char op, int b) {
        return (op == '+' ? (a + b) : (op == '-' ? (a - b) : (op == '*' ? (a * b) : (a / b))));
    };
    void split(const string &s, const string &delim) {
        tokens.clear();
        size_t beginPos = s.find_first_not_of(delim);
        size_t endPos = s.find_first_of(delim, beginPos);
        while (beginPos != string::npos) {
            tokens.push_back(s.substr(beginPos, endPos - beginPos)); //分割词序列
            if (endPos != string::npos) tokens.push_back(string(1, s[endPos])); //运算符号
            beginPos = s.find_first_not_of(delim, endPos);
            endPos = s.find_first_of(delim, beginPos);
        }
    }
    int procAndCalc() {    
        stack<int> nums;
        stack<char> ops;
        int n = tokens.size();
        for (int i = 0; i < n; ++i) {
            switch (tokens[i][0]) {
                case '+': 
                case '-':
                case '*':
                case '/': 
                    while (!ops.empty() && rec[tokens[i][0]] <= rec[ops.top()]) { //优先级更低,计算高优先级的运算符
                        int b = nums.top(); nums.pop();
                        int a = nums.top(); nums.pop();
                        char op = ops.top(); ops.pop();
                        nums.push(eval(a, op, b));
                    }
                    ops.push(tokens[i][0]);
                    break;
                default: 
                    nums.push(stoi(tokens[i])); 
                    break;
            }
        }
        while (!ops.empty()) {
            int b = nums.top(); nums.pop();
            int a = nums.top(); nums.pop();
            char op = ops.top(); ops.pop();
            nums.push(eval(a, op, b));     
        }
        return nums.top();
    }
public:
    int calculate(string s) { 
        string str;
        for (const char &c : s) if (c != ' ') str.push_back(c); //删除所有空格
        split(s, "+-*/"); //得到token序列;s中的整数都是非负整数,不然处理负数就很麻烦,不如直接扫描字符串
        return procAndCalc(); //处理得到数值栈和符号栈,同时计算结果
    }
};
```
运行效率如下：
```cpp
执行用时：108 ms, 在所有 C++ 提交中击败了6.41% 的用户
内存消耗：25.2 MB, 在所有 C++ 提交中击败了5.04% 的用户
```
下次写一下只使用一个**数值栈**的做法。
