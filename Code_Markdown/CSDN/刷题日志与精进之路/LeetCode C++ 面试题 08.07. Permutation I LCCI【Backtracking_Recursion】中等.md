
Write a method to compute all permutations of a string of unique characters.

**Example1:**
```swift
Input: S = "qwe"
Output: ["qwe", "qew", "wqe", "weq", "ewq", "eqw"]
```
**Example2:**
```swift
Input: S = "ab"
Output: ["ab", "ba"]
```

**Note:**
- All charaters are English letters.
- `1 <= S.length <= 9`


题意：编写一种方法，计算某字符串的所有排列组合，字符串每个字符均不相同。

---
### 解法1 `next_permutation` 
```cpp
class Solution {
public:
    vector<string> permutation(string s) {
        sort(s.begin(), s.end()); 
        vector<string> ans;
        do {
            ans.push_back(s);
        } while (next_permutation(s.begin(), s.end()));
        return ans;
    }
};
```
执行效率如下：
```cpp
执行用时：52 ms, 在所有 C++ 提交中击败了92.82% 的用户
内存消耗：19.9 MB, 在所有 C++ 提交中击败了62.08% 的用户
```
---
### 解法2 递归交换法
```cpp
class Solution {
private:
    vector<string> ans;
    void dfs(string &s, int begin, int end) {
        if (begin >= end) { ans.push_back(s); return; }
        for (int i = begin; i < end; ++i) {
            swap(s[begin], s[i]);
            dfs(s, begin + 1, end);
            swap(s[begin], s[i]);
        }  
    }
public:
    vector<string> permutation(string s) {
        dfs(s, 0, s.size());
        return ans;
    }
};
```
执行效率如下：
```cpp
执行用时：52 ms, 在所有 C++ 提交中击败了92.82% 的用户
内存消耗：22.9 MB, 在所有 C++ 提交中击败了15.09% 的用户
```
---
### 解法3 递归选择法
```cpp
class Solution {
private:
    vector<string> ans;
    void dfs(const string &s, int cur, int end, string& p) {
        if (cur >= end) { ans.emplace_back(p); return; }
        for (int i = 0; i < end; ++i) { 
            bool ok = true; //判断当前字符是否出现过
            for (int j = 0; j < cur; ++j) if (s[i] == p[j]) { ok = false; break; }
            if (ok) {
                p[cur] = s[i];
                dfs(s, cur + 1, end, p); 
            }
        }
    }
public:
    vector<string> permutation(string s) {
        string temp(s.size(), ' '); //用' '初始化字符串
        dfs(s, 0, s.size(), temp);
        return ans;
    }
};
```
执行效率如下：
```cpp
执行用时：76 ms, 在所有 C++ 提交中击败了66.20% 的用户
内存消耗：23 MB, 在所有 C++ 提交中击败了9.90% 的用户
```
