Given an input string, reverse the string word by word.

Example 1:
```clike
Input: "the sky is blue"
Output: "blue is sky the"
```

Example 2:
```clike
Input: "  hello world!  "
Output: "world! hello"
Explanation: Your reversed string should not contain leading or trailing spaces.
```

Example 3:
```clike
Input: "a good   example"
Output: "example good a"
Explanation: You need to reduce multiple spaces between two words to a single space in the reversed string.
```

**Note:**

-    A word is defined as **a sequence of non-space characters**.
-    Input string may contain **leading or trailing spaces**. However, your reversed string **should not** contain leading or trailing spaces.
-    You need to **reduce multiple spaces** between two words to a single space in the reversed string.


**Follow up:** For C programmers, try to solve it in-place in O(1) extra space.


题意：以单词为单位逆转一个字符串。注意：一个单词被定义为非空格的字符序列。输入字符串可以包含多个前导或者后缀空格，但是结果字符串不应该存在这些空格。另外，结果中两个单词间的空格必须只有一个。

---
思路1：分割字符串形成一个词序列，然后从后往前串接出结果字符串。

代码：
```cpp
class Solution {
public:
    void split(const string &s, vector<string> &tokens, char delim=' ') {
        size_t beginPos = s.find_first_not_of(delim, 0);
        size_t lastPos  = s.find_first_of(delim, beginPos);
        while (beginPos != string::npos) {
            tokens.emplace_back(s.substr(beginPos, lastPos - beginPos));
            beginPos = s.find_first_not_of(delim, lastPos);
            lastPos = s.find_first_of(delim, beginPos);
        }
    }

    string reverseWords(string s) {
        vector<string> tokens;
        split(s, tokens, ' ');
        string ans;  
        if (tokens.empty()) return ans;
        if (tokens.size() == 1) return tokens[0];
        ans = tokens.back();
        for (int i = tokens.size() - 2; i >= 0; --i) {
            ans += ' ';
            ans += tokens[i];
        }        
        return ans;
    }
};
```
效率：$\text{O(n)}$ 时间和 $\text{O(n)}$ 空间。
```
执行用时：8 ms, 在所有 C++ 提交中击败了90.44% 的用户
内存消耗：8.7 MB, 在所有 C++ 提交中击败了100.00% 的用户
```

---
思路2：不用额外的空间，在原串上操作。

代码：
```cpp
class Solution {
public: 
    string reverseWords(string s) { 
        reverse(s.begin(), s.end());
        int n = s.size();
        int idx = 0;
        for (int start = 0; start < n; ++start) {
            //找到第一个非空字符
            if (s[start] != ' ') {
                //如果idx不是字符串开始,需要填充一个' ',然后指向下一个单词开头
                if (idx != 0) s[idx++] = ' ';
                //复制单词
                int end = start;
                while (end < n && s[end] != ' ') s[idx++] = s[end++];
                //逆转单词
                reverse(s.begin() + idx - (end - start), s.begin() + idx);
                start = end;
            }
        }
        //删除字符串结尾的其他字符
        s.erase(s.begin() + idx, s.end());
        return s;
    }
};
```
