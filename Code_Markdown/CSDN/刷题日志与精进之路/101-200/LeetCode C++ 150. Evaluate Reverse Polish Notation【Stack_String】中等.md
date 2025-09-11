Evaluate the value of an arithmetic expression in Reverse Polish Notation.

Valid operators are `+`, `-`, `*`, `/`. Each operand may be an integer or another expression.

**Note:**
- Division between two integers should truncate toward zero.
-  The given RPN expression is always valid. That means the expression would always evaluate to a result and there won't be any divide by zero operation.

**Example 1:**

```haskell
Input: ["2", "1", "+", "3", "*"]
Output: 9
Explanation: ((2 + 1) * 3) = 9
```

**Example 2:**

```haskell
Input: ["4", "13", "5", "/", "+"]
Output: 6
Explanation: (4 + (13 / 5)) = 6
```

**Example 3:**

```haskell
Input: ["10", "6", "9", "3", "+", "-11", "*", "/", "*", "17", "+", "5", "+"]
Output: 22
Explanation: 
  ((10 * (6 / ((9 + 3) * -11))) + 17) + 5
= ((10 * (6 / (12 * -11))) + 17) + 5
= ((10 * (6 / -132)) + 17) + 5
= ((10 * 0) + 17) + 5
= (0 + 17) + 5
= 17 + 5
= 22
```
题意：根据逆波兰表示法，求表达式的值。有效的运算符包括 `+`, `-`, `*`, `/` 。每个运算对象可以是整数，也可以是另一个逆波兰表达式。

---
### 解法 栈
很简单的题目，不过这里用到了 `std::function` 来包装加减乘除四个函数：
```cpp
class Solution {
public:
    int evalRPN(vector<string>& tokens) {
        stack<int> st;
        int a, b;
        unordered_map<string, function<int(int, int)>> rec = {
            {"+", [](int x, int y) { return x + y; }}, {"-", [](int x, int y) { return x - y; }}, 
            {"*", [](int x, int y) { return x * y; }}, {"/", [](int x, int y) { return x / y; }}};
        for (const string &token : tokens) { 
            if (rec.find(token) != rec.end()) {
                b = st.top(); st.pop(); a = st.top(); st.pop();
                st.push(rec[token](a, b));
            } else st.push(stoi(token));
        }
        return st.top();
    }
};
```
虽然写起来很有意思，但是效率不高：
```cpp
执行用时：20 ms, 在所有 C++ 提交中击败了68.52% 的用户
内存消耗：10.7 MB, 在所有 C++ 提交中击败了8.54% 的用户
```
更普通的版本如下：
```cpp
class Solution {
public:
    int evalRPN(vector<string>& tokens) {
        stack<int> st;
        int a, b;
        for (const string &token : tokens) { 
            if (token.size() == 1 && !isdigit(token[0])) {
                b = st.top(); st.pop(); a = st.top(); st.pop();
                switch (token[0]) {
                    case '+': st.push(a + b); break;
                    case '-': st.push(a - b); break;
                    case '*': st.push(a * b); break;
                    case '/': st.push(a / b); break;
                }
            } else st.push(stoi(token));
        }
        return st.top();
    }
};
```
提交后运行结果如下：
```cpp
执行用时：16 ms, 在所有 C++ 提交中击败了89.75% 的用户
内存消耗：10.5 MB, 在所有 C++ 提交中击败了25.76% 的用户
```
