
Given a string `s`, remove duplicate letters so that every letter appears once and only once. You must make sure your result is **the smallest in lexicographical order** among all possible results.

**Example 1:**

```css
Input: s = "bcabc"
Output: "abc"
```

**Example 2:**

```css
Input: s = "cbacdcbc"
Output: "acdb"
```
  
**Constraints:**
- `1 <= s.length <= 10^4`
- `s` consists of lowercase English letters.


题意：给出一个字符串 `s` ，去除字符串中重复的字母，使得每个字母只出现一次。需保证 **返回结果的字典序最小**（要求不能打乱其他字符的相对位置）。

---
### 解法 贪心+栈
本题的做法有一点麻烦，因为题目要我们完成的任务比较多：
- 去重：保留每种字符各一个；
- 最小字典序：这些字符构成的字符串其字典序最小；
- 保持顺序：保留的字符之间的顺序不变。

因此不是一个**单调栈**就能够解决的事，尽管和**单调栈**的思路有一点相似——**单调上升**的字符序列，其字典序最小。在程序中，我们将要维护的这个栈**不完全是单调的**，比如说 `"bac"` ，当我们遍历到下标 `2` 时，栈中含有 `ba` ，这不是单调上升的。

最终的做法是：遇到一个已经出现的字符，就跳过；遇到一个新字符，如果**小于栈顶元素**，并且**在字符串（新字符的）后面还有同样的栈顶字符**，就不断弹出栈顶字符，之后入栈新字符。比如 `""bcabc"` ，遍历到下标 `2` 时，就要弹出前面的 `b, c` 字符，然后入栈 `a` 字符。

实际代码如下：
```cpp
class Solution {
public:
    string removeDuplicateLetters(string s) {
        vector<char> st;
        int n = s.size();
        bool vis[26] = {false};
        for (int i = 0; i < n; ++i) {
            if (vis[s[i] - 'a']) continue; //栈中已经含有这一字符
            //遇到一个新字符,如果小于栈顶,并且新字符后面还有和栈顶一样的,就弹出栈顶字符
            while (!st.empty() && st.back() > s[i] && s.find(st.back(), i) != string::npos) {
                vis[st.back() - 'a'] = false;
                st.pop_back();
            }
            vis[s[i] - 'a'] = true;
            st.push_back(s[i]);
        }
        string ans;
        for (const char &c : st) ans += c;
        return ans;
    }
};
```
提交后的结果如下：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了74.17% 的用户
内存消耗：6.5 MB, 在所有 C++ 提交中击败了96.64% 的用户
```


