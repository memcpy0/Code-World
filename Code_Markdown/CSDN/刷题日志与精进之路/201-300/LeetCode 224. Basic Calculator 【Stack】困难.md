Implement a basic calculator to evaluate a simple expression string.

The expression string may contain open `(` and closing parentheses `)`, the plus `+` or minus sign `-`, **non-negative** integers and empty spaces .

Example 1:
```cpp
Input: "1 + 1"
Output: 2
```

Example 2:
```cpp
Input: " 2-1 + 2 "
Output: 3
```

Example 3:
```cpp
Input: "(1+(4+5+2)-3)+(6+8)"
Output: 23
```

Note:
-    You may assume that the given expression is always valid.
-    **Do not** use the `eval` built-in library function.


题意：实现一个基本的运算器。

思路：见代码注释。一开始还打算用python的 `eval` ，结果爆栈了。还是得老实写。

代码：
```cpp
class Solution {
public:
    int calculate(string s) { 
        //可以用栈将其中缀转为后缀, 然后对后缀求值; 
        //不过这里只有加减法和(), 可以直接用栈处理中缀
        int ans = 0, sign = 1, size = s.size(), now = 0;   //ans答案,sign符号,now为正在处理的绝对值
        stack<int> st;
        for (int i = 0; i < size; ++i) {
            if (s[i] == '+') {         //前面的子表达式已经处理出结果
                ans += sign * now;
                sign = 1;              //接下来的是正值
                now = 0;               //now记录接下来的值
            } else if (s[i] == '-') {
                ans += sign * now;
                sign = -1;             //接下来的是负值
                now = 0;               //now记录接下来的值
            } else if (s[i] == '(') {
                ans += sign * now;
                st.push(ans);          //将前面的子表达式结果压入栈中
                st.push(sign);         //将知道的现在这部分表达式的符号压入栈中
                sign = 1;              //恢复初始状态
                ans = now = 0;         //处理子表达式的ans和now
            } else if (s[i] == ')') {
                ans += sign * now;          //加上前面的部分值
                ans *= st.top(); st.pop();  //乘以()部分的符号
                ans += st.top(); st.pop();  //加上前面的部分值
                sign = 1, now = 0;
            } else if (isdigit(s[i]))  //是数字
                now = now * 10 + (s[i] - '0');
        }
        return ans + now * sign;
    }
};
```
效率：代码可以写得简单一点的。
```cpp
执行用时 :12 ms, 在所有 C++ 提交中击败了94.00% 的用户
内存消耗 :8 MB, 在所有 C++ 提交中击败了100.00%的用户
```

