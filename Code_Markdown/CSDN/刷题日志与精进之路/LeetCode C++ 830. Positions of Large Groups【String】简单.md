<p>In a string <code><font face="monospace">s</font></code>&nbsp;of lowercase letters, these letters form consecutive groups of the same character.</p>

<p>For example, a string like <code>s = "abbxxxxzyy"</code> has the groups <code>"a"</code>, <code>"bb"</code>, <code>"xxxx"</code>, <code>"z"</code>, and&nbsp;<code>"yy"</code>.</p>

<p>A group is identified by an interval&nbsp;<code>[start, end]</code>, where&nbsp;<code>start</code>&nbsp;and&nbsp;<code>end</code>&nbsp;denote the start and end&nbsp;indices (inclusive) of the group. In the above example,&nbsp;<code>"xxxx"</code>&nbsp;has the interval&nbsp;<code>[3,6]</code>.</p>

<p>A group is considered&nbsp;<strong>large</strong>&nbsp;if it has 3 or more characters.</p>

<p>Return&nbsp;<em>the intervals of every <strong>large</strong> group sorted in&nbsp;<strong>increasing order by start index</strong></em>.</p>

 
<p><strong>Example 1:</strong></p>

```swift
Input: s = "abbxxxxzzy"
Output: [[3,6]]
Explanation: "xxxx" is the only large group with start index 3 and end index 6.
```
 
<p><strong>Example 2:</strong></p>

```swift
Input: s = "abc"
Output: []
Explanation: We have groups "a", "b", and "c", none of which are large groups.
```
<p><strong>Example 3:</strong></p>

```swift
Input: s = "abcdddeeeeaabbbcd"
Output: [[3,5],[6,9],[12,14]]
Explanation: The large groups are "ddd", "eeee", and "bbb".
```
 
<p><strong>Example 4:</strong></p>

```swift
Input: s = "aba"
Output: [] 
```

<p><strong>Constraints:</strong></p> 
<ul>
	<li><code>1 &lt;= s.length &lt;= 1000</code></li>
	<li><code>s</code> contains lower-case English letters only.</li>
</ul>

题意：在一个由小写字母构成的字符串 `s` 中，**连续的相同字符**被称为一个**分组**，所有包含大于或等于三个连续字符的分组为**较大分组**。找到每一个**较大分组**的区间，**按起始位置下标递增顺序排序后**，返回结果。

---
### 解法 顺序遍历
一次顺序扫描得到结果，需要处理最后一个可能的大分组。和**行程长度压缩算法**的写法差不多：
```cpp
class Solution {
public:
    vector<vector<int>> largeGroupPositions(string s) {
        vector<vector<int>> ans;
        int n = s.size();
        vector<int> temp{0, -1};
        for (int i = 1; i < n; ++i) {
            if (s[i] == s[i - 1]) temp[1] = i;
            else {
                if (temp[0] + 2 <= temp[1]) ans.push_back(temp);
                temp[0] = i; //temp[0]>temp[1]
            }
        }
        if (temp[0] + 2 <= temp[1]) ans.push_back(temp); //注意最后一个可能的大分组
        return ans;
    }
};
```
运行效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：7.6 MB, 在所有 C++ 提交中击败了46.77% 的用户
```
另一个想法是**使用哨兵**，在原字符串后面加一个大写字母，就不用特殊处理**连续字母在最后**的情况了：
```cpp
class Solution {
public:
    vector<vector<int>> largeGroupPositions(string s) {
        vector<vector<int>> ans;
        s.push_back('A'); //加一个大写字母作为哨兵
        int n = s.size(), begin = 0;
        for (int i = 1; i < n; ++i) {
            if (s[i] != s[i - 1]) {
                if (i - begin >= 3) ans.push_back({begin, i - 1});
                begin = i;
            }
        } 
        return ans;
    }
};
```
运行效率如下：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了90.54% 的用户
内存消耗：7.6 MB, 在所有 C++ 提交中击败了65.89% 的用户
```
或者改为**while循环**，同样不用处理最后的连续相同字符，代码还更短一点：
```cpp
class Solution {
public:
    vector<vector<int>> largeGroupPositions(string s) {
        vector<vector<int>> ans;
        int n = s.size(), i = 0;
        while (i < n) {
            int j = i + 1;
            while (s[i] == s[j]) ++j; //扫描后续的连续相同字符
            if (j - i >= 3) ans.push_back({i, j - 1});
            i = j;
        } 
        return ans;
    }
};
```
运行效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：7.4 MB, 在所有 C++ 提交中击败了89.15% 的用户
```
