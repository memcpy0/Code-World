Given a string, you need to reverse **the order of characters in each word** within a sentence while still **preserving whitespace and initial word order**.

**Example 1:**
```cpp
Input: "Let's take LeetCode contest"
Output: "s'teL ekat edoCteeL tsetnoc"
```

**Note:** In the string, each word is separated by **single space** and there will not be any extra space in the string. 

题意：翻转一个字符串中包含的单词的字符顺序，保持空格和单词的初始顺序。注意：这里用单个空格分割单词，没有多余的空格，意味着字符串尾没有空格。

---
思路1：和 `HDU 1062 Text Reverse` 一样，遇到空格，翻转前面的单词即可。

代码：
```cpp
class Solution {
public:
    string reverseWords(string s) {
        if (s.empty()) return s;
        int begin = 0;
        for (int endPos = 0; endPos < s.size(); ++endPos) {
            if (s[endPos] == ' ') {
                reverse(s.begin() + begin, s.begin() + endPos);
                begin = endPos + 1;
            } else if (endPos == s.size() - 1) 
                reverse(s.begin() + begin, s.end());
        }
        return s;
    }
};
```

---
思路2：用栈模拟 `reverse` 。

代码：
```cpp
class Solution {
public:
    string reverseWords(string s) {
        if (s.empty()) return s;
        stack<char> st;
        string ans;
        int slen = s.size();
        for (int i = 0; i < slen; ++i) {
            if (s[i] == ' ') {
                while (!st.empty()) {
                    ans.push_back(st.top());
                    st.pop();
                }
                if (i != slen - 1) ans.push_back(' ');
            } else st.push(s[i]);
        }
        while (!st.empty()) {
            ans.push_back(st.top());
            st.pop();
        }
        return ans;
    }
};
```
