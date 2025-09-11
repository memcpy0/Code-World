<p><code>S</code> and <code>T</code> are strings composed of lowercase letters. In <code>S</code>, no letter occurs more than once.</p>

<p><code>S</code> was sorted in some custom order previously. We want to permute the characters of <code>T</code> so that they match the order that <code>S</code> was sorted. More specifically, if <code>x</code> occurs before <code>y</code> in <code>S</code>, then <code>x</code> should occur before <code>y</code> in the returned string.</p>

<p>Return any permutation of <code>T</code> (as a string) that satisfies this property.</p>

<strong>Example :</strong>
```clike
Input: 
S = "cba"
T = "abcd"
Output: "cbad"
Explanation: 
"a", "b", "c" appear in S, so the order of "a", "b", "c" should be "c", "b", and "a". 
Since "d" does not appear in S, it can be at any position in T. "dcba", "cdba", "cbda" are also valid outputs.
```

 

<p><strong>Note:</strong> 
<ul>
	<li><code>S</code> has length at most <code>26</code>, and no character is repeated in <code>S</code>.</li>
	<li><code>T</code> has length at most <code>200</code>.</li>
	<li><code>S</code> and <code>T</code> consist of lowercase letters only.</li>
</ul>



题意：字符串 `S` 和 `T` 只包含小写字符。在 `S` 中，所有字符只会出现一次。`S` 已经根据某种规则进行了排序。我们要根据 `S` 中的字符顺序对 `T` 进行排序。更具体地说，如果 `S` 中 `x` 在 `y` 之前出现，那么返回的字符串中 `x` 也应出现在 `y` 之前。返回任意一种符合条件的字符串 `T` 。

---
### 解法1 自定义 `sort` 
按照给出的新的字母表顺序排序，没有出现在 `S` 中的字符都排在最后。这种做法的时间复杂度是 $O(n\log n)$ ：
```cpp
class Solution {
public:
    string customSortString(string s, string t) {
        vector<int> order(26, 26);
        int n = s.size();
        for (int i = 0; i < n; ++i) order[s[i] - 'a'] = i;
        sort(t.begin(), t.end(), [&](const char &a, const char &b) { return order[a - 'a'] < order[b - 'a']; });
        return t;
    }
};
```
运行效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：6.2 MB, 在所有 C++ 提交中击败了49.22% 的用户
```
或者对这些没有出现的字符，沿用原字母表中的顺序：
```cpp
class Solution {
public:
    string customSortString(string s, string t) {
        vector<int> order(26);
        int n = s.size();
        for (int i = 0; i < n; ++i) order[s[i] - 'a'] = i + 1;
        for (int i = 0, j = n; i < 26; ++i) if (!order[i]) order[i] = ++j;
        
        sort(t.begin(), t.end(), [&](const char &a, const char &b) { return order[a - 'a'] < order[b - 'a']; });
        return t;
    }
};
```
运行效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：6.1 MB, 在所有 C++ 提交中击败了67.40% 的用户
```
---
### 解法2 计数排序
计算 `T` 中每个字母的出现次数，然后按照 `S` 的字母表顺序放置字母。这种做法的时间复杂度是 $O(n)$ ：
```cpp
class Solution {
public:
    string customSortString(string s, string t) {
        vector<int> cnt(26); 
        for (const char &c : t) ++cnt[c - 'a'];
        string ans;
        for (const char &c : s) {
            while (cnt[c - 'a']) {
                ans.push_back(c);
                --cnt[c - 'a'];
            }
        }
        for (int i = 0; i < 26; ++i) {
            while (cnt[i]) {
                ans.push_back('a' + i);
                --cnt[i];
            }
        }
        return ans;
    }
};
```
运行效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：6.2 MB, 在所有 C++ 提交中击败了49.22% 的用户
```
