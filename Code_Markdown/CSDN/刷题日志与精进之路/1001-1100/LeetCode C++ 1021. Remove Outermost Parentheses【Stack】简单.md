

A valid parentheses string is either empty `("")`, `"(" + A + ")"`, or `A + B`, where `A` and `B` are valid parentheses strings, and `+` represents string concatenation.  For example, `""`, `"()"`, `"(())()"`, and `"(()(()))"` are all valid parentheses strings.

A valid parentheses string `S` is **primitive** if it is nonempty, and there does not exist a way to split it into `S = A+B`, with `A` and `B` nonempty valid parentheses strings.

Given a valid parentheses string `S`, consider its primitive decomposition: `S = P_1 + P_2 + ... + P_k`, where `P_i` are primitive valid parentheses strings.

Return `S` after removing the outermost parentheses of every primitive string in the primitive decomposition of `S`.

 

**Example 1:**

```swift
Input: "(()())(())"
Output: "()()()"
Explanation: 
The input string is "(()())(())", with primitive decomposition "(()())" + "(())".
After removing outer parentheses of each part, this is "()()" + "()" = "()()()".
```

**Example 2:**

```swift
Input: "(()())(())(()(()))"
Output: "()()()()(())"
Explanation: 
The input string is "(()())(())(()(()))", with primitive decomposition "(()())" + "(())" + "(()(()))".
After removing outer parentheses of each part, this is "()()" + "()" + "()(())" = "()()()()(())".
```

**Example 3:**

```swift
Input: "()()"
Output: ""
Explanation: 
The input string is "()()", with primitive decomposition "()" + "()".
After removing outer parentheses of each part, this is "" + "" = "".
```
**Note:**
- `S.length <= 10000`
- `S[i]` is `"("` or `")"`
- `S` is a valid parentheses string

题意：有效括号字符串为空 `("")`、`"(" + A + ")"` 或 `A + B`，其中 `A` 和 `B` 都是有效的括号字符串，`+` 代表字符串的连接。 如果有效字符串 `S` 非空，且不存在将其拆分为 `S = A+B` 的方法，我们称其为**原语**（**primitive**），其中 `A` 和 `B` 都是非空有效括号字符串。

给出一个非空有效字符串 `S`，考虑将其进行原语化分解，使得：`S = P_1 + P_2 + ... + P_k`，其中 `P_i` 是有效括号字符串原语。对 `S` 进行原语化分解，删除分解中每个原语字符串的最外层括号，返回 `S` 。

---
这道题可能看起来很麻烦，但是思考一下能够发现，每个**最外层括号**形成的括号组就是一个原语，将原语的外层括号去除，然后将所有的原语拼接起来，就得到了答案。所以难度在于代码书写，如何找到原语？


### 解法1 栈
和括号匹配那道题很像。因为题目说明是**非空有效括号字符串**，于是去除最外层括号，等价于**当遇到 `)` 而栈只剩下一个元素 `(` 时拼接括号内的字符串**。我们使用栈做到这一点，遇到 `(` 入栈，如果栈中元素个数 `> 1` 则拼接这一字符；遇到 `)` 时出栈：
```cpp
class Solution {
public:
    string removeOuterParentheses(string S) {
        string ans;
        stack<char> st;
        for (int i = 0; i < S.size(); ++i) {
            if (S[i] == '(') st.push(S[i]);
            if (st.size() > 1) ans += S[i];
            if (S[i] == ')') st.pop();
        }
        return ans;
    }
};
```
运行效率如下：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了90.48% 的用户
内存消耗：7.1 MB, 在所有 C++ 提交中击败了11.53% 的用户
```
或者遇到 `)` 就出栈，如果栈此时非空则拼接这一字符（显然对于最外层的右括号，会弹出栈中仅剩的左括号，不会拼接这两个字符）；遇到 `(` 就入栈：
```cpp
class Solution {
public:
    string removeOuterParentheses(string S) {
        string ans;
        stack<char> st;
        for (int i = 0; i < S.size(); ++i) {
            if (S[i] == ')') st.pop();
            if (!st.empty()) ans += S[i];
            if (S[i] == '(') st.push(S[i]);
        }
        return ans;
    }
};
```
效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：7 MB, 在所有 C++ 提交中击败了13.11% 的用户
```
---
### 解法2 计数
使用括号计数，可以不使用栈，节约空间：
```cpp
class Solution {
public:
    string removeOuterParentheses(string S) {
        string ans;
        int num = 0;
        for (int i = 0; i < S.size(); ++i) {
            if (S[i] == '(') ++num;
            if (num > 1) ans += S[i];
            if (S[i] == ')') --num;
        }
        return ans;
    }
};
```
效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：6.9 MB, 在所有 C++ 提交中击败了32.64% 的用户
```
或者写成：
```cpp
class Solution {
public:
    string removeOuterParentheses(string S) {
        string ans;
        int num = 0;
        for (int i = 0; i < S.size(); ++i) {
            if (S[i] == ')') --num;
            if (num != 0) ans += S[i];
            if (S[i] == '(') ++num;
        }
        return ans;
    }
};
```
---
### 解法3 随手写的
我一开始的写法是：
```cpp
class Solution {
public:
    string removeOuterParentheses(string S) {
        string ans;
        for (int i = 0; i < S.size(); ) {
            if (S[i] == '(') {
                int j = i + 1, sum = 1;
                while (sum > 0) {
                    if (S[j] == '(') ++sum;
                    else --sum;
                    ++j;
                } //分割原语子串
                ans += S.substr(i + 1, j - i - 2);
                i = j;
            }
        }
        return ans;
    }
};
```
效率如下，比较低：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了90.48% 的用户
内存消耗：6.9 MB, 在所有 C++ 提交中击败了28.23% 的用户
```
