

We are given two sentences `A` and `B`.  (A *sentence* is a string of space separated words.  Each *word* consists only of lowercase letters.)

A word is ***uncommon*** if it appears exactly once in one of the sentences, and does not appear in the other sentence.

Return a list of all uncommon words. 

You may return the list in any order.

 

**Example 1:**

```swift
Input: A = "this apple is sweet", B = "this apple is sour"
Output: ["sweet","sour"]
```

**Example 2:**

```swift
Input: A = "apple apple", B = "banana"
Output: ["banana"]
```

 

**Note:**
- `0 <= A.length <= 200`
- `0 <= B.length <= 200`
- `A` and `B` both contain only spaces and lowercase letters.


题意：如果一个单词在其中一个句子中只出现一次，在另一个句子中却没有出现，那么这个单词就是不常见的。返回所有不常用单词的列表。

---
### 解法 手写 `split` +哈希集合
这个题目无非就是**分别**求差集 `A - B` 和 `B - A` 中只出现一次的单词，和求交集的方法差不多：
```cpp
class Solution {
private:
    unordered_map<string, int> split(const string& s, char delim = ' ') {
        int beginPos = s.find_first_not_of(delim);
        int endPos = s.find_first_of(delim, beginPos);
        unordered_map<string, int> ans;
        while (beginPos != string::npos) {
            ++ans[s.substr(beginPos, endPos - beginPos)]; //单词频数计数
            beginPos = s.find_first_not_of(delim, endPos);
            endPos = s.find_first_of(delim, beginPos);
        }
        return ans;
    }
public:
    vector<string> uncommonFromSentences(string A, string B) {
        unordered_map<string, int> &&recA = split(A), &&recB = split(B);
        vector<string> ans;
        for (const pair<string, int> &p : recA) 
            if (p.second == 1 && recB.find(p.first) == recB.end())
                ans.push_back(p.first);
        for (const pair<string, int> &p : recB) 
            if (p.second == 1 && recA.find(p.first) == recA.end())
                ans.push_back(p.first);
        return ans;
    }
};
```
运行效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：7.2 MB, 在所有 C++ 提交中击败了12.73% 的用户
```
---
### 解法2 输入输出流
题意也可以理解为拼接字符串 `A + B = C`，然后返回拼接后的字符串 `C` 中只出现过一次的单词。下面使用输入输出流完成这一题：
```cpp
class Solution { 
public:
    vector<string> uncommonFromSentences(string A, string B) {
        stringstream is(A + " " + B);
        string w;
        unordered_map<string, int> mp;
        while (is >> w) ++mp[w];
        vector<string> ans;
        for (const auto &it : mp) 
            if (it.second == 1)
                ans.push_back(it.first);
        return ans;
    }
};
```
代码更加简洁，就是速度慢了一点：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了73.79% 的用户
内存消耗：7.4 MB, 在所有 C++ 提交中击败了7.37% 的用户
```
