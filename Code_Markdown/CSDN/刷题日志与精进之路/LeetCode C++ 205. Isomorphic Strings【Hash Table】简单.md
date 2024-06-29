Given two strings `s` and `t` , determine if they are isomorphic.

Two strings are isomorphic if the characters in `s` can be replaced to get `t` .

All occurrences of a character must be replaced with another character while preserving the order of characters. No two characters may map to the same character but a character may map to itself.

**Example 1:**
```cpp
Input: s = "egg", t = "add"
Output: true
```
**Example 2:**
```cpp
Input: s = "foo", t = "bar"
Output: false
```
**Example 3:**
```cpp
Input: s = "paper", t = "title"
Output: true
```
**Example 4:**
```cpp
Input: s = "ab", t = "aa"
Output: false
```
**Note:** You may assume both `s` and `t` have the same length.

题意： 给定两个等长字符串 `s` 和 `t` ，判断它们是否是同构的。如果 `s` 中的字符可以被一一替换得到 `t` ，那么这两个字符串是同构的。注意：所有出现的某个字符都必须用另一个字符替换，同时保留字符的顺序。两个字符不能映射到同一个字符上，但字符可以映射到自己本身。

---
### 解法 哈希表
这一题需要注意的就是同构的意义，回想**单射、满射、一一对应**的概念，可知同构即为：两个集合的元素一一对应。不存在一对多，也不存在多对一。因此，第一想法就是**将映射的键值对存入字典中**，同时保证**两个不同的字母映射得到的字母不能相同**：
- 如果不存在以 `s[i]` 为键的映射时，还需判断是否存在某个键对应的值为 `t[i]` ，如果存在则会属于多对一的情况，返回 `false` ；不存在则将 `<s[i]:t[i]>` 这一键值对存入字典中；
- 如果存在以 `s[i]` 为键的映射，则判断当前的 `t[i]` 是否等于 `dict[s[i]]` ，不等于则返回 `false` 。
- 最终返回 `true` ，此时 `dict` 记录的就是 `s<->t` 的一一映射表。

具体代码如下：
```cpp
class Solution {
public:
    bool isIsomorphic(string s, string t) {
        if (s.empty()) return t.empty();
        unordered_map<char, char> dict; 
        for (int i = 0; i < s.size(); ++i) { 
            if (dict.find(s[i]) == dict.end()) { //不存在从s[i]到t[i]的映射时
                for (const auto &it : dict) //判断t[i]是否在哈希表的值中
                    if (it.second == t[i]) //如果在里面说明属于多对一,返回false
                        return false; 
                dict[s[i]] = t[i]; //t[i]也不在dict中,记录s[i]:t[i],保证不同的字母映射得到的字母也不同
            } else { //存在从s[i]到t[i]的映射时 
                if (dict[s[i]] != t[i]) //如果之前出现过,现在对应的值不一样,返回false
                    return false;
            }  
        }
        return true;
    }
}; 
```
---
**20201227 Update** 今日打卡更新，使用两个**哈希映射**以提升效率：
```cpp
class Solution {
public:
    bool isIsomorphic(string s, string t) {
        if (s.empty()) return t.empty(); 
        unordered_map<char, char> st, ts;
        int n = s.size();
        for (int i = 0; i < n; ++i) {
            if (st.find(s[i]) == st.end()) { //没有s[i]到某个字符的映射
                if (ts.find(t[i]) != st.end()) return false; //但存在t[i]到某个s[j]的映射
                //此时s[i]和t[i]一一对应
                st[s[i]] = t[i]; //s[i]->t[i]
                ts[t[i]] = s[i]; //t[i]->s[i] //s[i] <-> t[i]被记录
            } else { //存在s[i]到某个字符的映射
                if (st[s[i]] != t[i]) return false; //不符合
                //于是st[s[i]] == t[i],同样就有ts[t[i]] = s[i]
            }
        }
        return true;
    }
};
```
运行效率如下：
```cpp
执行用时：12 ms, 在所有 C++ 提交中击败了54.99% 的用户
内存消耗：7.3 MB, 在所有 C++ 提交中击败了23.49% 的用户
```
