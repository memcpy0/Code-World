You are given a string `s` that consists of lower case English letters and brackets. 

Reverse the strings in each pair of matching parentheses, starting from the innermost one.

Your result should **not** contain any brackets. 

**Example 1:**

```css
Input: s = "(abcd)"
Output: "dcba"
```

**Example 2:**

```css
Input: s = "(u(love)i)"
Output: "iloveu"
Explanation: The substring "love" is reversed first, then the whole string is reversed.
```

**Example 3:**

```css
Input: s = "(ed(et(oc))el)"
Output: "leetcode"
Explanation: First, we reverse the substring "oc", then "etco", and finally, the whole string.
```

**Example 4:**

```css
Input: s = "a(bcdefghijkl(mno)p)q"
Output: "apmnolkjihgfedcbq"
```
 
**Constraints:**
- `0 <= s.length <= 2000`
- `s` only contains lower case English characters and parentheses.
- It's guaranteed that all parentheses are balanced.

题意：给出一个字符串 `s`（仅含有小写英文字母和括号）。按照从括号内到外的顺序，逐层反转每对匹配括号中的字符串，并返回最终的结果，结果中 **不应** 包含任何括号。

---
### 解法 栈
说是栈，其实代码中使用的是双端队列：
```cpp
class Solution {
public:
    string reverseParentheses(string s) {
        deque<char> st; //双端字符队列
        string ans;
        for (const char &c : s) {
            if (c == ')') { //碰到右括号,触发反转
                string temp;
                while (!st.empty() && st.back() != '(') {
                    temp.push_back(st.back());
                    st.pop_back();
                }
                st.pop_back(); //弹出右括号
                for (const char &ch : temp) st.push_back(ch); //将反转后的字符串(不含括号)重新进栈
            } else st.push_back(c);
        }
        while (!st.empty()) { ans.push_back(st.front()); st.pop_front(); }
        return ans;
    }
};
```
运行效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：6.5 MB, 在所有 C++ 提交中击败了67.83% 的用户
```
更加简洁一点，直接在原串上反转，最后收集非括号字符得到结果：
```cpp
class Solution {
public:
    string reverseParentheses(string s) {
        stack<int> st;
        string ans;
        int n = s.size(), l;
        for (int i = 0; i < n; ++i) {
            if (s[i] == '(') st.push(i);
            else if (s[i] == ')') { //碰到右括号,触发反转
                l = st.top(); st.pop();
                reverse(s.begin() + l + 1, s.begin() + i);
            }
        }
        for (const char &c : s) if (c != '(' && c != ')') ans.push_back(c);
        return ans;
    }
};
```
运行效率如下：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了48.81% 的用户
内存消耗：6.5 MB, 在所有 C++ 提交中击败了61.66% 的用户
```
